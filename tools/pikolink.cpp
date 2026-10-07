// pikolink: copy games to a PikoPlay over USB and manage its files.
// Builds on Linux, macOS and Windows. Without -p it finds the console by itself.
//   pikolink [-p PORT] hello | ls PATH | put LOCAL /dest | get /path LOCAL | rm /path
//                      | launch /path
#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#else
#include <fcntl.h>
#include <glob.h>
#include <termios.h>
#include <unistd.h>
#endif
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>
#include "util/crc32.h"

using Bytes = std::vector<uint8_t>;

// A serial port in raw mode with non-blocking reads.
class Port {
 public:
  explicit Port(const std::string& name) {
#ifdef _WIN32
    const std::string path = name.rfind("\\\\.\\", 0) == 0 ? name : "\\\\.\\" + name;
    h_ = CreateFileA(path.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
    if (h_ == INVALID_HANDLE_VALUE) throw std::runtime_error("cannot open " + name);
    DCB dcb{};
    dcb.DCBlength = sizeof dcb;
    GetCommState(h_, &dcb);
    dcb.BaudRate = 115200;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    dcb.fBinary = TRUE;
    dcb.fDtrControl = DTR_CONTROL_ENABLE;  // the console only sends while DTR is set
    dcb.fRtsControl = RTS_CONTROL_ENABLE;
    dcb.fOutxCtsFlow = dcb.fOutxDsrFlow = dcb.fOutX = dcb.fInX = FALSE;
    SetCommState(h_, &dcb);
    COMMTIMEOUTS t{};
    t.ReadIntervalTimeout = MAXDWORD;  // reads return at once with whatever is there
    t.WriteTotalTimeoutConstant = 2000;
    SetCommTimeouts(h_, &t);
#else
    fd_ = open(name.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fd_ < 0) throw std::runtime_error("cannot open " + name);
    termios t{};
    tcgetattr(fd_, &t);
    cfmakeraw(&t);
    t.c_cflag |= CREAD | CLOCAL;
    t.c_cc[VMIN] = 0;
    t.c_cc[VTIME] = 0;
    tcsetattr(fd_, TCSANOW, &t);
#endif
  }
  ~Port() {
#ifdef _WIN32
    CloseHandle(h_);
#else
    close(fd_);
#endif
  }
  Port(const Port&) = delete;
  Port& operator=(const Port&) = delete;

  void write(const Bytes& b) {
#ifdef _WIN32
    DWORD n = 0;
    if (!WriteFile(h_, b.data(), (DWORD)b.size(), &n, nullptr) || n != b.size()) throw std::runtime_error("write failed");
#else
    if (::write(fd_, b.data(), b.size()) != (ssize_t)b.size()) throw std::runtime_error("write failed");
#endif
  }
  // Returns the number of bytes read (0 when nothing is waiting).
  size_t read(uint8_t* buf, size_t n) {
#ifdef _WIN32
    DWORD got = 0;
    if (!ReadFile(h_, buf, (DWORD)n, &got, nullptr)) return 0;
    return got;
#else
    const ssize_t r = ::read(fd_, buf, n);
    return r > 0 ? (size_t)r : 0;
#endif
  }

  // Serial ports that could be a PikoPlay.
  static std::vector<std::string> candidates() {
    std::vector<std::string> out;
#ifdef _WIN32
    for (int i = 1; i <= 64; ++i) out.push_back("COM" + std::to_string(i));
#else
    for (const char* pattern : {"/dev/ttyACM*", "/dev/cu.usbmodem*"}) {
      glob_t g{};
      if (glob(pattern, 0, nullptr, &g) == 0)
        for (size_t i = 0; i < g.gl_pathc; ++i) out.push_back(g.gl_pathv[i]);
      globfree(&g);
    }
#endif
    return out;
  }

 private:
#ifdef _WIN32
  HANDLE h_;
#else
  int fd_;
#endif
};

class Link {
 public:
  explicit Link(const std::string& port) : port_(port) {}

  Bytes call(uint8_t cmd, const Bytes& payload, int timeoutMs = 1500) {
    ++seq_;
    Bytes body = {cmd, seq_, (uint8_t)payload.size(), (uint8_t)(payload.size() >> 8)};
    body.insert(body.end(), payload.begin(), payload.end());
    Bytes frame = {0xA5, 0x5A};
    frame.insert(frame.end(), body.begin(), body.end());
    put32(frame, piko::crc32(body.data(), body.size()));
    port_.write(frame);

    const auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeoutMs);
    uint8_t prev = 0;
    for (;;) {  // look for the start of the reply
      const uint8_t b = readN(1, deadline)[0];
      if (prev == 0xA5 && b == 0x5A) break;
      prev = b;
    }
    Bytes head = readN(4, deadline);
    const size_t n = head[2] | (head[3] << 8);
    Bytes data = readN(n, deadline);
    Bytes tail = readN(4, deadline);
    Bytes chk = head;
    chk.insert(chk.end(), data.begin(), data.end());
    const uint32_t crc = tail[0] | (tail[1] << 8) | (tail[2] << 16) | ((uint32_t)tail[3] << 24);
    if (piko::crc32(chk.data(), chk.size()) != crc) throw std::runtime_error("bad reply CRC");
    if (head[0] == 0xFF) throw std::runtime_error("device error " + std::to_string(data[0]) + ": " + std::string(data.begin() + 1, data.end()));
    return data;
  }

  static void put32(Bytes& b, uint32_t v) { for (int i = 0; i < 4; ++i) b.push_back((uint8_t)(v >> (8 * i))); }
  static void putStr(Bytes& b, const std::string& s) { b.push_back((uint8_t)s.size()); b.insert(b.end(), s.begin(), s.end()); }

 private:
  Bytes readN(size_t n, std::chrono::steady_clock::time_point deadline) {
    Bytes out;
    while (out.size() < n) {
      uint8_t buf[512];
      const size_t r = port_.read(buf, std::min(sizeof buf, n - out.size()));
      if (r > 0) { out.insert(out.end(), buf, buf + r); continue; }
      if (std::chrono::steady_clock::now() > deadline) throw std::runtime_error("no answer from the handheld");
      std::this_thread::sleep_for(std::chrono::microseconds(500));
    }
    return out;
  }
  Port port_;
  uint8_t seq_ = 0;
};

// The first serial port where a PikoPlay answers HELLO.
static std::string findConsole() {
  for (const std::string& name : Port::candidates()) {
    try {
      Link l(name);
      l.call(1, {}, 400);
      return name;
    } catch (const std::exception&) {
    }
  }
  throw std::runtime_error("no PikoPlay found: is it plugged in and on the home screen? (or pass -p PORT)");
}

static uint32_t u32(const Bytes& d, size_t o) { return d[o] | (d[o + 1] << 8) | (d[o + 2] << 16) | ((uint32_t)d[o + 3] << 24); }

int main(int argc, char** argv) {
  std::string port;
  int a = 1;
  if (argc > 2 && !strcmp(argv[1], "-p")) { port = argv[2]; a = 3; }
  if (a >= argc) {
    fprintf(stderr,
            "usage: pikolink [-p PORT] hello | ls PATH | put LOCAL /dest | get /path LOCAL | rm /path\n"
            "                          | launch /path\n");
    return 2;
  }
  const std::string cmd = argv[a];
  auto arg = [&](int i) -> std::string { if (a + i >= argc) throw std::runtime_error("missing argument"); return argv[a + i]; };
  try {
    if (port.empty()) port = findConsole();
    Link l(port);
    if (cmd == "hello") {
      Bytes d = l.call(1, {});
      printf("protocol %u  flash:%s  sd:%s  game running:%s\n", d[0], d[1] & 2 ? "ok" : "MISSING", d[1] & 1 ? "yes" : "no", d[1] & 4 ? "yes" : "no");
      printf("flash %u/%u KB   firmware %.*s\n", u32(d, 6) / 1024, u32(d, 2) / 1024, (int)d[10], (const char*)&d[11]);
    } else if (cmd == "ls") {
      for (uint16_t start = 0;;) {
        Bytes req = {(uint8_t)start, (uint8_t)(start >> 8)};
        Link::putStr(req, arg(1));
        Bytes d = l.call(2, req);
        size_t o = 2;
        for (int i = 0; i < d[0]; ++i) {
          const uint8_t fl = d[o + 4], nl = d[o + 5];
          if (fl & 1) printf("   [dir]"); else printf("%8u", u32(d, o));
          printf("  %s  %.*s\n", fl & 2 ? "SD   " : "FLASH", (int)nl, (const char*)&d[o + 6]);
          o += 6 + nl;
        }
        start += d[0];
        if (!d[1] || !d[0]) break;
      }
    } else if (cmd == "put") {
      std::ifstream f(arg(1), std::ios::binary);
      if (!f) throw std::runtime_error("cannot read " + arg(1));
      Bytes data((std::istreambuf_iterator<char>(f)), {});
      Bytes req; Link::put32(req, (uint32_t)data.size()); Link::putStr(req, arg(2));
      l.call(4, req, 3000);
      for (size_t off = 0; off < data.size(); off += 2048) {
        Bytes chunk; Link::put32(chunk, (uint32_t)off);
        chunk.insert(chunk.end(), data.begin() + off, data.begin() + std::min(data.size(), off + 2048));
        l.call(5, chunk, 3000);
        fprintf(stderr, "\r%zu / %zu bytes", std::min(data.size(), off + 2048), data.size());
      }
      Bytes end; Link::put32(end, piko::crc32(data.data(), data.size()));
      l.call(6, end, 3000);
      printf("\nsent %zu bytes to %s\n", data.size(), arg(2).c_str());
    } else if (cmd == "get") {
      Bytes out;
      for (;;) {
        Bytes req; Link::put32(req, (uint32_t)out.size()); req.push_back(2048 & 255); req.push_back(2048 >> 8); Link::putStr(req, arg(1));
        Bytes d = l.call(3, req);
        out.insert(out.end(), d.begin(), d.end());
        if (d.size() < 2048) break;
      }
      std::ofstream(arg(2), std::ios::binary).write((const char*)out.data(), (std::streamsize)out.size());
      printf("received %zu bytes\n", out.size());
    } else if (cmd == "launch") {
      Bytes req; Link::putStr(req, arg(1));
      l.call(11, req);
      printf("launching %s\n", arg(1).c_str());
    } else if (cmd == "rm") {
      Bytes req; Link::putStr(req, arg(1));
      l.call(7, req);
      printf("deleted %s\n", arg(1).c_str());
    } else {
      fprintf(stderr, "unknown command %s\n", cmd.c_str());
      return 2;
    }
  } catch (const std::exception& e) {
    fprintf(stderr, "pikolink: %s\n", e.what());
    return 1;
  }
}

# Host tools (no hardware, no Arduino).   make -C tools   builds build/pikolink
CXX ?= g++
CXXFLAGS := -std=c++17 -O1 -Wall -Wextra -I../src

all: build/pikolink

build/pikolink: pikolink.cpp ../src/util/crc32.h | build
	$(CXX) $(CXXFLAGS) $< -o $@

build:
	mkdir -p build

clean:
	rm -rf build
.PHONY: all clean

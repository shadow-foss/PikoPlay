// Shapes shared by the case parts.

// Rounded rectangle, centred, given its half size and corner radius.
module rrect(half, r) {
    offset(r = r) square(2 * (half - [r, r]), center = true);
}

// Rounded-rectangle slab from z0 to z1 with a rounded top edge (rt) and/or bottom edge (rb).
// The round turns into a 45 degree chamfer, so the edge prints face down without supports.
module slab(half, r, z0, z1, rt = 0, rb = 0, steps = 8) {
    function profile(rr) = concat(
        [for (i = [0:steps]) let(a = 45 * i / steps) [rr - rr * cos(a), rr * sin(a)]],
        [[2 * rr * (1 - cos(45)), rr]]);
    hull() {
        if (rt > 0) for (q = profile(rt))
            translate([0, 0, z1 - rt + q[1] - eps]) linear_extrude(eps) rrect(half - [q[0], q[0]], max(r - q[0], 0.5));
        if (rb > 0) for (q = profile(rb))
            translate([0, 0, z0 + rb - q[1]]) linear_extrude(eps) rrect(half - [q[0], q[0]], max(r - q[0], 0.5));
        if (rt == 0) translate([0, 0, z1 - eps]) linear_extrude(eps) rrect(half, r);
        if (rb == 0) translate([0, 0, z0]) linear_extrude(eps) rrect(half, r);
    }
}

// Vertical cylinder from z0 to z1 (optionally tapering to d2).
module zcyl(d, z0, z1, d2) {
    translate([0, 0, z0]) cylinder(d1 = d, d2 = is_undef(d2) ? d : d2, h = z1 - z0);
}

module pill2d(size) {
    hull() for (s = [-1, 1]) translate([s * (size[0] - size[1]) / 2, 0]) circle(d = size[1]);
}

module dpad2d() {
    offset(r = 1) offset(delta = -1) {
        square([dpad_arm[0], dpad_arm[1]], center = true);
        square([dpad_arm[1], dpad_arm[0]], center = true);
    }
}

// Outlines of all buttons in case coordinates, grown by `grow`.
module buttons2d(grow = 0) {
    translate(dpad_c) offset(delta = grow) dpad2d();
    for (p = abxy) translate(p) circle(d = abxy_d + 2 * grow);
    for (p = startsel) translate(p) offset(delta = grow) pill2d(pill);
}

// Turn a button upside down onto the print bed (its top face down).
module to_bed(top_z) {
    rotate([180, 0, 0]) translate([0, 0, -top_z]) children();
}

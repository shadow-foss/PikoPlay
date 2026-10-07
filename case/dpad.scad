// One-piece d-pad: rocks on a centre pin, with a post over each of its four switches.

include <config.scad>
include <shapes.scad>
use <buttons.scad>

module dpad() {
    translate(dpad_c) {
        difference() {
            cap(dpad_top) dpad2d();
            // arrow dimples
            for (a = [0:90:270]) rotate(a) translate([dpad_arm[0] / 2 - 4.5, 0, dpad_top - 0.4])
                linear_extrude(1) polygon([[1.2, 0], [-1, 1.6], [-1, -1.6]]);
            // centre dimple
            translate([0, 0, dpad_top - 0.6]) sphere(d = 5, $fn = 32);
        }
        // pivot pin
        zcyl(2.6, pcb_top + 0.25, cap_bot + eps);
        // posts onto the switches
        for (p = dpad_sw) translate(p) stem();
    }
}

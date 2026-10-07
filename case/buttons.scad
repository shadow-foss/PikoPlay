// Button caps. They are modelled where they sit in the case; to_bed() flips them for printing.

include <config.scad>
include <shapes.scad>

// Generic cap from a 2D outline (the child): body with a 0.5 mm top chamfer, and a retaining
// flange under the front plate with a 45 degree top so it prints without supports.
// The chamfers are built from thin offset steps so concave outlines (the d-pad cross) work too.
module cap(top_z) {
    n = 5;
    translate([0, 0, cap_bot]) linear_extrude(top_z - cap_bot - 0.5 + eps) children(0);
    for (i = [1:n]) translate([0, 0, top_z - 0.5 + (i - 1) * 0.5 / n])
        linear_extrude(0.5 / n + eps) offset(delta = -i * 0.5 / n) children(0);
    translate([0, 0, cap_bot]) linear_extrude(flange_top - flange_w - cap_bot + eps) offset(delta = flange_w) children(0);
    for (i = [1:4 * n]) translate([0, 0, flange_top - flange_w + (i - 1) * flange_w / (4 * n)])
        linear_extrude(flange_w / (4 * n) + eps) offset(delta = flange_w * (1 - i / (4 * n))) children(0);
}

// Post from the underside of a cap down to its switch.
module stem() {
    translate([0, 0, cap_bot - btn_stem]) cylinder(d = btn_stem_d, h = btn_stem + eps);
}

// Tab on the flange of a round cap that stops it from turning.
module abxy_key() {
    r0 = abxy_d / 2 + flange_w;
    hull() {
        translate([r0 - 0.5, -key_w / 2, cap_bot]) cube([key_l + 0.5, key_w, flange_top - flange_w - cap_bot]);
        translate([r0 - 0.5, -key_w / 2, min(flange_top - flange_w + key_l, flange_stop - 0.25) - eps]) cube([0.5, key_w, eps]);
    }
}

// One of the four face buttons (0 X, 1 B, 2 Y, 3 A), with its letter engraved on top.
module abxy_button(i) {
    translate(abxy[i]) difference() {
        union() {
            cap(cap_top) circle(d = abxy_d);
            rotate(abxy_key_dir[i]) abxy_key();
            stem();
        }
        translate([0, 0, cap_top - text_depth]) linear_extrude(1)
            text(abxy_labels[i], size = abxy_label_size, font = font, halign = "center", valign = "center");
    }
}

module abxy_buttons() {
    for (i = [0:len(abxy) - 1]) abxy_button(i);
}

module startsel_buttons() {
    for (i = [0:len(startsel) - 1]) translate(startsel[i]) difference() {
        union() {
            cap(ss_top) pill2d(pill);
            stem();
        }
        translate([0, 0, ss_top - text_depth]) linear_extrude(1)
            text(ss_labels[i], size = ss_label_size, font = font, halign = "center", valign = "center");
    }
}

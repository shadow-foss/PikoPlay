// Back lid: four M3 screws hold it to the shell; corner ribs hold the battery.

include <config.scad>
include <shapes.scad>

module lid() {
    lip_out = cav_half - [lip_gap, lip_gap];
    difference() {
        union() {
            slab(out_half, out_r, z_back, z_split, rb = back_round);
            // locating lip that fits inside the shell
            translate([0, 0, z_split - eps]) linear_extrude(lip_h) difference() {
                rrect(lip_out, cav_r - lip_gap);
                rrect(lip_out - [lip_t, lip_t], cav_r - lip_gap - lip_t);
                translate([usb_c[0], -cav_half[1]]) square([usb_cut[0] + 1, 6], center = true);
            }
            // screw bosses
            intersection() {
                for (p = m3_pos) translate(p) zcyl(m3_boss_d, z_split - eps, z_meet - 0.1);
                translate([0, 0, z_split - 1]) linear_extrude(z_meet - z_split + 2) rrect(lip_out, cav_r - lip_gap);
            }
            // battery corner ribs
            translate([bat_pos[0], bat_pos[1], z_split - eps]) linear_extrude(bat_rib_h) difference() {
                offset(delta = 1.2) square(bat_size + [0.6, 0.6], center = true);
                square(bat_size + [0.6, 0.6], center = true);
                square([bat_size[0] - 12, 100], center = true);
                square([100, bat_size[1] - 12], center = true);
            }
        }
        // screw holes and head recesses
        for (p = m3_pos) translate(p) {
            zcyl(m3_clear_d, z_back - eps, z_meet + eps);
            if (m3_head == "cap") {
                zcyl(m3_head_d, z_back - eps, z_back + m3_cbore);
                // two thin steps so the recess ceiling prints as short bridges
                translate([-m3_clear_d / 2, -m3_head_d / 2, z_back + m3_cbore - eps]) cube([m3_clear_d, m3_head_d, 0.2 + eps]);
                translate([-m3_clear_d / 2, -m3_clear_d / 2, z_back + m3_cbore + 0.2 - eps]) cube([m3_clear_d, m3_clear_d, 0.2 + eps]);
            } else {
                zcyl(m3_head_d, z_back - eps, z_back + (m3_head_d - m3_clear_d) / 2, m3_clear_d);
            }
        }
    }
}

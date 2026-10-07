// Front shell: the PCB screws into it; it holds the screen window and the button holes.
// Print it face down.

include <config.scad>
include <shapes.scad>

module shell() {
    difference() {
        union() {
            // hollow body
            difference() {
                slab(out_half, out_r, z_split, z_front, rt = front_round);
                translate([0, 0, z_split - 1]) linear_extrude(z_fi - z_split + 1) rrect(cav_half, cav_r);
            }
            // PCB bosses, each with a web to the nearest long wall
            for (p = pcb_holes) translate(p) {
                zcyl(m25_boss_d, pcb_top, z_fi + eps);
                web = cav_half[1] - abs(p[1]) + 0.5;
                translate([-1, p[1] > 0 ? 0 : -web, pcb_top + 1.5]) cube([2, web, z_fi - pcb_top - 1.5 + eps]);
            }
            // guide sleeves around every button, cut back around the display
            difference() {
                translate([0, 0, z_fi - sleeve_len]) linear_extrude(sleeve_len + eps) difference() {
                    buttons2d(flange_w + sleeve_clear + sleeve_wall);
                    buttons2d(flange_w + sleeve_clear);
                }
                for (b = lcd_keepout) translate(b[0]) cube(b[1] - b[0]);
                // slots for the ABXY anti-rotation tabs
                for (i = [0:len(abxy) - 1]) translate(abxy[i]) rotate(abxy_key_dir[i])
                    translate([abxy_d / 2, -(key_w + 2 * key_clear) / 2, z_fi - sleeve_len - 1])
                        cube([flange_w + sleeve_clear + sleeve_wall + 1, key_w + 2 * key_clear, sleeve_len + 1]);
            }
            // columns for the lid screws
            for (p = m3_pos) translate(p) zcyl(m3_boss_d, z_meet, z_fi + eps);
        }

        // M2.5 inserts, pressed in from the PCB side
        for (p = pcb_holes) translate(p) zcyl(m25_insert_d, pcb_top - eps, pcb_top + m25_insert_l + 0.3);
        // M3 inserts, pressed in from the open back, plus room for the screw tip
        for (p = m3_pos) translate(p) {
            zcyl(m3_insert_d, z_meet - eps, z_meet + m3_insert_l + 0.3);
            zcyl(2.6, z_meet, max(m3_tip_z, z_meet + m3_insert_l) + 1);
        }
        // screen opening, with 45 degree sides
        translate(lcd_aa_c) hull() {
            translate([0, 0, z_front - eps]) linear_extrude(1) rrect(lcd_open / 2, lcd_open_r);
            translate([0, 0, z_fi - eps]) linear_extrude(eps)
                rrect(lcd_open / 2 - [front_t, front_t], lcd_open_r - front_t);
            translate([0, 0, z_fi - 1]) linear_extrude(eps)
                rrect(lcd_open / 2 - [front_t + 1, front_t + 1], max(lcd_open_r - front_t - 1, 0.1));
        }
        // button holes
        translate([0, 0, z_fi - 1]) linear_extrude(front_t + 5) buttons2d(btn_clear);
        // USB-C
        translate([usb_c[0], -out_half[1] - 1, usb_c[1]]) rotate([-90, 0, 0]) linear_extrude(wall + 3) pill2d(usb_cut);
        // microSD slot and finger scoop
        translate([sd_x[0], cav_half[1] - 1, sd_z[0]]) cube([sd_x[1] - sd_x[0], wall + 2, sd_z[1] - sd_z[0]]);
        translate([(sd_x[0] + sd_x[1]) / 2, out_half[1] + sd_scoop - 1.4, (sd_z[0] + sd_z[1]) / 2])
            rotate([0, 90, 0]) cylinder(r = sd_scoop, h = sd_x[1] - sd_x[0] + 5, center = true);
        // power switch
        translate([sw_x - sw_hole[0] / 2 - tol_sw, cav_half[1] - 1, sw_z - sw_hole[1] / 2 - tol_sw])
            cube([sw_hole[0] + 2 * tol_sw, wall + 2, sw_hole[1] + 2 * tol_sw]);
        // logo
        translate([logo_pos[0], logo_pos[1], z_front - text_depth]) linear_extrude(1)
            text(logo_text, size = logo_size, font = font, halign = "center", valign = "center");
    }
}

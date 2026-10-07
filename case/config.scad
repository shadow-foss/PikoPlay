// PikoPlay case: every measurement in one place (millimetres).
//
// Coordinates match the KiCad board: the origin is the centre of the 130 x 60 PCB, z = 0 is the
// middle of the PCB, +z points out of the front (screen side), +y is the top edge (microSD side).

$fn = 48;
eps = 0.01;

// ---- Tolerances
tol_lcd_z = 0.5;    // air above the screen glass
tol_btn_z = 0.05;   // button flange to front plate: the up/down play of every button
tol_sd    = 1.3;    // microSD slot: +/- this around the card
tol_sw    = 0.2;    // power switch hole, added per side

// ---- Main PCB
pcb_size  = [130, 60];
pcb_t     = 1.6;
pcb_top   = pcb_t / 2;
pcb_bot   = -pcb_t / 2;
pcb_clear = 0.4;    // PCB edge to case wall
pcb_holes = [[-61.75, 27], [61.75, 27], [-61.75, -27], [61.75, -27]];   // M2.5

// ---- Buttons on the PCB (6 x 6 tactile switches)
btn_h     = 5.0;    // PCB top to the top of a switch plunger
btn_top_z = pcb_top + btn_h;
dpad_c    = [-49.656, 3.7];   // centre of the four d-pad switches
dpad_sw   = [[0, 9.5], [0, -9.5], [-10, 0], [10, 0]];   // relative to dpad_c
abxy      = [[40.344, 3.7], [60.344, 3.7], [50.344, 13.2], [50.344, -5.8]];   // X B Y A
startsel  = [[-20.7, -23.844], [19.8, -23.844]];                              // SELECT START

// ---- Boards mounted on headers
lcd_pcb_h = 6.5;    // main PCB top to the top of the display module's PCB
mcu_pcb_h = 4.5;    // main PCB back to the outside of the RP2040 module
mcu_pcb_t = 1.55;
lcd_lift  = lcd_pcb_h - 4.35;
mcu_drop  = (mcu_pcb_h - mcu_pcb_t) - 0.28;
mcu_low_z = -6.91 - mcu_drop;   // lowest part on the RP2040 module

// ---- Screen (2.0" ILI9225, landscape)
lcd_top_z   = 7.97 + lcd_lift;   // top of the glass
lcd_aa      = [39.6, 31.68];     // visible area
lcd_aa_c    = [-4.04, 8.17];     // its centre
lcd_open    = [52, 40.5];        // screen opening
lcd_open_r  = 3;

// ---- Openings
usb_c    = [-0.18, -4.3 - mcu_drop];   // USB-C centre [x, z]
usb_cut  = [12.4, 6.2];
sd_x     = [7.2, 20.2];                // microSD slot
sd_z     = [2.0 + lcd_lift - tol_sd, 2.0 + lcd_lift + tol_sd];
sd_scoop = 5;                          // finger scoop radius
sw_x     = 42;                         // power switch hole (top edge)
sw_hole  = [11, 5];
sw_z     = -4.9;

// ---- Case body
wall        = 2.2;
front_t     = 1.8;
z_fi        = lcd_top_z + tol_lcd_z;   // inside of the front plate
z_front     = z_fi + front_t;          // outside of the front plate
z_split     = min(-10.6, mcu_low_z - 0.5, usb_c[1] - usb_cut[1]/2 - 0.6);   // shell / lid joint
lid_t       = 1.8;
z_back      = z_split - lid_t;
front_round = 3.0;
back_round  = 1.5;
cav_half    = [72.0, pcb_size[1]/2 + pcb_clear];   // inside, half size
out_half    = cav_half + [wall, wall];             // outside, half size
out_r       = 8;
cav_r       = out_r - wall;

// ---- Lid fit
lip_h   = 1.4;
lip_t   = 1.2;
lip_gap = 0.2;

// ---- Lid screws: 4x M3 x 8 from the back into M3 x 5 heat-set inserts
m3_screw_l  = 8;
m3_head     = "cap";      // "cap" (counterbored) or "csk" (countersunk)
m3_head_d   = 6.2;
m3_cbore    = 2.0;
m3_insert_d = 4.0;
m3_insert_l = 5;
m3_clear_d  = 3.4;
m3_boss_d   = m3_insert_d + 2.4;
m3_pos_x    = 66.1 + m3_boss_d/2;
m3_pos      = [for (sx = [-1, 1], sy = [-1, 1]) [sx * m3_pos_x, sy * 24]];
lid_boss_h  = 2.0;
z_meet      = z_split + lid_boss_h;   // where the lid bosses meet the shell columns
m3_seat_z   = m3_head == "cap" ? z_back + m3_cbore : z_back;
m3_tip_z    = m3_seat_z + m3_screw_l;

// ---- PCB screws: 4x M2.5 x 5 from the back of the PCB into M2.5 x 5 heat-set inserts
m25_insert_d = 3.3;
m25_insert_l = 5;
m25_boss_d   = 6.0;

// ---- Battery (LiPo 34 x 50 x 7), held by corner ribs on the lid
bat_size  = [34, 50];
bat_t     = 7;
bat_pos   = [-48.6, 0];
bat_rib_h = 2.0;

// ---- Button caps
btn_clear    = 0.25;   // cap body to front plate hole, per side
sleeve_clear = 0.18;   // cap flange to guide sleeve, per side
sleeve_len   = 5;
sleeve_wall  = 1.0;
flange_w     = 1.2;
cap_bot      = btn_top_z + 0.02;
btn_stem     = 2.1 - 0.7 - 0.05;   // post from the cap down to the switch plunger
btn_stem_d   = 4.0;
flange_top   = z_fi - tol_btn_z + btn_clear;
flange_stop  = flange_top - btn_clear;
abxy_d       = 9;
pill         = [15, 6];      // START / SELECT
dpad_arm     = [28, 8.5];    // d-pad span and arm width
cap_top      = z_front + 3.0;   // ABXY top
dpad_top     = z_front + 2.5;
ss_top       = z_front + 1.8;   // START / SELECT top

// Anti-rotation tab on each ABXY cap, riding in a slot in its sleeve
abxy_key_dir = [180, 0, 90, 270];
key_w        = 1.6;
key_l        = 1.0;
key_clear    = 0.18;

// Keep the button sleeves away from the display module
lcd_keepout = [[[-33.9, -13.36, 0], [33.9, 29.7, 5.5 + lcd_lift]],
               [[-28.4, -12.2, 0], [26.45, 28.6, z_fi]]];

// ---- Text
font        = "DejaVu Sans:style=Bold";
logo_text   = "PIKOPLAY";
logo_size   = 5.212;           // about 35 mm wide
logo_pos    = [50.344, -19];   // right side, under the ABXY buttons
abxy_labels = ["X", "B", "Y", "A"];
abxy_label_size = 3.514;
ss_labels   = ["SELECT", "START"];
ss_label_size   = 2.352;
text_depth  = 0.6;

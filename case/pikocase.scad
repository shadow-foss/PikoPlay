// PikoPlay case. Open this file in OpenSCAD and pick a part below, or export from the command line:
//   openscad -D 'part="shell"' -o stl/pikocase_top.stl pikocase.scad
//
// Parts:
//   "assembly"  everything in place (preview only)
//   "shell"     front shell, print face down
//   "lid"       back lid
//   "dpad"      d-pad, print top down
//   "abxy"      the four face buttons, print top down
//   "A" "B" "X" "Y"  one face button (to print each in its own colour)
//   "startsel"  START and SELECT, print top down
//
// Hardware: the PCB mounts with 4x M2.5 x 5 screws into M2.5 x 5 brass heat-set inserts; the lid
// closes with 4x M3 x 8 screws into M3 x 5 brass heat-set inserts. Battery: 34 x 50 x 7 mm LiPo.

include <config.scad>
include <shapes.scad>
use <shell.scad>
use <lid.scad>
use <buttons.scad>
use <dpad.scad>

part = "assembly";

if (part == "assembly") {
    color("LightSteelBlue") shell();
    color("DimGray") lid();
    color("#202830") translate([lcd_aa_c[0], lcd_aa_c[1], lcd_top_z - 1]) cube([lcd_open[0] - 2, lcd_open[1] - 2, 2], center = true);   // the screen
    color("Crimson") { dpad(); abxy_buttons(); startsel_buttons(); }
    color("Gold") translate([bat_pos[0], bat_pos[1], z_split + bat_t / 2]) cube([bat_size[0], bat_size[1], bat_t], center = true);
}
if (part == "shell")    rotate([180, 0, 0]) translate([0, 0, -z_front]) shell();
if (part == "lid")      translate([0, 0, -z_back]) lid();
if (part == "dpad")     to_bed(dpad_top) translate(-[dpad_c[0], dpad_c[1], 0]) dpad();
if (part == "abxy")     to_bed(cap_top) abxy_buttons();
if (part == "startsel") to_bed(ss_top) startsel_buttons();
for (i = [0:len(abxy) - 1])
    if (part == abxy_labels[i]) to_bed(cap_top) translate(-[abxy[i][0], abxy[i][1], 0]) abxy_button(i);

# Local changes to walnut_cgb.h (upstream: github.com/Mr-PauI/Walnut-CGB, MIT)

1. `WALNUT_HOT` (empty by default) prefixes the definitions of the hot functions:
   `__gb_read*`, `__gb_write*`, `__gb_execute_cb`, `__gb_draw_line`, `gb_run_frame`,
   `__gb_step_cpu_x`. PikoPlay defines it to put them in the RP2040's RAM (`.time_critical`),
   because executing them from external flash through the 16 KB XIP cache was ~2x slower.

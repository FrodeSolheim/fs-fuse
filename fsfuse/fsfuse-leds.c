#include "fsfuse.h"

static struct {
    fsemu_led_t *power_led;
    fsemu_led_t *floppy_led;
    fsemu_led_t *tape_led;
} fsfuse;

void fsfuse_leds_update(void)
{
    int disk_state;
    int mdr_state;  // ZX Microdrive
    int tape_state;
    fusefs_media_state(&disk_state, &mdr_state, &tape_state);
    // State value 0 means icon disabled, 1 is off and 2 is on
    fsemu_led_set_state(fsfuse.power_led, 1);
    fsemu_led_set_state(fsfuse.floppy_led, disk_state == 2);
    fsemu_led_set_state(fsfuse.tape_led, tape_state == 2);
}

void fsfuse_leds_init(void)
{
    // The power lED is quite useless right now, and only has an estetic
    // function...
    fsfuse.power_led = fsemu_led_create();
    fsemu_led_set_id(fsfuse.power_led, "Power");
    fsemu_led_set_label(fsfuse.power_led, "POWER");
    fsemu_leds_add_led(fsfuse.power_led);
    fsfuse.floppy_led = fsemu_led_create();
    fsemu_led_set_id(fsfuse.floppy_led, "Floppy");
    fsemu_led_set_label(fsfuse.floppy_led, "FLOPPY");
    fsemu_leds_add_led(fsfuse.floppy_led);
    fsfuse.tape_led = fsemu_led_create();
    fsemu_led_set_id(fsfuse.tape_led, "Tape");
    fsemu_led_set_label(fsfuse.tape_led, "TAPE");
    fsemu_leds_add_led(fsfuse.tape_led);
}

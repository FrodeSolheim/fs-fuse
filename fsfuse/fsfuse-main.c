#include <stdio.h>

#include "fsemu/src/fsemu-all.h"
#include "fsfuse.h"
#include "fsfuse-media.h"

static struct {
    int argc;
    char **argv;
} fsfuse;

void fsfuse_emulation_thread(void)
{
    printf("fuse_emulation_thread\n");
    // fuse_main(fsfuse.argc, fsfuse.argv);
# if 0
    if (SDL_SetThreadPriority(SDL_THREAD_PRIORITY_TIME_CRITICAL) != 0) {

    } else {
        printf("ERROR: Could not set thread priority\n");
        printf("%s\n", SDL_GetError());
    }
#endif

    int argc;
    char **argv;
    fsfuse_config_args(&argc, &argv);
    fuse_main(argc, argv);
}

int main(int argc, char **argv)
{
    // fsemu_set_startup_time();
    fsemu_boot_log("main");

    fsfuse.argc = argc;
    fsfuse.argv = argv;
    fprintf(stderr, "FS-Fuse\n");

    fsemu_set_emulator_name("FS-Fuse");
    fsemu_set_video_format_flags(FSEMU_VIDEO_FORMAT_FLAG_RGB565);

    // fsemu_init_arguments();
    // fsemu_load_config_file();

    // fsemu_init();
    fsemu_set_config_file_extension(".fs-fuse");

    // FIXME: Init less in fsemu_init; so we can get the window appear even
    // faster. Actually, fsemu_init shows the window quite early by itself.

    int64_t t0 = fsemu_time_us();
    // FIXME: Windows support and unicode to UTF-8 translation...!
    fsemu_init_with_args(argc, argv);
    int64_t t1 = fsemu_time_us();
    printf("t1 - t0 = %d\n", (int) ((t1 - t0) / 1000));

    fsemu_begin();

    int64_t t2 = fsemu_time_us();

    fsfuse_action_init();
    fsfuse_config_init();
    fsfuse_input_init();
    fsfuse_keyboard_init();
    fsfuse_leds_init();
    fsfuse_media_init();
    fsfuse_oskeyboard_init();

    // FIXME: MOVE SOMEWHERE ELSE
    fsemu_input_autofill_devices();
    // FIXME: MOVE SOMEWHERE ELSE
    fsemu_input_reconfigure();

    int64_t t3 = fsemu_time_us();
    printf("t3 - t2 = %d\n", (int) ((t3 - t2) / 1000));

    // fsemu_start();

    fsemu_run(fsfuse_emulation_thread);

    while (fsemu_running()) {
        fsfuse_leds_update();
        fsemu_update();
        fsemu_handle_events();
    }

    // fsemu_end();
    // fsemu_stop();
    // fsemu_quit();
    fsemu_end();
}

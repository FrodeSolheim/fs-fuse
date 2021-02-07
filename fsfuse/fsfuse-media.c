#define FSFUSE_INTERNAL
#include "fsfuse-media.h"

#include "fsfuse.h"

// #define MAX_FLOPPY_DRIVES 4
// #define MAX_CDROM_DRIVES 1

static struct {
    bool initialized;
    // int floppy_drive_count;
    // fsemu_media_drive_t *floppy_drives[MAX_FLOPPY_DRIVES];
    // int cdrom_drive_count;
    // fsemu_media_drive_t *cdrom_drives[MAX_CDROM_DRIVES];
    // // The index for the next configured HFx device.
    // int hf_index;
    fsemu_media_drive_t *tape_drive;
    char *current_tape_path;
} fsfuse_media;

void fsfuse_media_on_tape_open(const char *path)
{
    // FIXME: Maybe handle this in FSEMU?
    // fsemu_media_drive_on_file_opened() ?
    // Well, basically, that's fsemu_media_drive_set_file
    printf("On tape open: %s\n", path);
    fsfuse_media.current_tape_path = strdup(path);

    fsemu_media_drive_set_file(fsfuse_media.tape_drive, path);
}

void fsfuse_media_on_tape_close(void)
{
    // FIXME: Maybe handle this in FSEMU?
    // fsemu_media_drive_on_file_closed() ?
    printf("On tape close\n");
    if (fsfuse_media.current_tape_path != NULL) {
        free(fsfuse_media.current_tape_path);
        fsfuse_media.current_tape_path = NULL;
    }

    fsemu_media_drive_set_file(fsfuse_media.tape_drive, NULL);
}

static void fsfuse_media_init_tape_drive(void)
{
    fsfuse_log("[MEDIA] Init tape drive\n");

    // for (int i = 0; i < MAX_FLOPPY_DRIVES; i++) {
    //     option_floppy_drive_x[13] = '0' + i;
    //     option_floppy_drive_x_sounds[13] = '0' + i;
    //     option_floppyx[6] = '0' + i;
    //     option_floppyxtype[6] = '0' + i;
    //     option_floppyxsound[6] = '0' + i;
    //     option_floppyxsoundext[6] = '0' + i;

    //     char *path = fs_config_get_string(option_floppy_drive_x);
    //     fsuae_log(
    //         "[MEDIA] Value for option %s: %s\n", option_floppy_drive_x,
    //         path);
    //     if (!path) {
    //         path = strdup("");
    //     }

    //     if (path[0] != '\0') {
    //         if (g_str_has_prefix(path, "dat://")) {
    //         } else {
    //             path = fsuae_path_expand_and_free(path);
    //             path = fsuae_path_resolve_and_free(path,
    //             FS_UAE_FLOPPY_PATHS); if (!g_file_test(path,
    //             G_FILE_TEST_EXISTS)) {
    //                 fs_emu_warning(
    //                     "Not found: %s",
    //                     fs_config_get_const_string(option_floppy_drive_x));
    //             }
    //         }
    //         auto_num_drives = MAX(auto_num_drives, i + 1);
    //     }
    //     amiga_set_option(option_floppyx, path);
    //     // if (path[0] != '\0') {
    //     floppypaths[i] = path;
    //     // }
    //     amiga_set_int_option(option_floppyxtype,
    //                          cfg->default_floppy_drive_type);
    //     // Free later
    //     // free(path);

    //     const char *floppy_sounds =
    //         fs_config_get_const_string(option_floppy_drive_x_sounds);
    //     if (floppy_sounds) {
    //         fsuae_log(
    //             "custom floppy sounds for drive %d: %s\n", i,
    //             floppy_sounds);
    //         amiga_set_option(option_floppyxsound, "-1");
    //         amiga_set_option(option_floppyxsoundext, floppy_sounds);
    //     }
    // }
    // const char *value;
    // value = fs_config_get_const_string("floppy_drive_speed");
    // if (value) {
    //     amiga_set_option("floppy_speed", value);
    // }

    // int num_drives = auto_num_drives;
    // value = fs_config_get_const_string("floppy_drive_count");
    // if (value) {
    //     if (g_ascii_strcasecmp(value, "auto") == 0) {
    //         // auto
    //     } else {
    //         num_drives = atoi(value);
    //     }
    // }

    int tape_count = 0;
    int k = 0;
    while (tape_count < 20) {
        char *config_key = g_strdup_printf("tape_image_%d", k);
        const char *config_value = fsemu_option_const_string(config_key);
        if (config_value) {
            // FIXME: Add before or after expand/resolve?

            // FIXME: Resolve to full path here?
            char *path = strdup(config_value);

            fsemu_media_add_file(FSEMU_MEDIA_DRIVE_TYPE_TAPE, path);
            free(path);

            tape_count++;
        }
        free(config_key);
        if (k > 0 && !config_value) {
            // Allow to start at floppy_image_0 or floppy_image_1
            // FIXME: Hmm...
            break;
        }
        k++;
    }
    // while (tape_count < 20) {
    //     // Set the remaining floppy list entries to empty strings.
    //     char *option = g_strdup_printf("diskimage%d", tape_count);
    //     amiga_set_option(option, "");
    //     free(option);
    //     tape_count++;
    // }

    // Now we create the FSEMU drive entries and add file paths.
    // module.floppy_drive_count = num_drives;
    // for (int i = 0; i < module.floppy_drive_count; i++) {
    fsemu_media_drive_t *drive = fsfuse_media.tape_drive =
        fsemu_media_drive_new_with_type(FSEMU_MEDIA_DRIVE_TYPE_TAPE);

    fsemu_media_drive_set_name(drive, "TAP");  // ? Used for ?
    fsemu_media_drive_set_title(drive, "Tape drive");
#if 1
#warning Not setting media drive file right now
#else
    // FIXME: Add before or after expand/resolve?
    if (floppypaths[i][0] != '\0') {
        fsemu_media_drive_set_file(drive, floppypaths[i]);
    }
#endif
    fsemu_media_add_drive(drive);
}

// ----------------------------------------------------------------------------

void fsfuse_media_init(void)
{
    if (fsfuse_media.initialized) {
        return;
    }
    fsfuse_media.initialized = true;

    fsemu_media_init();
    // fsuae_path_init();

    // FIXME: amiga_set_option might have to queue options until UAE has passed
    // initial init, and then apply them. Otherwise, the values might be
    // overwritten.

    fsfuse_media_init_tape_drive();

    // fsuae_media_init_floppy_drives();
    // fsuae_media_configure_cdrom();
    // fsuae_media_configure_hard_drives();
}

#ifndef FSFUSE_FSFUSE_H_
#define FSFUSE_FSFUSE_H_

#include "fsemu/src/fsemu-all.h"
#include "fsfuse-action.h"
#include "fsfuse-options.h"

#include <libxml/parser.h>
#include <libxml/xmlmemory.h>

#define FSEMU2 1

#define fsfuse_log fsemu_log
#define fsfuse_log_warning fsemu_log_warning
#define fsfuse_log_error fsemu_log_error

// ----------------------------------------------------------------------------

void fsfuse_config_init(void);
void fsfuse_input_init(void);
void fsfuse_keyboard_init(void);
void fsfuse_leds_init(void);
void fsfuse_oskeyboard_init(void);

void fsfuse_leds_update(void);

// ----------------------------------------------------------------------------

void fsfuse_config_args(int *argc, char ***argv);

// ----------------------------------------------------------------------------

// Original main function of Fuse.
int fuse_main(int argc, char **argv);

xmlDoc *fsfuse_config_create_settings_xml(void);

// Retrieve media state from sdldisplay module.
void fusefs_media_state(int *disk_state, int *mdr_state, int *tape_state);

// ----------------------------------------------------------------------------

#endif  // FSFUSE_FSFUSE_H_

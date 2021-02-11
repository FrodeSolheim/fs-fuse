#include "fsfuse.h"

static struct {
    fsemu_inputport_t *port_a;
    fsemu_inputport_t *port_b;
    // int selected_state_slot;
} fsuae_inputport;

// #define OPTION_ZXS_PORT_A "zxs_port_a"
#define OPTION_SPECTRUM_PORT_1 "spectrum_port_1"
#define OPTION_SPECTRUM_PORT_2 "spectrum_port_2"

#define MAP(input, output) \
    fsemu_inputmode_map(mode, FSEMU_INPUTDEVICE_##input, output)

enum {
    FSFUSE_INPUT_PORT_MODE_NONE,
    FSFUSE_INPUT_PORT_MODE_KEMPSTON_JOYSTICK,
    FSFUSE_INPUT_PORT_MODE_SINCLAR_JOYSTICK,
};

static void add_none_mode(fsemu_inputport_t *port, int num)
{
    fsemu_inputmode_t *mode = fsemu_inputmode_new();
    // FIXME: Do we want to map defaults here?
    // fsemu_inputmode_map_defaults(mode);
    fsemu_inputmode_set_config_name(mode, "none");
    fsemu_inputmode_set_title(mode, "None");
    fsemu_inputport_add_mode(port, mode);
}

static void add_joystick_mode(fsemu_inputport_t *port, int num, int type)
{
    fsemu_inputmode_t *mode = fsemu_inputmode_new();

    fsemu_inputmode_map_defaults(mode);
    // FIXME: What was the difference between name and title?
    if (type == FSFUSE_INPUT_PORT_MODE_KEMPSTON_JOYSTICK) {
        fsemu_inputmode_set_config_name(mode, "kempston");
        fsemu_inputmode_set_title(mode, "Kempston joystick");
    } else {
        fsemu_inputmode_set_config_name(mode, "sinclair");
        fsemu_inputmode_set_title(mode, "Sinclair joystick");
    }
    fsemu_inputport_add_mode(port, mode);

    // Controller mapping
    if (num == 0) {
        MAP(DPUP, FSFUSE_ACTION_JOYSTICK1_UP);
        MAP(DPRIGHT, FSFUSE_ACTION_JOYSTICK1_RIGHT);
        MAP(DPDOWN, FSFUSE_ACTION_JOYSTICK1_DOWN);
        MAP(DPLEFT, FSFUSE_ACTION_JOYSTICK1_LEFT);
        MAP(BUTTON_A, FSFUSE_ACTION_JOYSTICK1_FIRE);
        MAP(BUTTON_B, FSFUSE_ACTION_JOYSTICK1_UP);
        MAP(BUTTON_X, FSFUSE_ACTION_JOYSTICK1_FIRE);
        MAP(BUTTON_Y, FSFUSE_ACTION_JOYSTICK1_FIRE);
    } else {
        MAP(DPUP, FSFUSE_ACTION_JOYSTICK2_UP);
        MAP(DPRIGHT, FSFUSE_ACTION_JOYSTICK2_RIGHT);
        MAP(DPDOWN, FSFUSE_ACTION_JOYSTICK2_DOWN);
        MAP(DPLEFT, FSFUSE_ACTION_JOYSTICK2_LEFT);
        MAP(BUTTON_A, FSFUSE_ACTION_JOYSTICK2_FIRE);
        MAP(BUTTON_B, FSFUSE_ACTION_JOYSTICK2_UP);
        MAP(BUTTON_X, FSFUSE_ACTION_JOYSTICK2_FIRE);
        MAP(BUTTON_Y, FSFUSE_ACTION_JOYSTICK2_FIRE);
    }
    // Keyboard mapping
    if (num == 0) {
        MAP(UP, FSFUSE_ACTION_JOYSTICK1_UP);
        MAP(RIGHT, FSFUSE_ACTION_JOYSTICK1_RIGHT);
        MAP(DOWN, FSFUSE_ACTION_JOYSTICK1_DOWN);
        MAP(LEFT, FSFUSE_ACTION_JOYSTICK1_LEFT);
        MAP(RCTRL, FSFUSE_ACTION_JOYSTICK1_FIRE);
        MAP(RALT, FSFUSE_ACTION_JOYSTICK1_FIRE);
    } else {
        MAP(UP, FSFUSE_ACTION_JOYSTICK2_UP);
        MAP(RIGHT, FSFUSE_ACTION_JOYSTICK2_RIGHT);
        MAP(DOWN, FSFUSE_ACTION_JOYSTICK2_DOWN);
        MAP(LEFT, FSFUSE_ACTION_JOYSTICK2_LEFT);
        MAP(RCTRL, FSFUSE_ACTION_JOYSTICK2_FIRE);
        MAP(RALT, FSFUSE_ACTION_JOYSTICK2_FIRE);
    }

    // FIXME: These should maybe be pre-initialized by FSEMU, unless
    // overridden?
    // EDIT: Done now
    // MAP(GUIDE, FSEMU_ACTION_OSMENU);
    // MAP(START, FSEMU_ACTION_OSMENU);
    // MAP(BACK, FSEMU_ACTION_OSKEYBOARD);
}

#if 0
static void fsfuse_map_custom_action(fsemu_inputmode_t *mode,
                                        const char *prefix,
                                        const char *suffix,
                                        int input_event)
{
    char *option_name = g_strdup_printf("%s_%s", prefix, suffix);
    printf("option: %s\n", option_name);
    const char *action_name = fsemu_option_const_string(option_name);
    printf("action: %s\n", action_name);
    if (action_name == NULL) {
        return;
    }
    int action = fsemu_action_from_name(action_name);
    printf("action: %d\n", action);
    fsemu_inputmode_map(mode, input_event, action);
}

static void fsfuse_map_custom_actions(fsemu_inputmode_t *mode,
                                      const char *prefix)
{
    fsfuse_map_custom_action(mode, prefix, "button_a", FSEMU_INPUTDEVICE_BUTTON_A);
    fsfuse_map_custom_action(mode, prefix, "button_b", FSEMU_INPUTDEVICE_BUTTON_B);
    fsfuse_map_custom_action(mode, prefix, "button_x", FSEMU_INPUTDEVICE_BUTTON_X);
    fsfuse_map_custom_action(mode, prefix, "button_y", FSEMU_INPUTDEVICE_BUTTON_Y);
    fsfuse_map_custom_action(
        mode, prefix, "button_back", FSEMU_INPUTDEVICE_BACK);
    fsfuse_map_custom_action(
        mode, prefix, "button_guide", FSEMU_INPUTDEVICE_GUIDE);
    fsfuse_map_custom_action(
        mode, prefix, "button_start", FSEMU_INPUTDEVICE_START);
    fsfuse_map_custom_action(
        mode, prefix, "button_leftstick", FSEMU_INPUTDEVICE_LEFTSTICK);
    fsfuse_map_custom_action(
        mode, prefix, "button_rightstick", FSEMU_INPUTDEVICE_RIGHTSTICK);
    fsfuse_map_custom_action(
        mode, prefix, "button_leftshoulder", FSEMU_INPUTDEVICE_LEFTSHOULDER);
    fsfuse_map_custom_action(
        mode, prefix, "button_rightshoulder", FSEMU_INPUTDEVICE_RIGHTSHOULDER);
    fsfuse_map_custom_action(
        mode, prefix, "button_dpup", FSEMU_INPUTDEVICE_DPUP);
    fsfuse_map_custom_action(
        mode, prefix, "button_dpdown", FSEMU_INPUTDEVICE_DPDOWN);
    fsfuse_map_custom_action(
        mode, prefix, "button_dpleft", FSEMU_INPUTDEVICE_DPLEFT);
    fsfuse_map_custom_action(
        mode, prefix, "button_dpright", FSEMU_INPUTDEVICE_DPRIGHT);
}
#endif

static void add_custom_keys_mode(fsemu_inputport_t *port, int num)
{
    fsemu_inputmode_t *mode = fsemu_inputmode_new();
    fsemu_inputmode_map_defaults(mode);
    fsemu_inputmode_set_config_name(mode, "custom");
    fsemu_inputmode_set_title(mode, "Custom keys");
    fsemu_inputport_add_mode(port, mode);
}

static void fsfuse_input_configure_port(fsemu_inputport_t *port, int num)
{
    if (num == 0) {
        fsemu_inputport_set_name(port, "Joystick Port");
        fsemu_inputport_set_config_name(port, OPTION_SPECTRUM_PORT_1);
        // base = ACTION_JOYSTICK1_BEFOREFIRST;
    } else {
        fsemu_inputport_set_name(port, "Joystick Port");
        fsemu_inputport_set_config_name(port, OPTION_SPECTRUM_PORT_2);
        // fsemu_inputport_set_name(port, "Joystick Port 0");
        // fsemu_inputport_set_config_name(port, OPTION_JOYSTICK_PORT_0);
        // base = ACTION_JOYSTICK2_BEFOREFIRST;
    }

    // These must be in the same order as FSUAE_JOYSTICK_PORT_MODE_NONE, ...
    add_none_mode(port, num);
    // add_mouse_mode(port, num);
    // FIXME: Only for port 1
    // FIXME: Support disabling mode in the UI?
    add_joystick_mode(port, num, FSFUSE_INPUT_PORT_MODE_KEMPSTON_JOYSTICK);
    add_joystick_mode(port, num, FSFUSE_INPUT_PORT_MODE_SINCLAR_JOYSTICK);
    add_custom_keys_mode(port, num);
    // add_cd32_mode(port, num);

    fsemu_input_add_port(port);
}

void fsfuse_input_init(void)
{
    // fsfuse_action_init();

    fsuae_inputport.port_a = fsemu_inputport_new();
    fsfuse_input_configure_port(fsuae_inputport.port_a, 0);

    fsuae_inputport.port_b = fsemu_inputport_new();
    fsfuse_input_configure_port(fsuae_inputport.port_b, 1);

    // fsuae_inputport.joystick_port_0 = fsemu_inputport_new();
    // configure_joystick_port(fsuae_inputport.joystick_port_0, 0);

    fsemu_inputport_set_mode_index(fsuae_inputport.port_a,
                                   FSFUSE_INPUT_PORT_MODE_KEMPSTON_JOYSTICK);

    // fsemu_inputport_set_mode_index(fsuae_inputport.joystick_port_0,
    //                                FSUAE_JOYSTICK_PORT_MODE_MOUSE);

    // int action = INPUTEVENT_AMIGA_JOYPORT_MODE_0_NONE + modes * i +
    //                 port->new_mode;
    // fs_emu_queue_action(ACTION_PORT0_MOUSEMODE, 1);
    // fs_emu_queue_action(ACTION_PORT0_MOUSEMODE, 0);
    // fsemu_action_queue(ACTION_PORT0_MOUSEMODE, 1)

    // FSEMU port 0 is Amiga port 1 - set to joystick mode (2) by default.

    int port_a_mode_index = 1;
    const char *port_a_mode_str =
        fsemu_option_const_string(OPTION_SPECTRUM_PORT_1_TYPE);
    if (port_a_mode_str != NULL) {
        if (strcmp(port_a_mode_str, "kempston") == 0) {
            port_a_mode_index = 1;
        } else if (strcmp(port_a_mode_str, "sinclair") == 0) {
            port_a_mode_index = 2;
        } else if (strcmp(port_a_mode_str, "custom") == 0) {
            port_a_mode_index = 3;
        } else {
            // fsfuse_log_warning("Unknown port type specified\n");
            port_a_mode_index = 0;
        }
    }

    int port_b_mode_index = 0;
    const char *port_b_mode_str =
        fsemu_option_const_string(OPTION_SPECTRUM_PORT_2_TYPE);
    if (port_b_mode_str != NULL) {
        if (strcmp(port_b_mode_str, "kempston") == 0) {
            port_b_mode_index = 1;
        } else if (strcmp(port_b_mode_str, "sinclair") == 0) {
            port_b_mode_index = 2;
        } else if (strcmp(port_b_mode_str, "custom") == 0) {
            port_b_mode_index = 3;
        } else {
            // fsfuse_log_warning("Unknown port type specified\n");
            port_b_mode_index = 0;
        }
    }

    // FIXME
    fsemu_input_process_action(FSEMU_ACTION_PORT0TYPE0 + port_a_mode_index, 1);
    // FSEMU port 1 is Amiga port 0 - set to mouse mode (1) by default.

    // FIXME
    fsemu_input_process_action(FSEMU_ACTION_PORT1TYPE0 + port_b_mode_index, 1);

    // FIXME: Remove
    // fsemu_input_process_action(ACTION_PORT0_MOUSEMODE, 1);
}

#include "fsfuse.h"

void fsfuse_action_init(void)
{
    fsemu_action_register("key_1", FSFUSE_ACTION_KEY_1);
    fsemu_action_register("key_2", FSFUSE_ACTION_KEY_2);
    fsemu_action_register("key_3", FSFUSE_ACTION_KEY_3);
    fsemu_action_register("key_4", FSFUSE_ACTION_KEY_4);
    fsemu_action_register("key_5", FSFUSE_ACTION_KEY_5);
    fsemu_action_register("key_6", FSFUSE_ACTION_KEY_6);
    fsemu_action_register("key_7", FSFUSE_ACTION_KEY_7);
    fsemu_action_register("key_8", FSFUSE_ACTION_KEY_8);
    fsemu_action_register("key_9", FSFUSE_ACTION_KEY_9);
    fsemu_action_register("key_0", FSFUSE_ACTION_KEY_0);

    fsemu_action_register("key_q", FSFUSE_ACTION_KEY_Q);
    fsemu_action_register("key_w", FSFUSE_ACTION_KEY_W);
    fsemu_action_register("key_e", FSFUSE_ACTION_KEY_E);
    fsemu_action_register("key_r", FSFUSE_ACTION_KEY_R);
    fsemu_action_register("key_t", FSFUSE_ACTION_KEY_T);
    fsemu_action_register("key_y", FSFUSE_ACTION_KEY_Y);
    fsemu_action_register("key_u", FSFUSE_ACTION_KEY_U);
    fsemu_action_register("key_i", FSFUSE_ACTION_KEY_I);
    fsemu_action_register("key_o", FSFUSE_ACTION_KEY_O);
    fsemu_action_register("key_p", FSFUSE_ACTION_KEY_P);

    fsemu_action_register("key_a", FSFUSE_ACTION_KEY_A);
    fsemu_action_register("key_s", FSFUSE_ACTION_KEY_S);
    fsemu_action_register("key_d", FSFUSE_ACTION_KEY_D);
    fsemu_action_register("key_f", FSFUSE_ACTION_KEY_F);
    fsemu_action_register("key_g", FSFUSE_ACTION_KEY_G);
    fsemu_action_register("key_h", FSFUSE_ACTION_KEY_H);
    fsemu_action_register("key_j", FSFUSE_ACTION_KEY_J);
    fsemu_action_register("key_k", FSFUSE_ACTION_KEY_K);
    fsemu_action_register("key_l", FSFUSE_ACTION_KEY_L);
    fsemu_action_register("key_enter", FSFUSE_ACTION_KEY_ENTER);

    fsemu_action_register("key_capsshift", FSFUSE_ACTION_KEY_CAPSSHIFT);
    fsemu_action_register("key_z", FSFUSE_ACTION_KEY_Z);
    fsemu_action_register("key_x", FSFUSE_ACTION_KEY_X);
    fsemu_action_register("key_c", FSFUSE_ACTION_KEY_C);
    fsemu_action_register("key_v", FSFUSE_ACTION_KEY_V);
    fsemu_action_register("key_b", FSFUSE_ACTION_KEY_B);
    fsemu_action_register("key_n", FSFUSE_ACTION_KEY_N);
    fsemu_action_register("key_m", FSFUSE_ACTION_KEY_M);
    fsemu_action_register("key_symbolshift", FSFUSE_ACTION_KEY_SYMBOLSHIFT);
    fsemu_action_register("key_space", FSFUSE_ACTION_KEY_SPACE);

    fsemu_action_register("key_up", FSFUSE_ACTION_KEY_UP);
    fsemu_action_register("key_down", FSFUSE_ACTION_KEY_DOWN);
    fsemu_action_register("key_left", FSFUSE_ACTION_KEY_LEFT);
    fsemu_action_register("key_right", FSFUSE_ACTION_KEY_RIGHT);

    fsemu_action_register("joystick1_up", FSFUSE_ACTION_JOYSTICK1_UP);
    fsemu_action_register("joystick1_down", FSFUSE_ACTION_JOYSTICK1_DOWN);
    fsemu_action_register("joystick1_left", FSFUSE_ACTION_JOYSTICK1_LEFT);
    fsemu_action_register("joystick1_right", FSFUSE_ACTION_JOYSTICK1_RIGHT);
    fsemu_action_register("joystick1_fire", FSFUSE_ACTION_JOYSTICK1_FIRE);

    fsemu_action_register("joystick2_up", FSFUSE_ACTION_JOYSTICK2_UP);
    fsemu_action_register("joystick2_down", FSFUSE_ACTION_JOYSTICK2_DOWN);
    fsemu_action_register("joystick2_left", FSFUSE_ACTION_JOYSTICK2_LEFT);
    fsemu_action_register("joystick2_right", FSFUSE_ACTION_JOYSTICK2_RIGHT);
    fsemu_action_register("joystick2_fire", FSFUSE_ACTION_JOYSTICK2_FIRE);
}
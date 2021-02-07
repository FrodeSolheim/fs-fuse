#include "fsfuse.h"
#include "fsfuse-action.h"

static void fsfuse_oskeyboard_configure_default(fsemu_oskeyboard_t *keyboard)
{
    fsemu_oskeyboard_row_t *row;
    fsemu_oskeyboard_key_t *key;

    row = fsemu_oskeyboard_new_row(keyboard);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "CAPSSHIFT");
    fsemu_oskeyboard_key_set_action(key, FSFUSE_ACTION_KEY_CAPSSHIFT);
    fsemu_oskeyboard_key_set_width(key, 200);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "Z");
    fsemu_oskeyboard_key_set_action(key, FSFUSE_ACTION_KEY_Z);
    fsemu_oskeyboard_key_set_left_margin(key, 67);
    fsemu_oskeyboard_key_set_width(key, 133);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "X");
    fsemu_oskeyboard_key_set_action(key, FSFUSE_ACTION_KEY_X);
    fsemu_oskeyboard_key_set_left_margin(key, 67);
    fsemu_oskeyboard_key_set_width(key, 133);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "C");
    fsemu_oskeyboard_key_set_action(key, FSFUSE_ACTION_KEY_C);
    fsemu_oskeyboard_key_set_left_margin(key, 67);
    fsemu_oskeyboard_key_set_width(key, 133);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "V");
    fsemu_oskeyboard_key_set_action(key, FSFUSE_ACTION_KEY_V);
    fsemu_oskeyboard_key_set_left_margin(key, 67);
    fsemu_oskeyboard_key_set_width(key, 133);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "B");
    fsemu_oskeyboard_key_set_action(key, FSFUSE_ACTION_KEY_B);
    fsemu_oskeyboard_key_set_left_margin(key, 67);
    fsemu_oskeyboard_key_set_width(key, 133);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "N");
    fsemu_oskeyboard_key_set_action(key, FSFUSE_ACTION_KEY_N);
    fsemu_oskeyboard_key_set_left_margin(key, 67);
    fsemu_oskeyboard_key_set_width(key, 133);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "M");
    fsemu_oskeyboard_key_set_action(key, FSFUSE_ACTION_KEY_M);
    fsemu_oskeyboard_key_set_left_margin(key, 67);
    fsemu_oskeyboard_key_set_width(key, 133);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "SYMBOLSHIFT");
    fsemu_oskeyboard_key_set_action(key, FSFUSE_ACTION_KEY_SYMBOLSHIFT);
    fsemu_oskeyboard_key_set_left_margin(key, 67);
    fsemu_oskeyboard_key_set_width(key, 133);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "SPACE");
    fsemu_oskeyboard_key_set_action(key, FSFUSE_ACTION_KEY_SPACE);
    fsemu_oskeyboard_key_set_left_margin(key, 67);
    fsemu_oskeyboard_key_set_width(key, 233);

    row = fsemu_oskeyboard_new_row(keyboard);
    fsemu_oskeyboard_row_set_bottom_margin(row, 100);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "A");
    fsemu_oskeyboard_key_set_action(key, FSFUSE_ACTION_KEY_A);
    fsemu_oskeyboard_key_set_left_margin(key, 167);
    fsemu_oskeyboard_key_set_width(key, 133);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "S");
    fsemu_oskeyboard_key_set_action(key, FSFUSE_ACTION_KEY_S);
    fsemu_oskeyboard_key_set_left_margin(key, 67);
    fsemu_oskeyboard_key_set_width(key, 133);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "D");
    fsemu_oskeyboard_key_set_action(key, FSFUSE_ACTION_KEY_D);
    fsemu_oskeyboard_key_set_left_margin(key, 67);
    fsemu_oskeyboard_key_set_width(key, 133);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "F");
    fsemu_oskeyboard_key_set_action(key, FSFUSE_ACTION_KEY_F);
    fsemu_oskeyboard_key_set_left_margin(key, 67);
    fsemu_oskeyboard_key_set_width(key, 133);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "G");
    fsemu_oskeyboard_key_set_action(key, FSFUSE_ACTION_KEY_G);
    fsemu_oskeyboard_key_set_left_margin(key, 67);
    fsemu_oskeyboard_key_set_width(key, 133);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "H");
    fsemu_oskeyboard_key_set_action(key, FSFUSE_ACTION_KEY_H);
    fsemu_oskeyboard_key_set_left_margin(key, 67);
    fsemu_oskeyboard_key_set_width(key, 133);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "J");
    fsemu_oskeyboard_key_set_action(key, FSFUSE_ACTION_KEY_J);
    fsemu_oskeyboard_key_set_left_margin(key, 67);
    fsemu_oskeyboard_key_set_width(key, 133);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "K");
    fsemu_oskeyboard_key_set_action(key, FSFUSE_ACTION_KEY_K);
    fsemu_oskeyboard_key_set_left_margin(key, 67);
    fsemu_oskeyboard_key_set_width(key, 133);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "L");
    fsemu_oskeyboard_key_set_action(key, FSFUSE_ACTION_KEY_L);
    fsemu_oskeyboard_key_set_left_margin(key, 67);
    fsemu_oskeyboard_key_set_width(key, 133);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "ENTER");
    fsemu_oskeyboard_key_set_action(key, FSFUSE_ACTION_KEY_ENTER);
    fsemu_oskeyboard_key_set_left_margin(key, 67);
    fsemu_oskeyboard_key_set_width(key, 133);

    row = fsemu_oskeyboard_new_row(keyboard);
    fsemu_oskeyboard_row_set_bottom_margin(row, 100);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "Q");
    fsemu_oskeyboard_key_set_action(key, FSFUSE_ACTION_KEY_Q);
    fsemu_oskeyboard_key_set_left_margin(key, 100);
    fsemu_oskeyboard_key_set_width(key, 133);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "W");
    fsemu_oskeyboard_key_set_action(key, FSFUSE_ACTION_KEY_W);
    fsemu_oskeyboard_key_set_left_margin(key, 67);
    fsemu_oskeyboard_key_set_width(key, 133);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "E");
    fsemu_oskeyboard_key_set_action(key, FSFUSE_ACTION_KEY_E);
    fsemu_oskeyboard_key_set_left_margin(key, 67);
    fsemu_oskeyboard_key_set_width(key, 133);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "R");
    fsemu_oskeyboard_key_set_action(key, FSFUSE_ACTION_KEY_R);
    fsemu_oskeyboard_key_set_left_margin(key, 67);
    fsemu_oskeyboard_key_set_width(key, 133);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "T");
    fsemu_oskeyboard_key_set_action(key, FSFUSE_ACTION_KEY_T);
    fsemu_oskeyboard_key_set_left_margin(key, 67);
    fsemu_oskeyboard_key_set_width(key, 133);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "Y");
    fsemu_oskeyboard_key_set_action(key, FSFUSE_ACTION_KEY_Y);
    fsemu_oskeyboard_key_set_left_margin(key, 67);
    fsemu_oskeyboard_key_set_width(key, 133);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "U");
    fsemu_oskeyboard_key_set_action(key, FSFUSE_ACTION_KEY_U);
    fsemu_oskeyboard_key_set_left_margin(key, 67);
    fsemu_oskeyboard_key_set_width(key, 133);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "I");
    fsemu_oskeyboard_key_set_action(key, FSFUSE_ACTION_KEY_I);
    fsemu_oskeyboard_key_set_left_margin(key, 67);
    fsemu_oskeyboard_key_set_width(key, 133);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "O");
    fsemu_oskeyboard_key_set_action(key, FSFUSE_ACTION_KEY_O);
    fsemu_oskeyboard_key_set_left_margin(key, 67);
    fsemu_oskeyboard_key_set_width(key, 133);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "P");
    fsemu_oskeyboard_key_set_action(key, FSFUSE_ACTION_KEY_P);
    fsemu_oskeyboard_key_set_left_margin(key, 67);
    fsemu_oskeyboard_key_set_width(key, 133);

    row = fsemu_oskeyboard_new_row(keyboard);
    fsemu_oskeyboard_row_set_bottom_margin(row, 100);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "1");
    fsemu_oskeyboard_key_set_action(key, FSFUSE_ACTION_KEY_1);
    fsemu_oskeyboard_key_set_width(key, 133);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "2");
    fsemu_oskeyboard_key_set_action(key, FSFUSE_ACTION_KEY_2);
    fsemu_oskeyboard_key_set_left_margin(key, 67);
    fsemu_oskeyboard_key_set_width(key, 133);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "3");
    fsemu_oskeyboard_key_set_action(key, FSFUSE_ACTION_KEY_3);
    fsemu_oskeyboard_key_set_left_margin(key, 67);
    fsemu_oskeyboard_key_set_width(key, 133);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "4");
    fsemu_oskeyboard_key_set_action(key, FSFUSE_ACTION_KEY_4);
    fsemu_oskeyboard_key_set_left_margin(key, 67);
    fsemu_oskeyboard_key_set_width(key, 133);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "5");
    fsemu_oskeyboard_key_set_action(key, FSFUSE_ACTION_KEY_5);
    fsemu_oskeyboard_key_set_left_margin(key, 67);
    fsemu_oskeyboard_key_set_width(key, 133);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "6");
    fsemu_oskeyboard_key_set_action(key, FSFUSE_ACTION_KEY_6);
    fsemu_oskeyboard_key_set_left_margin(key, 67);
    fsemu_oskeyboard_key_set_width(key, 133);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "7");
    fsemu_oskeyboard_key_set_action(key, FSFUSE_ACTION_KEY_7);
    fsemu_oskeyboard_key_set_left_margin(key, 67);
    fsemu_oskeyboard_key_set_width(key, 133);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "8");
    fsemu_oskeyboard_key_set_action(key, FSFUSE_ACTION_KEY_8);
    fsemu_oskeyboard_key_set_left_margin(key, 67);
    fsemu_oskeyboard_key_set_width(key, 133);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "9");
    fsemu_oskeyboard_key_set_action(key, FSFUSE_ACTION_KEY_9);
    fsemu_oskeyboard_key_set_left_margin(key, 67);
    fsemu_oskeyboard_key_set_width(key, 133);

    key = fsemu_oskeyboard_row_new_key_with_name(row, "0");
    fsemu_oskeyboard_key_set_action(key, FSFUSE_ACTION_KEY_0);
    fsemu_oskeyboard_key_set_left_margin(key, 67);
    fsemu_oskeyboard_key_set_width(key, 133);
}

void fsfuse_oskeyboard_init(void)
{
    // fsfuse_log("fsuae_oskeyboard_init\n");
    fsemu_oskeyboard_t *keyboard;
    // fsemu_oskeyboard_layout_t *layout;
    // fsemu_oskeyboard_init();

    // FIXME: Auto-set as active keyboard?
    keyboard = fsemu_oskeyboard_new();

    // FIXME: Auto-set as active layout?
    // layout = fsemu_oskeyboard_new_layout();
    // fsemu_oskeyboard_add_layout(keyboard, layout);

    fsemu_oskeyboard_add(keyboard);
    fsemu_oskeyboard_set_active(keyboard);

    fsfuse_oskeyboard_configure_default(keyboard);
}

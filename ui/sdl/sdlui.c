/* sdlui.c: Routines for dealing with the SDL user interface
   Copyright (c) 2000-2002 Philip Kendall, Matan Ziv-Av, Fredrick Meunier
   Copyright (c) 2015 Stuart Brady

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation, Inc.,
   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

   Author contact information:

   E-mail: philip-fuse@shadowmagic.org.uk

*/

#include <config.h>

#include <stdio.h>
#ifdef FSEMU
#else
#include <SDL.h>
#endif

#include "display.h"
#include "fuse.h"
#include "ui/ui.h"
#include "ui/uidisplay.h"
#include "settings.h"
#include "sdldisplay.h"
#include "sdljoystick.h"
#include "sdlkeyboard.h"
#include "ui/scaler/scaler.h"
#include "menu.h"

#ifdef FSEMU
#include "fsfuse/fsfuse.h"
#include "keyboard.h"
#include "tape.h"
#include "ui/widget/widget_internals.h"
#endif

static void
atexit_proc( void )
{ 
#ifdef FSEMU
#else
  SDL_ShowCursor( SDL_ENABLE );
  SDL_Quit();
#endif
}

int 
ui_init( int *argc, char ***argv )
{
  int error;

  if( ui_widget_init() ) return 1;

/* Comment out to Work around a bug in OS X 10.1 related to OpenGL in windowed
   mode */
  atexit(atexit_proc);

  error = SDL_Init( SDL_INIT_VIDEO );
  if ( error )
    return error;

#ifndef __MORPHOS__
#ifdef FSEMU
  // Using SDL2
#else
  SDL_EnableUNICODE( 1 );
#endif
#endif				/* #ifndef __MORPHOS__ */

  sdlkeyboard_init();

  ui_mouse_present = 1;

  return 0;
}

#ifdef FSEMU
#include "../../peripherals/joystick.h"
#endif

int 
ui_event( void )
{
#ifdef FSEMU2
  // printf("ui_event!\n");
  uint16_t action;
  int16_t state;
  while (fsemu_input_next_action(&action, &state)) {
    printf("action %x (state %x)!\n", action, state);
    // SDL_Event event;

    if (action > FSFUSE_ACTION_KEY_BEFORE_FIRST &&
                   action < FSFUSE_ACTION_KEY_AFTER_LAST) {
      input_key fuse_keysym;
      input_event_t fuse_event;
      fuse_keysym = keysyms_remap(action);
      if (state) {
        fuse_event.type = INPUT_EVENT_KEYPRESS;
      } else {
        fuse_event.type = INPUT_EVENT_KEYRELEASE;
      }
      fuse_event.types.key.spectrum_key = fuse_keysym;
      printf("fuse key %d (%d)\n", fuse_keysym, !!state);
      input_event(&fuse_event);
      continue;
    }

    if (action == FSEMU_ACTION_PORT0TYPE0) {
      printf("Setting joystick type to NONE\n");
      settings_current.joystick_1_output = JOYSTICK_TYPE_NONE;
      // settings_current.joy_kempston = 0;
      // settings_current.interface2 = 1;
      periph_posthook();
      // Post the action back to the main thread to update the UI and
      // finalize the state switch.
      fsemu_action_post_to_main(action);
    } else if (action == FSEMU_ACTION_PORT0TYPE1) {
      printf("Setting joystick type to KEMPSTON\n");
      settings_current.joystick_1_output = JOYSTICK_TYPE_KEMPSTON;
      settings_current.joy_kempston = 1;
      // settings_current.interface2 = 0;
      periph_posthook();
      fsemu_action_post_to_main(action);
    } else if (action == FSEMU_ACTION_PORT0TYPE2) {
      printf("Setting joystick type to SINCLAIR\n");
      settings_current.joystick_1_output = JOYSTICK_TYPE_SINCLAIR_1;
      // settings_current.joy_kempston = 0;
      // settings_current.interface2 = 1;
      periph_posthook();
      fsemu_action_post_to_main(action);
    } else if (action == FSEMU_ACTION_PORT0TYPE3) {
      printf("Setting joystick type to CUSTOM\n");
      settings_current.joystick_1_output = JOYSTICK_TYPE_NONE;
      // settings_current.joy_kempston = 0;
      // settings_current.interface2 = 0;
      periph_posthook();
      fsemu_action_post_to_main(action);
    }

    if (action == FSEMU_ACTION_PORT1TYPE0) {
      printf("Setting joystick type (b) to NONE\n");
      settings_current.joystick_2_output = JOYSTICK_TYPE_NONE;
      // settings_current.joy_kempston = 0;
      // settings_current.interface2 = 1;
      periph_posthook();
      // Post the action back to the main thread to update the UI and
      // finalize the state switch.
      fsemu_action_post_to_main(action);
    } else if (action == FSEMU_ACTION_PORT1TYPE1) {
      printf("Setting joystick type (b) to KEMPSTON\n");
      settings_current.joystick_2_output = JOYSTICK_TYPE_KEMPSTON;
      settings_current.joy_kempston = 1;
      // settings_current.interface2 = 0;
      periph_posthook();
      fsemu_action_post_to_main(action);
    } else if (action == FSEMU_ACTION_PORT1TYPE2) {
      printf("Setting joystick type (b) to SINCLAIR (2)\n");
      settings_current.joystick_2_output = JOYSTICK_TYPE_SINCLAIR_2;
      // settings_current.joy_kempston = 0;
      // settings_current.interface2 = 1;
      periph_posthook();
      fsemu_action_post_to_main(action);
    } else if (action == FSEMU_ACTION_PORT1TYPE3) {
      printf("Setting joystick type (b) to CUSTOM\n");
      settings_current.joystick_2_output = JOYSTICK_TYPE_NONE;
      // settings_current.joy_kempston = 0;
      // settings_current.interface2 = 0;
      periph_posthook();
      fsemu_action_post_to_main(action);
    }

    if (action == FSEMU_ACTION_DRIVE0INSERT0) {
      // FIXME: Check that this is the tape drive...!
      // menu_media_tape_open( 0 );
      const char *path = fsemu_media_file(FSEMU_MEDIA_DRIVE_TYPE_TAPE, 0);
      printf("Insert %s\n", path);
      if (path) {
        tape_open(path, 0);
      } else {
        tape_close();
      }
      fsemu_action_post_to_main(action);
    } else if (action == FSEMU_ACTION_DRIVE0INSERT1) {
      // FIXME: Check that this is the tape drive...!
      // menu_media_tape_open( 0 );
      const char *path = fsemu_media_file(FSEMU_MEDIA_DRIVE_TYPE_TAPE, 1);
      printf("Insert %s\n", path);
      if (path) {
        tape_open(path, 0);
      } else {
        tape_close();
      }
      // fsemu_action_post_to_main(action);
    }

    if (false) {
      //
    } else if (action == FSFUSE_ACTION_JOYSTICK1_UP) {
      joystick_press(0, JOYSTICK_BUTTON_UP, state ? 1 : 0);
    } else if (action == FSFUSE_ACTION_JOYSTICK1_DOWN) {
      joystick_press(0, JOYSTICK_BUTTON_DOWN, state ? 1 : 0);
    } else if (action == FSFUSE_ACTION_JOYSTICK1_LEFT) {
      joystick_press(0, JOYSTICK_BUTTON_LEFT, state ? 1 : 0);
    } else if (action == FSFUSE_ACTION_JOYSTICK1_RIGHT) {
      joystick_press(0, JOYSTICK_BUTTON_RIGHT, state ? 1 : 0);
    } else if (action == FSFUSE_ACTION_JOYSTICK1_FIRE) {
      joystick_press(0, JOYSTICK_BUTTON_FIRE, state ? 1 : 0);

    } else if (action == FSFUSE_ACTION_JOYSTICK2_UP) {
      joystick_press(1, JOYSTICK_BUTTON_UP, state ? 1 : 0);
    } else if (action == FSFUSE_ACTION_JOYSTICK2_DOWN) {
      joystick_press(1, JOYSTICK_BUTTON_DOWN, state ? 1 : 0);
    } else if (action == FSFUSE_ACTION_JOYSTICK2_LEFT) {
      joystick_press(1, JOYSTICK_BUTTON_LEFT, state ? 1 : 0);
    } else if (action == FSFUSE_ACTION_JOYSTICK2_RIGHT) {
      joystick_press(1, JOYSTICK_BUTTON_RIGHT, state ? 1 : 0);
    } else if (action == FSFUSE_ACTION_JOYSTICK2_FIRE) {
      joystick_press(1, JOYSTICK_BUTTON_FIRE, state ? 1 : 0);
    }
  }

#else

  SDL_Event event;

#ifdef FSEMU
  fsemu_frame_log_epoch("ui_event\n");


  if (fsemu_quit_check()) {
    fuse_exiting = 1;
    widget_end_all(WIDGET_FINISHED_CANCEL);
    fuse_emulation_pause();
    // printf("menu_file_exit(0)\n");
    menu_file_exit(0);
    fuse_emulation_unpause();
  }
  // FIXME: Is this a good place for this call?
  fsemu_window_work(0);
#endif

  while ( SDL_PollEvent( &event ) ) {
#ifdef FSEMU
    if (fsemu_sdlwindow_handle_event(&event)) {
      printf("[FSEMU] Not passing on event to Fuse\n");
      continue;
    }
#endif
      switch ( event.type ) {
    case SDL_KEYDOWN:
      sdlkeyboard_keypress( &(event.key) );
      break;
    case SDL_KEYUP:
      sdlkeyboard_keyrelease( &(event.key) );
      break;

    case SDL_MOUSEBUTTONDOWN:
      ui_mouse_button( event.button.button, 1 );
      break;
    case SDL_MOUSEBUTTONUP:
      ui_mouse_button( event.button.button, 0 );
      break;
    case SDL_MOUSEMOTION:
      if( ui_mouse_grabbed ) {
        ui_mouse_motion( event.motion.x - 128, event.motion.y - 128 );
#ifdef FSEMU
        // Not needed for SDL2
#else
        if( event.motion.x != 128 || event.motion.y != 128 )
          SDL_WarpMouse( 128, 128 );
#endif
      }	
      break;

#if defined USE_JOYSTICK && !defined HAVE_JSW_H

    case SDL_JOYBUTTONDOWN:
      sdljoystick_buttonpress( &(event.jbutton) );
      break;
    case SDL_JOYBUTTONUP:
      sdljoystick_buttonrelease( &(event.jbutton) );
      break;
    case SDL_JOYAXISMOTION:
      sdljoystick_axismove( &(event.jaxis) );
      break;
    case SDL_JOYHATMOTION:
      sdljoystick_hatmove( &(event.jhat) );
      break;

#endif			/* if defined USE_JOYSTICK && !defined HAVE_JSW_H */

    case SDL_QUIT:
#ifdef FSEMU
      fuse_exiting = 1;
      widget_end_all(WIDGET_FINISHED_CANCEL);
#else
      fuse_emulation_pause();
      menu_file_exit(0);
      fuse_emulation_unpause();
#endif
      break;
#ifdef FSEMU
    // Handled by FSEMU
#else
    case SDL_VIDEOEXPOSE:
      display_refresh_all();
      break;
    case SDL_ACTIVEEVENT:
      if( event.active.state & SDL_APPINPUTFOCUS ) {
	if( event.active.gain ) ui_mouse_resume(); else ui_mouse_suspend();
      }
      break;
#endif
    default:
      break;
    }
  }

#ifdef FSEMU
  fsemu_frame_add_gui_time(0);
#endif
#endif
  return 0;
}

int 
ui_end( void )
{
  int error;

  error = uidisplay_end();
  if ( error )
    return error;

  sdlkeyboard_end();

#ifdef FSEMU
#else
  SDL_Quit();
#endif

  ui_widget_end();

  return 0;
}

int
ui_statusbar_update_speed( float speed )
{
#ifdef FSEMU
  // Just keep the regular window title.
  return 0;
#endif
  char buffer[15];
  const char fuse[] = "Fuse";

  snprintf( buffer, 15, "%s - %3.0f%%", fuse, speed );

#ifdef FSEMU
#else
  /* FIXME: Icon caption should be snapshot name? */
  SDL_WM_SetCaption( buffer, fuse );
#endif

  return 0;
}

int
ui_mouse_grab( int startup )
{
#ifdef FSEMU
  printf("FIXME: ui_mouse_grab startup=%d\n", startup);
  return 1;
#else
  if( settings_current.full_screen ) {
    SDL_WarpMouse( 128, 128 );
    return 1;
  }
  if( startup ) return 0;

  switch( SDL_WM_GrabInput( SDL_GRAB_ON ) ) {
  case SDL_GRAB_ON:
  case SDL_GRAB_FULLSCREEN:
    SDL_ShowCursor( SDL_DISABLE );
    SDL_WarpMouse( 128, 128 );
    return 1;
  default:
    ui_error( UI_ERROR_WARNING, "Mouse grab failed" );
    return 0;
  }
#endif
}

int
ui_mouse_release( int suspend )
{
#ifdef FSEMU
  printf("FIXME: ui_mouse_release suspend=%d\n", suspend);
  return 1;
#else
  if( settings_current.full_screen ) return !suspend;

  SDL_WM_GrabInput( SDL_GRAB_OFF );
  SDL_ShowCursor( SDL_ENABLE );
  return 0;
#endif
}

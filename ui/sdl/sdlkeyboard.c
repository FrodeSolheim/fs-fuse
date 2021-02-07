/* sdlkeyboard.c: routines for dealing with the SDL keyboard
   Copyright (c) 2000-2005 Philip Kendall, Matan Ziv-Av, Fredrick Meunier

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
#include <stdlib.h>
#include <SDL.h>

#include "display.h"
#include "fuse.h"
#include "keyboard.h"
#include "machine.h"
#include "settings.h"
#include "snapshot.h"
#include "spectrum.h"
#include "tape.h"
#include "ui/ui.h"
#include "utils.h"
#include "sdlkeyboard.h"

#ifdef FSEMU
// #include "fsgs.h"
#include "screenshot.h"
#include "../widget/widget.h"
#include "../widget/widget_internals.h"
#endif

/* Map low byte of UCS-2(?) Unicode to Fuse input layer keysym for
   upper case letters */
extern const keysyms_map_t unicode_keysyms_map[];

static GHashTable *unicode_keysyms_hash;

#ifdef FSEMU
#else
static input_key
unicode_keysyms_remap( libspectrum_dword ui_keysym )
{
  const input_key *ptr;

  ptr = g_hash_table_lookup( unicode_keysyms_hash, &ui_keysym );

  return ptr ? *ptr : INPUT_KEY_NONE;
}
#endif

void
sdlkeyboard_init(void)
{
  keysyms_map_t *ptr3;

  unicode_keysyms_hash = g_hash_table_new( g_int_hash, g_int_equal );

  for( ptr3 = (keysyms_map_t *)unicode_keysyms_map; ptr3->ui; ptr3++ )
    g_hash_table_insert( unicode_keysyms_hash, &( ptr3->ui ),
                         &( ptr3->fuse ) );

#ifdef FSEMU
  // Nope
#else
  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
#endif
}

void
sdlkeyboard_end(void)
{
  g_hash_table_destroy( unicode_keysyms_hash );
}

#ifdef FSEMU
#else

void
sdlkeyboard_keypress( SDL_KeyboardEvent *keyevent )
{
#ifdef FSEMU_XXX_FSGS
  static int paused = 0;
  if (keyevent->keysym.sym == FSGS_KEY_MENU) {
    if (ui_widget_level < 0) {
      ui_popup_menu(INPUT_KEY_F1);
    } else {
      widget_end_widget(WIDGET_FINISHED_CANCEL);
    }
    return;
  }
  if (keyevent->keysym.mod & FSGS_MOD_MOD) {
    switch (keyevent->keysym.sym) {
    case FSGS_KEY_BORDER:
        fsgs_cycle_border();
        // display_refresh_main_screen();
        break;
    case FSGS_KEY_STRETCH:
        fsgs_cycle_stretch();
        // display_refresh_main_screen();
        break;
    case FSGS_KEY_PAUSE:
      if (!paused) {
        fuse_emulation_pause();
        paused = 1;
      } else {
        fuse_emulation_unpause();
        paused = 0;
      }
      break;
    case FSGS_KEY_SCREENSHOT:
      screenshot_write(fsgs_screenshot_path("full"), SCALER_NORMAL);
      fsgs_set_screenshot_crop(32, 24, 256, 192);
      screenshot_write(fsgs_screenshot_path("crop"), SCALER_NORMAL);
      fsgs_set_screenshot_crop(0, 0, 0, 0);
      break;
    case FSGS_KEY_QUIT:
      fuse_exiting = 1;
      widget_end_all(WIDGET_FINISHED_CANCEL);
      break;
    default:
      break;
    }
    return;
  }
#endif
  input_key fuse_keysym, unicode_keysym;
  input_event_t fuse_event;

  fuse_keysym = keysyms_remap( keyevent->keysym.sym );

#ifdef FSEMU
  printf("FIXME: keysym.unicode\n");
#else
  /* Currently unicode_keysyms_map contains ASCII character keys */
  if( ( keyevent->keysym.unicode & 0xFF80 ) == 0 ) 
    unicode_keysym = unicode_keysyms_remap( keyevent->keysym.unicode );
  else
    unicode_keysym = INPUT_KEY_NONE;
#endif

  if( fuse_keysym == INPUT_KEY_NONE && unicode_keysym == INPUT_KEY_NONE )
    return;

  fuse_event.type = INPUT_EVENT_KEYPRESS;
  if( unicode_keysym == INPUT_KEY_NONE )
    fuse_event.types.key.native_key = fuse_keysym;
  else
    fuse_event.types.key.native_key = unicode_keysym;
  fuse_event.types.key.spectrum_key = fuse_keysym;

  input_event( &fuse_event );
}

void
sdlkeyboard_keyrelease( SDL_KeyboardEvent *keyevent )
{
#ifdef FSEMU_XXX_FSGS
  if (keyevent->keysym.sym == FSGS_KEY_MENU) {
    return;
  }
  if (keyevent->keysym.mod & FSGS_MOD_MOD) {
    return;
  }
#endif
  input_key fuse_keysym;
  input_event_t fuse_event;

  fuse_keysym = keysyms_remap( keyevent->keysym.sym );

  if( fuse_keysym == INPUT_KEY_NONE ) return;

  fuse_event.type = INPUT_EVENT_KEYRELEASE;
  /* SDL doesn't provide key release information for UNICODE, assuming that
     the values will just be used for dialog boxes et. al. so put in SDL keysym
     equivalent and hope for the best */
  fuse_event.types.key.native_key = fuse_keysym;
  fuse_event.types.key.spectrum_key = fuse_keysym;

  input_event( &fuse_event );
}

#endif

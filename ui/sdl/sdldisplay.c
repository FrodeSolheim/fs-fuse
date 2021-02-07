/* sdldisplay.c: Routines for dealing with the SDL display
   Copyright (c) 2000-2006 Philip Kendall, Matan Ziv-Av, Fredrick Meunier
   Copyright (c) 2015 Adrien Destugues

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

#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <SDL.h>

#include <libspectrum.h>

#include "display.h"
#include "fuse.h"
#include "machine.h"
#include "peripherals/scld.h"
#include "screenshot.h"
#include "settings.h"
#include "ui/ui.h"
#include "ui/scaler/scaler.h"
#include "ui/uidisplay.h"
#include "utils.h"

#ifdef FSEMU
#include "fsfuse/fsfuse.h"
#endif

SDL_Surface *sdldisplay_gc = NULL;   /* Hardware screen */
static SDL_Surface *tmp_screen=NULL; /* Temporary screen for scalers */

#ifdef FSEMU
// Not supported
#else
static SDL_Surface *red_cassette[2], *green_cassette[2];
static SDL_Surface *red_mdr[2], *green_mdr[2];
static SDL_Surface *red_disk[2], *green_disk[2];
#endif

static ui_statusbar_state sdl_disk_state, sdl_mdr_state, sdl_tape_state;
static int sdl_status_updated;

static int tmp_screen_width;

static Uint32 colour_values[16];

static SDL_Color colour_palette[] = {
  {   0,   0,   0,   0 }, 
  {   0,   0, 192,   0 }, 
  { 192,   0,   0,   0 }, 
  { 192,   0, 192,   0 }, 
  {   0, 192,   0,   0 }, 
  {   0, 192, 192,   0 }, 
  { 192, 192,   0,   0 }, 
  { 192, 192, 192,   0 }, 
  {   0,   0,   0,   0 }, 
  {   0,   0, 255,   0 }, 
  { 255,   0,   0,   0 }, 
  { 255,   0, 255,   0 }, 
  {   0, 255,   0,   0 }, 
  {   0, 255, 255,   0 }, 
  { 255, 255,   0,   0 }, 
  { 255, 255, 255,   0 }
};

static Uint32 bw_values[16];

/* This is a rule of thumb for the maximum number of rects that can be updated
   each frame. If more are generated we just update the whole screen */
#define MAX_UPDATE_RECT 300
static SDL_Rect updated_rects[MAX_UPDATE_RECT];
static int num_rects = 0;
static libspectrum_byte sdldisplay_force_full_refresh = 1;

#ifdef FSEMU
// Not used
#else
static int max_fullscreen_height;
static int min_fullscreen_height;
static int fullscreen_width = 0;
#endif
static int fullscreen_x_off = 0;
static int fullscreen_y_off = 0;

/* The current size of the display (in units of DISPLAY_SCREEN_*) */
static float sdldisplay_current_size = 1;

static libspectrum_byte sdldisplay_is_full_screen = 0;

static int image_width;
static int image_height;

static int timex;

static void init_scalers( void );
static int sdldisplay_allocate_colours( int numColours, Uint32 *colour_values,
                                        Uint32 *bw_values );

static int sdldisplay_load_gfx_mode( void );

static void
init_scalers( void )
{
  scaler_register_clear();

  scaler_register( SCALER_NORMAL );
  scaler_register( SCALER_DOUBLESIZE );
  scaler_register( SCALER_TRIPLESIZE );
  scaler_register( SCALER_2XSAI );
  scaler_register( SCALER_SUPER2XSAI );
  scaler_register( SCALER_SUPEREAGLE );
  scaler_register( SCALER_ADVMAME2X );
  scaler_register( SCALER_ADVMAME3X );
  scaler_register( SCALER_DOTMATRIX );
  scaler_register( SCALER_PALTV );
  scaler_register( SCALER_HQ2X );
  if( machine_current->timex ) {
    scaler_register( SCALER_HALF ); 
    scaler_register( SCALER_HALFSKIP );
    scaler_register( SCALER_TIMEXTV );
    scaler_register( SCALER_TIMEX1_5X );
  } else {
    scaler_register( SCALER_TV2X );
    scaler_register( SCALER_TV3X );
    scaler_register( SCALER_PALTV2X );
    scaler_register( SCALER_PALTV3X );
    scaler_register( SCALER_HQ3X );
  }
  
  if( scaler_is_supported( current_scaler ) ) {
    scaler_select_scaler( current_scaler );
  } else {
    scaler_select_scaler( SCALER_NORMAL );
  }
}

#ifdef FSEMU
// Don't have palette support
#else

static int
sdl_convert_icon( SDL_Surface *source, SDL_Surface **icon, int red )
{
  SDL_Surface *copy;   /* Copy with altered palette */
  int i;

  SDL_Color colors[ source->format->palette->ncolors ];

  copy = SDL_ConvertSurface( source, source->format, SDL_SWSURFACE );

  for( i = 0; i < copy->format->palette->ncolors; i++ ) {
    colors[i].r = red ? copy->format->palette->colors[i].r : 0;
    colors[i].g = red ? 0 : copy->format->palette->colors[i].g;
    colors[i].b = 0;
  }

  SDL_SetPalette( copy, SDL_LOGPAL, colors, 0, i );

  icon[0] = SDL_ConvertSurface( copy, tmp_screen->format, SDL_SWSURFACE );

  SDL_FreeSurface( copy );

  icon[1] = SDL_CreateRGBSurface( SDL_SWSURFACE,
                                  (icon[0]->w)<<1, (icon[0]->h)<<1,
                                  icon[0]->format->BitsPerPixel,
                                  icon[0]->format->Rmask,
                                  icon[0]->format->Gmask,
                                  icon[0]->format->Bmask,
                                  icon[0]->format->Amask
                                );

  ( scaler_get_proc16( SCALER_DOUBLESIZE ) )(
        (libspectrum_byte*)icon[0]->pixels,
        icon[0]->pitch,
        (libspectrum_byte*)icon[1]->pixels,
        icon[1]->pitch, icon[0]->w, icon[0]->h
      );

  return 0;
}

static int
sdl_load_status_icon( const char*filename, SDL_Surface **red, SDL_Surface **green )
{
  char path[ PATH_MAX ];
  SDL_Surface *temp;    /* Copy of image as loaded */

  if( utils_find_file_path( filename, path, UTILS_AUXILIARY_LIB ) ) {
    fprintf( stderr, "%s: Error getting path for icons\n", fuse_progname );
    return -1;
  }

  if((temp = SDL_LoadBMP(path)) == NULL) {
    fprintf( stderr, "%s: Error loading icon \"%s\" text:%s\n", fuse_progname,
             path, SDL_GetError() );
    return -1;
  }

  if(temp->format->palette == NULL) {
    fprintf( stderr, "%s: Icon \"%s\" is not paletted\n", fuse_progname, path );
    return -1;
  }

  sdl_convert_icon( temp, red, 1 );
  sdl_convert_icon( temp, green, 0 );

  SDL_FreeSurface( temp );

  return 0;
}

#endif

int
uidisplay_init( int width, int height )
{
#ifdef FSEMU
#ifdef FSEMU2
#else
    int fullscreen = 0;
    int vsync = 0;
    fsemu_startupinfo_set_emulator_fork_info("Fuse");
    fsemu_video_disallow_vsync(1);
    fsemu_helper_init_emulator("Fuse-FS", true, FSEMU_VIDEO_RENDERER_OPENGL,
                               fullscreen, vsync);
    fsemu_thread_set_emu();
#endif
#else
  SDL_Rect **modes;
  int no_modes;
  int i, mw = 0, mh = 0, mn = 0;

  /* Get available fullscreen/software modes */
  modes=SDL_ListModes(NULL, SDL_FULLSCREEN|SDL_SWSURFACE);

  no_modes = ( modes == (SDL_Rect **) 0 || modes == (SDL_Rect **) -1 ) ? 1 : 0;

  if( settings_current.sdl_fullscreen_mode &&
      strcmp( settings_current.sdl_fullscreen_mode, "list" ) == 0 ) {

    fprintf( stderr,
    "=====================================================================\n"
    " List of available SDL fullscreen modes:\n"
    "---------------------------------------------------------------------\n"
    "  No. width height\n"
    "---------------------------------------------------------------------\n"
    );
    if( no_modes ) {
      fprintf( stderr, "  ** The modes list is empty%s...\n",
                       no_modes == 2 ? ", all resolution allowed" : "" );
    } else {
      for( i = 0; modes[i]; i++ ) {
        fprintf( stderr, "% 3d  % 5d % 5d\n", i + 1, modes[i]->w, modes[i]->h );
      }
    }
    fprintf( stderr,
    "=====================================================================\n");
    fuse_exiting = 1;
    return 0;
  }

  for( i=0; modes[i]; ++i ); /* count modes */
  if( settings_current.sdl_fullscreen_mode ) {
    if( sscanf( settings_current.sdl_fullscreen_mode, " %dx%d", &mw, &mh ) != 2 ) {
      if( sscanf( settings_current.sdl_fullscreen_mode, " %d", &mn ) == 1 && mn <= i ) {
        mw = modes[mn - 1]->w; mh = modes[mn - 1]->h;
      }
    }
  }

  /* Check if there are any modes available, or if our resolution is restricted
     at all */
  if( no_modes ){
    /* Just try whatever we have and see what happens */
    max_fullscreen_height = 480;
    min_fullscreen_height = 240;
  } else if( mh > 0 ) {
    /* set from command line */
    max_fullscreen_height = min_fullscreen_height = mh;
    fullscreen_width = mw;
  } else {
    /* Record the largest supported fullscreen software mode */
    max_fullscreen_height = modes[0]->h;

    /* Record the smallest supported fullscreen software mode */
    for( i=0; modes[i]; ++i ) {
      min_fullscreen_height = modes[i]->h;
    }
  }
#endif

  image_width = width;
  image_height = height;

  timex = machine_current->timex;

  init_scalers();

  if ( scaler_select_scaler( current_scaler ) )
    scaler_select_scaler( SCALER_NORMAL );

  if( sdldisplay_load_gfx_mode() ) return 1;

#ifdef FSEMU
  // Handled by FSEMU
#else
  SDL_WM_SetCaption( "Fuse", "Fuse" );
#endif

  /* We can now output error messages to our output device */
  display_ui_initialised = 1;

#ifdef FSEMU
  // Not supported

#ifdef FSEMU2
  // Too soon?
  double hz = (double) machine_current->timings.processor_speed /
              machine_current->timings.tstates_per_frame;
  printf("First fsemu_frame_start hz=%f\n", hz);
  // printf("Tape file: %s\n", settings_current.tape_file);
  fsemu_frame_start(hz);
#endif

#else
  sdl_load_status_icon( "cassette.bmp", red_cassette, green_cassette );
  sdl_load_status_icon( "microdrive.bmp", red_mdr, green_mdr );
  sdl_load_status_icon( "plus3disk.bmp", red_disk, green_disk );
#endif

  return 0;
}

static int
sdldisplay_allocate_colours( int numColours, Uint32 *colour_values,
                             Uint32 *bw_values )
{
  int i;
  Uint8 red, green, blue, grey;

  for( i = 0; i < numColours; i++ ) {

      red = colour_palette[i].r;
    green = colour_palette[i].g;
     blue = colour_palette[i].b;

    /* Addition of 0.5 is to avoid rounding errors */
    grey = ( 0.299 * red + 0.587 * green + 0.114 * blue ) + 0.5;

    colour_values[i] = SDL_MapRGB( tmp_screen->format,  red, green, blue );
    bw_values[i]     = SDL_MapRGB( tmp_screen->format, grey,  grey, grey );
  }

  return 0;
}

#ifdef FSEMU
// Not used
#else
static void
sdldisplay_find_best_fullscreen_scaler( void )
{
  static int windowed_scaler = -1;
  static int searching_fullscreen_scaler = 0;

  /* Make sure we have at least more than half of the screen covered in
     fullscreen to avoid the "postage stamp" on machines that don't support
     320x240 anymore e.g. Mac notebooks */
  if( settings_current.full_screen ) {
    int i = 0;

    if( searching_fullscreen_scaler ) return;
    searching_fullscreen_scaler = 1;
    while( i < SCALER_NUM &&
           ( image_height*sdldisplay_current_size <= min_fullscreen_height/2 ||
             image_height*sdldisplay_current_size > max_fullscreen_height ) ) {
      if( windowed_scaler == -1) windowed_scaler = current_scaler;
      while( !scaler_is_supported(i) ) i++;
      scaler_select_scaler( i++ );
      sdldisplay_current_size = scaler_get_scaling_factor( current_scaler );
      /* if we failed to find a suitable size scaler, just use normal (what the
         user had originally may be too big) */
      if( image_height * sdldisplay_current_size <= min_fullscreen_height/2 ||
          image_height * sdldisplay_current_size > max_fullscreen_height ) {
        scaler_select_scaler( SCALER_NORMAL );
        sdldisplay_current_size = scaler_get_scaling_factor( current_scaler );
      }
    }
    searching_fullscreen_scaler = 0;
  } else {
    if( windowed_scaler != -1 ) {
      scaler_select_scaler( windowed_scaler );
      windowed_scaler = -1;
      sdldisplay_current_size = scaler_get_scaling_factor( current_scaler );
    }
  }
}
#endif

static int
sdldisplay_load_gfx_mode( void )
{
  Uint16 *tmp_screen_pixels;

  sdldisplay_force_full_refresh = 1;

  /* Free the old surface */
  if( tmp_screen ) {
    free( tmp_screen->pixels );
    SDL_FreeSurface( tmp_screen );
    tmp_screen = NULL;
  }

  tmp_screen_width = (image_width + 3);

  sdldisplay_current_size = scaler_get_scaling_factor( current_scaler );

#ifdef FSEMU
  // Use the scaler we've specified.
#else
  sdldisplay_find_best_fullscreen_scaler();
#endif

printf("AAAA\n");
  /* Create the surface that contains the scaled graphics in 16 bit mode */
#ifdef FSEMU

  if (sdldisplay_gc) {
    SDL_FreeSurface(sdldisplay_gc);
  }

  sdldisplay_gc = SDL_CreateRGBSurface(
              0,
              image_width * sdldisplay_current_size,
              image_height * sdldisplay_current_size, 16, 0, 0, 0, 0);
  printf("%d %d (%.2f) -> %p\n", image_width, image_height, sdldisplay_current_size, sdldisplay_gc);

#ifdef FSGS_XXX
  fsgs_create_window(settings_current.full_screen ? 1 : 0);
  /*
  fsgs_set_crop((32 - 6) * sdldisplay_current_size,
                (24 - 6) * sdldisplay_current_size,
                (256 + 6 * 2) * sdldisplay_current_size,
                (192 + 6 * 2) * sdldisplay_current_size);
  */
  fsgs_set_border_crop(FSGS_NO_BORDER,
                       (32 - 0) * sdldisplay_current_size,
                       (24 - 0) * sdldisplay_current_size,
                       (256 + 0 * 2) * sdldisplay_current_size,
                       (192 + 0 * 2) * sdldisplay_current_size);
  fsgs_set_border_crop(FSGS_SMALL_BORDER,
                       (32 - 6) * sdldisplay_current_size,
                       (24 - 6) * sdldisplay_current_size,
                       (256 + 6 * 2) * sdldisplay_current_size,
                       (192 + 6 * 2) * sdldisplay_current_size);
  // ZX Spectrum has about 1.0 pixel aspect ratio? Maybe not quite?
  // fsgs_set_aspect(1.0);
#endif

#else
  sdldisplay_gc = SDL_SetVideoMode(
    settings_current.full_screen && fullscreen_width ? fullscreen_width :
      image_width * sdldisplay_current_size,
    settings_current.full_screen && fullscreen_width ? max_fullscreen_height :
      image_height * sdldisplay_current_size,
    16,
    settings_current.full_screen ? (SDL_FULLSCREEN|SDL_SWSURFACE)
                                 : SDL_SWSURFACE
  );
#endif
  if( !sdldisplay_gc ) {
    fprintf( stderr, "%s: couldn't create SDL graphics context\n", fuse_progname );
    fuse_abort();
  }

#ifdef FSEMU
  // FIXME: Get from fsemu?
#else
  settings_current.full_screen =
      !!( sdldisplay_gc->flags & ( SDL_FULLSCREEN | SDL_NOFRAME ) );
#endif
  sdldisplay_is_full_screen = settings_current.full_screen;

  /* Distinguish 555 and 565 mode */
  if( sdldisplay_gc->format->Gmask >> sdldisplay_gc->format->Gshift == 0x1f )
    scaler_select_bitformat( 555 );
  else
    scaler_select_bitformat( 565 );

  /* Create the surface used for the graphics in 16 bit before scaling */

  /* Need some extra bytes around when using 2xSaI */
#ifdef FSEMU
  tmp_screen_pixels = (Uint16*)calloc(tmp_screen_width*(image_height+3), sizeof(Uint16));
  tmp_screen = SDL_CreateRGBSurfaceFrom(tmp_screen_pixels,
                                        tmp_screen_width,
                                        image_height + 3,
                                        16, tmp_screen_width*2,
                                        0,
                                        0,
                                        0,
                                        0);
#else
  tmp_screen_pixels = (Uint16*)calloc(tmp_screen_width*(image_height+3), sizeof(Uint16));
  tmp_screen = SDL_CreateRGBSurfaceFrom(tmp_screen_pixels,
                                        tmp_screen_width,
                                        image_height + 3,
                                        16, tmp_screen_width*2,
                                        sdldisplay_gc->format->Rmask,
                                        sdldisplay_gc->format->Gmask,
                                        sdldisplay_gc->format->Bmask,
                                        sdldisplay_gc->format->Amask );
#endif

  if( !tmp_screen ) {
    fprintf( stderr, "%s: couldn't create tmp_screen\n", fuse_progname );
    fuse_abort();
  }

  fullscreen_x_off = ( sdldisplay_gc->w - image_width * sdldisplay_current_size ) *
                     sdldisplay_is_full_screen  / 2;
  fullscreen_y_off = ( sdldisplay_gc->h - image_height * sdldisplay_current_size ) *
                     sdldisplay_is_full_screen / 2;

  sdldisplay_allocate_colours( 16, colour_values, bw_values );

  /* Redraw the entire screen... */
  display_refresh_all();

  return 0;
}

int
uidisplay_hotswap_gfx_mode( void )
{
  fuse_emulation_pause();

  /* Free the old surface */
  if( tmp_screen ) {
    free( tmp_screen->pixels );
    SDL_FreeSurface( tmp_screen ); tmp_screen = NULL;
  }

  /* Setup the new GFX mode */
  if( sdldisplay_load_gfx_mode() ) return 1;

#ifdef FSEMU
  // Not supported
#else
  /* reset palette */
  SDL_SetColors( sdldisplay_gc, colour_palette, 0, 16 );
#endif

#ifdef FSEMU
  // Not needed
#else
  /* Mac OS X resets the state of the cursor after a switch to full screen
     mode */
  if ( settings_current.full_screen || ui_mouse_grabbed ) {
    SDL_ShowCursor( SDL_DISABLE );
    SDL_WarpMouse( 128, 128 );
  } else {
    SDL_ShowCursor( SDL_ENABLE );
  }
#endif

  fuse_emulation_unpause();

  return 0;
}

SDL_Surface *saved = NULL;

void
uidisplay_frame_save( void )
{
  if( saved ) {
    SDL_FreeSurface( saved );
    saved = NULL;
  }

  saved = SDL_ConvertSurface( tmp_screen, tmp_screen->format,
                              SDL_SWSURFACE );
}

void
uidisplay_frame_restore( void )
{
  if( saved ) {
    SDL_BlitSurface( saved, NULL, tmp_screen, NULL );
    sdldisplay_force_full_refresh = 1;
  }
}

#ifdef FSEMU
// Not used
#else

static void
sdl_blit_icon( SDL_Surface **icon,
               SDL_Rect *r, Uint32 tmp_screen_pitch,
               Uint32 dstPitch )
{
  int x, y, w, h, dst_x, dst_y, dst_h;

  if( timex ) {
    r->x<<=1;
    r->y<<=1;
    r->w<<=1;
    r->h<<=1;
  }

  x = r->x;
  y = r->y;
  w = r->w;
  h = r->h;
  r->x++;
  r->y++;

  if( SDL_BlitSurface( icon[timex], NULL, tmp_screen, r ) ) return;

  /* Extend the dirty region by 1 pixel for scalers
     that "smear" the screen, e.g. 2xSAI */
  if( scaler_flags & SCALER_FLAGS_EXPAND )
    scaler_expander( &x, &y, &w, &h, image_width, image_height );

  dst_y = y * sdldisplay_current_size + fullscreen_y_off;
  dst_h = h;
  dst_x = x * sdldisplay_current_size + fullscreen_x_off;

  scaler_proc16(
	(libspectrum_byte*)tmp_screen->pixels +
			(x+1) * tmp_screen->format->BytesPerPixel +
	                (y+1) * tmp_screen_pitch,
	tmp_screen_pitch,
	(libspectrum_byte*)sdldisplay_gc->pixels +
			dst_x * sdldisplay_gc->format->BytesPerPixel +
			dst_y * dstPitch,
	dstPitch, w, dst_h
  );

  if( num_rects == MAX_UPDATE_RECT ) {
    sdldisplay_force_full_refresh = 1;
    return;
  }

  /* Adjust rects for the destination rect size */
  updated_rects[num_rects].x = dst_x;
  updated_rects[num_rects].y = dst_y;
  updated_rects[num_rects].w = w * sdldisplay_current_size;
  updated_rects[num_rects].h = dst_h * sdldisplay_current_size;

  num_rects++;
}

static void
sdl_icon_overlay( Uint32 tmp_screen_pitch, Uint32 dstPitch )
{
  SDL_Rect r = { 243, 218, red_disk[0]->w, red_disk[0]->h };

  switch( sdl_disk_state ) {
  case UI_STATUSBAR_STATE_ACTIVE:
    sdl_blit_icon( green_disk, &r, tmp_screen_pitch, dstPitch );
    break;
  case UI_STATUSBAR_STATE_INACTIVE:
    sdl_blit_icon( red_disk, &r, tmp_screen_pitch, dstPitch );
    break;
  case UI_STATUSBAR_STATE_NOT_AVAILABLE:
    break;
  }

  r.x = 264;
  r.y = 218;
  r.w = red_mdr[0]->w;
  r.h = red_mdr[0]->h;

  switch( sdl_mdr_state ) {
  case UI_STATUSBAR_STATE_ACTIVE:
    sdl_blit_icon( green_mdr, &r, tmp_screen_pitch, dstPitch );
    break;
  case UI_STATUSBAR_STATE_INACTIVE:
    sdl_blit_icon( red_mdr, &r, tmp_screen_pitch, dstPitch );
    break;
  case UI_STATUSBAR_STATE_NOT_AVAILABLE:
    break;
  }

  r.x = 285;
  r.y = 220;
  r.w = red_cassette[0]->w;
  r.h = red_cassette[0]->h;

  switch( sdl_tape_state ) {
  case UI_STATUSBAR_STATE_ACTIVE:
    sdl_blit_icon( green_cassette, &r, tmp_screen_pitch, dstPitch );
    break;
  case UI_STATUSBAR_STATE_INACTIVE:
  case UI_STATUSBAR_STATE_NOT_AVAILABLE:
    sdl_blit_icon( red_cassette, &r, tmp_screen_pitch, dstPitch );
    break;
  }

  sdl_status_updated = 0;
}

#endif

/* Set one pixel in the display */
void
uidisplay_putpixel( int x, int y, int colour )
{
  libspectrum_word *dest_base, *dest;
  Uint32 *palette_values = settings_current.bw_tv ? bw_values :
                           colour_values;

  Uint32 palette_colour = palette_values[ colour ];

  if( machine_current->timex ) {
    x <<= 1; y <<= 1;
    dest_base = dest =
      (libspectrum_word*)( (libspectrum_byte*)tmp_screen->pixels +
                           (x+1) * tmp_screen->format->BytesPerPixel +
                           (y+1) * tmp_screen->pitch);

    *(dest++) = palette_colour;
    *(dest++) = palette_colour;
    dest = (libspectrum_word*)
      ( (libspectrum_byte*)dest_base + tmp_screen->pitch);
    *(dest++) = palette_colour;
    *(dest++) = palette_colour;
  } else {
    dest =
      (libspectrum_word*)( (libspectrum_byte*)tmp_screen->pixels +
                           (x+1) * tmp_screen->format->BytesPerPixel +
                           (y+1) * tmp_screen->pitch);

    *dest = palette_colour;
  }
}

/* Print the 8 pixels in `data' using ink colour `ink' and paper
   colour `paper' to the screen at ( (8*x) , y ) */
void
uidisplay_plot8( int x, int y, libspectrum_byte data,
	         libspectrum_byte ink, libspectrum_byte paper )
{
  libspectrum_word *dest;
  Uint32 *palette_values = settings_current.bw_tv ? bw_values :
                           colour_values;

  Uint32 palette_ink = palette_values[ ink ];
  Uint32 palette_paper = palette_values[ paper ];

  if( machine_current->timex ) {
    int i;
    libspectrum_word *dest_base;

    x <<= 4; y <<= 1;

    dest_base =
      (libspectrum_word*)( (libspectrum_byte*)tmp_screen->pixels +
                           (x+1) * tmp_screen->format->BytesPerPixel +
                           (y+1) * tmp_screen->pitch);

    for( i=0; i<2; i++ ) {
      dest = dest_base;

      *(dest++) = ( data & 0x80 ) ? palette_ink : palette_paper;
      *(dest++) = ( data & 0x80 ) ? palette_ink : palette_paper;
      *(dest++) = ( data & 0x40 ) ? palette_ink : palette_paper;
      *(dest++) = ( data & 0x40 ) ? palette_ink : palette_paper;
      *(dest++) = ( data & 0x20 ) ? palette_ink : palette_paper;
      *(dest++) = ( data & 0x20 ) ? palette_ink : palette_paper;
      *(dest++) = ( data & 0x10 ) ? palette_ink : palette_paper;
      *(dest++) = ( data & 0x10 ) ? palette_ink : palette_paper;
      *(dest++) = ( data & 0x08 ) ? palette_ink : palette_paper;
      *(dest++) = ( data & 0x08 ) ? palette_ink : palette_paper;
      *(dest++) = ( data & 0x04 ) ? palette_ink : palette_paper;
      *(dest++) = ( data & 0x04 ) ? palette_ink : palette_paper;
      *(dest++) = ( data & 0x02 ) ? palette_ink : palette_paper;
      *(dest++) = ( data & 0x02 ) ? palette_ink : palette_paper;
      *(dest++) = ( data & 0x01 ) ? palette_ink : palette_paper;
      *dest     = ( data & 0x01 ) ? palette_ink : palette_paper;

      dest_base = (libspectrum_word*)
        ( (libspectrum_byte*)dest_base + tmp_screen->pitch);
    }
  } else {
    x <<= 3;

    dest =
      (libspectrum_word*)( (libspectrum_byte*)tmp_screen->pixels +
                           (x+1) * tmp_screen->format->BytesPerPixel +
                           (y+1) * tmp_screen->pitch);

    *(dest++) = ( data & 0x80 ) ? palette_ink : palette_paper;
    *(dest++) = ( data & 0x40 ) ? palette_ink : palette_paper;
    *(dest++) = ( data & 0x20 ) ? palette_ink : palette_paper;
    *(dest++) = ( data & 0x10 ) ? palette_ink : palette_paper;
    *(dest++) = ( data & 0x08 ) ? palette_ink : palette_paper;
    *(dest++) = ( data & 0x04 ) ? palette_ink : palette_paper;
    *(dest++) = ( data & 0x02 ) ? palette_ink : palette_paper;
    *dest     = ( data & 0x01 ) ? palette_ink : palette_paper;
  }
}

/* Print the 16 pixels in `data' using ink colour `ink' and paper
   colour `paper' to the screen at ( (16*x) , y ) */
void
uidisplay_plot16( int x, int y, libspectrum_word data,
		  libspectrum_byte ink, libspectrum_byte paper )
{
  libspectrum_word *dest_base, *dest;
  int i;
  Uint32 *palette_values = settings_current.bw_tv ? bw_values :
                           colour_values;
  Uint32 palette_ink = palette_values[ ink ];
  Uint32 palette_paper = palette_values[ paper ];
  x <<= 4; y <<= 1;

  dest_base =
    (libspectrum_word*)( (libspectrum_byte*)tmp_screen->pixels +
                         (x+1) * tmp_screen->format->BytesPerPixel +
                         (y+1) * tmp_screen->pitch);

  for( i=0; i<2; i++ ) {
    dest = dest_base;

    *(dest++) = ( data & 0x8000 ) ? palette_ink : palette_paper;
    *(dest++) = ( data & 0x4000 ) ? palette_ink : palette_paper;
    *(dest++) = ( data & 0x2000 ) ? palette_ink : palette_paper;
    *(dest++) = ( data & 0x1000 ) ? palette_ink : palette_paper;
    *(dest++) = ( data & 0x0800 ) ? palette_ink : palette_paper;
    *(dest++) = ( data & 0x0400 ) ? palette_ink : palette_paper;
    *(dest++) = ( data & 0x0200 ) ? palette_ink : palette_paper;
    *(dest++) = ( data & 0x0100 ) ? palette_ink : palette_paper;
    *(dest++) = ( data & 0x0080 ) ? palette_ink : palette_paper;
    *(dest++) = ( data & 0x0040 ) ? palette_ink : palette_paper;
    *(dest++) = ( data & 0x0020 ) ? palette_ink : palette_paper;
    *(dest++) = ( data & 0x0010 ) ? palette_ink : palette_paper;
    *(dest++) = ( data & 0x0008 ) ? palette_ink : palette_paper;
    *(dest++) = ( data & 0x0004 ) ? palette_ink : palette_paper;
    *(dest++) = ( data & 0x0002 ) ? palette_ink : palette_paper;
    *dest     = ( data & 0x0001 ) ? palette_ink : palette_paper;

    dest_base = (libspectrum_word*)
      ( (libspectrum_byte*)dest_base + tmp_screen->pitch);
  }
}

#ifdef FSEMU
void finalize_frame(fsemu_video_frame_t *frame)
{
  // fsemu_video_frame_t *frame = (SDL_Surface) *data;
  // fsemu_video_frame_t *frame = (SDL_Surface) *data;
  // SDL_FreeSurface((SDL_Surface *) frame->finalize_data);
  free(frame->finalize_data);
}
#endif

void
uidisplay_frame_end( void )
{
#ifdef FSEMU
  fsemu_frame_log_epoch("uidisplay_frame_end\n");
#endif

  SDL_Rect *r;
  Uint32 tmp_screen_pitch, dstPitch;
  SDL_Rect *last_rect;

  /* We check for a switch to fullscreen here to give systems with a
     windowed-only UI a chance to free menu etc. resources before
     the switch to fullscreen (e.g. Mac OS X) */
  if( sdldisplay_is_full_screen != settings_current.full_screen &&
      uidisplay_hotswap_gfx_mode() ) {
    fprintf( stderr, "%s: Error switching to fullscreen\n", fuse_progname );
    fuse_abort();
  }

#ifdef FSEMU
  sdldisplay_force_full_refresh = 1;
#endif
  /* Force a full redraw if requested */
  if ( sdldisplay_force_full_refresh ) {
    num_rects = 1;

    updated_rects[0].x = 0;
    updated_rects[0].y = 0;
    updated_rects[0].w = image_width;
    updated_rects[0].h = image_height;
  }

  if ( !(ui_widget_level >= 0) && num_rects == 0 && !sdl_status_updated )
    return;

  if( SDL_MUSTLOCK( sdldisplay_gc ) ) SDL_LockSurface( sdldisplay_gc );

  tmp_screen_pitch = tmp_screen->pitch;

  dstPitch = sdldisplay_gc->pitch;

  last_rect = updated_rects + num_rects;

#ifdef FSEMU_XXX
#else
  for( r = updated_rects; r != last_rect; r++ ) {
    // printf("RECT %d %d %d %d\n", r->x, r->y, r->w, r->h);

    int dst_y = r->y * sdldisplay_current_size + fullscreen_y_off;
    int dst_h = r->h;
    int dst_x = r->x * sdldisplay_current_size + fullscreen_x_off;

    scaler_proc16(
      (libspectrum_byte*)tmp_screen->pixels +
                        (r->x+1) * tmp_screen->format->BytesPerPixel +
	                (r->y+1)*tmp_screen_pitch,
      tmp_screen_pitch,
      (libspectrum_byte*)sdldisplay_gc->pixels +
	                 dst_x * sdldisplay_gc->format->BytesPerPixel +
			 dst_y*dstPitch,
      dstPitch, r->w, dst_h
    );

    /* Adjust rects for the destination rect size */
    r->x = dst_x;
    r->y = dst_y;
    r->w *= sdldisplay_current_size;
    r->h = dst_h * sdldisplay_current_size;

    // printf("  -> %d %d %d %d\n", r->x, r->y, r->w, r->h);
  }
#endif

#ifdef FSEMU
  // Not supported
#else
  if ( settings_current.statusbar )
    sdl_icon_overlay( tmp_screen_pitch, dstPitch );
#endif

  if( SDL_MUSTLOCK( sdldisplay_gc ) ) SDL_UnlockSurface( sdldisplay_gc );

#ifdef FSEMU
  
  // Reset r to first (and only) rect
  r = updated_rects;
  static SDL_Surface* dummy;
  if (dummy == NULL) {
    dummy = SDL_CreateRGBSurface(0, 1, 1, 32, 0, 0, 0, 0);
  }

  // printf("%d %d %d %d\n", r->x, r->y, r->w, r->h);

#if 1
  int tmp_w = sdldisplay_gc->w;
  int tmp_h = sdldisplay_gc->h;
  sdldisplay_gc->w = r->w;
  sdldisplay_gc->h = r->h;
  // FIXME: Don't reallocate new surfaces every time
  // SDL_Surface *rgba = SDL_ConvertSurface(sdldisplay_gc, dummy->format, 0);
  // SDL_Surface *rgba = SDL_ConvertSurface(sdldisplay_gc, sdldisplay_gc->format, 0);

  // Could also do this without mallocing per frame, and instead swap between
  // two pre-allocated buffers here...
  void *pixels = malloc(sdldisplay_gc->pitch * sdldisplay_gc->h);
  memcpy(pixels, sdldisplay_gc->pixels, sdldisplay_gc->pitch * sdldisplay_gc->h);
  sdldisplay_gc->w = tmp_w;
  sdldisplay_gc->h = tmp_h;
#else
  int tmp_w = tmp_screen->w;
  int tmp_h = tmp_screen->h;
  tmp_screen->w = r->w;
  tmp_screen->h = r->h;
  SDL_Surface *rgba = SDL_ConvertSurface(tmp_screen, dummy->format, 0);
  tmp_screen->w = tmp_w;
  tmp_screen->h = tmp_h;
#endif

  fsemu_frame_add_emulation_time(0);

  fsemu_video_frame_t *frame = fsemu_video_alloc_frame();

  // frame->layer = 0;
  // frame->buffer = rgba->pixels;
  // frame->stride = rgba->pitch;
  // frame->depth = 32;  // FIXME
  frame->buffer = pixels;
  frame->stride = sdldisplay_gc->pitch;
  frame->depth = 16;  // FIXME
  frame->width = r->w;
  frame->height = r->h;
  // frame->partial = 0;
  frame->frequency = 50;  // FIXME
  // frame->limits.x = 0;
  // frame->limits.y = 0;
  frame->limits.w = r->w;
  frame->limits.h = r->h;

  frame->finalize = finalize_frame;
  // frame->finalize_data = rgba;
  frame->finalize_data = pixels;
  // frame->flags = 0;

  static int current_frame_no = 0;
  frame->number = current_frame_no;

  fsemu_video_post_frame(frame);
#ifdef FSEMU2
#else
  fsemu_video_work(0);
#endif

  // Handle by finalize function
  // SDL_FreeSurface(rgba);

  // fsemu_time_sleep_until_us(fsemu_frame_end_at);
  // // while (fsemu_time_us() < fsemu_frame_end_at) {
  // // }
  // if (fsemu_time_us() > fsemu_frame_end_at + 1000) {
  //   printf("Overslept %d\n", (int) (fsemu_time_us() - fsemu_frame_end_at));
  // }
  // fsemu_frame_add_sleep_time(0);

  fsemu_frame_end();
  // printf("Tape file: %s\n", settings_current.tape_file);

  // Important to use as close to real (emulated) Hz as possible here, so that
  // the audio system knows how much data to expect and can correct if video
  // output rate deviates from the emulated frame rate.
  double hz = (double) machine_current->timings.processor_speed /
              machine_current->timings.tstates_per_frame;

  fsemu_frame_start(hz);
  // fsemu_time_sleep_until_us(fsemu_frame_begin_at);

#else
  /* Finally, blit all our changes to the screen */
  SDL_UpdateRects( sdldisplay_gc, num_rects, updated_rects );
#endif

  num_rects = 0;
  sdldisplay_force_full_refresh = 0;
}

void
uidisplay_area( int x, int y, int width, int height )
{
  if ( sdldisplay_force_full_refresh )
    return;

  if( num_rects == MAX_UPDATE_RECT ) {
    sdldisplay_force_full_refresh = 1;
    return;
  }

  /* Extend the dirty region by 1 pixel for scalers
     that "smear" the screen, e.g. 2xSAI */
  if( scaler_flags & SCALER_FLAGS_EXPAND )
    scaler_expander( &x, &y, &width, &height, image_width, image_height );

  updated_rects[num_rects].x = x;
  updated_rects[num_rects].y = y;
  updated_rects[num_rects].w = width;
  updated_rects[num_rects].h = height;

  num_rects++;
}

int
uidisplay_end( void )
{
#ifdef FSEMU
#else
  int i;
#endif

  display_ui_initialised = 0;

  if ( tmp_screen ) {
    free( tmp_screen->pixels );
    SDL_FreeSurface( tmp_screen ); tmp_screen = NULL;
  }

  if( saved ) {
    SDL_FreeSurface( saved ); saved = NULL;
  }

#ifdef FSEMU
  // Not supported
#else
  for( i=0; i<2; i++ ) {
    if ( red_cassette[i] ) {
      SDL_FreeSurface( red_cassette[i] ); red_cassette[i] = NULL;
    }
    if ( green_cassette[i] ) {
      SDL_FreeSurface( green_cassette[i] ); green_cassette[i] = NULL;
    }
    if ( red_mdr[i] ) {
      SDL_FreeSurface( red_mdr[i] ); red_mdr[i] = NULL;
    }
    if ( green_mdr[i] ) {
      SDL_FreeSurface( green_mdr[i] ); green_mdr[i] = NULL;
    }
    if ( red_disk[i] ) {
      SDL_FreeSurface( red_disk[i] ); red_disk[i] = NULL;
    }
    if ( green_disk[i] ) {
      SDL_FreeSurface( green_disk[i] ); green_disk[i] = NULL;
    }
  }
#endif

  return 0;
}

/* The statusbar handling function */
int
ui_statusbar_update( ui_statusbar_item item, ui_statusbar_state state )
{
  switch( item ) {

  case UI_STATUSBAR_ITEM_DISK:
    sdl_disk_state = state;
    sdl_status_updated = 1;
    return 0;

  case UI_STATUSBAR_ITEM_PAUSED:
    /* We don't support pausing this version of Fuse */
    return 0;

  case UI_STATUSBAR_ITEM_TAPE:
    sdl_tape_state = state;
    sdl_status_updated = 1;
    return 0;

  case UI_STATUSBAR_ITEM_MICRODRIVE:
    sdl_mdr_state = state;
    sdl_status_updated = 1;
    return 0;

  case UI_STATUSBAR_ITEM_MOUSE:
    /* We don't support showing a grab icon */
    return 0;

  }

  ui_error( UI_ERROR_ERROR, "Attempt to update unknown statusbar item %d",
            item );
  return 1;
}

#ifdef FSEMU

void fusefs_media_state(int *disk_state, int *mdr_state, int *tape_state)
{
  *disk_state = sdl_disk_state;
  *mdr_state = sdl_mdr_state;
  *tape_state = sdl_tape_state;
}

#endif
/* sdljoystick.h: routines for dealing with the SDL joystick
   Copyright (c) 2003 Fredrick Meunier

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

   Fred: fredm@spamcop.net

*/

#ifndef FUSE_SDLJOYSTICK_H
#define FUSE_SDLJOYSTICK_H

#ifdef FSEMU
#else
void sdljoystick_buttonpress( SDL_JoyButtonEvent *buttonevent );
void sdljoystick_buttonrelease( SDL_JoyButtonEvent *buttonevent );
void sdljoystick_axismove( SDL_JoyAxisEvent *axisevent );
void sdljoystick_hatmove( SDL_JoyHatEvent *hatevent );
#endif

#endif			/* #ifndef FUSE_SDLJOYSTICK_H */

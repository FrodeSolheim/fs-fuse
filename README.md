# FS-Fuse

Emulator for FS Game System (FSGS) based on the Free Unix Spectrum Emulator
(Fuse). Please no not contact the original author(s) of Fuse if there are
problems with FS-Fuse. At least first verify that the problem also occurs
with the unmodified Fuse emulator.

## Compiling

First, install dependencies. If you're using a Debian-based system or macOS
you can run:

    fsplugin/installdeps

Then bootstrap and configure:

    fsplugin/bootstrap
    fsplugin/configure

And finally build:

    fsplugin/make

## Limitations

- "Fast load" does not currently work in FS-Fuse, but you can fast-forward
  manually (Accelerate loaders, etc works).
- Combining Kempston joystick and Sinclair joystick interface might be buggy.

## Todo

- [DONE?] Support for two-sides tapes / media swap list.
- [DONE?] Media swapping via on-screen menu.
- [DONE?] Fast forward is not working correctly (when resuming).
- UI and config support for a second joystick port.
- On-screen LEDs needs to adapt to 16:10 and other display ratios.
- Tape controls in user interface + keyboard shortcuts.
- Close window button accessible via gamepad.
- Official support for floppy drive (incl. media swapping).
- Official support for loading snapshots as game media.
- Better v-sync @ 50 Hz (needs tuning).
- Adaptive audio buffer management (needs tweaking).
- Support for zooming to game / crop overscan borders + default
  gamepad buttons for this.
- Savestates in user interface + keyboard shortcuts.
- Make on-screen keyboard nicer.
- More official options in .fs-fuse (add on demand when needed).
- Check if we should use NSHighResolutionCapable in Info.plist (macOS).
- Handle modified tapes somehow?
- Close window button accessible via gamepad.
- Joystick/gamepad support
- Keyboard mapping for gamepads
- [DONE?] Support for two-sides tapes / media swap list
- [DONE] On-screen keyboard
- [DONE] Define and use a .fs-fuse config file format?

## Changes

Changes to the emulator include:

- Increased autoload delay to prevent interface from flashing.

Old:

* F12 to toggle the menu.
* Mod+Q to quit.
* Mod+P to pause/unpause.
* Mod+S to save a .png screenshot.
* Saves screenshots to the FSGS screenshots directory.
* Saves both cropped and uncropped screenshots.
* Can close window without confirmation.
* Added OpenGL rendering to be able to scale video better.
* Improved full-screen support.
* Mod+A to toggle stretch modes (aspect ratio).
* Mod+B to toggle border modes.
* Supports FSGS_BORDER, FSGS_STRETCH.
* Force full redraw for every frame so Fuse can work with G-SYNC.

## Copyright (Fuse)

Copyright (c) 1999-2016 Philip Kendall and others

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

## Copyright (FS-Fuse)

Copyright (c) 2017-2021 Frode Solheim

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

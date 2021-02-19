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

## To-do

To-do list moved to https://github.com/FrodeSolheim/fs-fuse/issues.

## Changes

FIMXE: Update this list

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

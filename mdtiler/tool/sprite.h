//***************************************************************************
// "sprite.h"
// Header file for "sprite.c"
//***************************************************************************
// mdtiler - Bitmap to tile conversion tool
// Copyright 2018 Javier Degirolmo
//
// This file is part of mdtiler.
//
// mdtiler is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// mdtiler is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with mdtiler.  If not, see <http://www.gnu.org/licenses/>.
//***************************************************************************

#ifndef SPRITE_H
#define SPRITE_H

// Required headers
#include <stdio.h>
#include "bitmap.h"

// Function prototypes
void set_sprite_origin(int, int);
const int generate_sprite(const Bitmap * const, FILE *, FILE *, const int, const int, const unsigned int, const unsigned int, const int);
int generate_sprite_end(FILE *);
const int generate_sprite_count(FILE *, const uint8_t);

#endif

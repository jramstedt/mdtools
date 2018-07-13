//***************************************************************************
// "palette.h"
// Header file for "palette.c"
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

#ifndef PALETTE_H
#define PALETTE_H

// Look-up table used to convert true color bitmaps into paletted ones
// Contains which color to use for each BGR combination.
#define PALTABLE_SIZE (0x20*0x20*0x20)
extern uint8_t pal_table[PALTABLE_SIZE];

// Function prototypes
unsigned get_palette_mapping(unsigned);
void remap_palette(unsigned, unsigned);

static uint8_t count_ones(uint8_t byte) {
	static const uint8_t NIBBLE_LOOKUP[16] = {
		0, 1, 1, 2, 1, 2, 2, 3,
		1, 2, 2, 3, 2, 3, 3, 4
	};

	return NIBBLE_LOOKUP[byte & 0x0F] + NIBBLE_LOOKUP[byte >> 4];
}

#endif // PALETTE_H

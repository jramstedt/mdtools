//***************************************************************************
// "sprite.c"
// Generates sprite mappings and tiles out of a bitmap
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

// Required headers
#include <stdint.h>
#include <stdio.h>
#include "main.h"
#include "offset.h"
#include "sprite.h"
#include "tiles.h"

// Sprite tile ID offset within a mapping
static size_t sprite_offset = 0;

// Center point of the sprite mapping
static int origin_x = 0;
static int origin_y = 0;

//***************************************************************************
// set_sprite_origin
// Takes care of the "origin" command in mdtiler. Changes the origin for
// sprite mappings.
//---------------------------------------------------------------------------
// param x: new X coordinate for origin
// param y: new Y coordinate for origin
//***************************************************************************

void set_sprite_origin(int x, int y)
{
   origin_x = x;
   origin_y = y;
}

//***************************************************************************
// generate_sprite
// Takes care of the "sprite" command in mdtiler. Generates the sprite
// mapping entry and the tiles out of it.
//---------------------------------------------------------------------------
// param in: input bitmap
// param outgfx: output file where tiles are stored
// param outmap: output file where mappings are stored
// param x: base X coordinate (leftmost tile)
// param y: base Y coordinate (topmost tile)
// param width: width in tiles
// param height: height in tiles
// param useVdp: use vdp sprite format
// return: error code
//***************************************************************************

const int generate_sprite(const Bitmap * const in, const FILE * outgfx, const FILE * outmap,
	const int x, const int y, const unsigned int width, const unsigned int height, const int useVdp)
{
   // Um...
   if (width < 1 || width > 4 || height < 1 || height > 4)
      return ERR_PARSE;

   // Determine sprite coordinates
   int temp_x = x - origin_x;
   int temp_y = y - origin_y;

   // Determine how many tiles the sprite takes up
   // For now we always insert new sprites (no deduplication attempted)
   size_t num_tiles = width * height;

   // Determine tile ID
   uint16_t tile_id = sprite_offset + get_map_offset();
   sprite_offset += num_tiles;

   if (useVdp != 0) {
	   int16_t sprite_x = ((temp_x & 0x80000000) >> 16) | (temp_x & 0x7FFF);
	   int16_t sprite_y = ((temp_y & 0x80000000) >> 16) | (temp_y & 0x7FFF);

	   int8_t priority = 0;
	   int8_t palette = 0;
	   int8_t vertical_flip = 0;
	   int8_t horizontal_flip = 1;

	   int16_t second = (width & 0b11) << 10 | (height & 0b11) << 8;
	   int16_t third = (priority & 0b1) << 15 | (palette & 0b11) << 13 | (vertical_flip & 0b1) << 12 | (horizontal_flip & 0b1) << 11 | tile_id & 0x7FF;

	   uint8_t buffer[] = {
		   sprite_y >> 8, sprite_y,
		   second >> 8, second,
		   third >> 8, third,
		   sprite_x >> 8, sprite_x
	   };
	   if (fwrite(buffer, sizeof(char), sizeof(buffer), outmap) != sizeof(buffer)) {
		   return ERR_CANTWRITESPR;
	   }
   }
   else {
	   // Ugly hackish way because C doesn't guarantee anything about signed
	   // integers and compilers are known to be brutal about that...
	   if (temp_x < 0) temp_x = (temp_x + 0x10000) % 0xFFFF;
	   if (temp_y < 0) temp_y = (temp_y + 0x10000) % 0xFFFF;
	   uint16_t sprite_x = temp_x;
	   uint16_t sprite_y = temp_y;

	   // Sprite size
	   uint8_t size = (width - 1) * 4 + (height - 1);

	   // Write sprite mapping entry
	   uint8_t buffer[8] = {
		   sprite_x >> 8, sprite_x,      // X offset
		   sprite_y >> 8, sprite_y,      // Y offset
		   tile_id >> 8, tile_id,        // tile ID + flags
		   0, size                       // sprite size
	   };
	   if (fwrite(buffer, 1, sizeof(buffer), outmap) != sizeof(buffer)) {
		   return ERR_CANTWRITESPR;
	   }
   }
   
   // Write sprite tiles
   return write_sprite(in, outgfx, x, y, width, height);
}

//***************************************************************************
// generate_sprite_end
// Takes care of the "sprite end" command in mdtiler. Generates the sentinel
// value indicating the end of a sprite mapping.
//---------------------------------------------------------------------------
// param outmap: output file where mappings are stored
// return: error code
//***************************************************************************

int generate_sprite_end(FILE *outmap)
{
   // Write sentinel value
   uint8_t buffer[2] = { 0x80, 0x00 };
   if (fwrite(buffer, 1, sizeof(buffer), outmap) != sizeof(buffer)) {
      return ERR_CANTWRITESPR;
   }

   // Reset mapping offset for next sprite
   if (is_continuous_offset())
      increment_offset(sprite_offset);
   sprite_offset = 0;

   // No error :)
   return ERR_NONE;
}

//***************************************************************************
// generate_sprite_count
// Writes one byte containing sprite count in metasprite.
//---------------------------------------------------------------------------
// param outmap: output file where mappings are stored
// param count: count of sprites in this metasprite
// return: error code
//***************************************************************************

const int generate_sprite_count(const FILE *outmap, const uint8_t count) {
	if (fwrite(&count, 1, sizeof(uint8_t), outmap) != sizeof(uint8_t)) {
		return ERR_CANTWRITESPR;
	}

	// Reset mapping offset for next sprite
	if (is_continuous_offset())
		increment_offset(sprite_offset);
	sprite_offset = 0;

	return ERR_NONE;
}
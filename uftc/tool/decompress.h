//***************************************************************************
// "decompress.h"
// Header file for "decompress.c"
//***************************************************************************
// Uftc compression tool
// Copyright 2011 Javier Degirolmo
//
// This file is part of the uftc tool.
//
// The uftc tool is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// The uftc tool is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with the uftc tool.  If not, see <http://www.gnu.org/licenses/>.
//***************************************************************************

#ifndef DECOMPRESS_H
#define DECOMPRESS_H

// Required headers
#include <stdio.h>

// Function prototypes
int decompress(FILE *, FILE *);

#endif

// Heirs of Avalon: A real-time civilization building game
// Copyright (C) 2014 David Ulrich
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
// 
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <stdint.h>

#include <SDL.h>
#include <SDL_image.h>

#ifndef HOA_DEFINES_H
#define HOA_DEFINES_H

#define HOA_IMG_FLAGS IMG_INIT_PNG
#define HOA_TILE_W 64
#define HOA_TILE_H 32

typedef uint32_t cmask;
typedef uint32_t eid;
typedef uint16_t err;

typedef SDL_Surface GFX;

#endif // HOA_DEFINES_H

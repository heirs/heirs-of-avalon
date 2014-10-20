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

#ifndef HOA_COMPONENTS_H
#define HOA_COMPONENTS_H

#include "defines.h"

#define HOA_ENTITIES_MAX 100

// ===== Component Definitions ===== //
typedef struct {
	char *name;
} Name;

typedef struct {
	float x;
	float y;
	float z;
} Position;

typedef struct {
	int x;
	int y;
} ScreenPosition;

typedef struct {
	float rgb[3];
	SDL_Surface *sprite;
} Sprite;

typedef struct {
	float x;
	float y;
	float z;
} Velocity;

// ===== Component Type IDs ===== //
typedef enum {
	C_NONE = 0,
	C_NAME = 1 << 0,
	C_POSITION = 1 << 1,
	C_VELOCITY = 1 << 2,
	C_SCREEN_POSITION = 1 << 3,
	C_SPRITE = 1 << 4
} Component;

// ===== Container for All Entities ===== //
typedef struct {
	cmask mask[HOA_ENTITIES_MAX];
	
	Name name[HOA_ENTITIES_MAX];
	Position position[HOA_ENTITIES_MAX];
	ScreenPosition screen_position[HOA_ENTITIES_MAX];
	Sprite sprite[HOA_ENTITIES_MAX];
	Velocity velocity[HOA_ENTITIES_MAX];
	
	SDL_Window *window;
	SDL_GLContext context;
	SDL_Surface *screen;
	
	time tick_last;
	time tick_this;
	time delta_ticks;
} World;

#endif // HOA_COMPONENTS_H

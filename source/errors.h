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

// ===== Stdlib Includes ===== //
#include <stdio.h>

// ===== Project Includes ===== //
#include "defines.h"

#ifndef HOA_ERRORS_H
#define HOA_ERRORS_H

int HOA_DEBUG;
int HOA_E_COUNT;

enum {
	E_ERR_INVALID,
	E_ENTITY_LIMIT,
	E_ENTITY_INVALID,
	E_ASSET_INVALID,
	E_SDL,
	E_SDL_IMG,
	E_INT_RANGE
};

extern const char * const err_message[];

int valid_err(err e);

void e_const(err e, const char *msg);
void e_float(err e, float v);
void e_int(err e, int v);
void e_none(err e);

#endif // HOA_ERRORS_H

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

#include "errors.h"

#define HOA_DEBUG   1
#define HOA_E_COUNT 6

const char * const err_message[HOA_E_COUNT] = {
	"E_ERR_INVALID",
	"E_ENTITY_LIMIT",
	"E_ENTITY_INVALID",
	"E_ASSET_INVALID",
	"E_SDL",
	"E_INT_RANGE"
};

// checks whether an error code is valid to print
// if not, warns of the invalid error
int valid_err(err e) {
	if (!HOA_DEBUG) return 0;
	
	if (e < HOA_E_COUNT) return 1;
	
	e_int(E_ERR_INVALID,e);
	
	return 0;
}

void e_const(err e, const char *msg) {
	if (!valid_err(e)) return;
	
	printf("ERR: %s, MSG: %s\n",err_message[e],msg);
}

void e_float(err e, float v) {
	if (!valid_err(e)) return;
	
	printf("ERR: %s, VAL: %.2d\n",err_message[e],v);
}

void e_int(err e, int v) {
	if (!valid_err(e)) return;
	
	printf("ERR: %s, VAL: %d\n",err_message[e],v);
}

void e_none(err e) {
	if (!valid_err(e)) return;
	
	printf("ERR: %s\n",err_message[e]);
}

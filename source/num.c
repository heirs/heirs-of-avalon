// Heirs of Avalon: A real-time civilization building game
// Copyright (C) 2014 - 2015  David Ulrich
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

#include "num.h"

#include "errors.h"
#include <limits.h>

int num_ftoi(float f) {
	if (f <= INT_MIN-0.5) {
		e_float(E_INT_RANGE,f);
		return INT_MIN;
	}
	else if (f >= INT_MAX+0.5) {
		e_float(E_INT_RANGE,f);
		return INT_MAX;
	}
	
	if (f >= 0) return (int) (f+0.5);
	else return (int) (f-0.5);
}

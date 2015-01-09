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

#include "systems.h"

void systems_init(World *w) {
	debug_init(w);
	movement_init(w);
	render_init(w);
	gui_init(w);
}

void systems_run(World *w) {
	if (w->tick_this % 1000 < HOA_FRAME_TICKS) debug_run(w);
	movement_run(w);
	render_run(w);
	gui_run(w);
};

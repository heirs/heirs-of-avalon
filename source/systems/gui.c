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

#include "gui.h"

#include "../gfx.h"
#include <SDL.h>

void gui_run(World *w) {
	eid n;
	ScreenPosition *p;
	Sprite *s;
	SDL_Rect r;
	
	for(n = 1; n < HOA_ENTITIES_MAX; n++) {
		if ((w->mask[n] & MASK_GUI) == MASK_GUI) {
			p = &(w->screen_position[n]);
			s = &(w->sprite[n]);
			
			r.x = p->x;
			r.y = p->y;
			r.w = s->sprite->w;
			r.h = s->sprite->h;
			
			// show sprite
			gfx_blit_sprite(w,s->sprite,&r);
		}
	}
}

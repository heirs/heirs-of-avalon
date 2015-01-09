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

#include "debug.h"

void debug_init(World *w) {
	// nothing, yet
}

void debug_run(World *w) {
	eid n;
	Name *a;
	Position *p;
	Velocity *v;
	ScreenPosition *sp;
	
	int win_w,win_h;
	
	SDL_GetWindowSize(w->window,&win_w,&win_h);
	
	printf("\n===== DEBUG FOR TIME %d =====\n",w->tick_this);
	printf("WINDOW IS %d x %d\n",win_w,win_h);
	
	for(n = 1; n < HOA_ENTITIES_MAX; n++) {
		printf("=== ENTITY %d ===\n",n);
		if (w->mask[n] == C_NONE) {
			printf("\tEMPTY\n");
			continue;
		}
		
		if ((w->mask[n] & C_NAME) == C_NAME) {
			a = &(w->name[n]);
			printf("\tNAME: %s\n",a->name);
		}
		
		if ((w->mask[n] & C_POSITION) == C_POSITION) {
			p = &(w->position[n]);
			printf("\tPOSITION: %.2f,%.2f,%.2f\n",
				p->x,
				p->y,
				p->z);
		}
		
		if ((w->mask[n] & C_VELOCITY) == C_VELOCITY) {
			v = &(w->velocity[n]);
			printf("\tVELOCITY: %.2f,%.2f,%.2f\n",
				v->x,
				v->y,
				v->z);
		}
		
		if ((w->mask[n] & C_SCREEN_POSITION) == C_SCREEN_POSITION) {
			sp = &(w->screen_position[n]);
			printf("\tSCREEN_POSITION: %d,%d\n",
				sp->x,
				sp->y);
		}
	}
}

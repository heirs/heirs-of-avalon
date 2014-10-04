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
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ===== Readline Includes ===== //
#include <readline/readline.h>
#include <readline/history.h>

// ===== Project Includes ===== //
#include "includes.h"
#include "systems.h"

void world_init(World *w) {
	int i;
	
	for(i=0;i<HOA_ENTITIES_MAX;i++) w->mask[i] = C_NONE;
}

eid entity_create(World *w) {
	eid n;
	
	for(n = 1; n < HOA_ENTITIES_MAX; n++) {
		if (w->mask[n] == C_NONE) {
			return n;
		}
	}
	
	e_none(E_ENTITY_LIMIT);
	return 0;
}

void entity_destroy(World *w, eid n) {
	if (n >= HOA_ENTITIES_MAX) {
		e_int(E_ENTITY_INVALID,n);
	}
	
	w->mask[n] = C_NONE;
}

eid create_tile(World *w, float x, float y, char *name) {
	eid n = entity_create(w);
	
	w->mask[n] = C_POSITION | C_NAME;
	
	w->position[n].x = x;
	w->position[n].y = y;
	
	w->name[n].name = name;
	
	return n;
}

eid create_unit(World *w, float p_x, float p_y, float v_x, float v_y, char *name) {
	eid n = entity_create(w);
	
	w->mask[n] = C_POSITION | C_VELOCITY | C_NAME;
	
	w->position[n].x = p_x;
	w->position[n].y = p_y;
	
	w->velocity[n].x = v_x;
	w->velocity[n].y = v_y;
	
	w->name[n].name = name;
	
	return n;
}

/* ===== MAIN ===== */
int main(/*int argc,char* argv[]*/) {
	World world;
	int i,limit,tick;
	eid ns[5];
	
	world_init(&world);
	
	printf("Welcome, Players\n");

	ns[0] = create_tile(&world,1,1,"sand");
	ns[1] = create_tile(&world,1,2,"sand");
	ns[2] = create_tile(&world,2,1,"water");
	ns[3] = create_tile(&world,2,2,"water");
	ns[4] = create_unit(&world,0,0,0.02,0.02,"deer");
	
	limit = 1000;
	tick = 0;
	while(tick++ < limit) {
		systems_run(&world);
	
		if (tick % 97 == 0) {
			printf("===== TICK %3d =====\n",tick);
			for(i=0; i < 5; i++) {
				printf("Entity #%d is %s at (%.2f,%.2f)\n",
					ns[i],
					world.name[ns[i]].name,
					world.position[ns[i]].x,
					world.position[ns[i]].y);
			}
		}
	}
	
	printf("Goodbye.\n");
	return 0;
}
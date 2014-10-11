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

// ===== Project Includes ===== //
#include "includes.h"
#include "systems.h"

// hacks
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void world_load(World *w) {
	int i;
	
	for(i=0;i<HOA_ENTITIES_MAX;i++) {
		w->mask[i] = C_NONE;
		w->sprite[i].sprite = NULL;
	}
	
	w->window = NULL;
	w->screen = NULL;
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0) return e_const(E_SDL,SDL_GetError());
	
	if ((IMG_Init(HOA_IMG_FLAGS) & HOA_IMG_FLAGS) != HOA_IMG_FLAGS)
		return e_const(E_SDL_IMG,IMG_GetError());
	
	w->window = SDL_CreateWindow(
		"Heirs of Avalon",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
	);
	
	if (w->window == NULL) return e_const(E_SDL,SDL_GetError());
	
	w->screen = SDL_GetWindowSurface(w->window);
	
	if (w->screen == NULL) return e_const(E_SDL,SDL_GetError());
}

void world_unload(World *w) {
	int i;
	
	for(i=0;i<HOA_ENTITIES_MAX;i++) {
		if(w->mask[i] != C_NONE) {
			SDL_FreeSurface(w->sprite[i].sprite);
			w->sprite[i].sprite = NULL;
		}
	}
	
	SDL_DestroyWindow(w->window);
	w->window = NULL;
	w->screen = NULL;
	
	IMG_Quit();
	SDL_Quit();
};

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

eid create_gui(World *w, float x, float y, const char *path) {
	eid n = entity_create(w);
	
	w->mask[n] = C_SCREEN_POSITION | C_SPRITE | C_NAME;
	
	w->screen_position[n].x = x;
	w->screen_position[n].y = y;
	
	w->name[n].name = "gui";
	
	w->sprite[n].sprite = gfx_load_asset(path);
	
	return n;
}

eid create_tile(World *w, float x, float y, char *name) {
	const char *asset;
	
	eid n = entity_create(w);
	
	w->mask[n] = C_POSITION | C_SPRITE | C_NAME;
	
	w->position[n].x = x;
	w->position[n].y = y;
	
	w->name[n].name = name;
	
	if (!strcmp(name,"grass")) asset = "assets/tiles/grass0_45.png";
	else asset = "assets/tiles/shallow0_45.png";
	w->sprite[n].sprite = gfx_load_asset(asset);
	
	return n;
}

eid create_unit(World *w, float p_x, float p_y, float v_x, float v_y, char *name) {
	eid n = entity_create(w);
	
	w->mask[n] = C_POSITION | C_SPRITE | C_VELOCITY | C_NAME;
	
	w->position[n].x = p_x;
	w->position[n].y = p_y;
	
	w->velocity[n].x = v_x;
	w->velocity[n].y = v_y;
	
	w->name[n].name = name;
	
	w->sprite[n].sprite = gfx_load_asset("assets/green_dot.bmp");
	
	return n;
}

/* ===== MAIN ===== */
int main(/*int argc,char* argv[]*/) {
	World world;
// 	int i,limit,tick;
// 	eid ns[7];
	bool quit = false;
	SDL_Event ev;
	err e = 0;
	
	world_load(&world);
	
	printf("Welcome, Players\n");

	create_tile(&world,1,1,"grass");
	create_tile(&world,1,2,"grass");
	create_tile(&world,2,1,"water");
	create_tile(&world,2,2,"water");
	
	create_unit(&world,0,0,0.002,0.002,"deer");
	
	create_gui(&world,10,10,"assets/green_dot.bmp");
	create_gui(&world,100,200,"assets/green_dot.bmp");
	
	while (!quit) {
		while (SDL_PollEvent(&ev) != 0) {
			if (ev.type == SDL_QUIT) quit = true;
		}
		
		world.screen = SDL_GetWindowSurface(world.window);
		
		systems_run(&world);
		
		e = SDL_UpdateWindowSurface(world.window);
		if (e != 0) e_const(E_SDL,SDL_GetError());
	}
// 	limit = 1000;
// 	tick = 0;
// 	while(tick++ < limit) {
// 		systems_run(&world);
// 	
// 		if (tick % 97 == 0 && 0) {
// 			printf("===== TICK %3d =====\n",tick);
// 			for(i=0; i < 5; i++) {
// 				printf("Entity #%d is %s at (%.2f,%.2f)\n",
// 					ns[i],
// 					world.name[ns[i]].name,
// 					world.position[ns[i]].x,
// 					world.position[ns[i]].y);
// 			}
// 		}
// 	}
	
	world_unload(&world);
	
	printf("Goodbye.\n");
	return 0;
}
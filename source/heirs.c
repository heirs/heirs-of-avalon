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
#include <stdbool.h>
#include <stdio.h>

// ===== SDL Includes ===== //
#include <SDL.h>
#include <SDL_image.h>

// ===== OpenGL Includes ===== //

#define GL3_PROTOTYPES
#include <gl.h>

// ===== Project Includes ===== //
#include "components.h"
#include "defines.h"
#include "errors.h"
#include "gfx.h"
#include "systems.h"
#include "util.h"

// hacks
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

void world_load(World *w) {
	int i;
	
	for(i=0;i<HOA_ENTITIES_MAX;i++) {
		w->mask[i] = C_NONE;
		w->sprite[i].sprite = NULL;
	}
	
	w->window = NULL;
	w->screen = NULL;
	
	w->tick_last = 0;
	w->tick_this = 0;
	w->delta_ticks = 0;
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0) return e_const(E_SDL,SDL_GetError());
	
	if ((IMG_Init(HOA_IMG_FLAGS) & HOA_IMG_FLAGS) != HOA_IMG_FLAGS)
		return e_const(E_SDL_IMG,IMG_GetError());
	
	/* Request opengl 3.2 context.
	 * SDL doesn't have the ability to choose which profile at this time of writing,
	 * but it should default to the core profile */
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	/* Enable multisampling for a nice antialiased effect */
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	
	/* Turn on double buffering with a 24bit Z buffer.
	 * You may need to change this to 16 or 32 for your system */
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	
	w->window = SDL_CreateWindow(
		"Heirs of Avalon",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);
	
	if (w->window == NULL) return e_const(E_SDL,SDL_GetError());
	
	w->context = SDL_GL_CreateContext(w->window);
	
	// check context error?
	/* Enable Z depth testing so objects closest to the viewpoint are in front of objects further away */
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	 /* This makes our buffer swap syncronized with the monitor's vertical refresh */
    SDL_GL_SetSwapInterval(1);
}

void world_unload(World *w) {
	int i;
	
	for(i=0;i<HOA_ENTITIES_MAX;i++) {
		if(w->mask[i] != C_NONE) {
			SDL_FreeSurface(w->sprite[i].sprite);
			w->sprite[i].sprite = NULL;
		}
	}
	
	SDL_GL_DeleteContext(w->context);
	
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

eid create_gui(World *w, float x, float y, float rgb[3], const char *path) {
	eid n = entity_create(w);
	
	w->mask[n] = C_SCREEN_POSITION | C_SPRITE | C_NAME;
	
	w->screen_position[n].x = x;
	w->screen_position[n].y = y;
	
	w->name[n].name = "gui";
	
	w->sprite[n].rgb[0] = rgb[0];
	w->sprite[n].rgb[1] = rgb[1];
	w->sprite[n].rgb[2] = rgb[2];
	w->sprite[n].sprite = util_load_asset_img(path);
	
	return n;
}

eid create_tile(World *w, float x, float y, char *name) {
	const char *asset;
	
	eid n = entity_create(w);
	
	w->mask[n] = C_POSITION | C_SPRITE | C_NAME;
	
	w->position[n].x = x;
	w->position[n].y = y;
	
	w->name[n].name = name;
	
	if (!strcmp(name,"grass")) {
		asset = "assets/tiles/grass0_45.png";
		w->sprite[n].rgb[0] = 0.5;
		w->sprite[n].rgb[1] = 1.5;
		w->sprite[n].rgb[2] = 0.5;
	}
	else {
		asset = "assets/tiles/shallow0_45.png";
		w->sprite[n].rgb[0] = 0.5;
		w->sprite[n].rgb[1] = 0.5;
		w->sprite[n].rgb[2] = 1.0;
	}
	w->sprite[n].sprite = util_load_asset_img(asset);
	
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
	
	w->sprite[n].sprite = util_load_asset_img("assets/green_dot.bmp");
	
	return n;
}

/* ===== MAIN ===== */
int main(/*int argc,char* argv[]*/) {
	World world;
	bool quit = false;
	SDL_Event ev;
	
	world_load(&world);
	
	printf("Welcome, Players\n");
	
	create_tile(&world,1,1,"grass");
	create_tile(&world,1,2,"grass");
	create_tile(&world,2,1,"water");
	create_tile(&world,2,2,"water");
	
	create_unit(&world,0,0,0.002,0.002,"deer");
	
	float red[3]   = { 1.0, 0.5, 0.5 };
	float green[3] = { 0.5, 1.0, 0.5 };
	float blue[3]  = { 0.5, 0.5, 1.0 };
	create_gui(&world,0,0,red,"assets/green_dot.bmp");
	create_gui(&world,10,10,blue,"assets/green_dot.bmp");
	create_gui(&world,100,200,green,"assets/green_dot.bmp");
	
	while (!quit) {
		while (SDL_PollEvent(&ev) != 0) {
			if (ev.type == SDL_QUIT) quit = true;
		}
		
		world.screen = SDL_GetWindowSurface(world.window);
		world.tick_last = world.tick_this;
		world.tick_this = SDL_GetTicks();
		world.delta_ticks += world.tick_this - world.tick_last;
		
		systems_run(&world);
		SDL_GL_SwapWindow(world.window);
// 		e = SDL_UpdateWindowSurface(world.window);
// 		if (e != 0) e_const(E_SDL,SDL_GetError());
	}
	
	world_unload(&world);
	
	printf("Goodbye.\n");
	return 0;
}

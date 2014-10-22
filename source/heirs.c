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

void world_load(World *W) {
	int i;
	
	for(i=0;i<HOA_ENTITIES_MAX;i++) {
		W->mask[i] = C_NONE;
		W->sprite[i].sprite = 0;
	}
	
	W->window = NULL;
	W->screen = NULL;
	
	W->tick_last = 0;
	W->tick_this = 0;
	W->delta_ticks = 0;
	
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
	
	W->window = SDL_CreateWindow(
		"Heirs of Avalon",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);
	
	if (W->window == NULL) return e_const(E_SDL,SDL_GetError());
	
	W->context = SDL_GL_CreateContext(W->window);
	
	// check context error?
	/* Enable Z depth testing so objects closest to the viewpoint are in front of objects further away */
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	 /* This makes our buffer swap syncronized with the monitor's vertical refresh */
    SDL_GL_SetSwapInterval(1);
}

void world_unload(World *W) {
	int i;
	
	for(i=0;i<HOA_ENTITIES_MAX;i++) {
		if(W->mask[i] != C_NONE) {
			gfx_free_asset_img(W->sprite[i].sprite);
			W->sprite[i].sprite = 0;
		}
	}
	
	SDL_GL_DeleteContext(W->context);
	
	SDL_DestroyWindow(W->window);
	W->window = NULL;
	W->screen = NULL;
	
	IMG_Quit();
	SDL_Quit();
};

eid entity_create(World *W) {
	eid n;
	
	for(n = 1; n < HOA_ENTITIES_MAX; n++) {
		if (W->mask[n] == C_NONE) {
			return n;
		}
	}
	
	e_none(E_ENTITY_LIMIT);
	return 0;
}

void entity_destroy(World *W, eid n) {
	if (n >= HOA_ENTITIES_MAX) {
		e_int(E_ENTITY_INVALID,n);
	}
	
	W->mask[n] = C_NONE;
}

eid create_gui(World *W, int x, int y, int w, int h, float rgb[3], const char *path) {
	eid n = entity_create(W);
	
	W->mask[n] = C_SCREEN_POSITION | C_SPRITE | C_NAME;
	
	W->screen_position[n].x = x;
	W->screen_position[n].y = y;
	W->screen_position[n].w = w;
	W->screen_position[n].h = h;
	
	W->name[n].name = "gui";
	
	W->sprite[n].rgb[0] = rgb[0];
	W->sprite[n].rgb[1] = rgb[1];
	W->sprite[n].rgb[2] = rgb[2];
	W->sprite[n].sprite = gfx_load_asset_img(path);
	
	return n;
}

eid create_tile(World *W, float x, float y, char *name) {
	const char *asset;
	
	eid n = entity_create(W);
	
	W->mask[n] = C_POSITION | C_SPRITE | C_NAME;
	
	W->position[n].x = x;
	W->position[n].y = y;
	
	W->name[n].name = name;
	
	if (!strcmp(name,"grass")) {
		asset = "assets/tiles/grass0_45.png";
		W->sprite[n].rgb[0] = 0.5;
		W->sprite[n].rgb[1] = 1.5;
		W->sprite[n].rgb[2] = 0.5;
	}
	else {
		asset = "assets/tiles/shallow0_45.png";
		W->sprite[n].rgb[0] = 0.5;
		W->sprite[n].rgb[1] = 0.5;
		W->sprite[n].rgb[2] = 1.0;
	}
	W->sprite[n].sprite = gfx_load_asset_img(asset);
	
	return n;
}

eid create_unit(World *W, float p_x, float p_y, float v_x, float v_y, char *name) {
	eid n = entity_create(W);
	
	W->mask[n] = C_POSITION | C_SPRITE | C_VELOCITY | C_NAME;
	
	W->position[n].x = p_x;
	W->position[n].y = p_y;
	
	W->velocity[n].x = v_x;
	W->velocity[n].y = v_y;
	
	W->name[n].name = name;
	
	W->sprite[n].sprite = gfx_load_asset_img("assets/green_dot.bmp");
	
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
	create_gui(&world,0,0,64,64,red,"assets/green_dot.bmp");
	create_gui(&world,10,10,64,64,blue,"assets/green_dot.bmp");
	create_gui(&world,100,200,64,64,green,"assets/green_dot.bmp");
	
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

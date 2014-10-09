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

void gfx_blit_sprite(World *w,GFX *sprite) {
	err e = 0;
	
	e = SDL_BlitSurface(sprite,NULL,w->screen,NULL);
	
	if (e != 0) e_const(E_SDL,SDL_GetError());
}

GFX* gfx_load_asset(const char *path) {
	GFX *sprite = SDL_LoadBMP(path);
	
	if (sprite == NULL) e_const(E_ASSET_INVALID,SDL_GetError());
	
	return sprite;
};

void gui_run(World *w) {
	eid n;
	ScreenPosition *p;
	Sprite *s;
	
	for(n = 1; n < HOA_ENTITIES_MAX; n++) {
		if ((w->mask[n] & MASK_GUI) == MASK_GUI) {
			p = &(w->screen_position[n]);
			s = &(w->sprite[n]);
			
			// show sprite
			gfx_blit_sprite(w,s->sprite);
		}
	}
}

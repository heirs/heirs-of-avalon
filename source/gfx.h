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

#ifndef HOA_GFX_H
#define HOA_GFX_H

#include "components.h"
#include "defines.h"
#include "gl.h"
#include <SDL.h>

int gfx_compile_shader(GLuint *shader,GLenum type,const GLchar *source);
void gfx_draw_3d(World *w,GLfloat *rgb,SDL_Rect *r,GLuint texture);
void gfx_blit_sprite(World *w,SDL_Surface *sprite,SDL_Rect *r);
GLuint gfx_load_asset_img(const char *path);
void gfx_free_asset_img(GLuint texture);

#endif // HOA_GFX_H

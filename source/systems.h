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

#ifndef HOA_SYSTEMS_H
#define HOA_SYSTEMS_H

#include "components.h"
#include "defines.h"

#include "systems/debug.h"
#include "systems/gui.h"
#include "systems/movement.h"
#include "systems/render.h"

void systems_init(World *w);
void systems_run(World *w);

#endif // HOA_SYSTEMS_H

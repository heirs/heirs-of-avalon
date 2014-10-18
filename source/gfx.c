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

#include "gfx.h"

#include "errors.h"
#include "gl.h"

const char *shader_frag =
	"#version 150\n"
	"precision highp float;"
	"in  vec3 ex_Color;"
	"out vec4 gl_FragColor;"
	"void main(void) { "
	"	gl_FragColor = vec4(ex_Color,1.0);"
	"}";

const char *shader_vertex =
	"#version 150\n"
	"in  vec2 in_Position;"
	"in  vec3 in_Color;"
	"out vec3 ex_Color;"
	"void main(void) {"
	"	gl_Position = vec4(in_Position.x, in_Position.y, 0.0, 1.0);"
	"	ex_Color = in_Color;"
	"}";

int gfx_compile_shader(GLuint *shader,GLenum type,const GLchar *source) {
	int compiled,max_len;
	char *err_msg;
	
	*shader = glCreateShader(type);
	
	glShaderSource(*shader,1,&source,0);
	glCompileShader(*shader);
	
	glGetShaderiv(*shader, GL_COMPILE_STATUS, &compiled);
	
	if(compiled == GL_FALSE) {
		glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &max_len);

		/* The maxLength includes the NULL character */
		err_msg = (char *)malloc(max_len);

		glGetShaderInfoLog(*shader, max_len, &max_len, err_msg);

		/* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
		/* In this simple program, we'll just leave */
		e_const(E_ASSET_INVALID,err_msg);
		free(err_msg);
		return 1;
	}
	
	return 0;
}

void gfx_draw_3d(World *w) {
	int win_w,win_h;
	SDL_GetWindowSize(w->window,&win_w,&win_h);
	
	int i;
	GLuint vao, vbo[2]; /* Create handles for our Vertex Array Object and two Vertex Buffer Objects */
	int IsLinked;
	int maxLength;
	char *shaderProgramInfoLog;

	/* We're going to create a simple diamond made from lines */
	const GLfloat diamond[4][2] = {
		{  0.2, -0.2  },
		{ -0.2, -0.2  },
		{  0.2,  0.2  },
		{ -0.2,  0.2  } };

	const GLfloat colors[4][3] = {
		{  1.0,  0.0,  0.0  }, /* Red */
		{  0.0,  1.0,  0.0  }, /* Green */
		{  0.0,  0.0,  1.0  }, /* Blue */
		{  1.0,  1.0,  1.0  } }; /* White */
 
/* These pointers will receive the contents of our shader source code files */
//     GLchar *vertexsource, *fragmentsource;
 
    /* These are handles used to reference the shaders */
    GLuint vertexshader, fragmentshader;
 
    /* This is a handle to the shader program */
    GLuint shaderprogram;
 
    /* Allocate and assign a Vertex Array Object to our handle */
    glGenVertexArrays(1, &vao);
 
    /* Bind our Vertex Array Object as the current used object */
    glBindVertexArray(vao);
 
    /* Allocate and assign two Vertex Buffer Objects to our handle */
    glGenBuffers(2, vbo);
 
    /* Bind our first VBO as being the active buffer and storing vertex attributes (coordinates) */
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
 
    /* Copy the vertex data from diamond to our buffer */
    /* 8 * sizeof(GLfloat) is the size of the diamond array, since it contains 8 GLfloat values */
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), diamond, GL_STATIC_DRAW);
 
    /* Specify that our coordinate data is going into attribute index 0, and contains two floats per vertex */
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
 
    /* Enable attribute index 0 as being used */
    glEnableVertexAttribArray(0);
 
    /* Bind our second VBO as being the active buffer and storing vertex attributes (colors) */
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
 
    /* Copy the color data from colors to our buffer */
    /* 12 * sizeof(GLfloat) is the size of the colors array, since it contains 12 GLfloat values */
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
 
    /* Specify that our color data is going into attribute index 1, and contains three floats per vertex */
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
 
    /* Enable attribute index 1 as being used */
    glEnableVertexAttribArray(1);

	int err = gfx_compile_shader(&vertexshader,GL_VERTEX_SHADER,shader_vertex);
	if (err) {
		e_const(E_ASSET_INVALID,"vertex shader");
		e_int(E_ASSET_INVALID,err);
	}

	err = gfx_compile_shader(&fragmentshader,GL_FRAGMENT_SHADER,shader_frag);
	if (err) {
		e_const(E_ASSET_INVALID,"fragment shader");
		e_int(E_ASSET_INVALID,err);
	}


    /* If we reached this point it means the vertex and fragment shaders compiled and are syntax error free. */
    /* We must link them together to make a GL shader program */
    /* GL shader programs are monolithic. It is a single piece made of 1 vertex shader and 1 fragment shader. */
    /* Assign our program handle a "name" */
    shaderprogram = glCreateProgram();
 
    /* Attach our shaders to our program */
    glAttachShader(shaderprogram, vertexshader);
    glAttachShader(shaderprogram, fragmentshader);
 
    /* Bind attribute index 0 (coordinates) to in_Position and attribute index 1 (color) to in_Color */
    /* Attribute locations must be setup before calling glLinkProgram. */
    glBindAttribLocation(shaderprogram, 0, "in_Position");
    glBindAttribLocation(shaderprogram, 1, "in_Color");
 
    /* Link our program */
    /* At this stage, the vertex and fragment programs are inspected, optimized and a binary code is generated for the shader. */
    /* The binary code is uploaded to the GPU, if there is no error. */
    glLinkProgram(shaderprogram);
 
    /* Again, we must check and make sure that it linked. If it fails, it would mean either there is a mismatch between the vertex */
    /* and fragment shaders. It might be that you have surpassed your GPU's abilities. Perhaps too many ALU operations or */
    /* too many texel fetch instructions or too many interpolators or dynamic loops. */
 
    glGetProgramiv(shaderprogram, GL_LINK_STATUS, (int *)&IsLinked);
    if(IsLinked == GL_FALSE)
    {
       /* Noticed that glGetProgramiv is used to get the length for a shader program, not glGetShaderiv. */
       glGetProgramiv(shaderprogram, GL_INFO_LOG_LENGTH, &maxLength);
 
       /* The maxLength includes the NULL character */
       shaderProgramInfoLog = (char *)malloc(maxLength);
 
       /* Notice that glGetProgramInfoLog, not glGetShaderInfoLog. */
       glGetProgramInfoLog(shaderprogram, maxLength, &maxLength, shaderProgramInfoLog);
 
       /* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
       /* In this simple program, we'll just leave */
	   e_const(E_ASSET_INVALID,shaderProgramInfoLog);
       free(shaderProgramInfoLog);
       return;
    }
 
    /* Load the shader into the rendering pipeline */
    glUseProgram(shaderprogram);
 
    /* Loop our display increasing the number of shown vertexes each time. */


//         glClearColor(0.0, 0.0, 0.0, 1.0);
//         glClear(GL_COLOR_BUFFER_BIT);
// 	for (i=3; i <= 6; i+=3) {

 
        /* Invoke glDrawArrays telling that our data is a line loop and we want to draw 2-4 vertexes */
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
 
        /* Swap our buffers to make our changes visible */
//         printf("SWAPPING %d\n",4);
// 		SDL_GL_SwapWindow(w->window);
//     }
 
    /* Cleanup all the things we bound and allocated */
    glUseProgram(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDetachShader(shaderprogram, vertexshader);
    glDetachShader(shaderprogram, fragmentshader);
    glDeleteProgram(shaderprogram);
    glDeleteShader(vertexshader);
    glDeleteShader(fragmentshader);
    glDeleteBuffers(2, vbo);
    glDeleteVertexArrays(1, &vao);
//     free(vertexsource);
//     free(fragmentsource);
}

void gfx_blit_sprite(World *w,SDL_Surface *sprite,SDL_Rect *r) {
	err e = 0;
	
	e = SDL_BlitSurface(sprite,NULL,w->screen,r);
	
	if (e != 0) e_const(E_SDL,SDL_GetError());
}

#include <GL/glew.h>
#include <GL/gl.h>
#include <stdio.h>
#include "shaders.h"
#include "cmd_textures.h"

GLubyte tunnel_tex_data[] =
"\255\255\255\255"
"\255\0\0\255"
"\0\255\0\255"
"\0\0\255\255";

#define NUM_TEXTURES 4
GLuint textures[NUM_TEXTURES];

/* Convenience fn for building a shader from source.
 * type must be GL_VERTEX_SHADER or GL_FRAGMENT_SHADER.
 * Single source string, NULL-terminated.
 */
GLuint buildShader(GLenum type, const char *source, const char *name)
{
	char log[1024];
	GLuint handle = glCreateShader(type);
	glShaderSource(handle, 1, &source, NULL);
	glCompileShader(handle);
	// more error checking would be good, for example only print the log on failure
	glGetShaderInfoLog(handle, 1024, NULL, log);
	printf("%s shader compile log: %s\n", name, log);
	return handle;
}

/* Convenience fn for building a program from shaders.
   demands one vertex shader and one fragment shader, compiled.
 */
GLuint buildProgram(GLuint vshad, GLuint fshad, const char *name)
{
	char log[1024];
	GLuint handle = glCreateProgram();
	glAttachShader(handle, vshad);
	glAttachShader(handle, fshad);
	glLinkProgram(handle);
	// more error checking would be good, for example only print the log on failure
	glGetProgramInfoLog(handle, 1024, NULL, log);
	printf("%s program link log: %s\n", name, log);
	return handle;
}

void textures_init()
{
	glGenTextures(NUM_TEXTURES, textures);
	
	/* CMD Icon Texture */
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, icon_image.width,
			icon_image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
			(GLubyte *)icon_image.pixel_data);

	/* CMD X Texture */
	GLubyte xTexData[8*8*4];
	for(int i = 0; i < 8*8; i++) {
		bool dat = x_bitmap[i>>3]&(1<<(i&7));
		xTexData[i<<2] = dat?0:236;
		xTexData[(i<<2)+1] = dat?0:233;
		xTexData[(i<<2)+2] = dat?0:216;
		xTexData[(i<<2)+3] = 255;
	}
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 8, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, (GLubyte *)xTexData);

	/* CMD Arrow Texture */
	GLubyte upTexData[4*8*4];
	for(int i = 0; i < 4*8; i++) {
		bool dat = arrow_bitmap[i>>3]&(1<<(i&7));
		upTexData[i<<2] = dat?0:236;
		upTexData[(i<<2)+1] = dat?0:233;
		upTexData[(i<<2)+2] = dat?0:216;
		upTexData[(i<<2)+3] = 255;
	}
	glBindTexture(GL_TEXTURE_2D, textures[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 4, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, (GLubyte *)upTexData);
	
	glBindTexture(GL_TEXTURE_2D, textures[3]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (GLubyte *)tunnel_tex_data);
}

#include <GL/glew.h>
#include <GL/gl.h>
#include <stdio.h>
#include "shaders.h"

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

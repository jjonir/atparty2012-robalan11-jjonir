#pragma once

#include <GL/gl.h>

extern const char *plasma_vshad, *plasma_fshad;
extern const char *tunnel_vshad, *tunnel_fshad;
extern const char *cmd_vshad, *cmd_fshad;
extern GLuint textures[];

extern GLuint plasma_program, tunnel_program, cmd_program;

GLuint buildShader(GLenum type, const char *source, const char *name);
GLuint buildProgram(GLuint vshad, GLuint fshad, const char *name);

void textures_init(void);

void plasma_init(void);
void plasma_render(void);
void plasma_animate(int val);

void tunnel_init(void);
void tunnel_render(void);
void tunnel_animate(int val);

void cmd_init(void);
void cmd_render(void);
void cmd_animate(int val);

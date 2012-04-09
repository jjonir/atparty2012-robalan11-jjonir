#pragma once

#include <GL/gl.h>

#define CONSOLE_PIXELS_W 640.0
#define CONSOLE_PIXELS_H 350.0
#define TOP_BORDER 32.0
#define LEFT_BORDER 6.0
#define RIGHT_BORDER 23.0
#define BOTTOM_BORDER 6.0

#define DESKTOP_TEXTURE 0
#define CMD_ICON_TEXTURE 1
#define CMD_X_TEXTURE 2
#define CMD_ARROW_TEXTURE 3
#define TUNNEL_TEXTURE 4
#define NUM_TEXTURES 5

extern float t0;

extern const char *plasma_vshad, *plasma_fshad;
extern const char *tunnel_vshad, *tunnel_fshad;
extern const char *cmd_vshad, *cmd_fshad;
extern GLuint textures[];

extern GLuint plasma_program, tunnel_program, cmd_program;

GLuint buildShader(GLenum type, const char *source, const char *name);
GLuint buildProgram(GLuint vshad, GLuint fshad, const char *name);

float demo_get_time(void);

void grab_screen(void);
void textures_init(void);

void plasma_init(void);
void plasma_render(void);
void plasma_animate(int val);

void tunnel_init(void);
void tunnel_render(void);
void tunnel_animate(int val);

void cmd_init(void);
void cmd_render(float windowW, float windowH, float consoleX, float consoleY, float consoleW, float consoleH);
void cmd_animate(int val);

void cube_init(void);
void cube_render(void);
void cube_animate(int val);
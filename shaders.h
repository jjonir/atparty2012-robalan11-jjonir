#pragma once

#include <GL/gl.h>

#define CONSOLE_PIXELS_W 640.0
#define CONSOLE_PIXELS_H 350.0
#define TOP_BORDER 32.0
#define LEFT_BORDER 6.0
#define RIGHT_BORDER 23.0
#define BOTTOM_BORDER 6.0

enum {
	DESKTOP_TEXTURE = 0,
	CMD_ICON_TEXTURE,
	CMD_X_TEXTURE,
	CMD_ARROW_TEXTURE,
	TUNNEL_TEXTURE,
	ROTO_1_TEXTURE,
	ROTO_2_TEXTURE,
	ROTO_3_TEXTURE,
	ROTO_4_TEXTURE,
	ROTO_5_TEXTURE,
	TITLE_TEXTURE,
	NUM_TEXTURES
};

extern float t0;

extern const char *plasma_vshad, *plasma_fshad;
extern const char *tunnel_vshad, *tunnel_fshad;
extern const char *cmd_vshad, *cmd_fshad;
extern const char *cubes_vshad, *cubes_fshad;
extern const char *roto_vshad, *roto_fshad;
extern GLuint textures[];

extern GLuint plasma_program, tunnel_program, cmd_program, cubes_program, roto_program;

GLuint buildShader(GLenum type, const char *source, const char *name);
GLuint buildProgram(GLuint vshad, GLuint fshad, const char *name);

float demo_get_time(void);

void grab_screen(void);
void cmd_textures_init(void);
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

void cubes_init(void);
void cubes_render(void);
void cubes_animate(int val);

void roto_init(void);
void roto_render(void);
void roto_animate(int val);

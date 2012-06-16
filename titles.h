#pragma once

#include <gl/glew.h>
#include <stdint.h>

#define CHARWID 5
#define PIXWID 8
#define CHARHEI 8
#define PIXHEI 14
#define STRHEI (CHARHEI * PIXHEI)

typedef struct chartexture {
	GLubyte bytes[CHARHEI*PIXHEI][CHARWID*PIXWID][4];
} chartexture_t;

typedef struct charbmp {
	uint8_t cols[CHARWID];
} charbmp_t;

extern GLubyte *partytex;
extern GLubyte *namestex;
extern GLubyte *titletex;
extern int partytexwid;
extern int namestexwid;
extern int titletexwid;

void titles_init(void);

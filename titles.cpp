#include <gl/glew.h>
#include <string.h>
#include <stdlib.h>
#include "titles.h"
#include "shaders.h"

charbmp_t upcase[26] = {
	{0x7F, 0x90, 0x90, 0x90, 0x7F},
	{0xFF, 0x91, 0x91, 0x91, 0x6E},
	{0x7E, 0x81, 0x81, 0x81, 0x42},
	{0xFF, 0x81, 0x81, 0x81, 0x7E},
	{0xFF, 0x91, 0x91, 0x91, 0x91},
	{0xFF, 0x90, 0x90, 0x90, 0x90},
	{0x7E, 0x81, 0x81, 0x89, 0x4E},

	{0xFF, 0x10, 0x10, 0x10, 0xFF},
	{0x81, 0x81, 0xFF, 0x81, 0x81},
	{0x81, 0x81, 0xFF, 0x80, 0x80},
	{0xFF, 0x08, 0x18, 0x24, 0xC3},
	{0xFF, 0x01, 0x01, 0x01, 0x01},
	{0xFF, 0x80, 0x7F, 0x80, 0xFF},
	{0xFF, 0x60, 0x18, 0x06, 0xFF},

	{0x7E, 0x81, 0x81, 0x81, 0x7E},
	{0xFF, 0x90, 0x90, 0x90, 0x60},
	{0x7E, 0x81, 0x81, 0x86, 0x7D},
	{0xFF, 0x90, 0x90, 0x9C, 0x63},
	{0x62, 0x91, 0x91, 0x91, 0x4E},
	{0x80, 0x80, 0xFF, 0x80, 0x80},
	{0xFE, 0x01, 0x01, 0x01, 0xFE},

	{0xE0, 0x1C, 0x03, 0x1C, 0xE0},
	{0xFC, 0x03, 0x3C, 0x03, 0xFC},
	{0xC7, 0x28, 0x10, 0x28, 0xC7},
	{0xC0, 0x30, 0x0F, 0x30, 0xC0},
	{0x83, 0x85, 0x99, 0xA1, 0xC1}
};

charbmp_t num[10] = {
	{0x7E, 0x87, 0x99, 0xE1, 0x7E},
	{0x01, 0x41, 0xFF, 0x01, 0x01},
	{0x81, 0x83, 0x85, 0x99, 0x61}
};

charbmp_t at[1] = {
	{0x7E, 0x81, 0x9D, 0x7C}
};

charbmp_t space[1] = {
	{0x00, 0x00, 0x00, 0x00}
};

chartexture_t chartextures[128];

GLubyte *partytex;
GLubyte *namestex;
GLubyte *titletex;
GLubyte *endtex;
int partytexwid;
int namestexwid;
int titletexwid;
int endtexwid;

static void assemble_charbmp(unsigned char i, charbmp_t *bmps, unsigned char base);
static void assemble_charbmps(void);
static int build_string_tex(const char *s, GLubyte **tex);

void titles_init(void) {
	assemble_charbmps();
	partytexwid = build_string_tex("@PARTY 2012", &partytex);

	glBindTexture(GL_TEXTURE_2D, textures[PARTY_TEXTURE]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // TODO not repeat
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, partytexwid, STRHEI, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, partytex);

	namestexwid = build_string_tex("PENDERPRODS", &namestex);
	glBindTexture(GL_TEXTURE_2D, textures[NAMES_TEXTURE]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // TODO not repeat
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, namestexwid, STRHEI, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, namestex);

	titletexwid = build_string_tex("UNTITLED 0", &titletex);
	glBindTexture(GL_TEXTURE_2D, textures[TITLE_TEXTURE]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // TODO not repeat
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, titletexwid, STRHEI, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, titletex);

	endtexwid = build_string_tex("END", &endtex);
	glBindTexture(GL_TEXTURE_2D, textures[END_TEXTURE]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // TODO not repeat
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, endtexwid, STRHEI, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, endtex);
}

void assemble_charbmp(unsigned char i, charbmp_t *bmps, unsigned char base) {
	GLubyte r_on=0xFF, g_on=0xFF, b_on=0xFF, a_on=0xFF;	// white
	GLubyte r_off=0, g_off=0, b_off=0, a_off=0;			// black, transparent
	GLubyte r, g, b, a;
	for(int x = 0; x < CHARWID; x++) {
		for(int y = 0; y < CHARHEI; y++) {
			if(((bmps[i-base].cols[x] >> y) & 0x01) == 1) {
				r = r_on; g = g_on; b = b_on; a = a_on;
			} else {
				r = r_off; g = g_off; b = b_off; a = a_off;
			}
			for(int yy = 0; yy < PIXHEI; yy++) {
				for(int xx = 0; xx < PIXWID; xx++) {
					int xs = xx + x * PIXWID;
					int ys = yy + y * PIXHEI;
					chartextures[i].bytes[ys][xs][0] = r;
					chartextures[i].bytes[ys][xs][1] = g;
					chartextures[i].bytes[ys][xs][2] = b;
					chartextures[i].bytes[ys][xs][3] = a;
				}
			}
		}
	}
}

void assemble_charbmps(void) {
	for(unsigned char i = 'A'; i <= 'Z'; i++) {
		assemble_charbmp(i, upcase, 'A');
	}
	for(unsigned char i = '0'; i <= '9'; i++) {
		assemble_charbmp(i, num, '0');
	}
	assemble_charbmp(' ', space, ' ');
	assemble_charbmp('@', at, '@');
}

int build_string_tex(const char *s, GLubyte **tex) {
	int rows = CHARHEI * PIXHEI;
	int row_size = CHARWID * PIXWID;
	int full_row_size = (CHARWID+1) * PIXWID;
	int tex_wid = full_row_size * strlen(s);
	int tex_hei = CHARHEI * PIXHEI;
	*tex = (GLubyte *)malloc(tex_wid * tex_hei * 4 * sizeof(GLubyte));

	for(unsigned int c = 0; c < strlen(s); c++) {
		for(int row = 0; row < tex_hei; row++) {
			int col;
			for(col = 0; col < row_size; col++) {
				memcpy(&(*tex)[(row*tex_wid + c*full_row_size + col) * 4], chartextures[s[c]].bytes[row][col], 4);
			}
			for(; col < full_row_size; col++) {
				memset(&(*tex)[(row*tex_wid + c*full_row_size + col) * 4], 0, 4);
			}
		}
	}

	return tex_wid;
}

#include <gl/glew.h>
#include <gl/glut.h>
#include <stdlib.h>
#include <stdint.h>
#include "texgen.h"
#include "shaders.h"

static void trifill(GLubyte *tex, GLubyte r, GLubyte g, GLubyte b,
					int16_t w, int16_t h, int16_t x1, int16_t y1,
					int16_t x2, int16_t y2, int16_t x3, int16_t y3);
static void quadfill(GLubyte *tex, GLubyte r, GLubyte g, GLubyte b,
					int16_t w, int16_t h, int16_t x1, int16_t y1,
					int16_t x2, int16_t y2, int16_t x3, int16_t y3, int16_t x4, int16_t y4);

void texgen_blank() {
	GLubyte *tex_data = (GLubyte *)malloc(ROTO_TEX_W*ROTO_TEX_H*4*sizeof(GLubyte));
	
	glBindTexture(GL_TEXTURE_2D, textures[ROTO_0_TEXTURE]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ROTO_TEX_W, ROTO_TEX_H, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
}

void texgen_compass() {
	GLubyte *tex_data = (GLubyte *)malloc(ROTO_TEX_W*ROTO_TEX_H*4*sizeof(GLubyte));

	quadfill(tex_data, 0xFF, 0x00, 0x00, ROTO_TEX_W, ROTO_TEX_H,
			ROTO_TEX_W/2, 0,
			ROTO_TEX_W/2-ROTO_TEX_W/8, ROTO_TEX_H/2,
			ROTO_TEX_W/2, ROTO_TEX_H,
			ROTO_TEX_W/2+ROTO_TEX_W/8, ROTO_TEX_H/2);
	quadfill(tex_data, 0x00, 0xFF, 0x00, ROTO_TEX_W, ROTO_TEX_H,
			0, ROTO_TEX_H/2,
			ROTO_TEX_W/2, ROTO_TEX_H/2-ROTO_TEX_H/8,
			ROTO_TEX_W, ROTO_TEX_H/2,
			ROTO_TEX_W/2, ROTO_TEX_H/2+ROTO_TEX_H/8);

	glBindTexture(GL_TEXTURE_2D, textures[ROTO_1_TEXTURE]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ROTO_TEX_W, ROTO_TEX_H, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
}

void texgen_diamond() {
	GLubyte *tex_data = (GLubyte *)malloc(1024*1024*4*sizeof(GLubyte));

	//trifill(
	quadfill(tex_data, 0xFF, 0x80, 0x80, ROTO_TEX_W, ROTO_TEX_H,
			ROTO_TEX_W/2, 0,
			ROTO_TEX_W/2+ROTO_TEX_W/16, ROTO_TEX_H/4,
			ROTO_TEX_W/2, ROTO_TEX_H/2,
			ROTO_TEX_W/2-ROTO_TEX_W/16, ROTO_TEX_H/4);
	quadfill(tex_data, 0x80, 0xFF, 0x80, ROTO_TEX_W, ROTO_TEX_H,
			ROTO_TEX_W/2, ROTO_TEX_H,
			ROTO_TEX_W/2+ROTO_TEX_W/16, ROTO_TEX_H*3/4,
			ROTO_TEX_W/2, ROTO_TEX_H/2,
			ROTO_TEX_W/2-ROTO_TEX_W/16, ROTO_TEX_H*3/4);
	trifill(tex_data, 0x80, 0x80, 0xFF, ROTO_TEX_W, ROTO_TEX_H,
			0, ROTO_TEX_H/2,
			ROTO_TEX_H/4, ROTO_TEX_H/2+ROTO_TEX_H/16,
			ROTO_TEX_W/2, ROTO_TEX_H/2);
	trifill(tex_data, 0x80, 0x80, 0xFF, ROTO_TEX_W, ROTO_TEX_H,
			0, ROTO_TEX_H/2,
			ROTO_TEX_W/4, ROTO_TEX_H/2-ROTO_TEX_H/16,
			ROTO_TEX_W/2, ROTO_TEX_H/2);
	/*
	quadfill(tex_data, 0xFF, 0xD0, 0xD0, ROTO_TEX_W, ROTO_TEX_H,
			512, 0, 512+32, 256, 512+96, 512, 512+96, 256);
	quadfill(tex_data, 0xD0, 0xFF, 0xD0, ROTO_TEX_W, ROTO_TEX_H,
			512, 0, 512-32, 256, 512-96, 512, 512-96, 256);
	trifill(tex_data, 0x0D, 0xD0, 0xFF, ROTO_TEX_W, ROTO_TEX_H,
			512+32, 256, 512, 512, 512+96, 512);
	trifill(tex_data, 0xD0, 0xD0, 0xFF, ROTO_TEX_W, ROTO_TEX_H,
			512-32, 256, 512, 512, 512-96, 512);
	*/

	glBindTexture(GL_TEXTURE_2D, textures[ROTO_2_TEXTURE]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ROTO_TEX_W, ROTO_TEX_H, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
}

// Fill a triangle with a color
static void trifill(GLubyte *tex, GLubyte r, GLubyte g, GLubyte b,
					int16_t w, int16_t h, int16_t x1, int16_t y1,
					int16_t x2, int16_t y2, int16_t x3, int16_t y3) {
	int16_t temp;
	if(y2 < y1) {
		temp = y1; y1 = y2; y2 = temp;
		temp = x1; x1 = x2; x2 = temp;
	}
	if(y3 < y2) {
		temp = y2; y2 = y3; y3 = temp;
		temp = x2; x2 = x3; x3 = temp;
	}
	if(y2 < y1) {
		temp = y1; y1 = y2; y2 = temp;
		temp = x1; x1 = x2; x2 = temp;
	}

	int16_t dx12, dy12, dx13, dy13, dx23, dy23;
	int32_t incx12, incx13, incx23;

	dy12 = y2 - y1;
	dy13 = y3 - y1;
	dy23 = y3 - y2;
	if(x2 > x1) dx12 = x2 - x1;
	else dx12 = x1 - x2;
	if(x3 > x1) dx13 = x3 - x1;
	else dx13 = x1 - x3;
	if(x3 > x2) dx23 = x3 - x2;
	else dx23 = x2 - x3;

	if(dy12 == 0) incx12 = 0;
	else incx12 = ((int32_t)dx12 << 16) / dy12;
	if(dy13 == 0) incx13 = 0;
	else incx13 = ((int32_t)dx13 << 16) / dy13;
	if(dy23 == 0) incx23 = 0;
	else incx23 = ((int32_t)dx23 << 16) / dy23;

	int16_t x, y;
	int16_t xleft, xright;
	int32_t j12, j13, j23;
	for(y = y1, j12 = 0, j13 = 0; y <= y2; y++, j12 += incx12, j13 += incx13) {
		if(y < 0) continue;
		if(y >= h) break;

		if(x2 > x1) xleft = x1 + (j12 >> 16);
		else xleft = x1 - (j12 >> 16);
		if(x3 > x1) xright = x1 + (j13 >> 16);
		else xright = x1 - (j13 >> 16);
		if(xright < xleft) { temp = xleft; xleft = xright; xright = temp; }

		if(xleft < 0) xleft = 0;
		if(xright >= w) xright = w - 1;

		for(x = xleft; x <= xright; x++) {
			tex[4 * (y * w + x) + 0] = r;
			tex[4 * (y * w + x) + 1] = g;
			tex[4 * (y * w + x) + 2] = b;
			tex[4 * (y * w + x) + 3] = 0xFF;
		}
	}

	for(y = y2, j13 = 0, j23 = 0; y <= y3; y++, j13 += incx13, j23 += incx23) {
		if(y < 0) continue;
		if(y >= h) break;

		if(x3 > x1) xleft = x1 + (j13 >> 16);
		else xleft = x1 - (j13 >> 16);
		if(x3 > x2) xright = x2 + (j23 >> 16);
		else xright = x2 - (j23 >> 16);
		if(xright < xleft) { temp = xleft; xleft = xright; xright = temp; }

		if(xleft < 0) xleft = 0;
		if(xright >= w) xright = w - 1;

		for(x = xleft; x <= xright; x++) {
			tex[4 * (y * w + x) + 0] = r;
			tex[4 * (y * w + x) + 1] = g;
			tex[4 * (y * w + x) + 2] = b;
			tex[4 * (y * w + x) + 3] = 0xFF;
		}
	}
}

static void quadfill(GLubyte *tex, GLubyte r, GLubyte g, GLubyte b,
					int16_t w, int16_t h, int16_t x1, int16_t y1,
					int16_t x2, int16_t y2, int16_t x3, int16_t y3, int16_t x4, int16_t y4) {
	trifill(tex, r, g, b, w, h, x1, y1, x2, y2, x3, y3);
	trifill(tex, r, g, b, w, h, x1, y1, x3, y3, x4, y4);
}

#include <gl/glew.h>
#include <gl/glut.h>
#include <stdint.h>
#include "texgen.h"

static void trifill(GLubyte *tex, GLubyte r, GLubyte g, GLubyte b,
					int16_t w, int16_t h, int16_t x1, int16_t y1,
					int16_t x2, int16_t y2, int16_t x3, int16_t y3);

void texgen(GLubyte *tex, int16_t w, int16_t h) {
	trifill(tex, 0x80, 0x80, 0xFF, w, h, 0, 0, 50, 0, 1, 50);
	trifill(tex, 0xFF, 0, 0, w, h, w/2-w/8, h/2-h/8, w/2+w/8, h/2-h/8, w/2, h/2+h/8);
	trifill(tex, 0, 0, 0xFF, w, h, 0, 0, 100, 0, 100, 50);
	trifill(tex, 0, 0xFF, 0, w, h, w/2-w/8, h/2, w/2+w/8, h/2, w/2, h/2+h/8);
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
#ifdef _WIN32
#include <Windows.h>
#endif
#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include "texgen.h"
#include "shaders.h"

#include "cmd_textures.h"

GLuint textures[NUM_TEXTURES];

int desk_width, desk_height;
GLubyte *desktop_tex_data;

GLubyte tunnel_tex_data[] =
"\x80\x80\x80\xFF"
"\xFF\0\0\xFF"
"\0\xFF\0\xFF"
"\0\0\xFF\xFF";

float t0;

float demo_get_time(void) {
	float t = (float)glutGet(GLUT_ELAPSED_TIME);
	return t-t0;
}

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

void grab_screen()
{
#ifdef _WIN32
	HDC screen_dc = GetDC(NULL);
	desk_width = GetDeviceCaps(screen_dc, HORZRES);
	desk_height = GetDeviceCaps(screen_dc, VERTRES);
	desktop_tex_data = (GLubyte *)malloc(desk_height*desk_width*4*sizeof(GLubyte));

	HBITMAP bmp = CreateCompatibleBitmap(screen_dc, desk_width, desk_height);
	HDC bmp_dc = CreateCompatibleDC(screen_dc);
	SelectObject(bmp_dc, bmp);
	BitBlt(bmp_dc, 0, 0, desk_width, desk_height, screen_dc, 0, 0, SRCCOPY);
	
	int i = 0;
	for(int y = desk_height-1; y >= 0; y--) {
		for(int x = 0; x < desk_width; x++) {
			COLORREF c = GetPixel(bmp_dc, x, y);
			desktop_tex_data[i++] = GetRValue(c);
			desktop_tex_data[i++] = GetGValue(c);
			desktop_tex_data[i++] = GetBValue(c);
			desktop_tex_data[i++] = 255;
		}
	}
#else
#endif
}

void textures_init()
{
	glGenTextures(NUM_TEXTURES, textures);

	/* Desktop Texture */
	glBindTexture(GL_TEXTURE_2D, textures[DESKTOP_TEXTURE]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, desk_width, desk_height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, desktop_tex_data);

	/* CMD Icon Texture */
	glBindTexture(GL_TEXTURE_2D, textures[CMD_ICON_TEXTURE]);
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
	glBindTexture(GL_TEXTURE_2D, textures[CMD_X_TEXTURE]);
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
	glBindTexture(GL_TEXTURE_2D, textures[CMD_ARROW_TEXTURE]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 4, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, (GLubyte *)upTexData);
	
	/* Tunnel Texture */
	glBindTexture(GL_TEXTURE_2D, textures[TUNNEL_TEXTURE]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (GLubyte *)tunnel_tex_data);

	/* Rotozoomer Textures */
	//generate bitmap data:
	GLubyte *roto_1_tex_data = (GLubyte *)malloc(1024*1024*4*sizeof(GLubyte));
	for(int i = 0; i < 1024; i++) {
		for(int j = 0; j < 1024; j++) {
			float x = (float)j / 1024 * 2.0 - 1.0;
			float y = (float)i / 1024 * 2.0 - 1.0;
			if((y < (0.8 + 3*x)) && (y < (0.8 - 3*x)) && (y > (-0.8 + 3*x)) && (y > (-0.8 - 3*x)))
				roto_1_tex_data[4*(1024*i+j)+0] = 0xFF;
			else
				roto_1_tex_data[4*(1024*i+j)+0] = 0x00;
			if((y < (0.8/3 + x/3)) && (y < (0.8/3 - x/3)) && (y > (-0.8/3 + x/3)) && (y > (-0.8/3 - x/3)))
				roto_1_tex_data[4*(1024*i+j)+1] = 0xFF;
			else
				roto_1_tex_data[4*(1024*i+j)+1] = 0x00;
			roto_1_tex_data[4*(1024*i+j)+2] = 0x00;
			roto_1_tex_data[4*(1024*i+j)+3] = 0xFF;
		}
	}
	//then:
	glBindTexture(GL_TEXTURE_2D, textures[ROTO_1_TEXTURE]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // TODO not what I want, lookup wrap params
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // TODO same here
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // TODO ?
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // TODO ? multisample ?
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 1024, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, roto_1_tex_data);

	FILE *f = fopen("final.tga", "rb");
	fseek(f, 12, 0);
	int wl = fgetc(f);
	int wh = fgetc(f);
	int w = wl + (wh << 8);
	int hl = fgetc(f);
	int hh = fgetc(f);
	int h = hl + (hh << 8);
	GLubyte *roto_2_tex_data = (GLubyte *)malloc(w*h*4*sizeof(GLubyte));
	for(int i = 0; i < h; i++) {
		for(int j = 0; j < w; j++) {
			roto_2_tex_data[4*(w*i+j)+2] = fgetc(f);
			roto_2_tex_data[4*(w*i+j)+1] = fgetc(f);
			roto_2_tex_data[4*(w*i+j)+0] = fgetc(f);
			roto_2_tex_data[4*(w*i+j)+3] = fgetc(f);
		}
	}
	fclose(f);
	glBindTexture(GL_TEXTURE_2D, textures[ROTO_2_TEXTURE]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // TODO not what I want, lookup wrap params
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // TODO same here
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // TODO ?
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // TODO ? multisample ?
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, roto_2_tex_data);

	GLubyte *roto_3_tex_data = (GLubyte *)malloc(4096*4096*4*sizeof(GLubyte));
	memset(roto_3_tex_data, 0, 4096*4096*4*sizeof(GLubyte));
	for(int i = 0; i < 4096; i++) {
		for(int j = 0; j < 4096; j++) {
			float x = (float)j / 4096 * 2.0 - 1.0;
			float y = (float)i / 4096 * 2.0 - 1.0;
			float r = sqrt(x*x+y*y);
			float theta = atan2(y, x);
			float rmin = theta / 6.28;
			float rmax = theta / 6.28 + 0.1;
			if((r > rmin) && (r < rmax))
				roto_3_tex_data[4*(4096*i+j)+0] = 0xFF;
			roto_3_tex_data[4*(4096*i+j)+3] = 0xFF;
		}
	}
	glBindTexture(GL_TEXTURE_2D, textures[ROTO_3_TEXTURE]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // TODO not what I want, lookup wrap params
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // TODO same here
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // TODO ?
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // TODO ? multisample ?
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 4096, 4096, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, roto_3_tex_data);

	GLubyte *roto_4_tex_data = (GLubyte *)malloc(1024*1024*4*sizeof(GLubyte));
	memset(roto_4_tex_data, 0, 1024*1024*4*sizeof(GLubyte));
	for(int i = 0; i < 1024; i++) {
		for(int j = 0; j < 1024; j++) {
			float x = (float)j / 1024 * 2.0 - 1.0;
			float y = (float)i / 1024 * 2.0 - 1.0;
			float val = 0;
			val += 1*cos(+1.0*y + 1.0*x);
			val += 1*sin(+1.0*y - 1.0*x);
			val += 2*cos(-2.0*y + 0.5*x);
			val += 3*sin(-0.3*y - 1.0*x);
			val += 4.0;
			val /= 8.0;
			roto_4_tex_data[4*(1024*i+j)+0] = (GLubyte)(val * 0xFF);
			roto_4_tex_data[4*(1024*i+j)+3] = 0xFF;
		}
	}
	glBindTexture(GL_TEXTURE_2D, textures[ROTO_4_TEXTURE]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // TODO not what I want, lookup wrap params
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // TODO same here
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // TODO ?
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // TODO ? multisample ?
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 1024, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, roto_4_tex_data);

	GLubyte *roto_5_tex_data = (GLubyte *)malloc(1024 * 1024 * 4 * sizeof(GLubyte));
	texgen(roto_5_tex_data, 1024, 1024);
	/*
	for(int i = 0; i < 1024; i++) {
		for(int j = 0; j < 1024; j++) {

			//foo
		}
	}
	*/
	glBindTexture(GL_TEXTURE_2D, textures[ROTO_5_TEXTURE]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // TODO not what I want, lookup wrap params
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // TODO same here
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // TODO ?
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // TODO ? multisample ?
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 1024, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, roto_5_tex_data);


	memset(temp_tex, 0, 14);
	temp_tex[0] |= 0x01;
	temp_tex[1] |= 0x03;
	temp_tex[2] |= 0x07;
	temp_tex[4] |= 0x0F;
	temp_tex[6] |= 0x1F;
	temp_tex[10] |= 0x3F;
	temp_tex[12] |= 0x7F;
	temp_tex[13] |= 0xFF;
	//memcpy(temp_tex, icon_image.pixel_data, 14*16*4+1);
}
GLubyte temp_tex[14];
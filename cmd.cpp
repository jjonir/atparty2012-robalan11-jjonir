#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>
#include "shaders.h"
#include "cmd_icon.h"
#include "cmd_textures.h"

// TODO everything's shifted up by 1 pixel (MinGW, I don't think it was on native gcc)
// fix this

GLuint cmd_program;
GLuint textures[3];

void cmd_init(void)
{
	GLuint vshad = buildShader(GL_VERTEX_SHADER, cmd_vshad, "cmd vertex");
	GLuint fshad = buildShader(GL_FRAGMENT_SHADER, cmd_fshad, "cmd fragment");
	cmd_program = buildProgram(vshad, fshad, "cmd");
	glDeleteShader(vshad);
	glDeleteShader(fshad);

	glGenTextures(3, textures);

	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, icon_image.width,
			icon_image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
			(GLubyte *)icon_image.pixel_data);

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
}

void cmd_render(void)
{
	float windowW = glutGet(GLUT_WINDOW_WIDTH);
	float windowH = glutGet(GLUT_WINDOW_HEIGHT);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	float consoleX = 0.5;
	float consoleY = 0.5;

	float consoleW = 668.0/windowW;
	float consoleH = 337.0/windowH;

	GLint var;
	var = glGetUniformLocation(cmd_program, "cmd_pos");
	glUniform2i(var, 0.5*windowW-668/2, 0.5*windowH-337/2);
	var = glGetUniformLocation(cmd_program, "cmd_size");
	glUniform2i(var, 668, 337);

	var = glGetUniformLocation(cmd_program, "IconTex");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glUniform1i(var, 0);

	var = glGetUniformLocation(cmd_program, "XTex");
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glUniform1i(var, 8);

	var = glGetUniformLocation(cmd_program, "ArrowTex");
	glActiveTexture(GL_TEXTURE16);
	glBindTexture(GL_TEXTURE_2D, textures[2]);
	glUniform1i(var, 16);

	glBegin(GL_QUADS);
		glVertex3f(consoleX-consoleW/2, consoleY-consoleH/2, -1);
		glVertex3f(consoleX+consoleW/2, consoleY-consoleH/2, -1);
		glVertex3f(consoleX+consoleW/2, consoleY+consoleH/2, -1);
		glVertex3f(consoleX-consoleW/2, consoleY+consoleH/2, -1);
	glEnd();

	glutSwapBuffers();
}

void cmd_animate(int val)
{
	glutTimerFunc(10, cmd_animate, 0);

	glutPostRedisplay();
}

const char *cmd_vshad =
"#version 120\n"
"void main() {"
"	gl_Position = ftransform();"
"}"
;

const char *cmd_fshad =
"#version 120\n"
"#define CL(R,G,B) gl_FragColor=vec4(R/256.0,G/256.0,B/256.0,1.0)\n"
"#define TX(T,V) gl_FragColor=texture2D(T,V)\n"
"#define RX rel_pos.x\n"
"#define RY rel_pos.y\n"
"#define CX cmd_size.x\n"
"#define CY cmd_size.y\n"
"#define EI else if\n"
"uniform ivec2 cmd_pos;"
"uniform ivec2 cmd_size;"
"uniform sampler2D IconTex, XTex, ArrowTex;"
"bool colorButtons(ivec2 rel_pos) {"
"	bool rv = true;"
"	if(RX == 49 || RX == 31 || RX == 15 || RY == 0)" //buttons br 0
"		CL(113, 111, 100);"
"	EI(RX == 0 || RX == 34 || RX == 16 || RY == 20)" //buttons tl 0
"		CL(255, 255, 255);"
"	EI(RX == 48 || RX == 30 || RX == 14 || RY == 1)" //buttons br 0
"		CL(172, 168, 153);"
"	EI(RX == 1 || RX == 35 || RX == 17 || RY == 19)" //buttons tl 0
"		CL(241, 239, 226);"
"	EI(RX >= 4 && RX <= 9 && RY >= 7 && RY <= 8)" //min
"		CL(0, 0, 0);"
"	EI((RX == 18 || RX == 27) && RY >= 7 && RY <= 15)" //max sides
"		CL(0, 0, 0);"
"	EI(RX >= 19 && RX <= 26 && (RY == 7 || RY == 14 || RY == 15))" //max top bottom
"		CL(0, 0, 0);"
"	EI(RX >= 38 && RX <= 45 && RY >= 7 && RY <= 14)" //x
"		TX(XTex, vec2(RX-38,RY-7)/vec2(8,8));"
"	EI(!(RX == 32 || RX == 33))" //the rest, sans gap
"		CL(236, 233, 216);"
"	else" //TODO button labels
"		rv = false;"
"	return rv;"
"}"
"void colorScrollBar(ivec2 rel_pos) {"
"	if(RY >= 17 && RY <= CY-38-40)" //bar checkerboard
"		if(fract(float(RX+RY)/2) == 0.5)"
"			CL(236, 233, 216);"
"		else"
"			CL(255, 255, 255);"
"	EI(RX == 16 || RY == 0 || RY == CY-38-17 || RY == CY-38-39)" //br0
"		CL(113, 111, 100);"
"	EI(RX == 0 || RY == 16 || RY == CY-38-1 || RY == CY-38-18)" //tl0
"		CL(241, 239, 226);"
"	EI(RX == 15 || RY == 1 || RY == CY-38-16 || RY == CY-38-38)" //br1
"		CL(172, 168, 153);"
"	EI(RX == 1 || RY == 15 || RY == CY-38-2 || RY == CY-38-19)" //tl1
"		CL(255, 255, 255);"
"	EI(RX >= 5 && RX <= 12 && RY >= 6 && RY <= 9)" //down arrow
"		TX(ArrowTex, vec2(RX-5,RY-6)/vec2(8,4));"
"	EI(RX >= 5 && RX <= 12 && RY >= CY-38-10 && RY <= CY-38-7)" //up arrow
"		TX(ArrowTex, vec2(RX-5,CY-49-RY)/vec2(8,4));"
"	else"
"		CL(236, 233, 216);" //scroll block and arrow block filler
"}"
"void main() {"
"	ivec2 rel_pos = ivec2(gl_FragCoord.xy) - cmd_pos;"
/* outer border */
"	if(RX == CX-1 || RY == 0)" //bottom right 0
"		CL(113, 111, 100);"
"	EI(RX == 0 || RY == CY-1)" //top left 0
"		CL(241, 239, 226);"
"	EI(RX == CX-2 || RY == 1)" //bottom right 1
"		CL(172, 168, 153);"
"	EI(RX == 1 || RY == CY-2)" //top left 1
"		CL(255, 255, 255);"
"	EI(RX == CX-3 || RY == 2 || RX == 2 || RY == CY-3)" //bltr 2
"		CL(212, 208, 200);"
"	EI(RX == CX-4 || RY == 3 || RX == 3 || RY == CY-4 || RY == CY-30)" //bltr 3, below titlebar
"		CL(236, 233, 216);"
/* title bar */
"	EI(RY >= CY-27 && RY <= CY-7 && ((RX >= CX-56 && RX <= CX-25) ||"
"	                                 (RX >= CX-22 && RX <= CX-7)))"
"		colorButtons(rel_pos-cmd_size+ivec2(56, 27));"
"	EI(RY >= CY-29) {"
"		if(RX >= 9 && RX <= 24 && RY >= CY-24 && RY <= CY-11)" //icon
"			TX(IconTex, vec2(RX-10,CY-RY-24)/vec2(15, 13));" //should be 16,14?!
"		else {" //title bar gradient (TODO text)
"			float p = float(RX-37)/float(CX-58-37);"
"			gl_FragColor = mix(vec4(0, 84/256.0, 227/256.0, 1),"
"				vec4(61/256.0, 149/256.0, 255/256.0, 1), vec4(p));"
"		}"
"	}"
/* inner border */
"	EI(RX == CX-5 || RY == 4)" //br 4
"		CL(255, 255, 255);"
"	EI(RX == 4 || RY == CY-31)" //tl 4, top shifted below titlebar
"		CL(172, 168, 153);"
"	EI(RX == CX-6 || RY == 5)" //br 5
"		CL(241, 239, 226);"
"	EI(RX == 5 || RY == CY-32)" //tl 5, top shifted below titlebar
"		CL(113, 111, 110);"
/* scroll bar */
"	EI(RX >= CX-23)"
"		colorScrollBar(rel_pos-ivec2(CX-23, 6));"
/* text area */
"	EI(RX >= 0 && RX <= CX-1 && RY >= 0 && RY <= CY-1)"
"		CL(0, 0, 0);" //or... discard?
/* everything else, there should not be anything else, account for it somehow! */
"	else"
"		CL(255, 0, 0);"
"}"
;

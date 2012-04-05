#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>
#include "shaders.h"

GLuint cmd_program;

void cmd_init(void)
{
	GLuint vshad = buildShader(GL_VERTEX_SHADER, cmd_vshad, "cmd vertex");
	GLuint fshad = buildShader(GL_FRAGMENT_SHADER, cmd_fshad, "cmd fragment");
	cmd_program = buildProgram(vshad, fshad, "cmd");
	glDeleteShader(vshad);
	glDeleteShader(fshad);
}

void cmd_render(float windowW, float windowH, float consoleX, float consoleY, float consoleW, float consoleH)
{
	glUseProgram(cmd_program);
	//render the cmd window
	GLint var;
	var = glGetUniformLocation(cmd_program, "cmd_pos");
	glUniform2i(var,
				consoleX*windowW-CONSOLE_PIXELS_W/2-LEFT_BORDER,
				consoleY*windowH-CONSOLE_PIXELS_H/2-BOTTOM_BORDER);
	var = glGetUniformLocation(cmd_program, "cmd_size");
	glUniform2i(var, CONSOLE_PIXELS_W+LEFT_BORDER+RIGHT_BORDER,
						CONSOLE_PIXELS_H+BOTTOM_BORDER+TOP_BORDER);
	var = glGetUniformLocation(cmd_program, "desk_size");
	glUniform2i(var, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

	var = glGetUniformLocation(cmd_program, "IconTex");
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[CMD_ICON_TEXTURE]);
	glUniform1i(var, 0);

	var = glGetUniformLocation(cmd_program, "XTex");
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, textures[CMD_X_TEXTURE]);
	glUniform1i(var, 4);

	var = glGetUniformLocation(cmd_program, "ArrowTex");
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, textures[CMD_ARROW_TEXTURE]);
	glUniform1i(var, 8);

	var = glGetUniformLocation(cmd_program, "DeskTex");
	glActiveTexture(GL_TEXTURE12);
	glBindTexture(GL_TEXTURE_2D, textures[DESKTOP_TEXTURE]);
	glUniform1i(var, 12);

	glBegin(GL_QUADS);
		glVertex3f(-1,-1,-1);
		glVertex3f(1,-1,-1);
		glVertex3f(1,1,-1);
		glVertex3f(-1,1,-1);
	glEnd();
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
"uniform ivec2 desk_size;"
"uniform sampler2D IconTex, XTex, ArrowTex, DeskTex;"
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
/* desktop! */
"	if(RX < 0 || RX >= CX || RY < 0 || RY >= CY)"
"		gl_FragColor = texture2D(DeskTex, gl_FragCoord.xy/vec2(desk_size));"
/* outer border */
"	EI(RX == CX-1 || RY == 0)" //bottom right 0
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
"		CL(255,0,0);" // just in case... blood everywhere (else)!
"}"
;

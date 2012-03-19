#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>

// TODO everything's shifted up by 1 pixel (MinGW, I don't think it was on native gcc)
// fix this

GLuint cmd_program;

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
"#define COLORI(R,G,B) gl_FragColor=vec4(R/256.0,G/256.0,B/256.0,1.0)\n"
"uniform ivec2 cmd_pos;"
"uniform ivec2 cmd_size;"
"bool colorButtons(ivec2 rel_pos) {"
"	bool rv = true;"
"	if(rel_pos.x == 49 || rel_pos.x == 31 || rel_pos.x == 15 || rel_pos.y == 0)" //buttons br 0
"		COLORI(113, 111, 100);"
"	else if(rel_pos.x == 0 || rel_pos.x == 34 || rel_pos.x == 16 || rel_pos.y == 20)" //buttons tl 0
"		COLORI(255, 255, 255);"
"	else if(rel_pos.x == 48 || rel_pos.x == 30 || rel_pos.x == 14 || rel_pos.y == 1)" //buttons br 0
"		COLORI(172, 168, 153);"
"	else if(rel_pos.x == 1 || rel_pos.x == 35 || rel_pos.x == 17 || rel_pos.y == 19)" //buttons tl 0
"		COLORI(241, 239, 226);"
"	else if(rel_pos.x >= 4 && rel_pos.x <= 9 && rel_pos.y >= 7 && rel_pos.y <= 8)"
"		COLORI(0, 0, 0);"
"	else if((rel_pos.x == 18 || rel_pos.x == 27) && rel_pos.y >= 7 && rel_pos.y <= 15)"
"		COLORI(0, 0, 0);"
"	else if(rel_pos.x >= 19 && rel_pos.x <= 26 && (rel_pos.y == 7 || rel_pos.y == 14 || rel_pos.y == 15))"
"		COLORI(0, 0, 0);"
"	else if((rel_pos.x == 38 || rel_pos.x == 45) &&"
"			(rel_pos.y == 7 || rel_pos.y == 8 || rel_pos.y == 13 || rel_pos.y == 14))"
"		COLORI(0, 0, 0);"
"	else if((rel_pos.x == 39 || rel_pos.x == 44) &&"
"			((rel_pos.y >=7 && rel_pos.y <=9) || (rel_pos.y >= 12 && rel_pos.y <= 14)))"
"		COLORI(0, 0, 0);"
"	else if((rel_pos.x == 40 || rel_pos.x == 43) && rel_pos.y >= 8 && rel_pos.y <= 13)"
"		COLORI(0, 0, 0);"
"	else if((rel_pos.x == 41 || rel_pos.x == 42) && rel_pos.y >= 9 && rel_pos.y <= 12)"
"		COLORI(0, 0, 0);"
"	else if(!(rel_pos.x == 32 || rel_pos.x == 33))"
"		COLORI(236, 233, 216);"
"	else" //TODO button labels
"		rv = false;"
"	return rv;"
"}"
"void colorScrollBar(ivec2 rel_pos) {"
"	if(rel_pos.y >= 17 && rel_pos.y <= cmd_size.y-38-40)"
"		if(fract(float(rel_pos.x+rel_pos.y)/2) == 0.5)"
"			COLORI(236, 233, 216);"
"		else"
"			COLORI(255, 255, 255);"
"	else if(rel_pos.x == 16 || rel_pos.y == 0 || rel_pos.y == cmd_size.y-38-17 || rel_pos.y == cmd_size.y-38-39)"
"		COLORI(113, 111, 100);"
"	else if(rel_pos.x == 0 || rel_pos.y == 16 || rel_pos.y == cmd_size.y-38-1 || rel_pos.y == cmd_size.y-38-18)"
"		COLORI(241, 239, 226);"
"	else if(rel_pos.x == 15 || rel_pos.y == 1 || rel_pos.y == cmd_size.y-38-16 || rel_pos.y == cmd_size.y-38-38)"
"		COLORI(172, 168, 153);"
"	else if(rel_pos.x == 1 || rel_pos.y == 15 || rel_pos.y == cmd_size.y-38-2 || rel_pos.y == cmd_size.y-38-19)"
"		COLORI(255, 255, 255);"
"	else if((rel_pos.y == 9 || rel_pos.y == cmd_size.y-38-10) && rel_pos.x >= 5 && rel_pos.x <= 11)"
"		COLORI(0, 0, 0);"
"	else if((rel_pos.y == 8 || rel_pos.y == cmd_size.y-38-9) && rel_pos.x >= 6 && rel_pos.x <= 10)"
"		COLORI(0, 0, 0);"
"	else if((rel_pos.y == 7 || rel_pos.y == cmd_size.y-38-8) && rel_pos.x >= 7 && rel_pos.x <= 9)"
"		COLORI(0, 0, 0);"
"	else if((rel_pos.y == 6 || rel_pos.y == cmd_size.y-38-7) && rel_pos.x == 8)"
"		COLORI(0, 0, 0);"
"	else"
"		COLORI(236, 233, 216);"
"}"
"void main() {"
"	ivec2 rel_pos = ivec2(gl_FragCoord.xy) - cmd_pos;"
/* outer border */
"	if(rel_pos.x == cmd_size.x-1 || rel_pos.y == 0)" //bottom right 0
"		COLORI(113, 111, 100);"
"	else if(rel_pos.x == 0 || rel_pos.y == cmd_size.y-1)" //top left 0
"		COLORI(241, 239, 226);"
"	else if(rel_pos.x == cmd_size.x-2 || rel_pos.y == 1)" //bottom right 1
"		COLORI(172, 168, 153);"
"	else if(rel_pos.x == 1 || rel_pos.y == cmd_size.y-2)" //top left 1
"		COLORI(255, 255, 255);"
"	else if(rel_pos.x == cmd_size.x-3 || rel_pos.y == 2 ||"
"			rel_pos.x == 2 || rel_pos.y == cmd_size.y-3)" //bltr 2
"		COLORI(212, 208, 200);"
"	else if(rel_pos.x == cmd_size.x-4 || rel_pos.y == 3 ||"
"			rel_pos.x == 3 || rel_pos.y == cmd_size.y-4 || rel_pos.y == cmd_size.y-30)" //bltr 3, below titlebar
"		COLORI(236, 233, 216);"
/* title bar */
"	else if(rel_pos.y >= cmd_size.y-27 && rel_pos.y <= cmd_size.y-7 &&"
"			((rel_pos.x >= cmd_size.x-56 && rel_pos.x <= cmd_size.x-25) ||"
"			(rel_pos.x >= cmd_size.x-22 && rel_pos.x <= cmd_size.x-7)) &&"
"			colorButtons(rel_pos-cmd_size+ivec2(56, 27)));"
"	else if(rel_pos.y >= cmd_size.y-29 && rel_pos.y <= cmd_size.y-5) {"
"		if(rel_pos.x >= 4 && rel_pos.x <= 37)"
"			COLORI(0, 84, 227);"
"		else if(rel_pos.x >= cmd_size.x-58 && rel_pos.x <= cmd_size.x-5)"
"			COLORI(61, 149, 255);"
"		else {"
"			float p = float(rel_pos.x-37)/float(cmd_size.x-58-37);"
"			gl_FragColor = mix(vec4(0, 84/256.0, 227/256.0, 1),"
"				vec4(61/256.0, 149/256.0, 255/256.0, 1), vec4(p,p,p,p));"
"		}"
"	}"
/* inner border */
"	else if(rel_pos.x == cmd_size.x-5 || rel_pos.y == 4)" //br 4
"		COLORI(255, 255, 255);"
"	else if(rel_pos.x == 4 || rel_pos.y == cmd_size.y-31)" //tl 4, top shifted below titlebar
"		COLORI(172, 168, 153);"
"	else if(rel_pos.x == cmd_size.x-6 || rel_pos.y == 5)" //br 5
"		COLORI(241, 239, 226);"
"	else if(rel_pos.x == 5 || rel_pos.y == cmd_size.y-32)" //tl 5, top shifted below titlebar
"		COLORI(113, 111, 110);"
/* scroll bar */
"	else if(rel_pos.x >= cmd_size.x-23)"
"		colorScrollBar(rel_pos-ivec2(cmd_size.x-23, 6));"
/* text area */
"	else if(rel_pos.x >= 0 && rel_pos.x <= cmd_size.x-1 && rel_pos.y >= 0 && rel_pos.y <= cmd_size.y-1)"
"		COLORI(0, 0, 0);"
/* everything else, there should not be anything else, account for it somehow! */
"	else"
"		gl_FragColor = vec4(1, 0, 0, 1);"
"}"
;

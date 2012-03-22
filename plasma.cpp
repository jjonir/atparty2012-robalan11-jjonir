#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>
#include "shaders.h"

GLuint plasma_program;
static int char_w = 8;
static int char_h = 14;

void plasma_init(void)
{
	GLuint vshad = buildShader(GL_VERTEX_SHADER, plasma_vshad, "plasma vertex");
	GLuint fshad = buildShader(GL_FRAGMENT_SHADER, plasma_fshad, "plasma fragment");
	plasma_program = buildProgram(vshad, fshad, "plasma");
	glDeleteShader(vshad);
	glDeleteShader(fshad);
}

void plasma_render(void)
{
	float windowW = glutGet(GLUT_WINDOW_WIDTH);
	float windowH = glutGet(GLUT_WINDOW_HEIGHT);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	float consoleX = 0.5 + 0.2*sin(6.28*(float)glutGet(GLUT_ELAPSED_TIME)/1000);
	float consoleY = 0.5 + 0.2*cos(6.28*(float)glutGet(GLUT_ELAPSED_TIME)/1000);

	// Clamp console to character borders
	consoleX = (floor(consoleX * windowW / char_w) * char_w) / windowW;
	consoleY = (floor(consoleY * windowH / char_h) * char_h + char_h/2) / windowH;
	// Should be 640x350
	float consoleW = 640.0/windowW;
	float consoleH = 350.0/windowH;

	glBegin(GL_QUADS);
		glVertex3f(consoleX-consoleW/2, consoleY-consoleH/2, -1);
		glVertex3f(consoleX+consoleW/2, consoleY-consoleH/2, -1);
		glVertex3f(consoleX+consoleW/2, consoleY+consoleH/2, -1);
		glVertex3f(consoleX-consoleW/2, consoleY+consoleH/2, -1);
	glEnd();

	glutSwapBuffers();
}

void plasma_animate(int val)
{
	glutTimerFunc(10, plasma_animate, 0);
	int t = glutGet(GLUT_ELAPSED_TIME);

	if (t > 5000)  { char_w = 4; char_h = 7; }
	if (t > 10000) { char_w = 3; char_h = 4; }
	if (t > 13000) { char_w = 2; char_h = 2; }
	if (t > 15000) { char_w = 1; char_h = 1; }

	GLint w_var = glGetUniformLocation(plasma_program, "width");
	glUniform1i(w_var, glutGet(GLUT_WINDOW_WIDTH));
	GLint h_var = glGetUniformLocation(plasma_program, "height");
	glUniform1i(h_var, glutGet(GLUT_WINDOW_HEIGHT));
	GLint t_var = glGetUniformLocation(plasma_program, "time");
	glUniform1i(t_var, t);
	GLint char_w_var = glGetUniformLocation(plasma_program, "char_w");
	glUniform1i(char_w_var, char_w);
	GLint char_h_var = glGetUniformLocation(plasma_program, "char_h");
	glUniform1i(char_h_var, char_h);

	glutPostRedisplay();
}

const char *plasma_vshad =
"#version 120\n"
"void main() {"
"	gl_Position = ftransform();"
"}"
;

const char *plasma_fshad =
"#version 120\n"
"uniform int width;"
"uniform int height;"
"uniform int time;"
"uniform int char_w;"
"uniform int char_h;"
"void main() {"
"	float xchar = floor(gl_FragCoord.x/char_w)*char_w;"
"	float ychar = floor(gl_FragCoord.y/char_h)*char_h;"
"	float color = (sin(sqrt(pow(width/2-xchar,2)+pow(height/2-ychar,2))/50-time/300.0)+"
"						sin((xchar+ychar)/30+time/230.0)+"
"						sin(xchar/37-time/600.0) + 1.5)/4.5;"
"	if(color < 0.167)"
"		gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);"
"	else if(color < 0.333)"
"		gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);"
"	else if(color < 0.5)"
"		gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);"
"	else if(color < 0.667)"
"		gl_FragColor = vec4(0.0, 1.0, 1.0, 1.0);"
"	else if(color < 0.833)"
"		gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);"
"	else"
"		gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0);"
"}"
;

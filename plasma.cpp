#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include "shaders.h"

GLuint plasma_program;
static int char_w = 8;
static int char_h = 14;
static float sharp = 1; // 1 is fully sharp, 0 is fully smooth

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
	
	float t = demo_get_time();
	float consoleX = 0.5;
	float consoleY = 0.5;

	if (t > 8000) {
		float theta = t/2000 - 4;
		float radius = 0.3*sin(3*theta);

		consoleX = 0.5 + radius*sin(theta);
		consoleY = 0.5 + radius*cos(theta);
	}

	// Clamp console to character borders
	consoleX = (floor(consoleX * windowW / char_w) * char_w) / windowW;
	consoleY = (floor(consoleY * windowH / char_h) * char_h + char_h/2) / windowH;
	// Should be 640x350
	float consoleW, consoleH;
	if(t < 20000) {
		consoleW = 640.0/windowW;
		consoleH = 350.0/windowH;
	} else {
		consoleW = (640.0 + (t - 20000) / 10) / windowW;
		consoleH = (350.0 + (t - 20000) / 10) / windowH;
	}

	cmd_render(windowW, windowH, consoleX, consoleY, consoleW, consoleH);

	glUseProgram(plasma_program);

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
	int t = demo_get_time();
	//if(t < 20000)
		glutTimerFunc(10, plasma_animate, 0);
	//else {
	//	glutDisplayFunc(tunnel_render);
	//	glutTimerFunc(10, tunnel_animate, 0);
	//}

	if (t > 10000)  { char_w = 4; char_h = 7; sharp = 1.0f; }
	if (t > 15000) { char_w = 3; char_h = 4; sharp = 1.0f; }
	if (t > 18000) { char_w = 2; char_h = 2; sharp = 7.0 - t/3000.0; }
	if (t > 20000) { char_w = 1; char_h = 1; sharp = 7.0 - t/3000.0; }
	
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
	GLint sharp_var = glGetUniformLocation(plasma_program, "sharp");
	glUniform1f(sharp_var, sharp);

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
"uniform float sharp;"
"void main() {"
"	float xchar = floor(gl_FragCoord.x/char_w)*char_w;"
"	float ychar = floor(gl_FragCoord.y/char_h)*char_h;"
"	float color = ( sin(sqrt(pow(width/2-xchar,2)+pow(height/2-ychar,2))/50-time/300.0)+"
"					sin((xchar+ychar)/30+time/230.0)+"
"					sin(xchar/37-time/600.0) + 1.5)/4.5;"
"	color = clamp(color, 0.0, 1.0);"
"	float fade = sharp;"
"	float red = clamp(2.0 - min(abs(6*color-0.5), abs(6*(color-1.0)-0.5)) + fade/2, 0.0, 1.0);"
"	float green = clamp(2.0 - abs(6*color-2.5) + fade/2, 0.0, 1.0);"
"	float blue = clamp(2.0 - abs(6*color-4.5) + fade/2, 0.0, 1.0);"
"	red = red < fade ? 0 : red;"
"	green = green < fade ? 0 : green;"
"	blue = blue < fade ? 0 : blue;"
"	gl_FragColor = vec4(red, green, blue, 1.0);"
"}"
;

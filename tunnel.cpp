#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>
#include "shaders.h"

GLuint tunnel_program;

void tunnel_init(void)
{
	GLuint vshad = buildShader(GL_VERTEX_SHADER, tunnel_vshad, "tunnel vertex");
	GLuint fshad = buildShader(GL_FRAGMENT_SHADER, tunnel_fshad, "tunnel fragment");
	tunnel_program = buildProgram(vshad, fshad, "tunnel");
	glDeleteShader(vshad);
	glDeleteShader(fshad);
}

#define CONSOLE_PIXELS_W 640.0
#define CONSOLE_PIXELS_H 350.0
#define TOP_BORDER 32
#define LEFT_BORDER 6
#define RIGHT_BORDER 23
#define BOTTOM_BORDER 8

extern GLuint textures[3];

void tunnel_render(void)
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	int t = glutGet(GLUT_ELAPSED_TIME);
	float windowW = glutGet(GLUT_WINDOW_WIDTH);
	float windowH = glutGet(GLUT_WINDOW_HEIGHT);
	float consoleX = 0.5;
	float consoleY = 0.5;
	// Clamp console to character borders
	//consoleX = (floor(consoleX * windowW / char_w) * char_w) / windowW;
	//consoleY = (floor(consoleY * windowH / char_h) * char_h + char_h/2) / windowH;
	// Should be 640x350
	float consoleW = CONSOLE_PIXELS_W/windowW;
	float consoleH = CONSOLE_PIXELS_H/windowH;

	glUseProgram(cmd_program);
	//render the cmd window
	GLint var;
	var = glGetUniformLocation(cmd_program, "cmd_pos");
	glUniform2i(var,
				0.5*windowW-CONSOLE_PIXELS_W/2-LEFT_BORDER,
				0.5*windowH-CONSOLE_PIXELS_H/2-BOTTOM_BORDER);
	var = glGetUniformLocation(cmd_program, "cmd_size");
	glUniform2i(var, CONSOLE_PIXELS_W+LEFT_BORDER+RIGHT_BORDER,
						CONSOLE_PIXELS_H+BOTTOM_BORDER+TOP_BORDER);

	var = glGetUniformLocation(cmd_program, "IconTex");
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glUniform1i(var, 0);

	var = glGetUniformLocation(cmd_program, "XTex");
	glActiveTexture(GL_TEXTURE4);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glUniform1i(var, 4);

	var = glGetUniformLocation(cmd_program, "ArrowTex");
	glActiveTexture(GL_TEXTURE8);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[2]);
	glUniform1i(var, 8);

	glBegin(GL_QUADS);
		glVertex3f((consoleX-LEFT_BORDER/windowW)-consoleW/2,
					(consoleY-BOTTOM_BORDER/windowH)-consoleH/2, -1);
		glVertex3f((consoleX+RIGHT_BORDER/windowW)+consoleW/2,
					(consoleY-BOTTOM_BORDER/windowH)-consoleH/2, -1);
		glVertex3f((consoleX+RIGHT_BORDER/windowW)+consoleW/2,
					(consoleY+TOP_BORDER/windowH)+consoleH/2, -1);
		glVertex3f((consoleX-LEFT_BORDER/windowW)-consoleW/2,
					(consoleY+TOP_BORDER/windowH)+consoleH/2, -1);
		//make sure they're right, consoleW/H/X/Y are different
	glEnd();

	glUseProgram(tunnel_program);
	//render the tunnel effect

	var = glGetUniformLocation(tunnel_program, "Center");
	glUniform2f(var, windowW/2+50*sin((float)t/1000), windowH/2+50*cos((float)t/2000));
	var = glGetUniformLocation(tunnel_program, "Tex");
	glUniform1i(var, 0);
	var = glGetUniformLocation(tunnel_program, "Move");
	glUniform2f(var, (float)t/1000, (float)t/10000);
	
	var = glGetUniformLocation(tunnel_program, "Tex");
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[3]);
	glUniform1i(var, 0);

	glBegin(GL_QUADS);
		glVertex3f(consoleX-consoleW/2, consoleY-consoleH/2, -1);
		glVertex3f(consoleX+consoleW/2, consoleY-consoleH/2, -1);
		glVertex3f(consoleX+consoleW/2, consoleY+consoleH/2, -1);
		glVertex3f(consoleX-consoleW/2, consoleY+consoleH/2, -1);
	glEnd();

	glutSwapBuffers();
}

void tunnel_animate(int val)
{
	glutTimerFunc(10, tunnel_animate, 0);
	glutPostRedisplay();
}

const char *tunnel_vshad =
"#version 120\n"
"void main() {"
"	gl_Position = ftransform();"
"}"
;

const char *tunnel_fshad =
"#version 120\n"
"uniform vec2 Center;"
"uniform sampler2D Tex;"
"uniform vec2 Move;"
"void main() {"
"	vec2 v = gl_FragCoord.xy - Center;"
"	float dist = length(v);"
"	float angle = atan(v.y, v.x);"
"	vec2 transformedCoord = vec2(10/dist, angle*2/3.1415)+Move;"
"	gl_FragColor = texture2D(Tex, transformedCoord);"
"}"
;

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

extern GLuint textures[3];

void tunnel_render(void)
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	int t = glutGet(GLUT_ELAPSED_TIME);
	float windowW = glutGet(GLUT_WINDOW_WIDTH);
	float windowH = glutGet(GLUT_WINDOW_HEIGHT);
	float consoleX = 0.5;
	float consoleY = 0.5;
	float consoleW = CONSOLE_PIXELS_W/windowW;
	float consoleH = CONSOLE_PIXELS_H/windowH;
	
	cmd_render(windowW, windowH, consoleX, consoleY, consoleW, consoleH);

	glUseProgram(tunnel_program);
	//render the tunnel effect

	GLint var;
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

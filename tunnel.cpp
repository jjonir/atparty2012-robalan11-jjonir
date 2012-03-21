#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>
#include "shaders.h"

GLuint tunnel_program;

GLubyte tunnel_tex_data[] =
"\255\255\255\255"
"\255\0\0\255"
"\0\255\0\255"
"\0\0\255\255";

void tunnel_init(void)
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (GLubyte *)tunnel_tex_data);
}

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
	float consoleW = 640.0/windowW;
	float consoleH = 350.0/windowH;

	//glUseProgram(cmd_program);
	//render the cmd window

	glUseProgram(tunnel_program);
	//render the tunnel effect

	GLuint var = glGetUniformLocation(tunnel_program, "Center");
	glUniform2f(var, windowW/2+50*sin((float)t/1000), windowH/2+50*cos((float)t/2000));
	var = glGetUniformLocation(tunnel_program, "Tex");
	glUniform1i(var, 0);
	var = glGetUniformLocation(tunnel_program, "Move");
	glUniform2f(var, (float)t/1000, (float)t/10000);
	var = glGetUniformLocation(tunnel_program, "time");
	glUniform1i(var, t);
	GLint w_var = glGetUniformLocation(plasma_program, "width");
	glUniform1i(w_var, glutGet(GLUT_WINDOW_WIDTH));
	GLint h_var = glGetUniformLocation(plasma_program, "height");
	glUniform1i(h_var, glutGet(GLUT_WINDOW_HEIGHT));

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
"uniform int time;"
"uniform int width;"
"uniform int height;"
"void main() {"
"	vec2 v = gl_FragCoord.xy - Center;"
"	float dist = length(v);"
"	float angle = atan(v.y, v.x);"
"	vec2 transformedCoord = vec2(10/dist, angle*2/3.1415)/*+Move*/;"
"	/*gl_FragColor = mix(vec4(0), vec4(1,0,0,0), fract(100/dist+angle*2/3.14));*/"
"	/*gl_FragColor = texture2D(Tex, transformedCoord);*/"
"	float tx = transformedCoord.x*100;"
"	float ty = transformedCoord.y*100;"
// TODO if we want to wrap plasma texture around the tunnel, it must wrap! i.e. pl(x,0)==pl(x,2pi)
"	float color = (sin(sqrt(pow(width/2-tx,2)+pow(height/2-ty,2))/50-time/300.0)+"
"						sin((tx+ty)/30+time/230.0)+"
"						sin(tx/37-time/600.0) + 1.5)/4.5;"
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

#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>
#include <string.h>
#include "shaders.h"
#include "titles.h"

GLuint tunnel_program;
static int char_w = 8;
static int char_h = 14;

void tunnel_init(void)
{
	GLuint vshad = buildShader(GL_VERTEX_SHADER, tunnel_vshad, "tunnel vertex");
	GLuint fshad = buildShader(GL_FRAGMENT_SHADER, tunnel_fshad, "tunnel fragment");
	tunnel_program = buildProgram(vshad, fshad, "tunnel");
	glDeleteShader(vshad);
	glDeleteShader(fshad);
}

void tunnel_render(void)
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	int t = demo_get_time();
	float windowW = glutGet(GLUT_WINDOW_WIDTH);
	float windowH = glutGet(GLUT_WINDOW_HEIGHT);
	float consoleX = 0.5;
	float consoleY = 0.5;
	float consoleW = CONSOLE_PIXELS_W/windowW;
	float consoleH = CONSOLE_PIXELS_H/windowH;
	
	// Clamp console to character borders
	consoleX = (floor(consoleX * windowW / char_w) * char_w) / windowW;
	consoleY = (floor(consoleY * windowH / char_h) * char_h + char_h/2) / windowH;

	cmd_render(windowW, windowH, consoleX, consoleY, consoleW, consoleH);

	glUseProgram(tunnel_program);
	//render the tunnel effect
	
	GLint var;

	float centerX = windowW/2 + 50*sin((float)t / 1000);
	float centerY = windowH/2 + 50*cos((float)t / 2000);
	var = glGetUniformLocation(tunnel_program, "Center");
	glUniform2f(var, centerX, centerY);
	var = glGetUniformLocation(tunnel_program, "Tex");
	glUniform1i(var, 0);
	var = glGetUniformLocation(tunnel_program, "Move");
	glUniform2f(var, (float)t/1000, (float)t/10000);
	
	var = glGetUniformLocation(tunnel_program, "Tex");
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[TUNNEL_TEXTURE]);
	glUniform1i(var, 0);

	var = glGetUniformLocation(tunnel_program, "Text");
	glActiveTexture(GL_TEXTURE4);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[TITLE_TEXTURE]);
	glUniform1i(var, 4);

	var = glGetUniformLocation(tunnel_program, "text_size");
	glUniform2f(var, titletexwid, STRHEI);
	
	int textX = centerX - titletexwid / 2;
	int textY = centerY - STRHEI / 2;
	textX = floor((float)textX / char_w) * char_w;
	textY = floor((float)textY / char_h) * char_h;
	var = glGetUniformLocation(tunnel_program, "text_pos");
	glUniform2i(var, textX, textY);

	glBegin(GL_QUADS);
		glVertex3f(consoleX-consoleW/2, consoleY-consoleH/2, -1);
		glVertex3f(consoleX+consoleW/2, consoleY-consoleH/2, -1);
		glVertex3f(consoleX+consoleW/2, consoleY+consoleH/2, -1);
		glVertex3f(consoleX-consoleW/2, consoleY+consoleH/2, -1);
	glEnd();

	/*
	char *alph = "A demo for @-Party 2012!        ";
	int alph_center = strlen(alph)/2;
	float centerX_q = (floor(centerX / char_w) * char_w) / windowW;
	float centerY_q = (floor((centerY + char_h/2) / char_h) * char_h + 3) / windowH;
	float chwid = 8.0 / windowW;
	float chhei = 14.0 / windowH;
	float lx = consoleX-16*chwid;
	float by = consoleY+2.0/windowH;
	for(int ii = 0; ii < 32; ii++) {
		glRasterPos2f(lx+ii*chwid, by);
		glRasterPos2f(centerX_q+(ii-alph_center)*chwid, centerY_q);
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, alph[(ii+t/100)%32]);
	}
	*/

	glutSwapBuffers();
}

void tunnel_animate(int val)
{
	int t = demo_get_time();
	if(t < 11500)
		glutTimerFunc(10, tunnel_animate, 0);
	else {
		t0 += demo_get_time();
		glutDisplayFunc(plasma_render);
		glutTimerFunc(10, plasma_animate, 0);
	}

	GLint char_w_var = glGetUniformLocation(tunnel_program, "char_w");
	glUniform1i(char_w_var, char_w);
	GLint char_h_var = glGetUniformLocation(tunnel_program, "char_h");
	glUniform1i(char_h_var, char_h);
	

	glutPostRedisplay();
}

const char *tunnel_vshad =
"#version 120\n"
"void main() {"
	"gl_Position = ftransform();"
"}"
;

const char *tunnel_fshad =
"#version 120\n"
"uniform vec2 Center;"
"uniform sampler2D Tex;"
"uniform sampler2D Text;"
"uniform vec2 text_size;"
"uniform ivec2 text_pos;"
"uniform vec2 Move;"
"uniform int char_w;"
"uniform int char_h;"
"void main() {"
	"float xchar = floor(gl_FragCoord.x/char_w)*char_w;"
	"float ychar = floor(gl_FragCoord.y/char_h)*char_h;"
	"vec2 v = vec2(xchar,ychar) - Center;"
	"float dist = length(v);"
	"float angle = atan(v.y, v.x);"
	"vec2 transformedCoord = vec2(10/dist, angle*2/3.1415)+Move;"
	"vec2 text_coord = (gl_FragCoord.xy-vec2(text_pos))/text_size;"
	"if (text_coord.x >= 0.0 && text_coord.y >= 0.0 && text_coord.x <= 1.0 && text_coord.y <= 1.0 &&"
			"texture2D(Text, text_coord) == vec4(1.0, 1.0, 1.0, 1.0)) {"
		"gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);"
	"} else if (dist < 20) {"
		"gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);"
	"} else {"
		"gl_FragColor = texture2D(Tex, transformedCoord);"
	"}"
"}"
;

#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "shaders.h"

GLuint buildShader(GLenum type, const char *source);
GLuint buildProgram(GLuint vshad, GLuint fshad);
void checkVersions(void);
void key(unsigned char key, int x, int y);
void reshape(int w, int h);
void render(void);
void animate(int val);

GLuint program;
int char_w;
int char_h;
int t0;

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1000, 700);
	glutCreateWindow("test");
	GLenum err = glewInit();

	checkVersions();
	if(err == GLEW_OK)
		printf("glew ok\n");
	else
		printf("glew not ok: %s\n", glewGetErrorString(err));

	GLuint vshad = buildShader(GL_VERTEX_SHADER, plasma_vshad);
	GLuint fshad = buildShader(GL_FRAGMENT_SHADER, plasma_fshad);
	program = buildProgram(vshad, fshad);

	printf("empty logs are good! means success.\n");

	glUseProgram(program);
	glDeleteShader(vshad);
	glDeleteShader(fshad);

	char_w = 8;
	char_h = 14;
	t0 = glutGet(GLUT_ELAPSED_TIME);

	glutDisplayFunc(render);
	glutTimerFunc(10, animate, 0);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);
	glutMainLoop();

	glDeleteProgram(program);

	return 0;
}

/* Convenience fn for building a shader.
 * type must be GL_VERTEX_SHADER or GL_FRAGMENT_SHADER.
 * Single source string, NULL-terminated.
 */
GLuint buildShader(GLenum type, const char *source)
{
	char log[1024];
	GLuint handle = glCreateShader(type);
	glShaderSource(handle, 1, &source, NULL);
	glCompileShader(handle);
	// more error checking would be good, for example only print the log on failure
	glGetShaderInfoLog(handle, 1024, NULL, log);
	printf("shader compile log: %s\n", log);
	return handle;
}

GLuint buildProgram(GLuint vshad, GLuint fshad)
{
	char log[1024];
	GLuint handle = glCreateProgram();
	glAttachShader(handle, vshad);
	glAttachShader(handle, fshad);
	glLinkProgram(handle);
	// more error checking would be good, for example only print the log on failure
	glGetProgramInfoLog(handle, 1024, NULL, log);
	printf("program link log: %s\n", log);
	return handle;
}

void key(unsigned char key, int x, int y)
{
	if(key == 0x1B)
		exit(0);
	else if(key == 'f')
		glutFullScreen();
}

float windowW, windowH;

void reshape(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);

	windowW = glutGet(GLUT_WINDOW_WIDTH);
	windowH = glutGet(GLUT_WINDOW_HEIGHT);
}
void render(void)
{
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

void animate(int val)
{
	glutTimerFunc(10, animate, 0);
	int t = glutGet(GLUT_ELAPSED_TIME);

	if (t > 5000)  { char_w = 4; char_h = 7; }
	if (t > 10000) { char_w = 3; char_h = 4; }
	if (t > 13000) { char_w = 2; char_h = 2; }
	if (t > 15000) { char_w = 1; char_h = 1; }

	GLint w_var = glGetUniformLocation(program, "width");
	glUniform1i(w_var, glutGet(GLUT_WINDOW_WIDTH));
	GLint h_var = glGetUniformLocation(program, "height");
	glUniform1i(h_var, glutGet(GLUT_WINDOW_HEIGHT));
	GLint t_var = glGetUniformLocation(program, "time");
	glUniform1i(t_var, t);
	GLint char_w_var = glGetUniformLocation(program, "char_w");
	glUniform1i(char_w_var, char_w);
	GLint char_h_var = glGetUniformLocation(program, "char_h");
	glUniform1i(char_h_var, char_h);

	glutPostRedisplay();
}

void checkVersions(void)
{
	if(glewIsSupported("GL_VERSION_4_2"))
		printf("GL ver: 4.2");
	else if(glewIsSupported("GL_VERSION_4_1"))
		printf("GL ver: 4.1");
	else if(glewIsSupported("GL_VERSION_4_0"))
		printf("GL ver: 4.0");
	else if(glewIsSupported("GL_VERSION_3_3"))
		printf("GL ver: 3.3");
	else if(glewIsSupported("GL_VERSION_3_2"))
		printf("GL ver: 3.2");
	else if(glewIsSupported("GL_VERSION_3_1"))
		printf("GL ver: 3.1");
	else if(glewIsSupported("GL_VERSION_3_0"))
		printf("GL ver: 3.0");
	else if(glewIsSupported("GL_VERSION_2_1"))
		printf("GL ver: 2.1");
	else if(glewIsSupported("GL_VERSION_2_0"))
		printf("GL ver: 2.0");
	else
		printf("GL ver: less than 2.0, man what the heck\n");
	printf("\nshader ver: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

#ifdef _WIN32
#include <Windows.h>
#endif
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include "shaders.h"

void checkVersions(void);
void key(unsigned char key, int x, int y);
void reshape(int w, int h);

#ifdef _WIN32
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int argc=1;
	char *argv[1] = {"foo"};
#else
int main(int argc, char *argv[])
{
#endif
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1000, 700);
	glutCreateWindow("test");
	glutSetCursor(GLUT_CURSOR_NONE);
	GLenum err = glewInit();

	checkVersions();
	if(err == GLEW_OK)
		printf("glew ok\n");
	else
		printf("glew not ok: %s\n", glewGetErrorString(err));

	grab_screen();
	textures_init();
	plasma_init();
	tunnel_init();
	cmd_init();
	cubes_init();
	roto_init();

	t0 = demo_get_time();

	glutDisplayFunc(tunnel_render);
	glutTimerFunc(10, tunnel_animate, 0);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);
	glutFullScreen();
	glutMainLoop();
	
	return 0;
}

void key(unsigned char key, int x, int y)
{
	if(key == 0x1B)
		exit(0);
	else if(key == 'f')
		glutFullScreen();
}

void reshape(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
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

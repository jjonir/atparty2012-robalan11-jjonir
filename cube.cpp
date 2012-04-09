#include <gl/glew.h>
#include <gl/glut.h>
#include <math.h>
#include <stdio.h>

#include "shaders.h"

struct point {
	float x, y, z;
};

class Cube {
private:
	point corners [8];
	point position;
	point rotation;
public:
	Cube() {
		corners[0].x = corners[1].x = corners[2].x = corners[3].x = 0.0f; // plane x=0
		corners[4].x = corners[5].x = corners[6].x = corners[7].x = 1.0f; // plane x=1
		corners[0].y = corners[1].y = corners[4].y = corners[5].y = 0.0f; // plane y=0
		corners[2].y = corners[3].y = corners[6].y = corners[7].y = 1.0f; // plane y=1
		corners[0].z = corners[3].z = corners[4].z = corners[7].z = 0.0f; // plane z=0
		corners[1].z = corners[2].z = corners[5].z = corners[6].z = 1.0f; // plane z=1

		position.x = position.y = position.z = 0;
		rotation.x = rotation.y = rotation.z = 0;
	}

	void drawFace(point c1, point c2, point c3, point c4) {
		point a, b, N;
		a.x = c1.x - c2.x;
		a.y = c1.y - c2.y;
		a.z = c1.z - c2.z;
		b.x = c2.x - c3.x;
		b.y = c2.y - c3.y;
		b.z = c2.z - c3.z;
		N.x = a.y * b.z - a.z * b.y;
		N.y = a.z * b.x - a.x * b.z;
		N.z = a.x * b.y - a.y * b.x;
		glNormal3f(N.x, N.y, N.z);
		glVertex3f(c1.x + position.x, c1.y + position.y, c1.z + position.z);
		glVertex3f(c2.x + position.x, c2.y + position.y, c2.z + position.z);
		glVertex3f(c3.x + position.x, c3.y + position.y, c3.z + position.z);
		glVertex3f(c4.x + position.x, c4.y + position.y, c4.z + position.z);
	}

	void draw() {
		glBegin(GL_QUADS);
			drawFace(corners[0],corners[1],corners[2],corners[3]);
			drawFace(corners[0],corners[3],corners[7],corners[4]);
			drawFace(corners[0],corners[4],corners[5],corners[1]);
			drawFace(corners[6],corners[5],corners[4],corners[7]);
			drawFace(corners[6],corners[2],corners[1],corners[5]);
			drawFace(corners[6],corners[7],corners[3],corners[2]);
		glEnd();
	}

	void setPos(float x, float y, float z) {
		position.x = x;
		position.y = y;
		position.z = z;
	}

	float x() { return position.x; }
	float y() { return position.y; }
	float z() { return position.z; }
};

Cube cubes[10];

void cube_init(void) {
	cubes[0].setPos(1.0, 1.0, 0.0);
	cubes[1].setPos(1.0, 0.0, 1.0);
}

void cube_render(void) {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	int t = demo_get_time();
	
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	GLfloat ambient[]  = {0.2f, 0.0f, 0.0f, 1.0f};
	GLfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat position[] = {5.0f, 5.0f, 2.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHT0);
	glLoadIdentity();
	gluLookAt(10.0 * sin(t/1000.0), 3.0, 10.0 * cos(t/1000.0), 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	for (int i = 0; i < 10; i++) {
		cubes[i].draw();
	}

	glutSwapBuffers();
}

void cube_animate(int val) {
	glutTimerFunc(10, cube_animate, 0);
	glutPostRedisplay();
}
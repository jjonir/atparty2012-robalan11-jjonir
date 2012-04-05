#include <gl/glew.h>

struct point {
	float x, y, z;
};

class Cube {
private:
	point corners [8];
	point position;
public:
	Cube() {
		corners[0].x = corners[1].x = corners[2].x = corners[3].x = 0.0f; // plane x=0
		corners[4].x = corners[5].x = corners[6].x = corners[7].x = 1.0f; // plane x=1
		corners[0].y = corners[1].y = corners[4].y = corners[5].y = 0.0f; // plane y=0
		corners[2].y = corners[3].y = corners[6].y = corners[7].y = 1.0f; // plane y=1
		corners[0].z = corners[3].z = corners[4].z = corners[7].z = 0.0f; // plane z=0
		corners[1].z = corners[2].z = corners[5].z = corners[6].z = 1.0f; // plane z=1

		position.x = 0;
		position.y = 0;
		position.z = 0;
	}

	void drawFace(point c1, point c2, point c3, point c4) {
		glVertex3f(c1.x, c1.y, c1.z);
		glVertex3f(c2.x, c2.y, c2.z);
		glVertex3f(c3.x, c3.y, c3.z);
		glVertex3f(c4.x, c4.y, c4.z);
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
};
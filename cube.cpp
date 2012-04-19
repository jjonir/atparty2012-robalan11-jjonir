#include <gl/glew.h>
#include <gl/glut.h>
#include <math.h>
#include <stdio.h>

#include "shaders.h"
#include "utils.h"

GLuint cube_program;

class Cube {
private:
	Point corners [8];
	Point position;
	Point rotation;
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

	void drawFace(Point c1, Point c2, Point c3, Point c4) {
		Point a, b, N;
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
	GLuint vshad = buildShader(GL_VERTEX_SHADER, cube_vshad, "cube vertex");
	GLuint fshad = buildShader(GL_FRAGMENT_SHADER, cube_fshad, "cube fragment");
	cube_program = buildProgram(vshad, fshad, "cube");
	glDeleteShader(vshad);
	glDeleteShader(fshad);
}

void cube_render(void) {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	int t = demo_get_time();

	float camera_x = 3.0 * sin(t/3000.0);
	float camera_y = 1.0;
	float camera_z = 3.0 * cos(t/3000.0);

	glUseProgram(cube_program);
	GLint t_var = glGetUniformLocation(cube_program, "time");
	glUniform1i(t_var, t);
	//float spheres[4*2] = {0.0, 0.0, 0.0, 0.5, 2.0, 2.0, 2.0, 0.75};
	float position[3] = {camera_x, camera_y, camera_z};
	glUniform3fv(glGetUniformLocation(cube_program, "rO"), 1, position);

	glBegin(GL_QUADS);
		glVertex3f(-1.0f, -1.0f, -1);
		glVertex3f(1.0f, -1.0f, -1);
		glVertex3f(1.0f, 1.0f, -1);
		glVertex3f(-1.0f, 1.0f, -1);
	glEnd();
	
	/*glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	GLfloat ambient[]  = {0.2f, 0.0f, 0.0f, 1.0f};
	GLfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat position[] = {5.0f, 5.0f, 2.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHT0);
	glLoadIdentity();
	gluLookAt(camera_x, camera_y, camera_z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	for (int i = 0; i < 10; i++) {
		cubes[i].draw();
	}*/

	glutSwapBuffers();
}

void cube_animate(int val) {
	glutTimerFunc(10, cube_animate, 0);
	glutPostRedisplay();
}

const char *cube_vshad =
"#version 120\n"
"uniform vec3 rO;"
"void main(void)"
"{"
    "gl_Position=gl_Vertex;"
	"vec3 d=normalize(vec3(0.0,0.0,0.0)-rO);"
    "vec3 r=normalize(cross(d,vec3(0.0,1.0,0.0)));"
    "vec3 u=cross(r,d);"
    "vec3 e=vec3(gl_Vertex.x*1.778,gl_Vertex.y,.75);"   //    eye space ray
    "gl_TexCoord[0].xyz=mat3(r,u,d)*e;"                 //  world space ray
    "gl_TexCoord[1]=vec4(.5)+gl_Vertex*.5;"             // screen space coordinate
"}";
;

const char *cube_fshad =
"#version 120\n"
"uniform int time;"
"uniform vec3 rO;"
"float intSph(in vec3 rS, in vec3 rD, in vec4 sph) {"
    "vec3  p = rS - sph.xyz;"
    "float b = dot( p, rD );"
    "float c = dot( p, p ) - sph.w*sph.w;"
    "float h = b*b - c;"
    "if( h>0.0 )"
    "{"
        "h = -b - sqrt( h );"
    "}"
	"return h;"
"}"

"float intAAQ(in vec3 rS, in vec3 rD, in vec3[] quad) {"
	"return 0;"
"}"

"void main() {"
	"vec3 l = vec3(2.0,-2.0,2.0);"
	"vec4 s[4] = vec4[4](vec4(0.0, 0.0, 0.0, 0.8),vec4(1.2, 0.0, 0.0, 0.25), vec4(2.0, 0.3, 0.4, 0.4), vec4(2.0, 0.3, -0.5, 0.4));"
	"vec3 rD = normalize(gl_TexCoord[0].xyz);"
	"float t = 100;"
	"float t0;"
	"int hit = -1;"
	"for (int i = 0; i < 4; i++) {"
		"t0 = intSph(rO, rD, s[i]);"
		"if (t0 > 0 && t0 < t) {"
			"t = t0;"
			"hit = i;"
		"}"
	"}"
	"if (hit == -1) { gl_FragColor = vec4(1.0, 0.9, 0.7, 1.0); }"
	"else {"
		"int ref_hit = -1;"
		"vec3 inter = rO + rD * t;"
		"vec3 norm = normalize(inter - s[hit].xyz);"
		"vec3 rR = reflect(rD, norm);"
		
		"for (int i = 0; i < 4; i++) {"
			"if (i == hit) { continue; }"
			"t0 = intSph(inter, rR, s[i]);"
			"if (t0 > 0 && t0 < t) {"
				"t = t0;"
				"ref_hit = i;"
			"}"
		"}"

		"if (ref_hit == -1) {"
			"vec3 lray = normalize(inter - l);"
			"float lamb = dot(lray, norm);"
			"gl_FragColor = lamb * vec4(0.8,0.7,0.3,0.0) + vec4(0.2, 0.2, 0.4, 1.0);"
		"}"
		"else {"
			"vec3 ref_inter = inter + rR * t;"
			"vec3 ref_norm = normalize(ref_inter - s[ref_hit].xyz);"
			
			"vec3 lray = normalize(ref_inter - l);"
			"float lamb = dot(lray, ref_norm);"

			"gl_FragColor = lamb * vec4(0.8,0.7,0.3,0.0) + vec4(0.2, 0.2, 0.4, 1.0);"
		"}"
	"}"
"}"
;
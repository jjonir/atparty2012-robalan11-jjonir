#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>

const char *vshad_str[] = {"#version 140\n\
uniform mat4 MVPMatrix; in vec4 mcVertex;\n\
void main() { gl_Position = MVPMatrix * mcVertex; }"};
const char *fshad_str[] = {"#version 140\n\
out vec3 FragmentColor;\n\
void main() { FragmentColor = vec3(1.0, 1.0, 1.0); }"};

void reshape(int w, int h);
void render(void);

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(320, 320);
	glutCreateWindow("test");
	GLenum err = glewInit();
	
	if(err == GLEW_OK)
		printf("glew ok\n");
	else
		printf("glew not ok: %s\n", glewGetErrorString(err));
	
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
	
	char log[1024];
	
	GLuint vshad = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vshad, 1, vshad_str, NULL);
	glCompileShader(vshad);
	glGetShaderInfoLog(vshad, 1024, NULL, log);
	printf("vertex shader compile log: %s\n", log);
	
	GLuint fshad = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fshad, 1, fshad_str, NULL);
	glCompileShader(fshad);
	glGetShaderInfoLog(fshad, 1024, NULL, log);
	printf("fragment shader compile log: %s\n", log);
	
	GLuint program = glCreateProgram();
	glAttachShader(program, vshad);
	glAttachShader(program, fshad);
	glLinkProgram(program);
	glGetProgramInfoLog(program, 1024, NULL, log);
	printf("program link log: %s\n", log);
	
	printf("empty logs are good! means success.\n");
	
	glUseProgram(program);
	glDeleteShader(vshad);
	glDeleteShader(fshad);
	
	glutDisplayFunc(render);
	glutReshapeFunc(reshape);
	glutMainLoop();
	
	glDeleteProgram(program);
	
	return 0;
}

void reshape(int w, int h)
{
	if(h == 0)
		h = 1;
	float ratio = (float)w / h;
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(80, ratio, 1, 100);
	glMatrixMode(GL_MODELVIEW);
}
void render(void)
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	
	glBegin(GL_TRIANGLES);
		glVertex3f(-2, -2, -5);
		glVertex3f(2, 0, -5);
		glVertex3f(0, 2, -5);
	glEnd();
	
	glutSwapBuffers();
}


#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

const char *vshad_str[] = {"#version 120\n\
void main() { gl_Position = ftransform(); }"};
const char *fshad_str[] = {"#version 140\n\
uniform int width;\
uniform int height;\
uniform int time;\
uniform int char_w;\
uniform int char_h;\
void main() { float xchar = floor(gl_FragCoord.x/char_w)*char_w;\
              float ychar = floor(gl_FragCoord.y/char_h)*char_h;\
              float color = (sin(sqrt(pow(width/2-xchar,2)+pow(height/2-ychar,2))/50-time/300.0)+\
                                  sin((xchar+ychar)/30+time/230.0)+\
								  sin(xchar/37-time/600.0) + 1.5)/4.5;\
              if(color < 0.167) gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\
			  else if(color < 0.333) gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);\
			  else if(color < 0.5) gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\
			  else if(color < 0.667) gl_FragColor = vec4(0.0, 1.0, 1.0, 1.0);\
			  else if(color < 0.833) gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);\
			  else gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0); }"};

void reshape(int w, int h);
void render(void);
void animate(void);

GLuint program;
int char_w;
int char_h;

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1000, 700);
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
	
	program = glCreateProgram();
	glAttachShader(program, vshad);
	glAttachShader(program, fshad);
	glLinkProgram(program);
	glGetProgramInfoLog(program, 1024, NULL, log);
	printf("program link log: %s\n", log);
	
	printf("empty logs are good! means success.\n");

	glUseProgram(program);
	glDeleteShader(vshad);
	glDeleteShader(fshad);
	
	char_w = 8;
	char_h = 14;

	glutDisplayFunc(render);
	glutIdleFunc(animate);
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
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}
void render(void)
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	
	float windowW = glutGet(GLUT_WINDOW_WIDTH);
	float windowH = glutGet(GLUT_WINDOW_HEIGHT);

	float consoleX = 0.5 + 0.2*sin(clock()/2000.0);
	float consoleY = 0.5 + 0.2*cos(clock()/2000.0);
	
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

void animate(void)
{	
	int t = clock();

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

	render();
}
#include <gl/glew.h>
#include <gl/glut.h>
#include <stdlib.h>
#include <math.h>
#include "shaders.h"

GLuint roto_program;

void roto_init(void)
{
	GLuint vshad = buildShader(GL_VERTEX_SHADER, roto_vshad, "roto vertex");
	GLuint fshad = buildShader(GL_FRAGMENT_SHADER, roto_fshad, "roto fragment");
	roto_program = buildProgram(vshad, fshad, "roto");
	glDeleteShader(vshad);
	glDeleteShader(fshad);
}

void roto_render(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(roto_program);

	float t = (float)demo_get_time() / 1000.0;
	int dtex;
	float divzoom;
	float zoomstep = 100;

	if(pow(1.5f, t) < zoomstep) { dtex = 0; divzoom = 1; }
	else if(pow(1.5f, t) < zoomstep*zoomstep) { dtex = 1; divzoom = zoomstep; }
	else {dtex = 2; divzoom = zoomstep*zoomstep; }

	GLint roto_var = glGetUniformLocation(roto_program, "roto");
	//glUniform1f(roto_var, 0);
	glUniform1f(roto_var, 2.0 * sin(0.5 * t));

	GLint zoom_var = glGetUniformLocation(roto_program, "zoom");
	//glUniform1f(zoom_var, 1);
	glUniform1f(zoom_var, pow(2.0f, t) / divzoom);
	//glUniform1f(zoom_var, 1.0);

	GLint zoomstep_var = glGetUniformLocation(roto_program, "zoomstep");
	glUniform1f(zoomstep_var, zoomstep);

	GLint pan_var = glGetUniformLocation(roto_program, "pan");
	glUniform2f(pan_var, 0.0, 0.0);
	//glUniform2f(pan_var, 3*sin(15*t/1000)/4, 2*cos(0.67*t/1000)/4);

	GLint res_var = glGetUniformLocation(roto_program, "resolution");
	//glUniform2f(res_var, (float)glutGet(GLUT_WINDOW_WIDTH), (float)glutGet(GLUT_WINDOW_WIDTH));
	glUniform2f(res_var, (float)glutGet(GLUT_WINDOW_WIDTH), (float)glutGet(GLUT_WINDOW_HEIGHT)+500.0*cos(1.2*t));

	GLint t0_var = glGetUniformLocation(roto_program, "tex0");
	glActiveTexture(GL_TEXTURE0);
	if(dtex <= 3)
		glBindTexture(GL_TEXTURE_2D, textures[ROTO_0_TEXTURE+dtex]);
	else
		glBindTexture(GL_TEXTURE_2D, textures[ROTO_0_TEXTURE]);
	glUniform1i(t0_var, 0);
	
	GLint t1_var = glGetUniformLocation(roto_program, "tex1");
	glActiveTexture(GL_TEXTURE4);
	if(dtex <= 2)
		glBindTexture(GL_TEXTURE_2D, textures[ROTO_1_TEXTURE+dtex]);
	else
		glBindTexture(GL_TEXTURE_2D, textures[ROTO_0_TEXTURE]);
	glUniform1i(t1_var, 4);
	
	GLint t2_var = glGetUniformLocation(roto_program, "tex2");
	glActiveTexture(GL_TEXTURE8);
	if(dtex <= 1)
		glBindTexture(GL_TEXTURE_2D, textures[ROTO_2_TEXTURE+dtex]);
	else
		glBindTexture(GL_TEXTURE_2D, textures[ROTO_0_TEXTURE]);
	glUniform1i(t2_var, 8);

	glBegin(GL_QUADS);
		glVertex3f(0, 0, -1);
		glVertex3f(1, 0, -1);
		glVertex3f(1, 1, -1);
		glVertex3f(0, 1, -1);
	glEnd();

	glutSwapBuffers();
}

void roto_animate(int val)
{
	int t = demo_get_time();
	if(t < 26000) {
		glutTimerFunc(10, roto_animate, 0);
	} else {
		t0 += demo_get_time();
		glutDisplayFunc(cubes_render);
		glutTimerFunc(10, cubes_animate, 0);
		//t0 = demo_get_time();
		//exit(0);
	}

	glutPostRedisplay();
}

const char *roto_vshad =
"#version 120\n"
"void main() {"
"	gl_Position = ftransform();"
"}"
;

const char *roto_fshad =
"#version 120\n"
"uniform float roto;"
"uniform float zoom;"
"uniform float zoomstep;"
"uniform vec2 pan;"
"uniform vec2 resolution;"
"uniform sampler2D tex0;"
"uniform sampler2D tex1;"
"uniform sampler2D tex2;"

"void main(void) {"
  "vec2 uv;"
  "float temp;"

  "uv = gl_FragCoord.xy / resolution.xy - vec2(0.5);"
  //"uv.y = -uv.y;"

  "uv += pan;"
  "temp = uv.x;"
  "uv.x = uv.x*cos(roto) - uv.y*sin(roto);"
  "uv.y = temp*sin(roto) + uv.y*cos(roto);"
  "uv /= vec2(zoom);"
  "if((abs(uv.x) > 1/zoomstep/2) || (abs(uv.y) > 1/zoomstep/2)) {"
    "gl_FragColor = texture2D(tex0, uv + vec2(0.5));"
  "} else if((abs(uv.x) > 1/zoomstep/zoomstep/2) || (abs(uv.y) > 1/zoomstep/zoomstep/2)) {"
    "gl_FragColor = texture2D(tex1, uv*zoomstep + vec2(0.5));"
  "} else {"
    "gl_FragColor = texture2D(tex2, uv*zoomstep*zoomstep + vec2(0.5));"
  "}"
"}"
;

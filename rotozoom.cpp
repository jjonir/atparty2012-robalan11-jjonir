#include <gl/glew.h>
#include <gl/glut.h>
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

	GLint roto_var = glGetUniformLocation(roto_program, "roto");
	glUniform1f(roto_var, 0);
	//glUniform1f(roto_var, 2.0 * sin(0.5 * t));

	GLint zoom_var = glGetUniformLocation(roto_program, "zoom");
	glUniform1f(zoom_var, 1);
	//glUniform1f(zoom_var, pow(1.5f, t));
	//glUniform1f(zoom_var, 1.0);

	GLint pan_var = glGetUniformLocation(roto_program, "pan");
	glUniform2f(pan_var, 0.0, 0.0);
	//"vec2(3.0*sin(1.5*time)/4.0, 2.0*cos(0.67*time)/4.0);"

	GLint res_var = glGetUniformLocation(roto_program, "resolution");
	glUniform2f(res_var, (float)glutGet(GLUT_WINDOW_WIDTH), (float)glutGet(GLUT_WINDOW_WIDTH));
	//glUniform2f(res_var, (float)glutGet(GLUT_WINDOW_WIDTH), (float)glutGet(GLUT_WINDOW_HEIGHT)+500.0*cos(1.2*t));

	GLint t0_var = glGetUniformLocation(roto_program, "tex0");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[ROTO_5_TEXTURE]);
	glUniform1i(t0_var, 0);
	
	GLint t1_var = glGetUniformLocation(roto_program, "tex1");
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, textures[ROTO_1_TEXTURE]);
	glUniform1i(t1_var, 4);
	
	GLint t2_var = glGetUniformLocation(roto_program, "tex2");
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, textures[ROTO_2_TEXTURE]);
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
	if(t < 10000) {
		glutTimerFunc(10, roto_animate, 0);
	} else {
		//t0 = demo_get_time();
		glutTimerFunc(10, roto_animate, 0);
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
"uniform vec2 pan;"
"uniform vec2 resolution;"
"uniform sampler2D tex0;"
"uniform sampler2D tex1;"
"uniform sampler2D tex2;"
// TODO only two (three?) textures here cycle sampler params and zoom level in calling code

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
  "if((abs(uv.x) > 0.025) || (abs(uv.y) > 0.025)) {"
    "gl_FragColor = texture2D(tex0, uv + vec2(0.5));"
  "} else if((abs(uv.x) > 0.00125) || (abs(uv.y) > 0.00125)) {"
    "gl_FragColor = texture2D(tex1, uv*20.0 + vec2(0.5));"
  "} else {"
    "gl_FragColor = texture2D(tex2, uv*400.0 + vec2(0.5));"
  "}"
"}"
;

#include <gl/glew.h>
#include <gl/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "shaders.h"
#include "utils.h"

GLuint cubes_program;

class CamPos {
public:
	CamPos() {
		curr = 0;
		max = 5;
		setPos(0, 0, 15.0, -1.5, 0);
		setPos(1, 10, 5.0, 2.5, 0);
		setPos(2, 10, -5.0, 8.0, 1.0);
		setPos(3, 20, -4.0, -5.0, 1.0);
		setPos(4, 20, -3.0, 1.0, -10.0);
		setPos(5, 30, 1.0, 1.0, 12.0);
	}

	void setPos(int i, float t, float x, float y, float z) {
		positions[i][0] = t;
		positions[i][1] = x;
		positions[i][2] = y;
		positions[i][3] = z;
	}

	void getPos(float t, float *pos) {
		if (t > positions[curr+1][0] && curr < max) curr++;
		if (t > positions[curr+1][0] && curr < max) curr++;
		float f = (t-positions[curr][0])/(positions[curr+1][0]-positions[curr][0]);
		float rx = (1-f)*positions[curr][1] + f*positions[curr+1][1];
		float ry = (1-f)*positions[curr][2] + f*positions[curr+1][2];
		float rz = (1-f)*positions[curr][3] + f*positions[curr+1][3];
		pos[0] = rx;
		pos[1] = ry;
		pos[2] = rz;
	}

	int curr;
	int max;
	float positions[16][4];
};

CamPos camPos = CamPos();

void cubes_init(void) {
	GLuint vshad = buildShader(GL_VERTEX_SHADER, cubes_vshad, "cube vertex");
	GLuint fshad = buildShader(GL_FRAGMENT_SHADER, cubes_fshad, "cube fragment");
	cubes_program = buildProgram(vshad, fshad, "cubes");
	glDeleteShader(vshad);
	glDeleteShader(fshad);
}

void cubes_render(void) {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	int t = demo_get_time();
	
	float windowW = glutGet(GLUT_WINDOW_WIDTH);
	float windowH = glutGet(GLUT_WINDOW_HEIGHT);

	glUseProgram(cubes_program);
	glUniform1f(glGetUniformLocation(cubes_program, "time"), t/1000.0);
	glUniform1f(glGetUniformLocation(cubes_program, "fade"), 1.0);
	float resolution[2] = {windowW, windowH};
	glUniform2fv(glGetUniformLocation(cubes_program, "resolution"), 1, resolution);
	float position[3];
	camPos.getPos(t/1000.0, position);
	glUniform3fv(glGetUniformLocation(cubes_program, "pos"), 1, position);

	glBegin(GL_QUADS);
		glVertex3f(0.0f, 0.0f, -1);
		glVertex3f(1.0f, 0.0f, -1);
		glVertex3f(1.0f, 1.0f, -1);
		glVertex3f(0.0f, 1.0f, -1);
	glEnd();
	
	glutSwapBuffers();
}

void cubes_animate(int val) {
	int t = demo_get_time();
	if(t < 30000)
		glutTimerFunc(10, cubes_animate, 0);
	else {
		//t0 += demo_get_time();
		//glutDisplayFunc(roto_render);
		//glutTimerFunc(10, roto_animate, 0);
		exit(0);
	}
	glutPostRedisplay();
}

const char *cubes_vshad =
"#version 120\n"
"void main() {"
"	gl_Position = ftransform();"
"}"
;

const char *cubes_fshad =
"#version 120\n"
"uniform vec2 resolution;\n"
"uniform vec3 pos;\n"
"uniform float time;\n"
"uniform float fade;\n"

"float ease(in float p) {return 3.0*p*p-2.0*p*p*p;}\n"

"vec4 map(in vec3 p, in bool norm)\n"
"{\n"
    "vec3 c = vec3(2.0, 2.0, 25.0);\n"
    "vec3 bbox = vec3(7.0,7.0,2.0);\n"
    "vec3 bbpos = vec3(-1.0,1.0,12.5);\n"

    "float cx = floor(p.x/c.x)+1.0;\n"
    "float cy = floor(p.y/c.y);\n"

    "vec3 q = mod(p,c) - 0.5*c;\n"
    "vec3 b = vec3(0.7,0.7,0.7);\n"
    "float bb = length(max(abs(p-bbpos)-bbox,0.0));\n"
	"vec3  di = abs(q) - b;"
	"float mc = max(max(di.x,di.y),di.z);"
	"float cub = min(mc,length(max(di,0.0))-0.2);"
	"if (!norm) cub = max(cub,bb);\n"

    "float f = dot(p, vec3(0.0, 1.0, 0.0)) + 6.0;\n"
    "float d = min(cub,f);\n"

    "return (d == f) ? vec4(d, -10.0, -10.0, 0.0) : vec4(d, cx, cy, 0.0);\n"
"}\n"

"vec4 intersect( in vec3 ro, in vec3 rd )\n"
"{\n"
    "float t = 0.0;\n"
    "vec4 res = vec4(-1.0);\n"
	"for(float i=0.0;i<64.0;i++){\n"
	     "vec4 h = map(ro + rd*t,false);\n"
		 "if( h.x<0.002 && res.x<0.0){\n"
		      "res = vec4(t,h.yz,i);\n"
			  "i = 64.0;"
         "}\n"
		 "t += h.x*pow(1.01,i);\n"
    "}\n"
    "return res;\n"
"}\n"

"vec3 calcNormal(in vec3 pos)\n"
"{\n"
    "vec3  eps = vec3(.005,0.0,0.0);\n"
    "vec3 nor;\n"
    "nor.x = map(pos+eps.xyy,true).x - map(pos-eps.xyy,true).x;\n"
    "nor.y = map(pos+eps.yxy,true).x - map(pos-eps.yxy,true).x;\n"
    "nor.z = map(pos+eps.yyx,true).x - map(pos-eps.yyx,true).x;\n"
    "return normalize(nor);\n"
"}\n"

"bool inAt(in float x, in float y)\n"
"{\n"
    "bool ret = false;\n"
    "if (abs(x) < 0.1 && abs(y) < 0.1) ret = false;\n"
    "else if (abs(x-2.0) < 0.1 && abs(y+1.0) < 0.1) ret = true;\n"
    "else if (abs(x-3.0) < 0.1 && abs(y+2.0) < 0.1) ret = false;\n"
    "else if (abs(x) <= 1.0 && abs(y) <= 1.0) ret = true;\n"
    "else if (abs(x) <= 2.0 && abs(y) <= 2.0) ret = false;\n"
    "else if (abs(x) == 3.0 && abs(y) == 3.0) ret = false;\n"
	"else if (abs(x) <= 3.0 && abs(y) <= 3.0) ret = true;\n"
    "return ret;\n"
"}\n"

"vec3 skyCol(in vec3 rd, in vec3 sunpos, in float t)\n"
"{\n"
    "float dt = clamp(t,0.0,1.3);\n"
    "float nt = clamp(t-1.1,0.2,1.2);\n"

    "vec3 col = vec3(0.6, 0.7, 0.9);\n"
    "float sun = max(pow(clamp(dot(rd,sunpos),0.0,1.0),16.0),0.0);\n"

    "col += vec3(0.6,0.6,0.4)*sun;\n"
    "vec3 daycol = vec3(0.5,0.5,0.5)*pow(0.7-clamp(rd.y,0.0,1.0),3.0);\n"
    "vec3 setcol = vec3(0.7,0.0,-0.6)*(pow(0.7-clamp(rd.y,0.0,1.0),3.0)+0.3*rd.z);\n"
    "col += mix(daycol,setcol,dt*dt*dt);\n"
    "col = mix(col,vec3(0.0,0.0,0.1),ease(nt-0.2));\n"
	
	"return col;\n"
"}\n"

"vec3 lamb(in vec3 norm, in vec3 light)\n"
"{\n"
    "float dif1 = max(0.4 + 0.6*dot(norm,light),0.0);\n"
    "vec3 col  = vec3(0.2,0.2,0.2);\n"
    "col += 2.0*dif1*vec3(1.0,0.97,0.85);\n"
    "col += 1.0*(0.5+0.5*norm.y)*vec3(0.1,0.15,0.2);\n"
    "return col;\n"
"}\n"

"vec3 matCol(in vec4 mat)\n"
"{\n"
    "if (inAt(mat.y, mat.z)) return vec3(0.1);\n"
    "else if (mat.y == -10.0) return vec3(0.4);\n"
	"else return vec3(0.9);\n"
"}\n"

"void main(void)\n"
"{\n"
    "vec2 p = -1.0 + 2.0 * gl_FragCoord.xy / resolution.xy;\n"
    "p.x *= resolution.x/resolution.y;\n"

    "float t = 0.1*(25.0-time);\n"

    // light
    "vec3 light = normalize(vec3(0.0,1.0-t,1.0));\n"
    "vec3 lightcol = skyCol(normalize(vec3(0.0,1.0,1.0)),light,t);\n"

    // camera
	"vec3 ro = pos;\n"
    "vec3 ww = normalize(vec3(1.0,1.0,12.5) - ro);\n"
    "vec3 uu = normalize(cross( vec3(0.0,1.0,0.0), ww ));\n"
    "vec3 vv = normalize(cross(ww,uu));\n"
    "vec3 rd = normalize( p.x*uu + p.y*vv + 1.5*ww );\n"

    "vec3 col = vec3(0.0);\n"
    "vec4 tmat = intersect(ro,rd);\n"
    "if( tmat.x>0.0 )\n"
    "{\n"
        "vec3 pos = ro + tmat.x*rd;\n"
        "vec3 nor = calcNormal(pos);\n"
        "vec3 rr = reflect(rd,nor);\n"

        "vec4 rmat = intersect(pos+0.1*rr,rr);\n"

        "if (rmat.x>0.0) {\n"
            "vec3 rpos = pos + rmat.x*rr;\n"
            "vec3 rnor = calcNormal(rpos);\n"
            "vec3 rr2 = reflect(rr,rnor);\n"

            "vec4 rmat2 = intersect(rpos+0.1*rr2,rr2);\n"
         
            "if (rmat2.x>0.0) {\n"
                "vec3 rpos2 = rpos + rmat2.x*rr2;\n"
                "vec3 rnor2 = calcNormal(rpos2);\n"
                "col = lamb(rnor2, light);\n"
                "col *= matCol(rmat2);\n"
                "col *= vec3(0.2,0.2,0.3);\n"
            "} else {\n"
                "col = skyCol(rr2,light,t);\n"
            "}\n"
            "col *= matCol(rmat);\n"
        "} else {\n"
            "col = skyCol(rr,light,t);\n"
        "}\n"
        "col *= matCol(tmat);\n"
        "if (tmat.y==-10.0) col = mix(col,matCol(tmat),0.6);\n"

        // shadow
        //float ldis = 4.0;
        //vec4 shadow = intersect( pos + light*ldis, -light );
        //if( shadow.x>0.0 && shadow.x<(ldis-0.01) ) dif1=0.0;

        "col *= lightcol;\n"
    "} else {\n"
        "col = skyCol(rd,light,t);\n"
    "}\n"
	"gl_FragColor = vec4(col*clamp(fade,0.0,1.0),1.0);\n"
"}\n"
;

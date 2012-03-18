const char *plasma_vshad =
"#version 120\n"
"void main() {"
"	gl_Position = ftransform();"
"}"
;

const char *plasma_fshad =
"#version 120\n"
"uniform int width;"
"uniform int height;"
"uniform int time;"
"uniform int char_w;"
"uniform int char_h;"
"void main() {"
"	float xchar = floor(gl_FragCoord.x/char_w)*char_w;"
"	float ychar = floor(gl_FragCoord.y/char_h)*char_h;"
"	float color = (sin(sqrt(pow(width/2-xchar,2)+pow(height/2-ychar,2))/50-time/300.0)+"
"						sin((xchar+ychar)/30+time/230.0)+"
"						sin(xchar/37-time/600.0) + 1.5)/4.5;"
"	if(color < 0.167)"
"		gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);"
"	else if(color < 0.333)"
"		gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);"
"	else if(color < 0.5)"
"		gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);"
"	else if(color < 0.667)"
"		gl_FragColor = vec4(0.0, 1.0, 1.0, 1.0);"
"	else if(color < 0.833)"
"		gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);"
"	else"
"		gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0);"
"}"
;

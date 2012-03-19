const char *tunnel_vshad =
"#version 120\n"
"void main() {"
"	gl_Position = ftransform();"
"}"
;

const char *tunnel_fshad =
"#version 120\n"
"uniform int width;"
"uniform int height;"
"uniform int time;"
"uniform int char_w;"
"uniform int char_h;"
"void main() {"
"	gl_FragColor = 1.0;"
"}"
;

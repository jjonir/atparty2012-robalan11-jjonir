const char *tunnel_vshad = {
"#version 120"
"int main() {"
"	gl_Position = ftransform();"
"}"
};

const char *tunnel_fshad = {
"#version 120"
"int main() {"
"	gl_FragColor = 1.0;"
"}"
};

#version 400

layout (vertices = 1) out;

void main() {
  gl_TessLevelOuter[0] = gl_in[0].gl_Position.x;
  gl_TessLevelOuter[1] = gl_in[0].gl_Position.y;
}
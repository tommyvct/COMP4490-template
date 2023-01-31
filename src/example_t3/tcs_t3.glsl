#version 400

layout (vertices = 4) out;

in vec4 color[];
out vec4 colorTES[];

void main() {
  if (gl_InvocationID == 0) {
    gl_TessLevelInner[0] = 11;
    gl_TessLevelInner[1] = 11;
    gl_TessLevelOuter[0] = 11;
    gl_TessLevelOuter[1] = 11;
    gl_TessLevelOuter[2] = 11;
    gl_TessLevelOuter[3] = 11;
  }
  
  gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
  colorTES[gl_InvocationID] = color[gl_InvocationID];
}
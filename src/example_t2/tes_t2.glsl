#version 400

layout (triangles, equal_spacing, ccw) in;

in vec4 colorTES[];
flat out vec4 color;

void main() {
  vec4 p1 = gl_TessCoord.s * gl_in[0].gl_Position;
  vec4 p2 = gl_TessCoord.t * gl_in[1].gl_Position;
  vec4 p3 = gl_TessCoord.p * gl_in[2].gl_Position;
  vec4 pos = p1 + p2 + p3;
  
  gl_Position = pos;
  
  // interpolate the color and send it to the fragment shader
  color = gl_TessCoord.s * colorTES[0] +
          gl_TessCoord.t * colorTES[1] +
          gl_TessCoord.p * colorTES[2];
}
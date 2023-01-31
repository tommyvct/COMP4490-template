#version 400

layout (quads, equal_spacing, ccw) in;

uniform mat4 ModelView;
uniform mat4 Projection;
uniform float Time;

in vec4 colorTES[];
flat out vec4 color;

void main() {
  float t = gl_TessCoord.s;
  vec4 p1 = gl_in[0].gl_Position*(1-t) + gl_in[3].gl_Position*t;
  vec4 p2 = gl_in[1].gl_Position*(1-t) + gl_in[2].gl_Position*t;
  t = gl_TessCoord.t;
  vec4 pos = p1*(1-t) + p2*t;

  // this is being done in screen space; could be better
  vec4 u = sin(Time + 3.5*pos);
  pos.y += 0.1 * u.x * u.y;

  gl_Position = pos;

  // illumination is still calculated on the flat surface; needs improvement
  t = gl_TessCoord.s;
  vec4 c1 = colorTES[0]*(1-t) + colorTES[3]*t;
  vec4 c2 = colorTES[1]*(1-t) + colorTES[2]*t;
  t = gl_TessCoord.t;
  color = c1*(1-t) + c2*t;
}
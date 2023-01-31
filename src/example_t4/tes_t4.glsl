#version 400

layout (isolines, equal_spacing) in;

uniform mat4 ModelView;
uniform mat4 Projection;
uniform bool Latitude;

flat out vec4 color;

#define PI 3.1415926535897932384626433832795

void main() {
  float r, x = gl_TessCoord.s, y = gl_TessCoord.t, z = 0;

  if (Latitude) {
    r = cos(2*PI*gl_TessCoord.t);
    x = r * sin(2*PI*gl_TessCoord.s);
    y = sin(2*PI*gl_TessCoord.t);
    z = r * cos(2*PI*gl_TessCoord.s);
  } else {
    r = cos(2*PI*gl_TessCoord.s);
    x = r * sin(2*PI*gl_TessCoord.t);
    y = sin(2*PI*gl_TessCoord.s);
    z = r * cos(2*PI*gl_TessCoord.t);
  }

  vec4 radial = vec4(x, y, z, 1);

  gl_Position = Projection * ModelView * radial;
  
  color = vec4(0,0,0,1);
}
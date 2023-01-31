#version 150

in vec4 vPosition;

uniform float Time; // in milliseconds
uniform mat4 ModelView, Projection;

flat out vec4 f_colour;

void main()
{
  vec4  v = vPosition;
  
  if (Time > 0) {
    // height field
    vec4  u = sin( Time + 5*v );
    v.y += 0.1 * u.x * u.z;
    f_colour = vec4(0,0,0,1);
  } else {
    // cube: give different faces different colours
    if (gl_VertexID % 3 == 0) {
      f_colour = vec4(1,0,0,1);
    } else if (gl_VertexID % 3 == 1) {
      f_colour = vec4(0,1,0,1);
    } else {
      f_colour = vec4(0,0,1,1);
    }
  }

  gl_Position = Projection * ModelView * v;
}


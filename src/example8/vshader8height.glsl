#version 150

in vec4 vPosition;

uniform float Time; // in milliseconds
uniform mat4 ModelView, Projection;

flat out vec4 f_colour;

void main()
{
  vec4  v = vPosition;
  
  // height field
  vec4  u = sin( Time + 5*v );
  v.y += 0.1 * u.x * u.z;

  gl_Position = Projection * ModelView * v;
}


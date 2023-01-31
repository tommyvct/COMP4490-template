#version 150

in vec4 vPosition;
uniform mat4 ModelView, Projection;

void main()
{
  gl_Position = Projection * ModelView * vPosition;
  
  // gl_Position = ModelView * vPosition;
  // gl_Position.xy += (gl_Position.z+ 2)/5;
  // gl_Position.z += 2;
}


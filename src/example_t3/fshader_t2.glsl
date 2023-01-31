#version 400

flat in vec4 color;
uniform bool Outline;
out vec4 fColor;

void main() 
{ 
  if ( Outline )
    fColor = vec4(0,0,0,1);
  else
    fColor = color;
}

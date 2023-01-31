#version 150

in vec4 vPosition;
in vec4 vNormal;

out vec3 N, L, E;

uniform mat4 ModelView;
uniform mat4 ModelViewInverseTranspose;
uniform mat4 Projection;
uniform vec4 LightPosition;

void main()
{
   vec3 pos = (ModelView * vPosition).xyz;

   L = LightPosition.xyz - pos;
   E = -pos;

   N = ( ModelViewInverseTranspose*vNormal ).xyz;
   // N = ( ModelView * vec4(vNormal.xyz,0) ).xyz;

   gl_Position = Projection * ModelView * vPosition;
}

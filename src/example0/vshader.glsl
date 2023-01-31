#version 150

in vec4 vPosition;
out vec3 N, L, E;

uniform float Time;
uniform float Distance;

uniform bool UseLighting;
uniform vec4 LightPosition;
uniform mat4 ModelView, ModelViewInverseTranspose;
uniform mat4 Projection;

void main()
{
  /*** From the original height field shader: ***/

  vec4  v = vPosition;
  vec4  u = sin( Time + 5*v );
  
  v.y = 0.1 * u.x * u.z;

  /*** Calculate the vertex normal: ***/

  // these are the four adjacent vertices on the mesh
  vec3 adjs[4];
  for (int i = 0; i < 4; i++)
  adjs[i] = vPosition.xyz;
  adjs[0].x -= Distance;
  adjs[1].z += Distance;
  adjs[2].x += Distance;
  adjs[3].z -= Distance;
  for (int i = 0; i < 4; i++) {
    vec3 uadj = sin(Time + 5*adjs[i]);
    adjs[i].y = 0.1 * uadj.x * uadj.z;
  }

  // turn each into a tangent vector
  for (int i = 0; i < 4; i++) {
    adjs[i] -= v.xyz;
  }

  // we need normals, calculated as cross products of each pair
  vec3 norms[4];
  for (int i = 0; i < 4; i++) {
    norms[i] = cross(adjs[i].xyz, adjs[(i + 1)%4].xyz);
    norms[i] = normalize(norms[i]);
  }
    
  // surface normal is the average of the four
  vec3 vNormal = vec3(0, 0, 0);
  for (int i = 0; i < 4; i++) {
    vNormal += norms[i];
  }
  vNormal /= 4.0;
    
  if (UseLighting) {
    /*** Blinn-Phong shader: ***/

    vec3 pos = (ModelView * v).xyz;

    L = LightPosition.xyz - pos;
    E = -pos;

    N = ( ModelViewInverseTranspose*vec4(vNormal,0) ).xyz;
  }

  gl_Position = Projection * ModelView * v;
}

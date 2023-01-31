#version 150

in  vec4 color;
out vec4 fColor;

in vec3 N, L, E;

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform float Shininess;

void main() 
{ 
   vec3 H = normalize( L + E );
   vec4 ambient = AmbientProduct;

   float Kd = max( dot(L, N), 0.0 );
   vec4  diffuse = Kd * DiffuseProduct;

   float Ks = pow( max(dot(N, H), 0.0), Shininess );
   vec4  specular = Ks * SpecularProduct;
   
   if ( dot(L, N) < 0.0 ) {
      specular = vec4(0.0, 0.0, 0.0, 1.0);
   }

   fColor = ambient + diffuse + specular;
   fColor.a = 1.0;
}

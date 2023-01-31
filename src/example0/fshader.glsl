#version 150

in  vec4 color;
out vec4 fColor;

// lighting...
in vec3 N, L, E;
uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform float Shininess;

// ...or solid colour
uniform vec4 SolidColour;
uniform bool UseLighting;
// (maybe this should be two separate shaders but then we'd have to
//  modify the main program to allow that)

void main() {
  if (!UseLighting) {
    fColor = SolidColour;
  } else {
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
}

#version 400

layout (vertices = 3) out;

in vec4 color[];
out vec4 colorTES[];

void main() {
  if (gl_InvocationID == 0) {
    // get the distance of this vertex from the view plane; may not be accurate!
    // this relies on the coordinate already being projected
    // (or we could get the distance value to this shader some other way)
    float dist = (1+gl_in[0].gl_Position.z)/2;
    
    gl_TessLevelInner[0] = 11.5 - dist * 3;
    gl_TessLevelOuter[0] = 8.5 - dist * 2;
    gl_TessLevelOuter[1] = 8.5 - dist * 2;
    gl_TessLevelOuter[2] = 8.5 - dist * 2;
  }
  
  gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
  colorTES[gl_InvocationID] = color[gl_InvocationID];
}
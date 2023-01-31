#version 400

// tessellation control shader

// this is invoked once for each vertex in the input patch
// the vertex has already gone through the vertex shader
// each invocation has access to all of the patch's data, in arrays

// default input is in an array gl_in[GL_PATCH_VERTICES] (from C++ code)
// it is an array of structs; one field is gl_Position from the vertex shader

// note: in the C++ code only four new lines of code are *required*
//       these are marked with the notation "TESS" in the comments

// the number of vertices in the output patch; here it is the same as input
layout (vertices = 3) out;

// the color array is received from the output of the vertex shader
// (again, one array element per vertex in the patch)
// the output is sent to the tessellation evaluation shader
in vec4 color[];
out vec4 colorTES[];

void main() {
  if (gl_InvocationID == 0) {
    // This determines how the patch will be tessellated; see
    // https://www.khronos.org/opengl/wiki/Tessellation#Tessellating_primitives
    // done once for each patch
    gl_TessLevelInner[0] = 5.0;
    gl_TessLevelOuter[0] = 4.0;
    gl_TessLevelOuter[1] = 1.0;
    gl_TessLevelOuter[2] = 6.0;
  }
  
  // copy input to output
  gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
  colorTES[gl_InvocationID] = color[gl_InvocationID];
  
  // Trivial example of using gl_InvocationID: we're going to offset each
  // output position by 1, causing edge 0 to become edge 1, etc.
  // Compare to the output of the unmodified copy!
  //int off_id = (gl_InvocationID+1) % gl_PatchVerticesIn;
  //gl_out[off_id].gl_Position = gl_in[off_id].gl_Position;
  //colorTES[off_id] = color[off_id];
}
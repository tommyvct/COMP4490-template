// Based on SIGGRAPH 2013: An Introduction to OpenGL Programming
// Example #5: height fields (modified for lighting)

#include "common.h"

#include <iostream>
#include <chrono>
#include <cassert>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

const char *WINDOW_TITLE = "Height Field (lit)";
const double FRAME_RATE_MS = 1000.0/60.0;

typedef glm::vec4 color4;
typedef glm::vec4 point4;

const int N = 16;
const int NumVertices = N * N;
const int FillIndices = (2 + N * 2) * (N - 1);
const int EdgeIndices = N * N * 2;

point4 vertices[NumVertices];
GLuint indices[FillIndices+EdgeIndices];

// Array of rotation angles (in degrees) for each coordinate axis
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int Axis = Yaxis;
GLfloat Theta[NumAxes] = { 30.0, 0.0, 0.0 };
bool rotate = true;

// Model-view and projection matrices uniform location
GLuint ModelView, ModelViewInverseTranspose, Projection;
GLuint Time;
GLboolean UseLighting;

//----------------------------------------------------------------------------

// OpenGL initialization
void init() {
  // create the height field vertices (a 2D grid in the x-z plane)
  int Index = 0;
  float dx = 1.0/N, dz = 1.0/N;

  for (int i = 0; i < N; i++) {
    float z = i*dz;

    for (int j = 0; j < N; j++) {
      float x = j*dx;
      float y = 0;

      vertices[Index++] = point4(x, y, z, 1);
    }
  }
  assert(Index == NumVertices);

  Index = 0;
  // fill the vertices using triangle strips
  // e.g. first strip:
  //      0    1    2    3
  //      +----+----+----+-- ...
  //      +   /|   /|   /|
  //      +  / |  / |  / |
  //      + /  | /  | /  |
  //      +/   |/   |/   |
  //      +----+----+----+-- ...
  //      N   N+1  N+2  N+3
  int Base = 0;
  for (int strip = 0; strip < N-1; strip++) {
    for (int tri = 0; tri < N+1; tri++) {
      indices[Index++] = Base + tri;
      indices[Index++] = Base + tri + N;
    }
    Base += N;
  }
  
  // also add line strips for the edges of the quads
  // first one direction, then the other
  Base = 0;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      indices[Index++] = Base + j;
    }
    Base += N;
  }
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      indices[Index++] = i + N * j;
    }
  }
  
  assert(Index == FillIndices+EdgeIndices);

  // Create a vertex array object
  GLuint vao;
  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );

  // Create and initialize a buffer object
  GLuint buffer;
  glGenBuffers( 1, &buffer );
  glBindBuffer( GL_ARRAY_BUFFER, buffer );
  glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), NULL, GL_STATIC_DRAW );
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices );

  // Another for the index buffer
  glGenBuffers( 1, &buffer );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buffer );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW );

  // Load shaders and use the resulting shader program
  GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );
  glUseProgram( program );

  // set up vertex arrays
  GLuint vPosition = glGetAttribLocation( program, "vPosition" );
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

  // Initialize shader lighting parameters
  point4 light_position( 0.0, 0.0, -1.0, 0.0 );
  color4 light_ambient( 0.2, 0.2, 0.2, 1.0 );
  color4 light_diffuse( 1.0, 1.0, 1.0, 1.0 );
  color4 light_specular( 1.0, 1.0, 1.0, 1.0 );

  // see, for example: http://devernay.free.fr/cours/opengl/materials.html
  color4 material_ambient( 0.0215, 0.1745, 0.0215, 1.0 );
  color4 material_diffuse( 0.07568, 0.61424, 0.07568, 1.0 );
  color4 material_specular( 0.633, 0.727811, 0.633, 1.0 );
  float  material_shininess = 60.0;

  color4 ambient_product = light_ambient * material_ambient;
  color4 diffuse_product = light_diffuse * material_diffuse;
  color4 specular_product = light_specular * material_specular;

  glUniform4fv( glGetUniformLocation(program, "AmbientProduct"), 1, glm::value_ptr(ambient_product) );
  glUniform4fv( glGetUniformLocation(program, "DiffuseProduct"), 1, glm::value_ptr(diffuse_product) );
  glUniform4fv( glGetUniformLocation(program, "SpecularProduct"), 1, glm::value_ptr(specular_product) );
	
  glUniform4fv( glGetUniformLocation(program, "LightPosition"), 1, glm::value_ptr(light_position) );

  glUniform1f( glGetUniformLocation(program, "Shininess"), material_shininess );

  glUniform1f( glGetUniformLocation(program, "Distance"), 1.0f/N );

  // for no lighting
  color4 solid_colour( 0.0, 0.0, 0.0, 1.0 );
  glUniform4fv( glGetUniformLocation(program, "SolidColour"), 1, glm::value_ptr(solid_colour) );

  // Retrieve transformation uniform variable locations
  ModelView = glGetUniformLocation( program, "ModelView" );
  ModelViewInverseTranspose = glGetUniformLocation( program, "ModelViewInverseTranspose" );
  Projection = glGetUniformLocation( program, "Projection" );
  Time = glGetUniformLocation(program, "Time");
  UseLighting = glGetUniformLocation(program, "UseLighting");

  glEnable( GL_DEPTH_TEST );

  glShadeModel( GL_FLAT );

  glClearColor( 1.0, 1.0, 1.0, 1.0 ); 
}

//----------------------------------------------------------------------------

void display( void ) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //  Generate the model-view matrix

  const glm::vec3 viewer_pos( 0.0, 0.0, 2.0 );
  const glm::vec3 model_trans( -0.5, 0.0, -0.5 );
  glm::mat4 trans, rot, model_view;
  trans = glm::translate(trans, -viewer_pos);
  rot = glm::rotate(rot, glm::radians(Theta[Xaxis]), glm::vec3(1,0,0));
  rot = glm::rotate(rot, glm::radians(Theta[Yaxis]), glm::vec3(0,1,0));
  rot = glm::rotate(rot, glm::radians(Theta[Zaxis]), glm::vec3(0,0,1));
  model_view = trans * rot * glm::translate(glm::mat4(), model_trans);
    
  long long ms = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch()).count();

  glUniform1f( Time, (ms % 1000000) / 1000.0 );
  glUniformMatrix4fv( ModelView, 1, GL_FALSE, glm::value_ptr(model_view) );
  glUniformMatrix4fv( ModelViewInverseTranspose, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(model_view))) );

  glUniform1f( UseLighting, true );
  for (int bufferStart = 0; bufferStart < FillIndices ; bufferStart += 2*N+2) {
    glDrawElements( GL_TRIANGLE_STRIP, 2*N, GL_UNSIGNED_INT, (void *)(bufferStart * sizeof(GLuint)) );
  }

  glUniform1f( UseLighting, false );
  model_view = model_view * glm::translate(glm::mat4(), glm::vec3(0, 0.001, 0));
  glUniformMatrix4fv( ModelView, 1, GL_FALSE, glm::value_ptr(model_view) );
  // no lighting: don't need inverse transpose
  int bufferOffset = FillIndices;
  for (int bufferStart = 0; bufferStart < EdgeIndices ; bufferStart += N) {
    glDrawElements( GL_LINE_STRIP, N, GL_UNSIGNED_INT, (void *)((bufferStart + bufferOffset) * sizeof(GLuint)) );
  }

  glutSwapBuffers();
}

//----------------------------------------------------------------------------

void mouse( int button, int state, int x, int y ) {
  if (state == GLUT_DOWN) {
    switch (button) {
      case GLUT_LEFT_BUTTON:
        Axis = Xaxis;
        break;
      case GLUT_MIDDLE_BUTTON:
        Axis = Yaxis;
        break;
      case GLUT_RIGHT_BUTTON:
        Axis = Zaxis;
        break;
    }
  }
}

//----------------------------------------------------------------------------

void update( void ) {
  if (rotate) {
    Theta[Axis] += 0.5;

    if ( Theta[Axis] > 360.0 ) {
      Theta[Axis] -= 360.0;
    }
  }
}

//----------------------------------------------------------------------------

void keyboard( unsigned char key, int x, int y ) {
  switch( key ) {
    case 033: // Escape Key
    case 'q': case 'Q':
      exit( EXIT_SUCCESS );
      break;
    case ' ':
      rotate = !rotate;
      break;
  }
}

//----------------------------------------------------------------------------

void reshape( int width, int height ) {
  glViewport( 0, 0, width, height );

  GLfloat aspect = GLfloat(width)/height;
  glm::mat4  projection = glm::perspective( glm::radians(45.0f), aspect, 0.5f, 3.0f );

  glUniformMatrix4fv( Projection, 1, GL_FALSE, glm::value_ptr(projection) );
}

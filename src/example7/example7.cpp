// From SIGGRAPH 2013: An Introduction to OpenGL Programming
// This example combines the objects from example6 & example7 to
// demonstrate multiple VAOs

#include "common.h"

#include <iostream>
#include <chrono>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

const char *WINDOW_TITLE = "Height Field and Cube";
const double FRAME_RATE_MS = 1000.0/60.0;

typedef glm::vec4  color4;
typedef glm::vec4  point4;

const int N = 16;
const int hNumVertices = N * N * 4;

point4 hvertices[hNumVertices];

// Vertices of a unit cube centered at origin, sides aligned with axes
point4 vertices[8] = {
  point4( -0.5, -0.5,  0.5, 1.0 ),
  point4( -0.5,  0.5,  0.5, 1.0 ),
  point4(  0.5,  0.5,  0.5, 1.0 ),
  point4(  0.5, -0.5,  0.5, 1.0 ),
  point4( -0.5, -0.5, -0.5, 1.0 ),
  point4( -0.5,  0.5, -0.5, 1.0 ),
  point4(  0.5,  0.5, -0.5, 1.0 ),
  point4(  0.5, -0.5, -0.5, 1.0 )
};

GLuint indices[] = {
  // each group of 3 is a triangle face
  1, 0, 3, 1, 3, 2,
  2, 3, 7, 2, 7, 6,
  3, 0, 4, 3, 4, 7,
  6, 5, 1, 6, 1, 2,
  4, 5, 6, 4, 6, 7,
  5, 4, 0, 5, 0, 1
};

// Array of rotation angles (in degrees) for each coordinate axis
enum     { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int      Axis = Yaxis;
GLfloat  Theta[NumAxes] = { 30.0, 0.0, 0.0 };

// Model-view and projection matrices uniform location
GLuint  ModelView, Projection;
GLuint  Time;

//----------------------------------------------------------------------------

// need global access to VAOs
GLuint VAOs[2];

// OpenGL initialization
void
  init()
{
  int Index = 0;
  float dx = 1.0/N, dz = 1.0/N;
  for( int i = 0; i < N; ++i ) {
    float x = i*dx;

    for( int j = 0; j < N; ++j ) {
      float z = j*dz;

      // float y = f( x, z );
      float y = 0;

      hvertices[Index++] = point4(      x, y, z, 1 );
      hvertices[Index++] = point4(      x, y, z + dz, 1 );
      hvertices[Index++] = point4( x + dx, y, z + dz, 1 );
      hvertices[Index++] = point4( x + dx, y, z, 1 );
    }
  }

  // Load shaders and use the resulting shader program
  GLuint program = InitShader( "vshader7.glsl", "fshader7.glsl" );
  glUseProgram( program );

  GLuint vPosition = glGetAttribLocation( program, "vPosition" );

  // Create a vertex array object
  glGenVertexArrays( 2, VAOs );

  // object #1: height field
  glBindVertexArray( VAOs[0] );

  // Create and initialize a buffer object
  GLuint buffer;
  glGenBuffers( 1, &buffer );
  glBindBuffer( GL_ARRAY_BUFFER, buffer );
  glBufferData( GL_ARRAY_BUFFER, sizeof(hvertices), NULL, GL_STATIC_DRAW );
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(hvertices), hvertices );

  // set up vertex data for this vao
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

  // object #2: cube
  glBindVertexArray( VAOs[1] );

  // Create and initialize a buffer object
  glGenBuffers( 1, &buffer );
  glBindBuffer( GL_ARRAY_BUFFER, buffer );
  glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );
  // Another for the index buffer
  glGenBuffers( 1, &buffer );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buffer );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW );

  // set up vertex data for this vao
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

  // Retrieve transformation uniform variable locations
  ModelView = glGetUniformLocation( program, "ModelView" );
  Projection = glGetUniformLocation( program, "Projection" );
  Time = glGetUniformLocation(program, "Time");
    
  // this is used with "flat" in the shaders to get the same solid
  // colour for each face of the cube
  glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);

  glEnable( GL_DEPTH_TEST );

  glShadeModel(GL_FLAT);

  glClearColor( 1.0, 1.0, 1.0, 1.0 ); 
}

//----------------------------------------------------------------------------

void
  display( void )
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //  Generate the model-view matrix

  const glm::vec3 viewer_pos( 0.0, -0.3, 2.0 );
  const glm::vec3 model_trans( -0.5, -0.6, -0.5 );

  glm::mat4 trans, rot, view, model_view;
  rot = glm::rotate(rot, glm::radians(Theta[Xaxis]), glm::vec3(1,0,0));
  rot = glm::rotate(rot, glm::radians(Theta[Yaxis]), glm::vec3(0,1,0));
  rot = glm::rotate(rot, glm::radians(Theta[Zaxis]), glm::vec3(0,0,1));
  view = glm::translate(glm::mat4(), -viewer_pos) * rot;

  long long ms = std::chrono::duration_cast< std::chrono::milliseconds >(
    std::chrono::system_clock::now().time_since_epoch()).count();

  model_view = view * glm::translate(glm::mat4(), model_trans);
    
  glUniform1f( Time, (ms % 1000000) / 1000.0 );
  glUniformMatrix4fv( ModelView, 1, GL_FALSE, glm::value_ptr(model_view) );

  // draw the height field

  glBindVertexArray( VAOs[0] );

  for( int i = 0; i < hNumVertices ; i += 4 ) {
    glDrawArrays( GL_LINE_LOOP, i, 4 );
  }
    
  // now, draw the cube

  model_view = view * glm::scale(glm::mat4(), glm::vec3(0.5, 0.5, 0.5));
  glUniform1f( Time, -1 );
  glUniformMatrix4fv( ModelView, 1, GL_FALSE, glm::value_ptr(model_view) );

  glBindVertexArray( VAOs[1] );
    
  // for( int i = 0; i < sizeof(indices)/sizeof(GLuint) ; i += 3 ) {
  //   glDrawElements( GL_LINE_LOOP, 3, GL_UNSIGNED_INT, (void *)(i * sizeof(GLuint)) );
  // }
  glDrawElements( GL_TRIANGLES, sizeof(indices)/sizeof(GLuint), GL_UNSIGNED_INT, 0 );
        
  glutSwapBuffers();
}

//----------------------------------------------------------------------------

void
  mouse( int button, int state, int x, int y )
{
  if ( state == GLUT_DOWN ) {
    switch( button ) {
      case GLUT_LEFT_BUTTON:    Axis = Xaxis;  break;
      case GLUT_MIDDLE_BUTTON:  Axis = Yaxis;  break;
      case GLUT_RIGHT_BUTTON:   Axis = Zaxis;  break;
    }
  }
}

//----------------------------------------------------------------------------

void
  update( void )
{
  Theta[Axis] += 0.5;

  if ( Theta[Axis] > 360.0 ) {
    Theta[Axis] -= 360.0;
  }
}

//----------------------------------------------------------------------------

void
  keyboard( unsigned char key, int x, int y )
{
  switch( key ) {
    case 033: // Escape Key
    case 'q': case 'Q':
    exit( EXIT_SUCCESS );
    break;
  }
}

//----------------------------------------------------------------------------

void
  reshape( int width, int height )
{
  glViewport( 0, 0, width, height );

  GLfloat aspect = GLfloat(width)/height;
  glm::mat4  projection = glm::perspective( glm::radians(45.0f), aspect, 0.5f, 3.0f );

  glUniformMatrix4fv( Projection, 1, GL_FALSE, glm::value_ptr(projection) );
}

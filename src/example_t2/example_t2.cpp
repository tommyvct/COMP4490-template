// Based on: http://www.cs.unm.edu/~angel/BOOK/INTERACTIVE_COMPUTER_GRAPHICS/SIXTH_EDITION/CODE/CHAPTER05/WINDOWS_VERSIONS/example3.cpp
// Modified to isolate the main program and use GLM

//
//  More interesting tessellation shader example, LOD with one triangle.
//

#include "common_tessellation.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const char *WINDOW_TITLE = "Triangle LOD";
const double FRAME_RATE_MS = 1000.0/60.0;

typedef glm::vec4  color4;
typedef glm::vec4  point4;

const int NumVertices = 3; // One triangle

point4 points[NumVertices];
glm::vec3 normals[NumVertices];

point4 vertices[3] = {
    point4( 0, 0.5, 0, 1.0 ),
    point4( -0.5, -0.5, 0, 1.0 ),
    point4( 0.5, -0.5, 0, 1.0 )
};

// Array of rotation angles (in degrees) for each coordinate axis
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int      Axis = -1;
GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 };

// Model-view and projection matrices uniform location
GLuint  ModelView, Projection, Outline;

//----------------------------------------------------------------------------

// OpenGL initialization
void
init()
{
    points[0] = vertices[0];
    normals[0] = glm::vec3(0, 0, 1);
    points[1] = vertices[1];
    normals[1] = glm::vec3(0, 0, 1);
    points[2] = vertices[2];
    normals[2] = glm::vec3(0, 0, 1);

    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(normals),
      NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(points),
         sizeof(normals), normals );

    // Load shaders and use the resulting shader program
    // TESS: use a version of InitShader that loads tessellation shaders
    GLuint program = InitShader( "vshader_t2.glsl", "fshader_t2.glsl", "tcs_t2.glsl", "tes_t2.glsl" );
    glUseProgram( program );

    // set up vertex arrays
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
         BUFFER_OFFSET(0) );

    GLuint vNormal = glGetAttribLocation( program, "vNormal" ); 
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0,
         BUFFER_OFFSET(sizeof(points)) );

    // Initialize shader lighting parameters
    point4 light_position( -0.5, 0.5, -1.0, 0.0 );
    color4 light_ambient( 0.2, 0.2, 0.2, 1.0 );
    color4 light_diffuse( 1.0, 1.0, 1.0, 1.0 );
    color4 light_specular( 1.0, 1.0, 1.0, 1.0 );

    color4 material_ambient( 1.0, 0.0, 1.0, 1.0 );
    color4 material_diffuse( 1.0, 0.8, 0.0, 1.0 );
    color4 material_specular( 1.0, 0.8, 0.0, 1.0 );
    float  material_shininess = 100.0;

    color4 ambient_product = light_ambient * material_ambient;
    color4 diffuse_product = light_diffuse * material_diffuse;
    color4 specular_product = light_specular * material_specular;

    glUniform4fv( glGetUniformLocation(program, "AmbientProduct"),
      1, glm::value_ptr(ambient_product) );
    glUniform4fv( glGetUniformLocation(program, "DiffuseProduct"),
      1, glm::value_ptr(diffuse_product) );
    glUniform4fv( glGetUniformLocation(program, "SpecularProduct"),
      1, glm::value_ptr(specular_product) );
  
    glUniform4fv( glGetUniformLocation(program, "LightPosition"),
      1, glm::value_ptr(light_position) );

    glUniform1f( glGetUniformLocation(program, "Shininess"),
     material_shininess );
     
    // Retrieve transformation uniform variable locations
    ModelView = glGetUniformLocation( program, "ModelView" );
    Projection = glGetUniformLocation( program, "Projection" );
    Outline = glGetUniformLocation( program, "Outline" );

    // TESS: this number is required to determine how many vertices are
    //       grouped into a patch; here, one triangle == 3 vertices
    glPatchParameteri(GL_PATCH_VERTICES, 3);

    glEnable( GL_DEPTH_TEST );

    glShadeModel(GL_FLAT);

    glClearColor( 1.0, 1.0, 1.0, 1.0 ); 
}

//----------------------------------------------------------------------------

float PlusRange = 0.75;
float MinusRange = -5;
float Dist = 0.0;
float Inc = 0.01;

void
display( void )
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //  Generate the model-view matrixn

    const glm::vec3 viewer_pos( 0.0, 0.0, 2.0 );
    glm::mat4 trans, rot, model_view;
    trans = glm::translate(trans, -viewer_pos);
    rot = glm::rotate(rot, glm::radians(Theta[Xaxis]), glm::vec3(1,0,0));
    rot = glm::rotate(rot, glm::radians(Theta[Yaxis]), glm::vec3(0,1,0));
    rot = glm::rotate(rot, glm::radians(Theta[Zaxis]), glm::vec3(0,0,1));
    model_view = trans * glm::translate(glm::mat4(), glm::vec3(0, 0, Dist)) * rot;
    
    Dist += Inc;
    if (Dist > PlusRange) {
      Dist = PlusRange;
      Inc = -Inc;
    } else if (Dist < MinusRange) {
      Dist = MinusRange;
      Inc = -Inc;
    }
    
    glUniformMatrix4fv( ModelView, 1, GL_FALSE, glm::value_ptr(model_view) );

    glUniform1i( Outline, 0 );
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    // TESS: draw patches, in groups of GL_PATCH_VERTICES vertices
    glDrawArrays( GL_PATCHES, 0, NumVertices );

    glUniformMatrix4fv( ModelView, 1, GL_FALSE, glm::value_ptr(model_view * glm::translate(glm::mat4(), glm::vec3(0, 0, 0.01))) );
    glUniform1i( Outline, 1 );
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glLineWidth(1.5f);
    glDrawArrays( GL_PATCHES, 0, NumVertices );

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
    glm::mat4  projection = glm::perspective( glm::radians(45.0f), aspect, 0.5f, 10.0f );

    glUniformMatrix4fv( Projection, 1, GL_FALSE, glm::value_ptr(projection) );
}

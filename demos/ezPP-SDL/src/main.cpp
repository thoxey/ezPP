#include <GL/glew.h>

#include <SDL.h>
#include <cstdlib>
#include <iostream>
#include <GL/gl.h>

#include <ezpostprocessor.h>
#include <eznegative.h>

#define SCREENSIZE 800

//--------------------------------------------------------------------------------------------------------------------------------------------
/// @author Jon Macey & Tom Hoxey
//--------------------------------------------------------------------------------------------------------------------------------------------

const GLchar* vertexSource =
    "#version 150 core\n"
    "in vec2 position;"
    "in vec3 color;"
    "out vec3 Color;"
    "void main()"
    "{"
    "    Color = color;"
    "    gl_Position = vec4(position, -1.0, 1.0);"
    "}";
const GLchar* fragmentSource =
    "#version 150 core\n"
    "in vec3 Color;"
    "out vec4 outColor;"
    "void main()"
    "{"
    "    outColor = vec4(1.0, 0.5, 0.1, 1.0);"
    "}";



/// @brief function to quit SDL with error message
/// @param[in] _msg the error message to send
void SDLErrorExit(const std::string &_msg);

/// @brief initialize SDL OpenGL context
SDL_GLContext createOpenGLContext( SDL_Window *window);



int main()
{
  //Init SDL
  SDL_Init(SDL_INIT_VIDEO);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_Window* window = SDL_CreateWindow("OpenGL", 100, 100, SCREENSIZE, SCREENSIZE, SDL_WINDOW_OPENGL);
  SDL_GLContext context = SDL_GL_CreateContext(window);
  SDL_GL_SwapWindow(window);
  // flag to indicate if we need to exit
  bool quit=false;
  // sdl event processing data structure
  SDL_Event event;

  //Init Glew
  glewExperimental = GL_TRUE;
  glewInit();

///TEST TRIANGLE/////////////////////////////////////////////////////////////////////////////////////////////////
///Taken from "https://learnopengl.com/code_viewer.php?code=getting-started/hello-triangle-exercise1" Accesesed 21/02
  //---------------------------------------------------------------------------------------------------------------------------------------------------DECLARE VERTEX ARRAY
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  //---------------------------------------------------------------------------------------------------------------------------------------------------VERTSHADER
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, NULL);
  glCompileShader(vertexShader);
  //---------------------------------------------------------------------------------------------------------------------------------------------------INIT VERTEX BUFFER
  GLuint vbo;
  glGenBuffers(1, &vbo); // Generate 1 buffer
  //---------------------------------------------------------------------------------------------------------------------------------------------------DECLARE VERTICIES FOR TRIANGLE
  float vertices[] = {
    0.0f,  0.5f, // Vertex 1 (X, Y)
    0.5f, -0.5f, // Vertex 2 (X, Y)
    -0.5f, -0.5f  // Vertex 3 (X, Y)
  };
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  //---------------------------------------------------------------------------------------------------------------------------------------------------FRAGSHADER
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  glCompileShader(fragmentShader);
  //---------------------------------------------------------------------------------------------------------------------------------------------------SHADERPROGRAMCREATION
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glBindFragDataLocation(shaderProgram, 0, "outColor");//Not neccesary, but ensures frag shader only writes to one buffer
  glLinkProgram(shaderProgram);
  glUseProgram(shaderProgram);
  //---------------------------------------------------------------------------------------------------------------------------------------------------CONFIGURING VERTEX DATA
  GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
  glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(posAttrib);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


  //Init ezPP
  ezPostProcessor myezPPer;
  myezPPer.ezInit(SCREENSIZE, SCREENSIZE);

  ezNegative effect1;
  myezPPer.ezAddEffect(effect1);
  myezPPer.ezCompileEffects();

  while(!quit)
  {

    while ( SDL_PollEvent(&event) )
    {
      switch (event.type)
      {
        // this is the window x being clicked.
        case SDL_QUIT : quit = true; break;
        // process the mouse data by passing it to ngl class

        // if the window is re-sized pass it to the ngl class to change gl viewport
        // note this is slow as the context is re-create by SDL each time
        case SDL_WINDOWEVENT :
          int w,h;
          // get the new window size
          SDL_GetWindowSize(window,&w,&h);
        break;

        // now we look for a keydown event
        case SDL_KEYDOWN:
        {
          switch( event.key.keysym.sym )
          {
            // if it's the escape key quit
            case SDLK_ESCAPE :  quit = true; break;
            break;

            case SDLK_g : SDL_SetWindowFullscreen(window,SDL_FALSE); break;
            default : break;
          } // end of key process
        } // end of keydown

        default : break;
      } // end of event switch
    } // end of poll events



    //Bind ezFBO
    myezPPer.ezCapture();
    //draw testing triangle
    glBindVertexArray(vao);
    glUseProgram(shaderProgram);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    //Now we draw to the scren using our texture
    //Return to the default frame buffer and clear color buffer
    myezPPer.ezRender();
    glBindVertexArray(0);
    SDL_GL_SwapWindow(window);
  }
  //Tempory clean up until ezCleanUp is implemented
  //glDeleteFramebuffers(1, &framebuffer);
  // now tidy up and exit SDL
 SDL_Quit();
 // whilst this code will never execute under windows we need to have a return from
 // SDL_Main!
 return EXIT_SUCCESS;
}


SDL_GLContext createOpenGLContext(SDL_Window *window)
{
  // Request an opengl 3.2 context first we setup our attributes, if you need any
  // more just add them here before the call to create the context
  // SDL doesn't have the ability to choose which profile at this time of writing,
  // but it should default to the core profile
  // for some reason we need this for mac but linux crashes on the latest nvidia drivers
  // under centos
  #ifdef __APPLE__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  #else
    // Note you may have to change this depending upon the driver (Windows is fussy)
    // stick to 3.2 as the core base level for NGL works ok
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  #endif
  // set multi sampling else we get really bad graphics that alias
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,4);
  // Turn on double buffering with a 24bit Z buffer.
  // You may need to change this to 16 or 32 for your system
  // on mac up to 32 will work but under linux centos build only 16
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  // enable double buffering (should be on by default)
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  //
  return SDL_GL_CreateContext(window);

}

void SDLErrorExit(const std::string &_msg)
{
  std::cerr<<_msg<<"\n";
  std::cerr<<SDL_GetError()<<"\n";
  SDL_Quit();
  exit(EXIT_FAILURE);
}




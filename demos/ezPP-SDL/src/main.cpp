#include <SDL.h>
#include <cstdlib>
#include <iostream>
#ifndef __APPLE__
#include <GL/glew.h>
#include <GL/gl.h>
#else
#include <gl3.h>
#endif

#include <ezpostprocessor.h>
#include <ezgreyscale.h>
#include <eznegative.h>
#include <ezedgedetect.h>
#include <eznoeffects.h>
#include <ez3x3kernel.h>
#include <ezgaussianblur.h>
#include <ezemboss.h>
#include <ezsharpness.h>
#include <ezbrightness.h>
#include <ezcontrast.h>

#define SCREENSIZE 800

//--------------------------------------------------------------------------------------------------------------------------------------------
/// @author Jon Macey & Tom Hoxey
//--------------------------------------------------------------------------------------------------------------------------------------------

const GLchar* vertexShaderSource = "#version 330 core\n"
                                   "layout (location = 0) in vec3 position;\n"
                                   "layout (location = 1) in vec3 color;\n"
                                   "out vec3 ourColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "gl_Position = vec4(position, 1.0);\n"
                                   "ourColor = color;\n"
                                   "}\0";
const GLchar* fragmentShaderSource = "#version 330 core\n"
                                     "in vec3 ourColor;\n"
                                     "out vec4 color;\n"
                                     "void main()\n"
                                     "{\n"
                                     "color = vec4(ourColor, 1.0f);\n"
                                     "}\n\0";



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
  /*SDL_GLContext context = */SDL_GL_CreateContext(window);
  SDL_GL_SwapWindow(window);
  // flag to indicate if we need to exit
  bool quit=false;
  // sdl event processing data structure
  SDL_Event event;

  //Init Glew
#ifndef __APPLE__
  glewExperimental = GL_TRUE;
  glewInit();
#endif
  ///TEST TRIANGLE/////////////////////////////////////////////////////////////////////////////////////////////////
  ///Taken from "https://learnopengl.com/code_viewer.php?code=getting-started/hello-triangle-exercise1" Accesesed 21/02
  //---------------------------------------------------------------------------------------------------------------------------------------------------DECLARE VERTEX ARRAY
  // Vertex shader
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  // Check for compile time errors
  GLint success;
  GLchar infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success)
    {
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
  // Fragment shader
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  // Check for compile time errors
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
    {
      glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
  // Link shaders
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  // Check for linking errors
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  //---------------------------------------------------------------------------------------------------------------------------------------------------DECLARE VERTICIES FOR TRIANGLE
  GLfloat vertices[] = {
    // Positions         // Colors
    0.8f, -0.8f, 1.0f,  1.0f, 0.2f, 0.2f,  // Bottom Right
    -0.8f, -0.8f, 0.0f,  0.2f, 1.0f, 0.2f,  // Bottom Left
    0.0f,  0.8f, -1.0f,  0.2f, 0.2f, 1.0f   // Top
  };

  GLuint VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);
  // Color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0); // Unbind VAO
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


  //Init ezPP
  ezPostProcessor myezPPer;
  myezPPer.ezInit(SCREENSIZE, SCREENSIZE);

  //Some custom Kernels to show the 3x3Kernel class in use


  std::string sharpness = "-1.0, -1.0, -1.0,"
                          "-1.0,  9.0, -1.0,"
                          "-1.0, -1.0, -1.0";

  std::string emboss =    "-2.0, -1.0,  0.0,"
                          "-1.0,  1.0,  1.0,"
                          " 0.0,  1.0,  2.0";

  std::string average =   "-1.5, -1.0,  -0.5,"
                          "-1.0,  5.0,   0.0,"
                          "-0.5,  0.0,   0.5";


  ezNoEffects     effect0;
  ezGreyscale     effect1;
  ezNegative      effect2;
  ezEdgeDetect    effect3;
  ezGaussianBlur  effect4;
  ezEmboss        effect5;
  ezSharpness     effect6;
  ez3x3Kernel     effect7(sharpness, 0);
  ez3x3Kernel     effect8(emboss, 1);
  ez3x3Kernel     effect9(average, 2);
  ezBrightness    effect10(true, 0.1f);
  ezBrightness    effect11(false, 0.1f);
  //ezContrast      effect12(true, 100);
  //ezContrast      effect13(false, 100);

  myezPPer.ezAddEffect(effect0);
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
                  case SDLK_g :     myezPPer.ezAddEffect(effect1); myezPPer.ezCompileEffects(); break;
                  case SDLK_n :     myezPPer.ezAddEffect(effect2); myezPPer.ezCompileEffects(); break;
                  case SDLK_e :     myezPPer.ezAddEffect(effect3); myezPPer.ezCompileEffects(); break;
                  case SDLK_b :     myezPPer.ezAddEffect(effect4); myezPPer.ezCompileEffects(); break;
                  case SDLK_m :     myezPPer.ezAddEffect(effect5); myezPPer.ezCompileEffects(); break;
                  case SDLK_s :     myezPPer.ezAddEffect(effect6); myezPPer.ezCompileEffects(); break;
                  case SDLK_1 :     myezPPer.ezAddEffect(effect7); myezPPer.ezCompileEffects(); break;
                  case SDLK_2 :     myezPPer.ezAddEffect(effect8); myezPPer.ezCompileEffects(); break;
                  case SDLK_3 :     myezPPer.ezAddEffect(effect9); myezPPer.ezCompileEffects(); break;
                  case SDLK_UP :    myezPPer.ezAddEffect(effect10); myezPPer.ezCompileEffects(); break;
                  case SDLK_DOWN :  myezPPer.ezAddEffect(effect11); myezPPer.ezCompileEffects(); break;
                  //case SDLK_RIGHT : myezPPer.ezAddEffect(effect12); myezPPer.ezCompileEffects();break;
                  //case SDLK_LEFT :  myezPPer.ezAddEffect(effect13); myezPPer.ezCompileEffects();break;
                  case SDLK_c :     myezPPer.ezCleanUp(); myezPPer.ezAddEffect(effect0); myezPPer.ezCompileEffects();break;
                  case SDLK_RETURN : std::cerr<<myezPPer.returnEzFrag()<<"\n"; break;
                  default : break;
                  } // end of key process
              } // end of keydown

            default : break;
            } // end of event switch
        } // end of poll events

      //Bind ezFBO
      myezPPer.ezCapture();
      //draw testing triangle
      glBindVertexArray(VAO);
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




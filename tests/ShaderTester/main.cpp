#include <SDL.h>
#include <iostream>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <fstream>
#include <streambuf>
#include <string>
#ifndef __APPLE__
#include <GL/glew.h>
#include <GL/gl.h>
#else
#include <OpenGL/gl3.h>
#endif

bool debugShader(GLint _shader)
{
    GLint isCompiled = 0;
    glGetShaderiv(_shader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(_shader, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(_shader, maxLength, &maxLength, &errorLog[0]);
        std::cerr<<"Shader Error: ";
        for(auto i : errorLog)
            std::cerr<<i;
        return false;
    }
    else
        return true;
}

SDL_GLContext createOpenGLContext(SDL_Window *window)
{
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
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,4);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  return SDL_GL_CreateContext(window);
}



int main()
{

    //Init SDL
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_Window* window = SDL_CreateWindow("OpenGL", 100, 100, 0, 0, SDL_WINDOW_OPENGL);
    SDL_GL_CreateContext(window);
    createOpenGLContext(window);
    SDL_GL_SwapWindow(window);
    //Init Glew
  #ifndef __APPLE__
    glewExperimental = GL_TRUE;
    glewInit();
  #endif


    std::ifstream vertFile("vertSource.glsl");
    std::string vertString((std::istreambuf_iterator<char>(vertFile)), std::istreambuf_iterator<char>());

    if(vertString.length())
    {
        GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
        const GLchar * vertSource = (GLchar *)vertString.c_str();
        glShaderSource(vertShader, 1, &vertSource, NULL);
        glCompileShader(vertShader);
        if(debugShader(vertShader))
            std::cout<<"Vert Shader Compiled \n";
        else
            std::cerr<<"Vert ";
    }
    else
        std::cout<<"Vert Shader Blank, Ignored \n";

    std::ifstream fragFile("fragSource.glsl");
    std::string fragString((std::istreambuf_iterator<char>(fragFile)), std::istreambuf_iterator<char>());
    if(fragString.length())
    {
        GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
        const GLchar * fragSource = (GLchar *)fragString.c_str();
        glShaderSource(fragShader, 1, &fragSource, NULL);
        glCompileShader(fragShader);
        if(debugShader(fragShader))
            std::cout<<"Frag Shader Compiled \n";
        else
            std::cerr<<"Vert ";
    }
    else
        std::cout<<"Frag Shader Blank, Ignored \n";

    SDL_Quit();
}

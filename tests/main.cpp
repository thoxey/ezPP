#include<gtest/gtest.h>
#include <string>
#include <sstream>
#include <SDL.h>

#include<GL/glew.h>
#include<GL/gl.h>

#include <ezpostprocessor.h>
#include <eznoeffects.h>
#include "eztesteffect.h"


int main(int argc, char **argv)
{
  //Init SDL
  SDL_Init(SDL_INIT_VIDEO);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_Window* window = SDL_CreateWindow("OpenGL", 100, 100, 0, 0, SDL_WINDOW_OPENGL);
  /*SDL_GLContext context = */SDL_GL_CreateContext(window);
  SDL_GL_SwapWindow(window);
#ifndef __APPLE__
  glewExperimental = GL_TRUE;
  GLenum GlewError = glewInit();
  // error check
  if (GLEW_OK != GlewError)
    {
      std::cerr<<"Glew Error: "<<glewGetErrorString(GlewError)<<"\n";
    }
#endif
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
TEST(ezPostProcessor,DefaultCtor)
{
  ezPostProcessor test;
  EXPECT_TRUE(sizeof(test) != 0);
}
TEST(ezPostProcessor, DuplicateEffects)
{//This test checks that we cannot added multiple duplicate effects to our master vector
  ezPostProcessor test;
  ezNoEffects testEff;
  test.ezAddEffect(testEff);
  test.ezAddEffect(testEff);
  test.ezAddEffect(testEff);
  test.ezAddEffect(testEff);
  test.ezAddEffect(testEff);
  test.ezAddEffect(testEff);
  EXPECT_TRUE(test.getEffectsVector().size() == 1);
}
TEST(ezPostProcessor, SameEffectDifferentIDs)
{
  ezPostProcessor test;
  ezTestEffect testEff0(0);
  ezTestEffect testEff1(1);
  ezTestEffect testEff2(2);
  ezTestEffect testEff3(3);
  ezTestEffect testEff4(4);
  ezTestEffect testEff5(5);
  ezTestEffect testEff6(6);
  ezTestEffect testEff7(7);
  ezTestEffect testEff8(8);
  ezTestEffect testEff9(9);
  test.ezAddEffect(testEff0);
  test.ezAddEffect(testEff1);
  test.ezAddEffect(testEff2);
  test.ezAddEffect(testEff3);
  test.ezAddEffect(testEff4);
  test.ezAddEffect(testEff5);
  test.ezAddEffect(testEff6);
  test.ezAddEffect(testEff7);
  test.ezAddEffect(testEff8);
  test.ezAddEffect(testEff9);
  EXPECT_TRUE(test.getEffectsVector().size() == 10);
}
TEST(ezPostProcessor, ShadersAreValid)
{
  ezPostProcessor test;
  ezNoEffects     effect0;
  test.ezAddEffect(effect0);
  test.ezCompileEffects();

  GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
  const GLchar * fragSource = (GLchar *)(test.returnEzFrag()).c_str();
  glShaderSource(fragShader, 1, &fragSource, NULL);
  glCompileShader(fragShader);

  EXPECT_TRUE(test.debugShader(fragShader) == true);
}

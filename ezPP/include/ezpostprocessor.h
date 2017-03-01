#ifndef EZPOSTPROCESSOR_H
#define EZPOSTPROCESSOR_H
#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>
#include "ezeffect.h"

class ezPostProcessor
{
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//Making into a Singleton, kept seperate for ease of reading
private : static ezPostProcessor *instance;
public : static ezPostProcessor *getInstance()
  {
    if(!instance)
      instance = new ezPostProcessor;
    return instance;
  }
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


public:
  //Constructer & Destructor
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief ctor
  //----------------------------------------------------------------------------------------------------------------------
  ezPostProcessor();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief dtor
  //----------------------------------------------------------------------------------------------------------------------
  ~ezPostProcessor();

  //ez Functions
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Initializes the ezPPer with data from the users GL context
  //----------------------------------------------------------------------------------------------------------------------
  void ezInit(int _screenWidth, int screenHeight);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief adds an effect to the master vector to be compiled, will become template function to accept presets in future
  //----------------------------------------------------------------------------------------------------------------------
  void ezAddEffect(ezEffect _addedEffect);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief allows you to make a prest that means you can add a group of effects at once
  //----------------------------------------------------------------------------------------------------------------------
  void ezMakePreset(std::vector<ezEffect> _preset);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief performs the calculations for the effcts and then applies them
  //----------------------------------------------------------------------------------------------------------------------
  void ezCompileEffects();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief captres scene to framebuffer and creates texture
  //----------------------------------------------------------------------------------------------------------------------
  void ezCapture();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief applies 'compiled' texture to our camera covering quad
  //----------------------------------------------------------------------------------------------------------------------
  void ezRender();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief alternative dtor, clears buffers with killing PPer if needed
  //----------------------------------------------------------------------------------------------------------------------
  void ezCleanUp();
  //Taken from https://learnopengl.com/#!Advanced-OpenGL/Framebuffers Accesed 17/02
  const GLchar * ezVert =
    "#version 330 core\n"
    "layout (location = 0) in vec2 position;"
    "layout (location = 1) in vec2 texCoords;"
    "out vec2 TexCoords;"
    "void main()"
    "{"
        "gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);"
        "TexCoords = texCoords;"
    "}";

  //Taken from https://learnopengl.com/#!Advanced-OpenGL/Framebuffers Accesed 17/02
  const GLchar * ezFrag =
    "#version 330 core\n"
    "in vec2 TexCoords;"
    "out vec4 color;"
    "uniform sampler2D screenTexture;"
    "void main()"
    "{"
        "color = texture(screenTexture, TexCoords);"
    "}";


private:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief vector of effects on the current scene
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<ezEffect> m_effectMasterVector;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief stores the active buffers
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<GLuint> m_activeBuffers;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief The width and height of the screen
  //----------------------------------------------------------------------------------------------------------------------
  int m_screenWidth, m_screenHeight;

  GLfloat quadVertices[24] =
  {// Positions   // TexCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f};
  //Split these and write individual briefs!
  GLuint quadVAO, quadVBO, vertShader, fragShader, ezShaderProgram, textureColorbuffer, framebuffer;

};

#endif // EZPOSTPROCESSOR_H

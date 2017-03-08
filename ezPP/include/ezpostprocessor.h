#ifndef EZPOSTPROCESSOR_H
#define EZPOSTPROCESSOR_H
#define DEBUGEZPPER
#include <vector>
#include <iostream>
#ifndef __APPLE__
#include <GL/glew.h>
#include <GL/gl.h>
#else
#include <gl3.h>
#endif
#include "ezeffect.h"

class ezPostProcessor
{
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//Making into a Singleton class, kept seperate for ease of reading
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
  /// @brief Returns the effects vector
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<ezEffect> getEffectsVector();
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

  std::string returnEzFrag();

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

  //Split these and write individual briefs!
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief The VAO and the VBO for the screen space quad
  //----------------------------------------------------------------------------------------------------------------------
  GLuint quadVAO, quadVBO;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief These are used to initialise the shaders
  //----------------------------------------------------------------------------------------------------------------------
  GLuint vertShader, fragShader, ezShaderProgram;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief The buffers for holding the different textures
  //----------------------------------------------------------------------------------------------------------------------
  GLuint textureColorbuffer;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief The frame buffer object that we will manipulate
  //----------------------------------------------------------------------------------------------------------------------
  GLuint framebuffer;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief The compiled shaders that get applied to the screenspace quad
  //----------------------------------------------------------------------------------------------------------------------
  std::string m_compiledFragShader, m_compiledVertShader = "";

  //Adapted from https://learnopengl.com/#!Advanced-OpenGL/Framebuffers Accesed 17/02
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief The source for the vertex shader it just passes on the texture coordinates
  //----------------------------------------------------------------------------------------------------------------------
  std::string m_VertSource =
    R"m_VertSource(
    #version 330 core
    layout (location = 0) in vec2 position;
    layout (location = 1) in vec2 texCoords;
    out vec2 TexCoords;
    void main()
    {
        gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);
        TexCoords = texCoords;
    }
    )m_VertSource";
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief The start of the fragment shader, it declares common variables and assigns the screen texture to a Vec4
  //----------------------------------------------------------------------------------------------------------------------
  std::string m_FragSource =
    R"m_FragSource(
    #version 330 core
    in vec2 TexCoords;
    out vec4 color;
    uniform sampler2D screenTexture;
    float offset;
    float kernel[9];
    vec2 offsets[9];
    vec3 sampleTex[9];
    vec3 col;
    void main()
    {
        vec4 outColour = texture(screenTexture, TexCoords);
    )m_FragSource";
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief This is the end of the frag shader, it just pushes the texture as the colour and ends the shader
  //----------------------------------------------------------------------------------------------------------------------
  std::string m_FragSourceEnd =
        "color = outColour;\n}";
  //End Citation
};

#endif // EZPOSTPROCESSOR_H

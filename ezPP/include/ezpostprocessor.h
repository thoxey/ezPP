#ifndef EZPOSTPROCESSOR_H
#define EZPOSTPROCESSOR_H
#define DEBUGEZPPER
#include <vector>
#include <iostream>
#include <algorithm>
#ifndef __APPLE__
#include <GL/glew.h>
#include <GL/gl.h>
#else
#include <gl3.h>
#endif
#include "ezeffect.h"

//----------------------------------------------------------------------------------------------------------------------
/// @file ezPostProcessor.h
/// @brief The 'master class' that handles the storage, processing, and exectuction of the effects
/// @author Tom Hoxey
/// @version 1.0
/// @date 20/3/17 Initial version
//----------------------------------------------------------------------------------------------------------------------

class ezPostProcessor
{
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
  /// @brief Resizes our FBO texture
  //----------------------------------------------------------------------------------------------------------------------
  void ezResize(int _screenWidth, int screenHeight);
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
  void ezRender(GLuint frameBuffer = 0);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief alternative dtor, clears buffers with killing PPer if needed
  //----------------------------------------------------------------------------------------------------------------------
  void ezCleanUp();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief returns our complete frag shader
  //----------------------------------------------------------------------------------------------------------------------
  std::string returnEzFrag();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief takes our texture and refreshes it with our current changes
  //----------------------------------------------------------------------------------------------------------------------
  void ezSubRender(std::string _compilingFragShader);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief prints any shader compilation errors to cerr
  //----------------------------------------------------------------------------------------------------------------------
  bool debugShader(GLint _shader);
private:

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief vector of effects on the current scene
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<ezEffect> m_effectMasterVector;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief stores the active shaders
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<GLuint> m_activeShaders;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief stores the ids of all the effects
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<int> m_ids;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief takes our texture and refreshes it with our current changes
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<std::string> m_effectSource;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief takes our texture and refreshes it with our current changes
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<std::string> m_shaders;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Used to check the sequence init compile capture has been called in the right order
  //----------------------------------------------------------------------------------------------------------------------
  bool m_inited, m_compiled, m_captured;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief The width and height of the screen
  //----------------------------------------------------------------------------------------------------------------------
  int m_screenWidth, m_screenHeight;
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
  GLuint m_textureColorbuffer1, m_textureColorbuffer2, m_activeTexture;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief The frame buffer object that we will manipulate
  //----------------------------------------------------------------------------------------------------------------------
  GLuint m_framebuffer1, m_framebuffer2, m_activeFramebuffer;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief The frame buffer object that we will manipulate
  //----------------------------------------------------------------------------------------------------------------------
  GLuint m_DepthStencil1, m_DepthStencil2, m_activeDepthStencil;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief The shader source for the final render
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

      //Variables for different effects
      float offset;
      float brightnessIncrement = 0.0f;
      float kernel[9];
      float factor = 0;
      float average;
      vec2 offsets[9];
      vec3 sampleTex[9];
      vec3 col;

      float clamp(float toclamp)
      {
      if(toclamp > 255.0f)
      toclamp = 255.0f;
      else if (toclamp < 0.0f)
      toclamp = 0.0f;
      return toclamp / 255.0f;
      }

      void main()
      {
      offset = 0.003;
      vec4 outColour = texture(screenTexture, TexCoords);
      offsets = vec2[](
      vec2(-offset, offset),  // top-left
      vec2(0.0f,    offset),  // top-center
      vec2(offset,  offset),  // top-right
      vec2(-offset, 0.0f),    // center-left
      vec2(0.0f,    0.0f),    // center-center
      vec2(offset,  0.0f),    // center-right
      vec2(-offset, -offset), // bottom-left
      vec2(0.0f,    -offset), // bottom-center
      vec2(offset,  -offset)  // bottom-right
      );
      )m_FragSource";
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief This is the end of the frag shader, it just pushes the texture as the colour and ends the shader
  //----------------------------------------------------------------------------------------------------------------------
  std::string m_FragSourceEnd ="color = outColour;\n}";
  //End Citation
};

#endif // EZPOSTPROCESSOR_H

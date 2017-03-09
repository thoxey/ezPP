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
<<<<<<< HEAD
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //Making into a Singleton, kept seperate for ease of reading
=======
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//Making into a Singleton class, kept seperate for ease of reading
>>>>>>> origin/master
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

<<<<<<< HEAD
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

#ifdef DEBUGEZPPER
    void returnEzFrag();
#endif
=======
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
>>>>>>> origin/master

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

<<<<<<< HEAD
    const GLfloat quadVertices[24] =
    {// Positions   // TexCoords
     -1.0f,  1.0f,  0.0f, 1.0f,
     -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,

     -1.0f,  1.0f,  0.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f};
    //Split these and write individual briefs!
    GLuint quadVAO, quadVBO, vertShader, fragShader, ezShaderProgram, textureColorbuffer, framebuffer;

    std::string m_compiledFragShader, m_compiledVertShader = "";

    //Adapted from https://learnopengl.com/#!Advanced-OpenGL/Framebuffers Accesed 17/02
    std::string m_VertSource =
            "#version 330 core\n"
            "layout (location = 0) in vec2 position;\n"
            "layout (location = 1) in vec2 texCoords;\n"
            "out vec2 TexCoords;\n"
            "void main()\n"
            "{\n"
            "gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);\n"
            "TexCoords = texCoords;\n"
            "}\n";

    std::string m_FragSource =
            "#version 330 core\n"
            "in vec2 TexCoords;\n"
            "out vec4 color;\n"
            "uniform sampler2D screenTexture;\n"
            "float offset;\n"
            "float kernel[9];\n"
            "vec2 offsets[9];\n"
            "vec3 sampleTex[9];\n"
            "vec3 col;\n"
            "void main()\n"
            "{\n"
            "vec4 outColour = texture(screenTexture, TexCoords);\n"
            "      offset = 0.00333;\n"
            "      offsets = vec2[](\n"
            "              vec2(-offset, offset),  // top-left      \n"
            "              vec2(0.0f,    offset),  // top-center    \n"
            "              vec2(offset,  offset),  // top-right     \n"
            "              vec2(-offset, 0.0f),    // center-left   \n"
            "              vec2(0.0f,    0.0f),    // center-center \n"
            "              vec2(offset,  0.0f),    // center-right  \n"
            "              vec2(-offset, -offset), // bottom-left   \n"
            "              vec2(0.0f,    -offset), // bottom-center \n"
            "              vec2(offset,  -offset)  // bottom-right  \n"
            "          );\n";
    std::string m_FragSourceEnd =
            "color = outColour;\n"
            "}\n";
    //End Citation
=======
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
>>>>>>> origin/master
};

#endif // EZPOSTPROCESSOR_H

#include "ez3x3kernel.h"
//----------------------------------------------------------------------------------------------------------------------
/// @file ez3x3Kernel.cpp
/// @brief The implementation of the ez3x3Kernel class
//----------------------------------------------------------------------------------------------------------------------
ez3x3Kernel::ez3x3Kernel(std::string kernel)
{
    //Adapted from https://learnopengl.com/#!Advanced-OpenGL/Framebuffers Accesed 17/02
    m_FragSource = R"m_FragSource(
                   #version 410 core
                   in vec2 TexCoords;
                   out vec4 color;
                   uniform sampler2D screenTexture;

                   //Variables for different effects
                   float offset;
                   vec2 offsets[9];
                   vec3 sampleTex[9];
                   vec3 col;
                   )m_FragSource";
    m_FragSource += "//ez3x3Kernel \n";
    m_FragSource +="kernel = float[](\n";
    m_FragSource += kernel;
    m_FragSource.append(R"calcPixelVals(
                           );
                           for(int i = 0; i < 9; i++)
                           {
                           sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
                           }\n
                           col = vec3(0.0);\n
                           for(int i = 0; i < 9; i++)\n
                           col += sampleTex[i] * kernel[i];
                           )calcPixelVals");
    m_FragSource += "outColour = vec4(col, 1.0);\n";




    //End Citation

    ezCompileEffect();
}
//----------------------------------------------------------------------------------------------------------------------

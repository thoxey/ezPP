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
                   float kernel[9];
                   vec2 offsets[9];
                   vec3 sampleTex[9];
                   vec3 col;
                   void main()
                   {
                   float offset = 0.003;
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
    m_FragSource += "//ez3x3Kernel \n";
    m_FragSource +="kernel = float[](\n";
    m_FragSource += kernel;
    m_FragSource.append(R"calcPixelVals(
                           );
                           for(int i = 0; i < 9; i++)
                           {
                           sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
                           }
                           col = vec3(0.0);
                           for(int i = 0; i < 9; i++)
                           col += sampleTex[i] * kernel[i];
                           )calcPixelVals");
    m_FragSource += "outColour = vec4(col, 1.0);\n}";




    //End Citation

    ezCompileEffect();
}
//----------------------------------------------------------------------------------------------------------------------

#include "eznegative.h"
//----------------------------------------------------------------------------------------------------------------------
/// @file ezNegative.cpp
/// @brief The implementation of the ezNegative class
//----------------------------------------------------------------------------------------------------------------------
ezNegative::ezNegative()
{
    //Adapted from https://learnopengl.com/#!Advanced-OpenGL/Framebuffers Accesed 17/02
    m_FragSource =
            R"changePixelVals(
            #version 330 core
            in vec2 TexCoords;
            out vec4 color;
            uniform sampler2D screenTexture;

            void main()
            {
            vec4 outColour = texture(screenTexture, TexCoords);
            color = vec4(1.0-outColour.r,1.0-outColour.g,1.0-outColour.b, 1.0);;
            }
            )changePixelVals";
    ezCompileEffect();

}
//----------------------------------------------------------------------------------------------------------------------

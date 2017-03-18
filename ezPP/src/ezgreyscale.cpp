#include "ezgreyscale.h"
//----------------------------------------------------------------------------------------------------------------------
/// @file ezGreyscale.cpp
/// @brief The implementation of the ezGreyscale class
//----------------------------------------------------------------------------------------------------------------------
ezGreyscale::ezGreyscale()
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
            //Weights that make it look a bit nicer (We see more green so weight it heavier)
            float average = ((outColour.r * 0.25) + (outColour.g * 0.65) + (outColour.b * 0.1) );
            color = vec4(average, average, average, 1.0);
            }
            )changePixelVals";

    ezCompileEffect();
}
//----------------------------------------------------------------------------------------------------------------------

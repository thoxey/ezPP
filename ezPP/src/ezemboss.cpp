#include "ezemboss.h"
//----------------------------------------------------------------------------------------------------------------------
/// @file ezEmboss.cpp
/// @brief The implementation of the ezEmboss class
//----------------------------------------------------------------------------------------------------------------------
ezEmboss::ezEmboss()
{
    //Adapted from https://learnopengl.com/#!Advanced-OpenGL/Framebuffers Accesed 17/02
    m_FragSource =
            R"changePixelVals(
            #version 330 core
            in vec2 TexCoords;
            out vec4 color;
            uniform sampler2D screenTexture;

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
            kernel = float[](
            -2.0, -1.0,  0.0,
            -1.0,  1.0,  1.0,
            0.0,  1.0,  2.0
            );
            col = vec3(0.0);
            for(int i = 0; i < 9; i++)\
            {\
            sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
            col += sampleTex[i] * kernel[i];
            }
            outColour = vec4(col, 1.0);
            color = outColour;
            }
            )changePixelVals";

    ezCompileEffect();
}
//----------------------------------------------------------------------------------------------------------------------

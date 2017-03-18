#include "ezcontrast.h"
//----------------------------------------------------------------------------------------------------------------------
/// @file ezContrast.cpp
/// @brief The implementation of the ezContrast class
//----------------------------------------------------------------------------------------------------------------------
ezContrast::ezContrast(int _increment)
{
    std::stringstream stream;
    stream << std::fixed << _increment;
    std::string incrStr = stream.str();

    //Functions derived from - http://www.dfstudios.co.uk/articles/programming/image-programming-algorithms/image-processing-algorithms-part-5-contrast-adjustment/
    m_FragSource  = R"m_FragSource(
                    #version 410 core
                    in vec2 TexCoords;
                    out vec4 color;
                    uniform sampler2D screenTexture;
                    float factor =)m_FragSource";
    m_FragSource += incrStr;
    m_FragSource +=R"m_FragSource(.0f;
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
                   vec4 outColour = texture(screenTexture, TexCoords);
                   factor = (259.0f*(factor+255.0f))/(255.0f*(259.0f-factor));
                   outColour = vec4(
                   outColour.r = clamp(factor * ((outColour.r*255)-128.0f) + 128.0f),
                   outColour.g = clamp(factor * ((outColour.g*255)-128.0f) + 128.0f),
                   outColour.b = clamp(factor * ((outColour.b*255)-128.0f) + 128.0f),
                   1.0);
                   color = outColour;
                   }
                   )m_FragSource";
    ezCompileEffect();
}
//----------------------------------------------------------------------------------------------------------------------

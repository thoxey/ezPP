#include "ezbrightness.h"
//----------------------------------------------------------------------------------------------------------------------
/// @file ezBrightness.cpp
/// @brief The implementation of the ezBrightness class
//----------------------------------------------------------------------------------------------------------------------
ezBrightness::ezBrightness(float _increment)
{

    std::stringstream stream;
    stream << std::fixed << _increment;
    std::string incrStr = stream.str();

    m_FragSource  = R"m_FragSource(
                    #version 410 core
                    in vec2 TexCoords;
                    out vec4 color;
                    uniform sampler2D screenTexture;
                    float brightnessIncrement =)m_FragSource";
    m_FragSource += incrStr;
    m_FragSource +=R"m_FragSource(f;
                   void main()
                   {
                   vec4 outColour = texture(screenTexture, TexCoords);
                   outColour =vec4(outColour.r+brightnessIncrement,outColour.g+brightnessIncrement,outColour.b+brightnessIncrement,1.0f);
                   color = outColour;
                   }
                   )m_FragSource";

    ezCompileEffect();

}
//----------------------------------------------------------------------------------------------------------------------

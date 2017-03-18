#include "ezEditChannel.h"
//----------------------------------------------------------------------------------------------------------------------
/// @file ezEditChannel.cpp
/// @brief The implementation of the ezEditChannel class
//----------------------------------------------------------------------------------------------------------------------
ezEditChannel::ezEditChannel(float _increment, char _channel)
{

    std::stringstream stream;
    stream << std::fixed << _increment;
    std::string incrStr = stream.str();

    m_FragSource  = R"m_FragSource(
                    #version 410 core
                    in vec2 TexCoords;
                    out vec4 color;
                    uniform sampler2D screenTexture;
                    float increment =)m_FragSource";
    m_FragSource += incrStr;
    m_FragSource +=R"m_FragSource(f;

                   float clamp(float _clamped)
                   {
                   if(_clamped > 1.0f)
                   return 1.0f;
                   else if(_clamped < 0.0f)
                   return 0.0f;
                   else
                   return _clamped;
                   }

                   void main()
                   {
                   vec4 outColour = texture(screenTexture, TexCoords);
                   )m_FragSource";
    if(_channel == 'r')
        m_FragSource += "outColour =vec4(clamp(outColour.r+increment),outColour.g,outColour.b,1.0f);";
    if(_channel == 'g')
        m_FragSource += "outColour =vec4(outColour.r,clamp(outColour.g+increment),outColour.b,1.0f);";
    if(_channel == 'b')
        m_FragSource += "outColour =vec4(outColour.r,outColour.g,clamp(outColour.b+increment),1.0f);";
    m_FragSource +=R"m_FragSource(
                   color = outColour;
                   }
                   )m_FragSource";

    ezCompileEffect();

}
//----------------------------------------------------------------------------------------------------------------------

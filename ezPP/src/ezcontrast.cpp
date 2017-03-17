#include "ezcontrast.h"
//----------------------------------------------------------------------------------------------------------------------
/// @file ezContrast.cpp
/// @brief The implementation of the ezContrast class
//----------------------------------------------------------------------------------------------------------------------
ezContrast::ezContrast(bool _up, int _increment)
{
    ezID = 10;
    m_isComplex = false;
    std::stringstream stream;
    stream << std::fixed << _increment;
    std::string incrStr = stream.str();
    m_calcPixelVals = "//ezContrast \n";
    //Functions derived from - http://www.dfstudios.co.uk/articles/programming/image-programming-algorithms/image-processing-algorithms-part-5-contrast-adjustment/
    if(_up)
        m_calcPixelVals += "factor += ";
    else
        m_calcPixelVals += "factor -= ";

    m_calcPixelVals.append(incrStr);
    m_calcPixelVals += ".0f;\n factor = (259.0f*(factor+255.0f))/(255.0f*(259.0f-factor));";

    m_changePixelVals+=R"changePixelVals(
                       outColour = vec4(
                       outColour.r = clamp(factor * ((outColour.r*255)-128.0f) + 128.0f),
                       outColour.g = clamp(factor * ((outColour.g*255)-128.0f) + 128.0f),
                       outColour.b = clamp(factor * ((outColour.b*255)-128.0f) + 128.0f),
                       1.0);
                       )changePixelVals";
}
//----------------------------------------------------------------------------------------------------------------------

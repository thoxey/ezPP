#include "ezcontrast.h"

ezContrast::ezContrast(bool _up, int _increment)
{
    id = 10;
    m_isComplex = false;
    m_isMultiple = true;
    std::stringstream stream;
    stream << std::fixed << _increment;
    std::string incrStr = stream.str();
    //Functions derived from - http://www.dfstudios.co.uk/articles/programming/image-programming-algorithms/image-processing-algorithms-part-5-contrast-adjustment/
    m_changePixelVals = "factor = ";
    m_changePixelVals.append(incrStr);
    m_changePixelVals.append(R"changePixelVals(.0f;
                             factor = (259.0f*(factor+255.0f))/(255.0f*(259.0f-factor));
                             outColour = vec4(
                             outColour.r = clamp(factor * (outColour.r-128.0f) + 128.0f),
                             outColour.g = clamp(factor * (outColour.g-128.0f) + 128.0f),
                             outColour.b = clamp(factor * (outColour.b-128.0f) + 128.0f),
                             1.0);
                             )changePixelVals");
}

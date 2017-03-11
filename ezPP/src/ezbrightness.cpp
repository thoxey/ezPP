#include "ezbrightness.h"

ezBrightness::ezBrightness(bool _up, float _increment)
{
    if(_up)
        ezID = 8;
    else
        ezID = 9;
    m_isComplex = false;
    m_isMultiple = true;
    float increment = 0.0f+_increment;

    std::stringstream stream;
    stream << std::fixed << increment;
    std::string incrStr = stream.str();

    if(_up)
    {
        m_calcPixelVals = "brightnessIncrement += ";
        m_calcPixelVals.append(incrStr);
        m_calcPixelVals.append("f;\n");
    }
    else
    {
        m_calcPixelVals = "brightnessIncrement -= ";
        m_calcPixelVals.append(incrStr);
        m_calcPixelVals.append("f;\n");
    }
    m_changePixelVals = "outColour =vec4(outColour.r+brightnessIncrement,outColour.g+brightnessIncrement,outColour.b+brightnessIncrement,1.0f);\n";

}

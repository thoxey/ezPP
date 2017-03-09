#include "ezbrightness.h"

ezBrightness::ezBrightness(bool _up, float _increment)
{
  if(_up)
    id = 8;
  else
    id = 9;
  m_isComplex = false;
  m_isMultiple = true;
  float increment;
  if(_up)
    increment = 0.0f+_increment;
  else
    increment = 0.0f-_increment;

  std::stringstream stream;
  stream << std::fixed << increment;
  std::string incrStr = stream.str();




  if(_up)
    {
      m_changePixelVals = "brightnessIncrement += ";
      m_changePixelVals.append(incrStr);
      m_changePixelVals.append(";\n outColour =vec4(outColour.r+brightnessIncrement,outColour.g+brightnessIncrement,outColour.b+brightnessIncrement,1.0f);\n");
    }
  else
    {
      m_changePixelVals = "brightnessIncrement -= ";
      m_changePixelVals.append(incrStr);
      m_changePixelVals.append(";\n outColour =vec4(outColour.r-brightnessIncrement,outColour.g-brightnessIncrement,outColour.b-brightnessIncrement,1.0f);\n");
    }
}

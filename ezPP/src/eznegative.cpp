#include "eznegative.h"

ezNegative::ezNegative()
{
  id = 2;
  m_isComplex = false;
  m_changePixelVals = "outColour = vec4(1.0-outColour.r,1.0-outColour.g,1.0-outColour.b, 1.0);\n";
}

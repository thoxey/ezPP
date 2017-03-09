#include "ezgreyscale.h"

ezGreyscale::ezGreyscale()
{
  id = 1;
  m_isComplex = false;
  m_isMultiple = false;
  m_changePixelVals = "float average = (outColour.r * 0.3+outColour.g * 0.6+outColour.b * 0.1)/3.0;\n"
                      "outColour = vec4(average, average, average, 1.0);\n";
}


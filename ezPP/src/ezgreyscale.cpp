#include "ezgreyscale.h"
//----------------------------------------------------------------------------------------------------------------------
/// @file ezGreyscale.cpp
/// @brief The implementation of the ezGreyscale class
//----------------------------------------------------------------------------------------------------------------------
ezGreyscale::ezGreyscale()
{
  ezID = 1;
  m_isComplex = false;
  m_changePixelVals = "//ezGreyscale \n"
                      "average = ((outColour.r * 0.25) + (outColour.g * 0.65) + (outColour.b * 0.1) );\n"
                      "outColour = vec4(average, average, average, 1.0);\n";
}
//----------------------------------------------------------------------------------------------------------------------

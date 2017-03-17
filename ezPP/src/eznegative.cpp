#include "eznegative.h"
//----------------------------------------------------------------------------------------------------------------------
/// @file ezNegative.cpp
/// @brief The implementation of the ezNegative class
//----------------------------------------------------------------------------------------------------------------------
ezNegative::ezNegative()
{
  ezID = 2;
  m_isComplex = false;
  m_changePixelVals = "//ezNegative \n"
                      "outColour = vec4(1.0-outColour.r,1.0-outColour.g,1.0-outColour.b, 1.0);\n";
}
//----------------------------------------------------------------------------------------------------------------------

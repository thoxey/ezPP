#ifndef EZBRIGHTNESS_H
#define EZBRIGHTNESS_H

#include "ezeffect.h"

//----------------------------------------------------------------------------------------------------------------------
/// @file ezBrightness.h
/// @brief The class for generating the ezBrightness effect
/// @author Tom Hoxey
/// @version 1.0
/// @date 20/3/17 Initial version
//----------------------------------------------------------------------------------------------------------------------

class ezBrightness : public ezEffect
{
public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief The ctor for the ezBrightness class
    /// @param _up Whether the effect will increment or decrement, set true to increment
    /// @param _increment The amount to increment by
    //----------------------------------------------------------------------------------------------------------------------
  ezBrightness(float _increment);
};
#endif // EZBRIGHTNESS_H

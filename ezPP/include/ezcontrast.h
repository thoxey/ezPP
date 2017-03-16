#ifndef EZCONTRAST_H
#define EZCONTRAST_H

#include "ezeffect.h"

//----------------------------------------------------------------------------------------------------------------------
/// @file ezContrast.h
/// @brief The class for generating the ezContrast effect
/// @author Tom Hoxey
/// @version 1.0
/// @date 20/3/17 Initial version
//----------------------------------------------------------------------------------------------------------------------

class ezContrast : public ezEffect
{
public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief The ctor for the ezContrast class
    /// @param _up Whether the effect will increment or decrement, set true to increment
    /// @param _increment The amount to increment by
    //----------------------------------------------------------------------------------------------------------------------
  ezContrast(bool _up, int _increment);

};

#endif // EZCONTRAST_H

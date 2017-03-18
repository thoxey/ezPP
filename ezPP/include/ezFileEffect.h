#ifndef EZFILEEFFECT_H
#define EZFILEEFFECT_H

#include "ezeffect.h"

//----------------------------------------------------------------------------------------------------------------------
/// @file ezFileEffect.h
/// @brief The class for loading in an effect from a file
/// @author Tom Hoxey
/// @version 1.0
/// @date 20/3/17 Initial version
//----------------------------------------------------------------------------------------------------------------------

class ezFileEffect : public ezEffect
{
public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief The ctor for the ezFileEffect class
    /// @param _filename The string for the filename to load in
    //----------------------------------------------------------------------------------------------------------------------
  ezFileEffect(std::string _filename);

};
#endif // EZFILEEFFECT_H

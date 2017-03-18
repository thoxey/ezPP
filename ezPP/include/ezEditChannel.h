#ifndef EZEDITCHANNELS_H
#define EZEDITCHANNELS_H

#include "ezeffect.h"

//----------------------------------------------------------------------------------------------------------------------
/// @file ezEditChannel.h
/// @brief The class for generating the ezEditChannel effect, it changes the colour value of a channel
/// @author Tom Hoxey
/// @version 1.0
/// @date 20/3/17 Initial version
//----------------------------------------------------------------------------------------------------------------------

class ezEditChannel : public ezEffect
{
public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief The ctor for the ezEditChannel class
    /// @param _increment The amount to increment by
    /// @param _channel The amount to increment by
    //----------------------------------------------------------------------------------------------------------------------
  ezEditChannel(float _increment, char _channel);

};
#endif // EZEDITCHANNELS_H

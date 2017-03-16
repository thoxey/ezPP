#ifndef EZ3X3KERNEL_H
#define EZ3X3KERNEL_H
#include "ezeffect.h"

//----------------------------------------------------------------------------------------------------------------------
/// @file ez3x3Kernel.h
/// @brief The class for generating your own 3x3 Kernel effect, using a custom kernel
/// @author Tom Hoxey
/// @version 1.0
/// @date 20/3/17 Initial version
//----------------------------------------------------------------------------------------------------------------------

class ez3x3Kernel : public ezEffect
{
public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief The ctor for the ez3x3Kernel class
  /// @param kernel This allows the user to input a custom kernel
  /// @param _id This prompts the user to add thier own unique id to ensure it doesnt clash with other effects
  //----------------------------------------------------------------------------------------------------------------------
  ez3x3Kernel(std::string kernel, int _id);

};

#endif // EZ3X3KERNEL_H

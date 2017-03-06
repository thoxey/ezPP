#ifndef EZ3X3KERNEL_H
#define EZ3X3KERNEL_H
#include <glm/gtc/type_ptr.hpp>
#include <GL/gl.h>
#include <string>

#include "ezeffect.h"

class ez3x3Kernel : public ezEffect
{
public:
  ez3x3Kernel(std::string kernel, int _id);

};

#endif // EZ3X3KERNEL_H

#ifndef EZEFFECT_H
#define EZEFFECT_H
#include <glm/gtc/type_ptr.hpp>
#include <GL/gl.h>
#include <string>

class ezEffect
{
public:
  ezEffect();

  std::string getPixelValChange();

  int id = 0;

protected:
  std::string m_changePixelVals = "";
private:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the RGB change to be applied to each pixel, likely to change form
  //----------------------------------------------------------------------------------------------------------------------
  glm::vec3 m_RGBchange;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a bool for the children to determine whether the effect needs to know its neighbors values
  //----------------------------------------------------------------------------------------------------------------------
  bool m_isComplex;

};

#endif // EZEFFECT_H

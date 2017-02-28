#ifndef EZEFFECT_H
#define EZEFFECT_H
#include <glm/gtc/type_ptr.hpp>

class ezEffect
{
public:
  ezEffect();
  void changePixelVals();
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

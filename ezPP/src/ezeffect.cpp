#include "ezeffect.h"

ezEffect::ezEffect()
{

}
bool ezEffect::getIsComplex()
{
  return m_isComplex;
}
std::string ezEffect::getPixelValChange()
{
  return m_changePixelVals;
}
std::string ezEffect::getPixelCalc()
{
  return m_calcPixelVals;
}

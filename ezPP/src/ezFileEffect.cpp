#include "ezFileEffect.h"
//----------------------------------------------------------------------------------------------------------------------
/// @file ezFileEffect.cpp
/// @brief The implementation of the ezBrightness class
//----------------------------------------------------------------------------------------------------------------------
ezFileEffect::ezFileEffect(std::string _filename)
{
    std::ifstream fragFile(_filename);
    std::string fragString((std::istreambuf_iterator<char>(fragFile)), std::istreambuf_iterator<char>());
    m_FragSource  = fragString;
    ezCompileEffect();
}
//----------------------------------------------------------------------------------------------------------------------

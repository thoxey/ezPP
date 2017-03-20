#include "mockezpper.h"

void mockezPPer::initializeMockezPPer()
{
    m_inited = false;
    m_captured = false;
}

const std::vector<std::unique_ptr<ezEffect> > &mockezPPer::getEffectVector() const noexcept
{
    return m_effectMasterVector;
}
const bool &mockezPPer::getIsInited() const noexcept
{
    return m_inited;
}
const bool &mockezPPer::getIsCaptured() const noexcept
{
    return m_captured;
}

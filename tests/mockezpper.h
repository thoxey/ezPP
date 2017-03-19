#ifndef MOCKEZPPER_H
#define MOCKEZPPER_H
#include <ezpostprocessor.h>

class mockezPPer : public ezPostProcessor
{
public:
    mockezPPer() = default;
    void initializeMockezPPer();
    const std::vector<ezEffect *> getEffectVector() const noexcept;
    const bool getIsInited() const noexcept;
    const bool getIsCaptured() const noexcept;
};

#endif // MOCKEZPPER_H

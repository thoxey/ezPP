#include "ezpostprocessor.h"

ezPostProcessor::ezPostProcessor()
{
  //Initialize ezPPer
}
ezPostProcessor::~ezPostProcessor()
{
  //Clear buffer etc.
}
void ezPostProcessor::ezAddEffect(ezEffect _addedEffect)
{
  m_effectMasterVector.push_back(_addedEffect);
}
void ezPostProcessor::ezMakePreset(std::vector<ezEffect> _preset)
{
  //Convert from void, makes templates from multiple efects, e.g. old film with grain, B&W and DoF
}
void ezPostProcessor::ezCapture()
{
  //takes the framebuffer and converts to texture
}
void ezPostProcessor::ezCompileEffects()
{
  //This takes the texture and carries out the operations on it defined in m_effectMasterVector
  //This will be the 'meat' of the library and will likely be the biggest function
}
void ezPostProcessor::ezRender()
{
  //pushes to screen our newer sexier post processed image
}
void ezPostProcessor::ezCleanUp()
{
  //An alternative to the destructor in case you wanted to turn off the effects and clean the buffers
  //But keep the post processor around to use later
}

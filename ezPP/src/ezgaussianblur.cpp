#include "ezgaussianblur.h"
//----------------------------------------------------------------------------------------------------------------------
/// @file ezGaussianBlur.cpp
/// @brief The implementation of the ezGaussianBlur class
//----------------------------------------------------------------------------------------------------------------------
ezGaussianBlur::ezGaussianBlur()
{
  ezID = 4;
  m_isComplex = true;
  //Adapted from https://learnopengl.com/#!Advanced-OpenGL/Framebuffers Accesed 17/02
  m_changePixelVals =
      R"changePixelVals(
      kernel = float[](
      0.0625, 0.125, 0.0625,
      0.125,  0.25,  0.125,
      0.0625, 0.125, 0.0625
      );
      col = vec3(0.0);
      for(int i = 0; i < 9; i++)
      {
      sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
      col += sampleTex[i] * kernel[i];
      }
      outColour = vec4(col, 1.0);
      )changePixelVals";
  //End Citation
}
//----------------------------------------------------------------------------------------------------------------------

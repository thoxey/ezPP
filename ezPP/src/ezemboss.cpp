#include "ezemboss.h"
//----------------------------------------------------------------------------------------------------------------------
/// @file ezEmboss.cpp
/// @brief The implementation of the ezEmboss class
//----------------------------------------------------------------------------------------------------------------------
ezEmboss::ezEmboss()
{
  ezID = 5;
  m_isComplex = true;
  //Adapted from https://learnopengl.com/#!Advanced-OpenGL/Framebuffers Accesed 17/02
  m_changePixelVals =
      R"changePixelVals(
      //ezEmboss
      kernel = float[](
      -2.0, -1.0,  0.0,
      -1.0,  1.0,  1.0,
      0.0,  1.0,  2.0
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

#include "ezedgedetect.h"

ezEdgeDetect::ezEdgeDetect()
{
  ezID = 3;
  m_isComplex = true;
  //Adapted from https://learnopengl.com/#!Advanced-OpenGL/Framebuffers Accesed 17/02
  m_changePixelVals =
      R"changePixelVals(
      kernel = float[](
      -0.125f, -0.125f, -0.125f,
      -0.125f,    1.0f, -0.125f,
      -0.125f, -0.125f, -0.125f
      );
      col = vec3(0.0);
      for(int i = 0; i < 9; i++)\
      {\
      sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
      col += sampleTex[i] * kernel[i];
      }
      outColour = vec4(col, 1.0);
      )changePixelVals";
  //End Citation
}



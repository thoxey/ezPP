#include "ez3x3kernel.h"

ez3x3Kernel::ez3x3Kernel(std::string kernel, int _id)
{
  id = 100+_id;
  m_isComplex = true;
  //Adapted from https://learnopengl.com/#!Advanced-OpenGL/Framebuffers Accesed 17/02
  m_changePixelVals =
"          kernel = float[](\n";
           m_changePixelVals.append(kernel);
           m_changePixelVals.append(
"          );\n"
"          for(int i = 0; i < 9; i++)\n"
"          {\n"
"              sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));\n"
"          }\n"
"          col = vec3(0.0);\n"
"          for(int i = 0; i < 9; i++)\n"
"              col += sampleTex[i] * kernel[i];\n"
"          outColour = vec4(col, 1.0);\n");
  //End Citation
}


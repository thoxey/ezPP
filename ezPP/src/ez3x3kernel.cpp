#include "ez3x3kernel.h"

ez3x3Kernel::ez3x3Kernel(std::string kernel, int _id)
{
  id = 100+_id;
  m_isComplex = true;
  //Adapted from https://learnopengl.com/#!Advanced-OpenGL/Framebuffers Accesed 17/02
  m_changePixelVals =
"      offset = 1.0 / 300;\n"
"      offsets = vec2[](\n"
"              vec2(-offset, offset),  // top-left      \n"
"              vec2(0.0f,    offset),  // top-center    \n"
"              vec2(offset,  offset),  // top-right     \n"
"              vec2(-offset, 0.0f),    // center-left   \n"
"              vec2(0.0f,    0.0f),    // center-center \n"
"              vec2(offset,  0.0f),    // center-right  \n"
"              vec2(-offset, -offset), // bottom-left   \n"
"              vec2(0.0f,    -offset), // bottom-center \n"
"              vec2(offset,  -offset)  // bottom-right  \n"
"          );\n"
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


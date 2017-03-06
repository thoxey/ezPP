#include "ezedgedetect.h"

ezEdgeDetect::ezEdgeDetect()
{
  id = 3;
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
"          kernel = float[](\n"
"              -0.125f, -0.125f, -0.125f,\n"
"              -0.125f,    1.0f, -0.125f,\n"
"              -0.125f, -0.125f, -0.125f \n"
"          );\n"
"          col = vec3(0.0);\n"
"          for(int i = 0; i < 9; i++)\n"
"          {\n"
"              sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));\n"
"              col += sampleTex[i] * kernel[i];\n"
"          }\n"
"          outColour = vec4(col, 1.0);\n";
  //End Citation
}


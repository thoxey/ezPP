#include "ezemboss.h"

ezEmboss::ezEmboss()
{
    id = 5;
    m_isComplex = true;
    //Adapted from https://learnopengl.com/#!Advanced-OpenGL/Framebuffers Accesed 17/02
    m_changePixelVals =
            "          kernel = float[](\n"
            "                          -2.0, -1.0,  0.0,"
            "                          -1.0,  1.0,  1.0,"
            "                           0.0,  1.0,  2.0"
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

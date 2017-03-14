#include "ez3x3kernel.h"

ez3x3Kernel::ez3x3Kernel(std::string kernel, int _id)
{
    ezID = 100+_id;
    m_isComplex = true;
    //Adapted from https://learnopengl.com/#!Advanced-OpenGL/Framebuffers Accesed 17/02
    m_calcPixelVals ="kernel = float[](\n";
    m_calcPixelVals.append(kernel);
    m_calcPixelVals.append(R"calcPixelVals(
                           );
                           for(int i = 0; i < 9; i++)
                           {
                           sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
                           }\n
                           col = vec3(0.0);\n
                           for(int i = 0; i < 9; i++)\n
                           col += sampleTex[i] * kernel[i];
                           )calcPixelVals");
            m_changePixelVals = "outColour = vec4(col, 1.0);\n";
    //End Citation
}


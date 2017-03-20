#version 410 core
                    in vec2 TexCoords;
                    out vec4 color;
                    uniform sampler2D screenTexture;
                    float brightnessIncrement =0.100000f;
                   void main()
                   {
                   vec4 outColour = texture(screenTexture, TexCoords);
                   outColour =vec4(outColour.r+brightnessIncrement,outColour.g+brightnessIncrement,outColour.b+brightnessIncrement,1.0f);
                   color = outColour;
                   }
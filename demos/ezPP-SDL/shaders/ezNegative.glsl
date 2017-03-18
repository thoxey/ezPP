#version 330 core
            in vec2 TexCoords;
            out vec4 color;
            uniform sampler2D screenTexture;

            void main()
            {
            vec4 outColour = texture(screenTexture, TexCoords);
            color = vec4(1.0-outColour.r,1.0-outColour.g,1.0-outColour.b, 1.0);;
            }

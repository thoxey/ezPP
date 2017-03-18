#version 330 core
            in vec2 TexCoords;
            out vec4 color;
            uniform sampler2D screenTexture;

            void main()
            {
            //Weights that make it look a bit nicer (We see more green so weight it heavier)
            float average = ((outColour.r * 0.25) + (outColour.g * 0.65) + (outColour.b * 0.1) );
            color = vec4(average, average, average, 1.0);
            }
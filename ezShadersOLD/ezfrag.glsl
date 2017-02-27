#version 330 core

//Taken from https://learnopengl.com/#!Advanced-OpenGL/Framebuffers Accesed 17/02

in vec2 TexCoords;
out vec4 color;

uniform sampler2D screenTexture;

void main()
{
    color = texture(screenTexture, TexCoords);
}

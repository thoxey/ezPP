#include "eznoeffects.h"

ezNoEffects::ezNoEffects()
{
  m_VertSource = "#version 330 core\n"
                 "layout (location = 0) in vec2 position;"
                 "layout (location = 1) in vec2 texCoords;"
                 "out vec2 TexCoords;"
                 "void main()"
                 "{"
                     "gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);"
                     "TexCoords = texCoords;"
                 "}";

  m_FragSource = "#version 330 core\n"
                 "in vec2 TexCoords;"
                 "out vec4 color;"
                 "uniform sampler2D screenTexture;"
                 "void main()"
                 "{"
                     "color = texture(screenTexture, TexCoords);"
                 "}";

}

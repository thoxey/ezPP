#ifndef EZEFFECT_H
#define EZEFFECT_H
#include <glm/gtc/type_ptr.hpp>
#include <GL/gl.h>
#include <string>

class ezEffect
{
public:
  ezEffect();
  void changePixelVals();
  std::string getVertSource();
  std::string getFragSource();
protected:
  std::string m_VertSource, m_FragSource;
private:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the RGB change to be applied to each pixel, likely to change form
  //----------------------------------------------------------------------------------------------------------------------
  glm::vec3 m_RGBchange;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a bool for the children to determine whether the effect needs to know its neighbors values
  //----------------------------------------------------------------------------------------------------------------------
  bool m_isComplex;

  const GLchar * ezVert =
    "#version 330 core\n"
    "layout (location = 0) in vec2 position;"
    "layout (location = 1) in vec2 texCoords;"
    "out vec2 TexCoords;"
    "void main()"
    "{"
        "gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);"
        "TexCoords = texCoords;"
    "}";

  //Taken from https://learnopengl.com/#!Advanced-OpenGL/Framebuffers Accesed 17/02
  const GLchar * ezFrag =
    "#version 330 core\n"
    "in vec2 TexCoords;"
    "out vec4 color;"
    "uniform sampler2D screenTexture;"
    "void main()"
    "{"
        "color = texture(screenTexture, TexCoords);"
    "}";
};

#endif // EZEFFECT_H

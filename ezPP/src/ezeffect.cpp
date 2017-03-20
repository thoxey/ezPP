#include "ezeffect.h"
//----------------------------------------------------------------------------------------------------------------------
/// @file ezEffect.cpp
/// @brief The implementation of the ezEffect class
//----------------------------------------------------------------------------------------------------------------------
ezEffect::ezEffect()
{
    m_FragSource = R"m_FragSource(
                   #version 410 core
                   in vec2 TexCoords;
                   out vec4 color;
                   uniform sampler2D screenTexture;
                   void main()
                   {
                   vec4 outColour = texture(screenTexture, TexCoords);
                   color = outColour;
                   }
                   )m_FragSource";
}
ezEffect::~ezEffect()
{
    glDeleteProgram(m_ShaderProg);
}
const std::string ezEffect::m_VertSource = R"m_VertSource(
                                           #version 410 core
                                           layout (location = 0) in vec2 position;
                                           layout (location = 1) in vec2 texCoords;
                                           out vec2 TexCoords;
                                           void main()
                                           {
                                           gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);
                                           TexCoords = texCoords;
                                           }
                                           )m_VertSource";
//----------------------------------------------------------------------------------------------------------------------
const GLuint &ezEffect::getShaderProg() const noexcept
{
    return m_ShaderProg;
}
//----------------------------------------------------------------------------------------------------------------------
void ezEffect::ezCompileEffect()
{
    GLuint newvertShader = glCreateShader(GL_VERTEX_SHADER);
    const GLchar * vertSource = (GLchar *)m_VertSource.c_str();
    glShaderSource(newvertShader, 1, &vertSource, NULL);
    glCompileShader(newvertShader);
    debugShader(newvertShader);

    //Create and compile our frag shader
    GLuint newFragShader = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar * const fragSource = (GLchar *)m_FragSource.c_str();
    glShaderSource(newFragShader, 1, &fragSource, NULL);
    glCompileShader(newFragShader);
    debugShader(newFragShader);

    //Create our shader program
    m_ShaderProg = glCreateProgram();
    glAttachShader(m_ShaderProg, newvertShader);
    glAttachShader(m_ShaderProg, newFragShader);
    glLinkProgram(m_ShaderProg);

    //Clean up
    glDeleteShader(newvertShader);
    glDeleteShader(newFragShader);
}
//----------------------------------------------------------------------------------------------------------------------
bool ezEffect::debugShader(GLint shader)
{
    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
        std::cerr<<"Shader Error: ";
        for(auto i : errorLog)
            std::cerr<<i;
        return false;
    }
    else
        return true;
}
//----------------------------------------------------------------------------------------------------------------------

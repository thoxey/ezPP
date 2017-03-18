#include "ezpostprocessor.h"
#include <algorithm>
//----------------------------------------------------------------------------------------------------------------------
/// @file ezPostProcessor.cpp
/// @brief The implementation of the ezPostProcessor class
//----------------------------------------------------------------------------------------------------------------------
ezPostProcessor::ezPostProcessor() :
    m_effectMasterVector(),
    m_activeShaders(),
    m_ids(),
    m_effectSource(),
    m_shaders(),
    m_inited(false),
    m_compiled(false),
    m_captured(false),
    m_screenWidth(1),
    m_screenHeight(1),
    quadVAO(0),
    quadVBO(0),
    vertShader(0),
    fragShader(0),
    ezShaderProgram(0),
    m_textureColorbuffer1(0),
    m_textureColorbuffer2(0),
    m_activeTexture(0),
    m_framebuffer1(0),
    m_framebuffer2(0),
    m_activeFramebuffer(0),
    m_DepthStencil1(0),
    m_DepthStencil2(0),
    m_activeDepthStencil(0),
    m_compiledFragShader(""),
    m_compiledVertShader("")
{;}
//----------------------------------------------------------------------------------------------------------------------
ezPostProcessor::~ezPostProcessor()
{
    //Clear buffer etc.
}
//----------------------------------------------------------------------------------------------------------------------
void ezPostProcessor::ezInit(int _screenWidth, int _screenHeight)
{
    //////////////////////////////////////////SAFETY CHECKS
    GLint contextTest;
    glGetIntegerv(GL_MAJOR_VERSION, &contextTest);
    if(contextTest < 3)
    {
        std::cerr<<"No Valid Open GL Context Detected\nOr major GL version is below 3";
        return;
    }
    //////////////////////////////////////////SAFETY CHECKS

    //The vertex positions (the 4 corners corners of the screen -1 to 1).
    //and the texture coordinates (also the four corners of the screen 0 to 1).
    const GLfloat quadVertices[24] =
    {// Positions   // TexCoords
     -1.0f,  1.0f,  0.0f, 1.0f,
     -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,

     -1.0f,  1.0f,  0.0f, 1.0f,
     1.0f,  1.0f,  1.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f};
#ifndef __APPLE__
    glewExperimental = GL_TRUE;
    GLenum GlewError = glewInit();
    // error check
    if (GLEW_OK != GlewError)
    {
        std::cerr<<"Glew Error: "<<glewGetErrorString(GlewError)<<"\n";
    }
#endif
    m_screenHeight = _screenHeight;
    m_screenWidth = _screenWidth;
    //-------------------------------------"Adapted code from https://learnopengl.com/#!Advanced-OpenGL/Framebuffers Accesed 17/02"


    //Brace for large setup!


    //Create a framebuffer & bind it so that it can recieve a texture
    //---------------------------------------------------------------------------------------------------------
    glGenFramebuffers(1, &m_framebuffer1);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer1);
    //Create said color attachment texture
    glGenTextures(1, &m_textureColorbuffer1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureColorbuffer1);
    //Define its properties
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_screenWidth, m_screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //Attach it to our frame buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureColorbuffer1, 0);
    // Create first framebuffer's Renderbuffer Object to hold depth and stencil buffers
    glGenRenderbuffers(1, &m_DepthStencil1);
    glBindRenderbuffer(GL_FRAMEBUFFER, m_DepthStencil1);
    glRenderbufferStorage(GL_FRAMEBUFFER, GL_DEPTH24_STENCIL8, m_screenWidth, m_screenHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthStencil1);
    //---------------------------------------------------------------------------------------------------------


    //Do all that again for a second buffer
    //---------------------------------------------------------------------------------------------------------
    glGenFramebuffers(1, &m_framebuffer2);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer2);
    glGenTextures(1, &m_textureColorbuffer2);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_textureColorbuffer2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_screenWidth, m_screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureColorbuffer2, 0);
    glGenRenderbuffers(1, &m_DepthStencil2);
    glBindRenderbuffer(GL_RENDERBUFFER, m_DepthStencil2);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_screenWidth, m_screenHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthStencil2);
    //---------------------------------------------------------------------------------------------------------

    //Unbind everything so it was like this never happened!
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Setup screen VAO
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    //Pos attrib
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);

    //Colour attrib
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    glBindVertexArray(0);
    //-------------------------------------end citation

    m_effectMasterVector.push_back(new ezEffect);

    m_inited = true;


}
//----------------------------------------------------------------------------------------------------------------------
void ezPostProcessor::ezResize(int _screenWidth, int _screenHeight)
{
    m_screenHeight = _screenHeight;
    m_screenWidth = _screenWidth;
}
//----------------------------------------------------------------------------------------------------------------------
void ezPostProcessor::ezAddEffect(ezEffect * _addedEffect)
{
    m_effectMasterVector.push_back(_addedEffect);
}
//----------------------------------------------------------------------------------------------------------------------
void ezPostProcessor::ezCapture()
{
    //////////////////////////////////////////SAFETY CHECKS
    if(!m_inited)
    {
        std::cerr<<"ezCapture called before ezInit \n";
        return;
    }
    //////////////////////////////////////////SAFETY CHECKS

    //Redirect to my framebuffer
    //This seems lacking??
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer1);
    glViewport(0,0,m_screenWidth,m_screenHeight);

    //Set stage as true so we can continue
    m_captured = true;

}
//----------------------------------------------------------------------------------------------------------------------
void ezPostProcessor::ezRender(GLuint frameBuffer)
{
    //Turn off the depth and stencil test
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);

    //////////////////////////////////////////SAFETY CHECKS
    GLint CurrVAO;//This is used to rebind the VAO that was bound on entering the function
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &CurrVAO);
    if(!m_inited)
    {
        std::cerr<<"ezRender called before ezInit \n";
        return;
    }
    else if(!m_captured)
    {
        std::cerr<<"ezRender called before ezCapture \n";
        return;
    }
    //////////////////////////////////////////SAFETY CHECKS

    //Bind the Screen Space Quad
    glBindVertexArray(quadVAO);


    //  Bool used to swap between buffers
    bool pingPong = false;


    //Swap between the two FBOs using the last ones texture
    for(const auto &i : m_effectMasterVector)
    {
        if(pingPong)
        {
            //Bind FB1 and bind tex2, and then push them to the shader and draw
            glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer1);
            glUseProgram(i->getShaderProg());
            glBindTexture(GL_TEXTURE_2D, m_textureColorbuffer2);

            //Cause swap
            pingPong = false;
        }
        else
        {
            //Bind FB2 and bind tex1, and then push them to the shader and draw
            glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer2);
            glUseProgram(i->getShaderProg());
            glBindTexture(GL_TEXTURE_2D, m_textureColorbuffer1);

            //Cause swap
            pingPong = true;
        }

        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,m_textureColorbuffer1,0);
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,m_textureColorbuffer2,0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }


    //  //Bind our screen texture
    //  glUseProgram(ezShaderProgram);
    //  glBindTexture(GL_TEXTURE_2D, lastTex);
    //  glUniform1i(glGetUniformLocation(ezShaderProgram, "screenTexture"), 0);

    //Bind to the default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    glClearColor(0.f,0.f,0.f,1.f);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    //Renable the depth test
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);


    //Draw to screen
    glDrawArrays(GL_TRIANGLES, 0, 6);


    //Rebind the VAO that was bound on entering, leaving things as they were found
    glBindVertexArray(CurrVAO);

}

//----------------------------------------------------------------------------------------------------------------------
void ezPostProcessor::ezCleanUp()
{
    //Clears all but the default buffer

    for(auto i : m_effectMasterVector)
    {
        if (m_effectMasterVector.size() == 1)
        {
            return;
        }
        m_effectMasterVector.pop_back();
    }


}
//----------------------------------------------------------------------------------------------------------------------
std::string ezPostProcessor::returnEzFrag()
{
    return m_compiledFragShader + "\n";
}
//----------------------------------------------------------------------------------------------------------------------
const std::vector<ezEffect *> &ezPostProcessor::getEffectsVector() const noexcept
{
    return m_effectMasterVector;
}
//----------------------------------------------------------------------------------------------------------------------

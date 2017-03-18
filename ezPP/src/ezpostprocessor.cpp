#include "ezpostprocessor.h"
#include <algorithm>
//----------------------------------------------------------------------------------------------------------------------
/// @file ezPostProcessor.cpp
/// @brief The implementation of the ezPostProcessor class
//----------------------------------------------------------------------------------------------------------------------
ezPostProcessor::ezPostProcessor() :
    m_effectMasterVector(),
    m_inited(false),
    m_captured(false),
    m_screenWidth(1),
    m_screenHeight(1),
    quadVAO(0),
    quadVBO(0),
    m_framebuffer{0, 0},
    m_textureColorbuffer{0,0},
    m_DepthStencil{0, 0}
{;}
//----------------------------------------------------------------------------------------------------------------------
ezPostProcessor::~ezPostProcessor()
{
    m_effectMasterVector.empty();
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
    for(int i = 0; i <2; i++)
    {
        glGenFramebuffers(1, &m_framebuffer[i]);
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer[i]);
        //Create said color attachment texture
        glGenTextures(1, &m_textureColorbuffer[i]);
        glActiveTexture(GL_TEXTURE0+i);
        glBindTexture(GL_TEXTURE_2D, m_textureColorbuffer[i]);
        //Define its properties
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_screenWidth, m_screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //Attach it to our frame buffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureColorbuffer[i], 0);
        // Create first framebuffer's Renderbuffer Object to hold depth and stencil buffers
        glGenRenderbuffers(1, &m_DepthStencil[i]);
        glBindRenderbuffer(GL_FRAMEBUFFER, m_DepthStencil[0]);
        glRenderbufferStorage(GL_FRAMEBUFFER, GL_DEPTH24_STENCIL8, m_screenWidth, m_screenHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthStencil[i]);
    }

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

    //Add a blank effect to the master vector to allow the user to see an output
    m_effectMasterVector.push_back(new ezEffect);

    //Confirm we reached the end of this stage and allow us to continue
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
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer[0]);
    glViewport(0,0,m_screenWidth,m_screenHeight);

    //Set stage as true so we can continue
    m_captured = true;

}
//----------------------------------------------------------------------------------------------------------------------
void ezPostProcessor::ezRender(GLuint frameBuffer)
{
    //////////////////////////////////////////SAFETY CHECKS
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
    //Turn off the depth and stencil test
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);

    //Bind the Screen Space Quad
    glBindVertexArray(quadVAO);

    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    //  Bool used to swap between buffers
    bool pingPong = false;
    bool first = true;
    //Swap between the two FBOs using the last ones texture
    for(const auto &i : m_effectMasterVector)
    {
        //Bind FB1 and bind tex2, and then push them to the shader and draw
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer[pingPong]);
        glUseProgram(i->getShaderProg());
        //Bind the texture
        glBindTexture(GL_TEXTURE_2D, first ? m_textureColorbuffer[1] : m_textureColorbuffer[!pingPong]);  // m_textureColorbuffer[!pingPong]);
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,m_textureColorbuffer[pingPong],0);
        //Swap
        //Uncommenting this line will allow us to stach the effects but causes weird effects
        glDrawArrays(GL_TRIANGLES, 0, 6);
        pingPong = !pingPong;
        if(first)
            first = false;
    }
    //Bind to the default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glClearColor(0.f,0.f,0.f,1.f);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    //Draw to screen
    glDrawArrays(GL_TRIANGLES, 0, 6);

    //Renable the depth test
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);

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

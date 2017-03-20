#include "ezpostprocessor.h"
#include <algorithm>
#include "eznoeffects.h"
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
    if(contextTest < 4)
    {
        std::cerr<<"No Valid Open GL Context Detected\nOr major GL version is below 3";
        return;
    }
    //////////////////////////////////////////SAFETY CHECKS

#ifndef __APPLE__
    glewExperimental = GL_TRUE;
    GLenum GlewError = glewInit();
    // error check
    if (GLEW_OK != GlewError)
    {
        std::cerr<<"Glew Error: "<<glewGetErrorString(GlewError)<<"\n";
    }
#endif
    //Set the screensize
    m_screenHeight = _screenHeight;
    m_screenWidth = _screenWidth;
    //The vertex positions (the 4 corners corners of the screen -1 to 1).
    //and the texture coordinates (also the four corners of the screen 0 to 1).
    //Generate two frame buffers
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
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        //Attach it to our frame buffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, m_textureColorbuffer[i], 0);
        // Create first framebuffer's Renderbuffer Object to hold depth and stencil buffers
        glGenRenderbuffers(1, &m_DepthStencil[i]);
        glBindRenderbuffer(GL_FRAMEBUFFER, m_DepthStencil[0]);
        glRenderbufferStorage(GL_FRAMEBUFFER, GL_DEPTH24_STENCIL8, m_screenWidth, m_screenHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthStencil[i]);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //-------------------------------------"Adapted code from https://learnopengl.com/#!Advanced-OpenGL/Framebuffers Accesed 17/02"
    const GLfloat quadVertices[24] =
    {// Positions   // TexCoords
     -1.0f,  1.0f,  0.0f, 1.0f,
     -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,

     -1.0f,  1.0f,  0.0f, 1.0f,
     1.0f,  1.0f,  1.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f};
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
    m_effectMasterVector.emplace_back(new ezNoEffects());

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
void ezPostProcessor::ezAddEffect(std::unique_ptr<ezEffect> &&_addedEffect)
{
    m_effectMasterVector.emplace_back(std::move(_addedEffect));
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

    for(int i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer[i]);
        glClearColor(0.f,0.f,0.f,1.f);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

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
    glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    //Bind the Screen Space Quad
    glBindVertexArray(quadVAO);

    //glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    //  Bool used to swap between buffers
    bool pingPong = true;
//    //Swap between the two FBOs using the last ones texture
    for(const auto &i : m_effectMasterVector)
    {
        glClearColor(0.f,0.f,0.f,1.f);
//        Bind FB1 and bind tex2, and then push them to the shader and draw
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer[pingPong]);
        glUseProgram(i->getShaderProg());


       ///Uncommenting this allows stacking effects but causes strange visual bugs
//        //Bind the texture
        glBindTexture(GL_TEXTURE_2D, m_textureColorbuffer[!pingPong]);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        ///End
        //Swap
        pingPong = !pingPong;
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
    while(m_effectMasterVector.size() > 1)
    {
        m_effectMasterVector.pop_back();
    }

    for(int i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer[i]);
        glClearColor(0.f,0.f,0.f,1.f);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }


}

#include "ezpostprocessor.h"

ezPostProcessor::ezPostProcessor()
{
    //Initialize ezPPer
}
ezPostProcessor::~ezPostProcessor()
{
    //Clear buffer etc.
}
void ezPostProcessor::ezInit(int _screenWidth, int _screenHeight)
{
    const GLfloat quadVertices[24] =
    {// Positions   // TexCoords
     -1.0f,  1.0f,  0.0f, 1.0f,
     -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,

     -1.0f,  1.0f,  0.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f};
#ifndef __APPLE__
    glewExperimental = GL_TRUE;
    glewInit();
#endif
    m_screenHeight = _screenHeight;
    m_screenWidth = _screenWidth;
    //-------------------------------------"Adapted code from https://learnopengl.com/#!Advanced-OpenGL/Framebuffers Accesed 17/02"
    //Create a color attachment texture
    glGenTextures(1, &textureColorbuffer);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    //Colour Buffer
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_screenWidth, m_screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    //DepthBuffer
    //glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT24, m_screenWidth, m_screenWidth, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    //Create a framebuffer
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //Create a Depthbuffer (Renderbuffer
    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_screenWidth, m_screenHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

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

    m_compiledVertShader = m_VertSource;//Here incase I might want to change the vert shader for some reason

}
void ezPostProcessor::ezAddEffect(ezEffect _addedEffect)
{
    if( std::find(m_ids.begin(), m_ids.end(), _addedEffect.ezID) == m_ids.end() )
    {
        m_effectMasterVector.push_back(_addedEffect);
        if(!_addedEffect.getIsMultiple())
            m_ids.push_back(_addedEffect.ezID);
    }

}
void ezPostProcessor::ezMakePreset(std::vector<ezEffect> _preset)
{
    //Convert from void, makes templates from multiple efects, e.g. old film with grain, B&W and DoF
}
void ezPostProcessor::ezCapture()
{
    if(m_effectMasterVector.size() == 0)
    {
        std::cerr<<"No effects present, please add an effect using ezAddEffect()\n";
        return;
    }

    //Setting up shaders for Screen aligned quad
    vertShader = glCreateShader(GL_VERTEX_SHADER);
    const GLchar * vertSource = (GLchar *)m_compiledVertShader.c_str();
    glShaderSource(vertShader, 1, &vertSource, NULL);
    glCompileShader(vertShader);
    debugShader(vertShader);

    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar * fragSource = (GLchar *)m_compiledFragShader.c_str();
    glShaderSource(fragShader, 1, &fragSource, NULL);
    glCompileShader(fragShader);
    debugShader(fragShader);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

    ezShaderProgram = glCreateProgram();
    glAttachShader(ezShaderProgram, vertShader);
    glAttachShader(ezShaderProgram, fragShader);
    glLinkProgram(ezShaderProgram);

    //Redirects to my framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0,0,m_screenWidth,m_screenHeight);
}
void ezPostProcessor::ezCompileEffects()
{
    //This takes the texture and carries out the operations on it defined in m_effectMasterVector
    std::string compilingFragShader = "";
    std::vector<int> multID;
    for(ezEffect i : m_effectMasterVector)
    {
        //if(i.getIsComplex() && getIsMultiple()) //No effects use this as of yet

        if(i.getIsMultiple() && std::find(multID.begin(), multID.end(), i.ezID) != multID.end())
        {
            multID.push_back(i.ezID);
            for(auto i : m_effectMasterVector)
                compilingFragShader.append(i.getPixelCalc());
            compilingFragShader.append(i.getPixelValChange());
        }
        else if(i.getIsComplex())
        {
            compilingFragShader.append(i.getPixelCalc());
            compilingFragShader.append(i.getPixelValChange());
            //ezSubRender();
        }
        else
        {
            compilingFragShader.append(i.getPixelCalc());
            compilingFragShader.append(i.getPixelValChange());
        }
    }
    m_compiledFragShader = m_FragSource + compilingFragShader + m_FragSourceEnd;
}
void ezPostProcessor::ezSubRender()
{
    std::cerr<<"TESTING Sub Rendering \n";
    //'Cache' the effect to a framebuffer and repeat

    //WIP

    GLuint   newFrameBuffer;
    //takes the framebuffer and converts to texture
    //    glBindFramebuffer(GL_FRAMEBUFFER, newFramebuffer);
    //    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    //    glClear(GL_COLOR_BUFFER_BIT);
    //    glViewport(0,0,m_screenWidth,m_screenHeight);

    //    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    //    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    //    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureColorbuffer, 0);

    //    glBindTexture(GL_TEXTURE_2D, 0);
    glGenFramebuffers(1, &newFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, newFrameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glBindVertexArray(quadVAO);
    glUseProgram(ezShaderProgram);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void ezPostProcessor::ezRender()
{
    //pushes to screen our newer sexier post processed image
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glBindVertexArray(quadVAO);
    glUseProgram(ezShaderProgram);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
void ezPostProcessor::ezCleanUp()
{
    //An alternative to the destructor in case you wanted to turn off the effects and clean the buffers
    //But keep the post processor around to use later

    m_ids.clear();
    ezEffect tmp = m_effectMasterVector[0];
    m_effectMasterVector.clear();
    m_effectMasterVector.push_back(tmp);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(0);
}
std::string ezPostProcessor::returnEzFrag()
{
    return m_compiledFragShader;
}
std::vector<ezEffect> ezPostProcessor::getEffectsVector()
{
    return m_effectMasterVector;
}
void ezPostProcessor::debugShader(GLint shader)
{
    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
        for(auto i : errorLog)
            std::cerr<<i;
    }
}

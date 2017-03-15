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
    GLenum GlewError = glewInit();
    // error check
    if (GLEW_OK != GlewError)
    {
        std::cerr<<"Glew Error: "<<glewGetErrorString(GlewError)<<"\n";
    }
#endif
    m_screenHeight = _screenHeight;
    m_screenWidth = _screenWidth;
    //-------------------------------------"originally adapted from https://learnopengl.com/#!Advanced-OpenGL/Framebuffers Accesed 17/02"
    //Create the first color attachment texture
    glGenTextures(1, &m_textureColorbuffer1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureColorbuffer1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_screenWidth, m_screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    //Create the second color attachment texture
    glGenTextures(1, &m_textureColorbuffer2);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_textureColorbuffer2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_screenWidth, m_screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    //Create the first framebuffer
    glGenFramebuffers(1, &m_framebuffer1);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer1);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureColorbuffer1, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //Create the second framebuffer
    glGenFramebuffers(1, &m_framebuffer2);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer2);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureColorbuffer2, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    m_activeFramebuffer = m_framebuffer1;
    m_activeTexture = m_textureColorbuffer1;

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
    m_compiledFragShader = m_FragSource + m_FragSourceEnd;

    vertShader = glCreateShader(GL_VERTEX_SHADER);
    const GLchar * vertSource = (GLchar *)m_compiledVertShader.c_str();
    glShaderSource(vertShader, 1, &vertSource, NULL);
    glCompileShader(vertShader);
    if(debugShader(vertShader))
        std::cout<<"Vert Shader Initial Load OK \n";

    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar * fragSource = (GLchar *)m_compiledFragShader.c_str();
    glShaderSource(fragShader, 1, &fragSource, NULL);
    glCompileShader(fragShader);
    if(debugShader(fragShader))
        std::cout<<"Frag Shader Initial Load OK \n";

    ezShaderProgram = glCreateProgram();
    glAttachShader(ezShaderProgram, vertShader);
    glAttachShader(ezShaderProgram, fragShader);
    glLinkProgram(ezShaderProgram);
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

}


void ezPostProcessor::ezResize(int _screenWidth, int _screenHeight)
{
    m_screenHeight = _screenHeight;
    m_screenWidth = _screenWidth;
}

void ezPostProcessor::ezAddEffect(ezEffect _addedEffect)
{
    m_effectMasterVector.push_back(_addedEffect);
    //m_ids.push_back(_addedEffect.ezID);
}
void ezPostProcessor::ezMakePreset(std::vector<ezEffect> _preset)
{
    //Convert from void, makes templates from multiple efects, e.g. old film with grain, B&W and DoF
}
void ezPostProcessor::ezCapture()
{
    GLint CurrVAO;//This is used to rebind the VAO that was bound on entering the function, in case the user has bound the VAO beforehand
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &CurrVAO);
    if(m_effectMasterVector.size() == 0)
    {
        std::cerr<<"No effects present, pleaseezPostProcessor myezPPer;\n";
        return;
    }

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

    //Redirects to my framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0,0,m_screenWidth,m_screenHeight);

    glBindVertexArray(CurrVAO);

}


void ezPostProcessor::ezCompileEffects()
{
    //This takes the texture and carries out the operations on it defined in m_effectMasterVector
    std::string compilingFragShader = "";
    m_effectSource.clear();

    for(auto i : m_effectMasterVector)
    {
//        if(i.getIsComplex())
//        {
//            m_effectSource.push_back(i.getPixelCalc());
//            m_effectSource.push_back(i.getPixelValChange());
//            break;
//        }
//        else
//        {
//            m_effectSource.push_back(i.getPixelCalc());
//            m_effectSource.push_back(i.getPixelValChange());
//        }
        compilingFragShader.append(i.getPixelCalc());
        compilingFragShader.append(i.getPixelValChange());
    }

    for(auto i : m_effectSource)
        compilingFragShader.append(i);

    m_compiledFragShader = m_FragSource + compilingFragShader + m_FragSourceEnd;

        fragShader = glCreateShader(GL_FRAGMENT_SHADER);
        const GLchar * fragSource = (GLchar *)m_compiledFragShader.c_str();
        glShaderSource(fragShader, 1, &fragSource, NULL);
        glCompileShader(fragShader);
        if(debugShader(fragShader))
            std::cout<<"Frag Shader Initial Load OK \n";

        ezShaderProgram = glCreateProgram();
        glAttachShader(ezShaderProgram, vertShader);
        glAttachShader(ezShaderProgram, fragShader);
        glLinkProgram(ezShaderProgram);
        glDeleteShader(vertShader);
        glDeleteShader(fragShader);

//    GLuint newFragShader = glCreateShader(GL_FRAGMENT_SHADER);
//    const GLchar * fragSource = (GLchar *)m_compiledFragShader.c_str();
//    glShaderSource(newFragShader, 1, &fragSource, NULL);
//    glCompileShader(newFragShader);
//    debugShader(newFragShader);

//    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

//    GLuint newEzShaderProgram = glCreateProgram();
//    glAttachShader(newEzShaderProgram, vertShader);
//    glAttachShader(newEzShaderProgram, newFragShader);
//    glLinkProgram(newEzShaderProgram);
//    glDeleteShader(vertShader);
//    glDeleteShader(newEzShaderProgram);

//    glLinkProgram(newEzShaderProgram);
//    GLint texLoc = glGetUniformLocation(newEzShaderProgram, "screenTexture");

//    glUseProgram(newEzShaderProgram);
//    glUniform1i(texLoc, 0);

//    if(m_activeFramebuffer != m_framebuffer1)
//        m_activeTexture = glActiveTexture(GL_TEXTURE0);
//    else
//        m_activeTexture = glActiveTexture(GL_TEXTURE1);

//    glBindTexture(GL_TEXTURE_2D, m_activeTexture);
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_activeTexture, 0);

//    m_activeShaders.push_back(newEzShaderProgram);

//    //std::cerr<<returnEzFrag()<<"\n";

//    m_compiledFragShader = m_FragSource + m_FragSourceEnd;
}

void ezPostProcessor::ezRender(GLuint frameBuffer)
{
    glBindVertexArray(quadVAO);

//    for(auto i : m_activeShaders)
//    {
//        if(m_activeFramebuffer != m_framebuffer1)
//            m_activeFramebuffer = m_framebuffer1;
//        else
//            m_activeFramebuffer = m_framebuffer2;

//        if(m_activeTexture != m_textureColorbuffer1)
//            m_activeTexture = m_textureColorbuffer1;
//        else
//            m_activeTexture = m_textureColorbuffer2;

//        glBindFramebuffer(GL_FRAMEBUFFER, m_activeFramebuffer);

//        glUseProgram(i);

//        glDrawArrays(GL_TRIANGLES, 0, 6);
//    }

    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    glUseProgram(ezShaderProgram);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

}
void ezPostProcessor::ezCleanUp()
{
    //clears all effects

    m_ids.clear();
    m_effectMasterVector.clear();
    m_effectSource.clear();
    m_shaders.clear();
    m_activeShaders.clear();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(0);
}
std::string ezPostProcessor::returnEzFrag()
{
    return m_compiledFragShader + "\n";
}
std::vector<ezEffect> ezPostProcessor::getEffectsVector()
{
    return m_effectMasterVector;
}
bool ezPostProcessor::debugShader(GLint shader)
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

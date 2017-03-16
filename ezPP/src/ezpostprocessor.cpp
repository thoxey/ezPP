#include "ezpostprocessor.h"
//----------------------------------------------------------------------------------------------------------------------
/// @file ezPostProcessor.cpp
/// @brief The implementation of the ezPostProcessor class
//----------------------------------------------------------------------------------------------------------------------
ezPostProcessor::ezPostProcessor()
{
  //Initialize ezPPer
  m_inited = false;
  m_captured = false;
}
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
  glBindRenderbuffer(GL_RENDERBUFFER, m_DepthStencil1);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_screenWidth, m_screenHeight);
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

  //Unbind everything so it was like this never happened! (I wont tell if you dont)
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

  m_inited = true;
}
//----------------------------------------------------------------------------------------------------------------------
void ezPostProcessor::ezResize(int _screenWidth, int _screenHeight)
{
  m_screenHeight = _screenHeight;
  m_screenWidth = _screenWidth;
}
//----------------------------------------------------------------------------------------------------------------------
void ezPostProcessor::ezAddEffect(ezEffect _addedEffect)
{
  m_effectMasterVector.push_back(_addedEffect);
  m_ids.push_back(_addedEffect.ezID);
}
//----------------------------------------------------------------------------------------------------------------------
void ezPostProcessor::ezCompileEffects()
{
  //////////////////////////////////////////SAFETY CHECKS
  if(!m_inited)
    {
      std::cerr<<"ezCompile called before ezInit \n";
      return;
    }
  //////////////////////////////////////////SAFETY CHECKS
  //This takes the texture and carries out the operations on it defined in m_effectMasterVector
  std::string compilingFragShader = "";
  m_effectSource.clear();
  m_activeShaders.clear();

  for(auto i : m_effectMasterVector)
    {
      compilingFragShader.append(i.getPixelCalc());
      compilingFragShader.append(i.getPixelValChange());


      m_compiledFragShader = m_FragSource + compilingFragShader + m_FragSourceEnd;

      glBindTexture(GL_TEXTURE_2D, m_activeTexture);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_activeTexture, 0);


      GLuint newvertShader = glCreateShader(GL_VERTEX_SHADER);
      const GLchar * vertSource = (GLchar *)m_compiledVertShader.c_str();
      glShaderSource(newvertShader, 1, &vertSource, NULL);
      glCompileShader(newvertShader);
      debugShader(newvertShader);


      GLuint newFragShader = glCreateShader(GL_FRAGMENT_SHADER);
      const GLchar * fragSource = (GLchar *)m_compiledFragShader.c_str();
      glShaderSource(newFragShader, 1, &fragSource, NULL);
      glCompileShader(newFragShader);
      if(debugShader(newFragShader))
        std::cout<<"New Frag Shader OK\n";

      glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

      GLuint newEzShaderProgram = glCreateProgram();
      glAttachShader(newEzShaderProgram, newvertShader);
      glAttachShader(newEzShaderProgram, newFragShader);

      glLinkProgram(newEzShaderProgram);
      glUseProgram(newEzShaderProgram);


      m_activeShaders.push_back(newEzShaderProgram);
      glDeleteShader(vertShader);
      glDeleteShader(fragShader);
      m_compiledFragShader = m_FragSource + m_FragSourceEnd;
    }

  m_compiled = true;
}
//----------------------------------------------------------------------------------------------------------------------
void ezPostProcessor::ezMakePreset(std::vector<ezEffect> _preset)
{
  //Convert from void, makes templates from multiple efects, e.g. old film with grain, B&W and DoF
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
  else if(!m_compiled)
    {
      std::cerr<<"ezCapture called before ezCompile \n";
      return;
    }
  if(m_effectMasterVector.size() == 0)
    {
      std::cerr<<"No effects present, add an ezNoEffect if you want no effects\n";
      return;
    }
  //////////////////////////////////////////SAFETY CHECKS

  //Redirects to my framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  glViewport(0,0,m_screenWidth,m_screenHeight);

  m_captured = true;

}
//----------------------------------------------------------------------------------------------------------------------
void ezPostProcessor::ezRender(GLuint frameBuffer)
{
  //////////////////////////////////////////SAFETY CHECKS
  GLint CurrVAO;//This is used to rebind the VAO that was bound on entering the function
  glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &CurrVAO);
  if(!m_inited)
    {
      std::cerr<<"ezRender called before ezInit \n";
      return;
    }
  else if(!m_compiled)
    {
      std::cerr<<"ezRender called before ezCompile \n";
      return;
    }
  else if(!m_captured)
    {
      std::cerr<<"ezRender called before ezCapture \n";
      return;
    }
  //////////////////////////////////////////SAFETY CHECKS
  glBindVertexArray(quadVAO);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_STENCIL_TEST);
  bool pingPong = true;
  for(auto i : m_activeShaders)
    {
      if(pingPong)
        {
          glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer1);
          glUseProgram(i);
          glBindTexture(GL_TEXTURE_2D, m_textureColorbuffer2);
          glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureColorbuffer2, 0);
          glDrawArrays(GL_TRIANGLES, 0, 6);
          //glBindFramebuffer(GL_FRAMEBUFFER, 0);
          m_activeTexture = m_textureColorbuffer2;
          pingPong = false;
        }
      else
        {
          glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer2);
          glUseProgram(i);
          glBindTexture(GL_TEXTURE_2D, m_textureColorbuffer1);
          glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureColorbuffer1, 0);
          glDrawArrays(GL_TRIANGLES, 0, 6);
          //glBindFramebuffer(GL_FRAMEBUFFER, 0);
          m_activeTexture = m_textureColorbuffer1;
          pingPong = true;
        }
    }

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_STENCIL_TEST);
  glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


  glBindTexture(GL_TEXTURE_2D, m_activeTexture);
  //glLinkProgram(ezShaderProgram);
  glUseProgram(ezShaderProgram);
  //glUniform1i(glGetUniformLocation(ezShaderProgram, "screenTexture"), 0);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  glBindVertexArray(CurrVAO);

}
//----------------------------------------------------------------------------------------------------------------------
void ezPostProcessor::ezCleanUp()
{
  //An alternative to the destructor in case you wanted to turn off the effects and clean the buffers
  //But keep the post processor around to use later

  m_ids.clear();
  m_effectMasterVector.clear();
  m_effectSource.clear();
  m_shaders.clear();
  for(auto i : m_activeShaders)
    glDeleteProgram(i);
  m_activeShaders.clear();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  glBindVertexArray(0);
}
//----------------------------------------------------------------------------------------------------------------------
std::string ezPostProcessor::returnEzFrag()
{
  return m_compiledFragShader + "\n";
}
//----------------------------------------------------------------------------------------------------------------------
std::vector<ezEffect> ezPostProcessor::getEffectsVector()
{
  return m_effectMasterVector;
}
//----------------------------------------------------------------------------------------------------------------------
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
//----------------------------------------------------------------------------------------------------------------------

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

  //Create a Depthbuffer (Renderbuffer)
  //glGenRenderbuffers(1, &depthBuffer);
  //glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
  //glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_screenWidth, m_screenHeight);
  //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

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
  GLint CurrVAO;//This is used to rebind the VAO that was bound on entering the function
  glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &CurrVAO);

  if(m_effectMasterVector.size() == 0)
    {
      std::cerr<<"No effects present, please add an effect using ezAddEffect()\n";
      return;
    }

  //Setting up shaders for Screen aligned quad, Vert no need to recompile since it never changes
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
  glDeleteShader(vertShader);
  glDeleteShader(fragShader);

  //Redirects to my framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  glClear(GL_COLOR_BUFFER_BIT);
  glViewport(0,0,m_screenWidth,m_screenHeight);

  glBindVertexArray(CurrVAO);

}
void ezPostProcessor::ezCompileEffects()
{
  //This takes the texture and carries out the operations on it defined in m_effectMasterVector
  std::string compilingFragShader = "";
  std::vector<int> multID;
  m_effectSourceFirst.clear();
  //m_effectSourceSecond.clear();
  for(auto i : m_effectMasterVector)
    {
      //if(i.getIsComplex() && getIsMultiple()) //No effects use this as of yet

      if(i.getIsMultiple())
        {
          if(std::find(multID.begin(), multID.end(), i.ezID) == multID.end())
            {
              multID.push_back(i.ezID);
              m_effectSourceFirst.push_back(i.getPixelCalc());
              m_effectSourceSecond.push_back(i.getPixelValChange());
            }
          else
            m_effectSourceFirst.push_back(i.getPixelCalc());
        }

      else if(i.getIsComplex())
        {
          m_effectSourceFirst.push_back(i.getPixelCalc());
          m_effectSourceFirst.push_back(i.getPixelValChange());
          for(auto i : m_effectSourceFirst)
            compilingFragShader.append(i);
          for(auto i : m_effectSourceSecond)
            compilingFragShader.append(i);
          ezSubRender(compilingFragShader);
          m_effectSourceFirst.pop_back();
          m_effectSourceFirst.pop_back();
        }
      else
        {
          m_effectSourceFirst.push_back(i.getPixelValChange());
        }
    }
  for(auto i : m_effectSourceFirst)
    compilingFragShader.append(i);
  for(auto i : m_effectSourceSecond)
    compilingFragShader.append(i);
  m_compiledFragShader = m_FragSource + compilingFragShader + m_FragSourceEnd;

}
void ezPostProcessor::ezSubRender(std::string _compilingFragShader)
{
  m_compiledFragShader = m_FragSource + _compilingFragShader + m_FragSourceEnd;

  //std::cerr<<returnEzFrag()<<"\n";

  GLuint newFrameBuffer;
  glGenFramebuffers(1, &newFrameBuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, newFrameBuffer);
  glBindVertexArray(quadVAO);

  glLinkProgram(ezShaderProgram);
  GLint texLoc = glGetUniformLocation(ezShaderProgram, "screenTexture");

  glUseProgram(ezShaderProgram);
  glUniform1i(texLoc, 0);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

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
  glBindVertexArray(0);
}
void ezPostProcessor::ezCleanUp()
{
  //An alternative to the destructor in case you wanted to turn off the effects and clean the buffers
  //But keep the post processor around to use later

  m_ids.clear();
  m_effectMasterVector.clear();
  m_effectSourceFirst.clear();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT);
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
      for(auto i : errorLog)
        std::cerr<<i;
      return false;
    }
  else
    return true;
}

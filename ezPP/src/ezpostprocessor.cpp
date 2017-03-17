#include "ezpostprocessor.h"
#include <algorithm>
//----------------------------------------------------------------------------------------------------------------------
/// @file ezPostProcessor.cpp
/// @brief The implementation of the ezPostProcessor class
//----------------------------------------------------------------------------------------------------------------------
ezPostProcessor::ezPostProcessor() :  m_effectMasterVector(),
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

const std::string ezPostProcessor::m_VertSource = R"m_VertSource(
                                                  #version 410 core
                                                  layout (location = 0) in vec2 position;
                                                  layout (location = 1) in vec2 texCoords;
                                                  out vec2 TexCoords;
                                                  void main()
                                                  {
                                                  gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);
                                                  TexCoords = texCoords;
                                                  }
                                                  )m_VertSource" ;

const std::string ezPostProcessor::m_FragSource = R"m_FragSource(
                                                  #version 410 core
                                                  in vec2 TexCoords;
                                                  out vec4 color;
                                                  uniform sampler2D screenTexture;

                                                  //Variables for different effects
                                                  float offset;
                                                  float brightnessIncrement = 0.0f;
                                                  float kernel[9];
                                                  float factor = 0;
                                                  float average;
                                                  vec2 offsets[9];
                                                  vec3 sampleTex[9];
                                                  vec3 col;

                                                  float clamp(float toclamp)
                                                  {
                                                  if(toclamp > 255.0f)
                                                  toclamp = 255.0f;
                                                  else if (toclamp < 0.0f)
                                                  toclamp = 0.0f;
                                                  return toclamp / 255.0f;
                                                  }

                                                  void main()
                                                  {
                                                  offset = 0.003;
                                                  vec4 outColour = texture(screenTexture, TexCoords);
                                                  offsets = vec2[](
                                                  vec2(-offset, offset),  // top-left
                                                  vec2(0.0f,    offset),  // top-center
                                                  vec2(offset,  offset),  // top-right
                                                  vec2(-offset, 0.0f),    // center-left
                                                  vec2(0.0f,    0.0f),    // center-center
                                                  vec2(offset,  0.0f),    // center-right
                                                  vec2(-offset, -offset), // bottom-left
                                                  vec2(0.0f,    -offset), // bottom-center
                                                  vec2(offset,  -offset)  // bottom-right
                                                  );
                                                  )m_FragSource";

const std::string ezPostProcessor::m_FragSourceEnd = "color = outColour;\n}";

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
  if(debugShader(fragShader));
    std::cout<<"Frag Shader Initial Load OK \n";

  ezShaderProgram = glCreateProgram();
  glAttachShader(ezShaderProgram, vertShader);
  glAttachShader(ezShaderProgram, fragShader);
  glLinkProgram(ezShaderProgram);
  glDeleteShader(vertShader);
  glDeleteShader(fragShader);

  m_inited = true;


  m_activeShaders.push_back(ezShaderProgram);
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

  //For each effect in the list make a shader and push the program to a vector to use later
  for(const auto &i : m_effectMasterVector)
    {
      //Concatinate our shader
      compilingFragShader.append(m_FragSource);
      compilingFragShader.append(i.getPixelCalc());
      compilingFragShader.append(i.getPixelValChange());
      compilingFragShader.append(m_FragSourceEnd);

      //Create compile and check our vert shader
      GLuint newvertShader = glCreateShader(GL_VERTEX_SHADER);
      const GLchar * vertSource = (GLchar *)m_compiledVertShader.c_str();
      glShaderSource(newvertShader, 1, &vertSource, NULL);
      glCompileShader(newvertShader);
      debugShader(newvertShader);

      //Create compile and check our frag shader
      GLuint newFragShader = glCreateShader(GL_FRAGMENT_SHADER);
      const GLchar * const fragSource = (GLchar *)compilingFragShader.c_str();
      glShaderSource(newFragShader, 1, &fragSource, NULL);
      glCompileShader(newFragShader);
      if(debugShader(newFragShader))
        std::cout<<"New Frag Shader OK\n";

      //Create our shader program
      GLuint newEzShaderProgram = glCreateProgram();
      glAttachShader(newEzShaderProgram, newvertShader);
      glAttachShader(newEzShaderProgram, newFragShader);
      //glBindFragDataLocation??
      glLinkProgram(newEzShaderProgram);

      //Clean up
      glDeleteShader(newvertShader);
      glDeleteShader(newFragShader);

      //Push back to our shader vector to use it later and then res
      m_activeShaders.push_back(newEzShaderProgram);
      compilingFragShader = "";
    }
  //Set this step as true so we can continue
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
//      std::cerr<<"No effects present, add an ezNoEffect if you want no effects\n";
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

  //Bind the Screen Space Quad
  glBindVertexArray(quadVAO);


//  Bool used to swap between buffers
  bool pingPong = false;


  //Swap between the two FBOs using the last ones texture
    for(const auto &i : m_activeShaders)
    {
        if(pingPong)
        {
            //Bind FB1 and bind tex2, and then push them to the shader and draw
            glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer1);
            glUseProgram(i);
            glBindTexture(GL_TEXTURE_2D, m_textureColorbuffer2);
            glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,m_textureColorbuffer1,0);

            //Cause swap
            pingPong = false;
        }
        else
        {            
            //Bind FB2 and bind tex1, and then push them to the shader and draw
            glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer2);
            glUseProgram(i);
            glBindTexture(GL_TEXTURE_2D, m_textureColorbuffer1);
            glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,m_textureColorbuffer2,0);

            //Cause swap
            pingPong = true;
        }
        //glDrawArrays(GL_TRIANGLES, 0, 6);
    }


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
  //An alternative to the destructor in case you wanted to turn off the effects and clean the buffers
  //But keep the post processor around to use later

  if (m_activeShaders.size() == 1)
  {
    std::cout<<"Cannot delete elements from array." <<std::endl;
    return;
  }

  m_ids.clear();
  m_effectMasterVector.clear();
  m_effectSource.clear();
  m_shaders.clear();

  m_activeShaders.erase(std::remove_if(m_activeShaders.begin(),m_activeShaders.end(),[this](const GLuint &itr){ return (itr != this->ezShaderProgram)?glDeleteProgram(itr),true:false;} ));

//  glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer1);
//  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
//  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

//  glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer2);
//  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
//  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
//  glBindVertexArray(0);
}
//----------------------------------------------------------------------------------------------------------------------
std::string ezPostProcessor::returnEzFrag()
{
  return m_compiledFragShader + "\n";
}
//----------------------------------------------------------------------------------------------------------------------
const std::vector<ezEffect> &ezPostProcessor::getEffectsVector() const noexcept
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

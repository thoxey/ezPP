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
  glewExperimental = GL_TRUE;
  glewInit();

  m_screenHeight = _screenHeight;
  m_screenWidth = _screenWidth;
//-------------------------------------"Using code from https://learnopengl.com/#!Advanced-OpenGL/Framebuffers Accesed 17/02"
  //Create a color attachment texture
  textureColorbuffer;
  glGenTextures(1, &textureColorbuffer);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _screenWidth, _screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  //Create a framebuffer
  framebuffer;
  glGenFramebuffers(1, &framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

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
  //Setting up shaders for Screen aligned quad
    vertShader = glCreateShader(GL_VERTEX_SHADER);
    //glShaderSource(vertShader, 1, &vertexSource, NULL);
    glShaderSource(vertShader, 1, &ezVert, NULL);
    glCompileShader(vertShader);

    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    //glShaderSource(fragShader, 1, &fragmentSource, NULL);
    glShaderSource(fragShader, 1, &ezFrag, NULL);
    glCompileShader(fragShader);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

    ezShaderProgram = glCreateProgram();
    glAttachShader(ezShaderProgram, vertShader);
    glAttachShader(ezShaderProgram, fragShader);
    glLinkProgram(ezShaderProgram);
}

void ezPostProcessor::ezAddEffect(ezEffect _addedEffect)
{
  m_effectMasterVector.push_back(_addedEffect);
}
void ezPostProcessor::ezMakePreset(std::vector<ezEffect> _preset)
{
  //Convert from void, makes templates from multiple efects, e.g. old film with grain, B&W and DoF
}
void ezPostProcessor::ezCapture()
{
  //takes the framebuffer and converts to texture
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glViewport(0,0,m_screenWidth,m_screenHeight);
}
void ezPostProcessor::ezCompileEffects()
{
  //This takes the texture and carries out the operations on it defined in m_effectMasterVector
  //This will be the 'meat' of the library and will likely be the biggest function

}
void ezPostProcessor::ezRender()
{
  //pushes to screen our newer sexier post processed image
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT);
  glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindVertexArray(quadVAO);
  glUseProgram(ezShaderProgram);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}
void ezPostProcessor::ezCleanUp()
{
  //An alternative to the destructor in case you wanted to turn off the effects and clean the buffers
  //But keep the post processor around to use later
}

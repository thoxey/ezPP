#include <QString>
#include <QtTest>

#include <SDL.h>

#include <ezgreyscale.h>
#include <eznegative.h>
#include <ezedgedetect.h>
#include <eznoeffects.h>
#include <ez3x3kernel.h>
#include <ezgaussianblur.h>
#include <ezemboss.h>
#include <ezsharpness.h>
#include <ezbrightness.h>
#include <ezcontrast.h>
#include <ezEditChannel.h>
#include <ezFileEffect.h>

#include "mockezpper.h"



SDL_GLContext createOpenGLContext(SDL_Window *window)
{
#ifdef __APPLE__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
#else
    // Note you may have to change this depending upon the driver (Windows is fussy)
    // stick to 3.2 as the core base level for NGL works ok
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

#endif
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,4);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    return SDL_GL_CreateContext(window);
}

bool debugShaderProgram(GLint _shaderProgram)
{

    GLuint shaders[2];
    GLsizei count;

    glGetAttachedShaders(_shaderProgram, 2, &count, shaders);

    for(int i = 0; i < 2; i++)
    {
        GLint isCompiled = 0;
        glGetShaderiv(shaders[i], GL_COMPILE_STATUS, &isCompiled);
        if(isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(shaders[i], GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> errorLog(maxLength);
            glGetShaderInfoLog(shaders[i], maxLength, &maxLength, &errorLog[0]);
            std::cerr<<"Shader Error: ";
            for(auto i : errorLog)
                std::cerr<<i;
            return false;
        }
    }
    return true;

}

class ezTest : public QObject
{
    Q_OBJECT

public:
    ezTest();

private Q_SLOTS:
    void initedTRUE();
    void initedFALSE();
    void capturedTRUE();
    void capturedFALSEnoInit();
    void capturedFALSEnoCapture();
    void defaultEffectAddedTRUE();
    void defaultEffectAddedFALSE();
    void addMultipleEffectsTRUE();
    void addMultipleEffectsFALSE();
    void ezCleanUpClearsVector();
    void testEzGreyscale();
    void testEzNegative();
    void testEzEdgeDetect();
    void testEz3x3Kernel();
    void testEzgaussianblur();
    void testEzEmboss();
    void testEzSharpness();
    void testEzBrightness();
    void testEzContrast();
    void testEzEditChannel();
    void testEzFileEffect();
    void benchmarkNoEffectEzLoop();
    void benchmarkStandardEzLoop();
    void benchmarkExtremeEzLoop();
    void benchmarkCompileEzEffect();
    void benchmarkCompileLotsOfEzEffect();
};

ezTest::ezTest()
{
    //Init SDL and therefore a GL context
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_Window* window = SDL_CreateWindow("OpenGL", 100, 100, 0, 0, SDL_WINDOW_OPENGL);
    SDL_GL_CreateContext(window);
    createOpenGLContext(window);
    SDL_GL_SwapWindow(window);
    //Init Glew
#ifndef __APPLE__
    glewExperimental = GL_TRUE;
    glewInit();
#endif
}
//-------------------------------------------------------------------------------------------------------------------Testing Library Code
void ezTest::initedTRUE()//Test that ezInit set m_inited to true
{
    mockezPPer testPPer;
    testPPer.ezInit(1,1);
    QVERIFY2(testPPer.getIsInited(), "ezInit does not set m_inited to true");
}
void ezTest::initedFALSE()//Test that ezInit set m_inited to true
{
    mockezPPer testPPer;
    QVERIFY2(!testPPer.getIsInited(), "m_inited set to true by default");
}
void ezTest::capturedTRUE()//Test that ezCapture sets m_capture to true
{
    mockezPPer testPPer;
    testPPer.ezInit(1,1);
    testPPer.ezCapture();
    QVERIFY2(testPPer.getIsCaptured(), "ezCapture does not set m_capture to true");
}
void ezTest::capturedFALSEnoInit()//Test that ezCapture doesnt finish if ezInit hasnt been called
{
    mockezPPer testPPer;
    testPPer.ezCapture();
    QVERIFY2(testPPer.getIsCaptured() == false, "m_capture set to true by default");
}
void ezTest::capturedFALSEnoCapture()//Test that ezInit doesnt effect m_capture
{
    mockezPPer testPPer;
    testPPer.ezInit(1,1);
    QVERIFY2(!testPPer.getIsCaptured(), "ezInit set m_capture to true");
}
void ezTest::defaultEffectAddedTRUE()//Checks that we add a default effect to the effect vector as part of init
{
    mockezPPer testPPer;
    testPPer.ezInit(1,1);
    QVERIFY2(testPPer.getEffectVector().size() == 1, "ezInit has not added the default effect");
}
void ezTest::defaultEffectAddedFALSE()//Checks that m_masterEffectsVector is not one without ezInit
{
    mockezPPer testPPer;
    QVERIFY2(!testPPer.getEffectVector().size() == 1, "an effect has been added without ezInit being called");
}
void ezTest::addMultipleEffectsTRUE()
{
    mockezPPer testPPer;
    uint numOfEffects = 10;
    for(uint i = 0; i < numOfEffects; i++)
        testPPer.ezAddEffect(std::unique_ptr<ezEffect>(new ezEffect));
    QVERIFY2(testPPer.getEffectVector().size() == numOfEffects, "ezAdd not adding the right amount of effects");
}
void ezTest::addMultipleEffectsFALSE()
{
    mockezPPer testPPer;
    uint numOfEffects = 10;
    QVERIFY2(testPPer.getEffectVector().size() != numOfEffects, "PPer constructing with the wrong number of effects");
}
void ezTest::ezCleanUpClearsVector()
{
    mockezPPer testPPer;
    uint numOfEffects = 10;
    for(uint i = 0; i < numOfEffects; i++)
        testPPer.ezAddEffect(std::unique_ptr<ezEffect>(new ezEffect));
    testPPer.ezCleanUp();
    QVERIFY2(testPPer.getEffectVector().size() == 1, "ezCleanUp not clearing all effects");
}
//-------------------------------------------------------------------------------------------------------------------Testing Shaders

void ezTest::testEzGreyscale()
{
    ezGreyscale testEff;
    QVERIFY2(debugShaderProgram(testEff.getShaderProg()), "ezGreyScale not a valid shader");
}
void ezTest::testEzNegative()
{
    ezNegative testEff;
    QVERIFY2(debugShaderProgram(testEff.getShaderProg()), "ezNegative not a valid shader");
}
void ezTest::testEzEdgeDetect()
{
    ezEdgeDetect testEff;
    QVERIFY2(debugShaderProgram(testEff.getShaderProg()), "ezEdgeDetect not a valid shader");
}
void ezTest::testEz3x3Kernel()
{
    std::string testString =    "-2.0, -1.0,  0.0,"
                                "-1.0,  1.0,  1.0,"
                                " 0.0,  1.0,  2.0";
    ez3x3Kernel testEff(testString);

    QVERIFY2(debugShaderProgram(testEff.getShaderProg()), "ez3x3Kernel not a valid shader");

}
void ezTest::testEzgaussianblur()
{
    ezGaussianBlur testEff;
    QVERIFY2(debugShaderProgram(testEff.getShaderProg()), "ezGaussianBlur not a valid shader");
}
void ezTest::testEzEmboss()
{
    ezEmboss testEff;
    QVERIFY2(debugShaderProgram(testEff.getShaderProg()), "ezEmboss not a valid shader");
}
void ezTest::testEzSharpness()
{
    ezSharpness testEff;
    QVERIFY2(debugShaderProgram(testEff.getShaderProg()), "ezSharpness not a valid shader");
}
void ezTest::testEzBrightness()
{
    ezBrightness testEff(0.0f);
    QVERIFY2(debugShaderProgram(testEff.getShaderProg()), "ezBrightness not a valid shader");
}
void ezTest::testEzContrast()
{
    ezContrast testEff(0);
    QVERIFY2(debugShaderProgram(testEff.getShaderProg()), "ezContrast not a valid shader");
}
void ezTest::testEzEditChannel()
{
    ezEditChannel testEff(0.0f,'r');
    QVERIFY2(debugShaderProgram(testEff.getShaderProg()), "ezGreyScale not a valid shader");
}
void ezTest::testEzFileEffect()
{
    ezFileEffect testEff("ShaderTester/fragSource.glsl");
    QVERIFY2(debugShaderProgram(testEff.getShaderProg()), "ezGreyScale not a valid shader");
}
//-------------------------------------------------------------------------------------------------------------------Benchmarks
void ezTest::benchmarkNoEffectEzLoop()//Testing how long it takes to run a loop with no effects
{
    mockezPPer testPPer;
    testPPer.ezInit(1,1);
    uint numOfEffects = 10;
    for(uint i = 0; i < numOfEffects; i++)
        testPPer.ezAddEffect(std::unique_ptr<ezEffect>(new ezEffect));
    QBENCHMARK
    {
        testPPer.ezCapture();
        testPPer.ezRender();
    }
}
void ezTest::benchmarkStandardEzLoop()//Testing how long it takes to run a loop with 10 effects
{
    mockezPPer testPPer;
    testPPer.ezInit(1,1);
    QBENCHMARK
    {
        testPPer.ezCapture();
        testPPer.ezRender();
    }
}
void ezTest::benchmarkExtremeEzLoop()//Testing how long it takes to run a loop with 1000 effects
{
    mockezPPer testPPer;
    testPPer.ezInit(1,1);
    uint numOfEffects = 1000;
    for(uint i = 0; i < numOfEffects; i++)
        testPPer.ezAddEffect(std::unique_ptr<ezEffect>(new ezEffect));
    QBENCHMARK
    {
        testPPer.ezCapture();
        testPPer.ezRender();
    }
}
void ezTest::benchmarkCompileEzEffect()//Testing how long it takes to add (and therefore compile) an effect
{
    mockezPPer testPPer;
    QBENCHMARK
    {
        testPPer.ezAddEffect(std::unique_ptr<ezEffect>(new ezNoEffects));
    }
}
void ezTest::benchmarkCompileLotsOfEzEffect()//Testing how long it takes toadd (and therefore compile) 1000 effects
{
    mockezPPer testPPer;
    uint numOfEffects = 1000;
    QBENCHMARK
    {
        for(uint i = 0; i < numOfEffects; i++)
            testPPer.ezAddEffect(std::unique_ptr<ezEffect>(new ezNoEffects));
    }
}

QTEST_APPLESS_MAIN(ezTest)

#include "tst_eztest.moc"

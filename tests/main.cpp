#include<gtest/gtest.h>
#include <string>
#include <sstream>

#include<GL/glew.h>
#include<GL/gl.h>

#include <ezpostprocessor.h>
#include <eznoeffects.h>
#include "eztesteffect.h"


int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
TEST(ezPostProcessor,DefaultCtor)
{
  ezPostProcessor test;
  EXPECT_TRUE(sizeof(test) != 0);
}
TEST(ezPostProcessor, DuplicateEffects)
{//This test checks that we cannot added multiple duplicate effects to our master vector
  ezPostProcessor test;
  ezNoEffects testEff;
  test.ezAddEffect(testEff);
  test.ezAddEffect(testEff);
  EXPECT_TRUE(test.getEffectsVector().size() == 1);
}
TEST(ezPostProcessor, SameEffectDifferentIDs)
{
  ezPostProcessor test;
  ezTestEffect testEff0(0);
  ezTestEffect testEff1(1);
  ezTestEffect testEff2(2);
  ezTestEffect testEff3(3);
  ezTestEffect testEff4(4);
  ezTestEffect testEff5(5);
  ezTestEffect testEff6(6);
  ezTestEffect testEff7(7);
  ezTestEffect testEff8(8);
  ezTestEffect testEff9(9);
  test.ezAddEffect(testEff0);
  test.ezAddEffect(testEff1);
  test.ezAddEffect(testEff2);
  test.ezAddEffect(testEff3);
  test.ezAddEffect(testEff4);
  test.ezAddEffect(testEff5);
  test.ezAddEffect(testEff6);
  test.ezAddEffect(testEff7);
  test.ezAddEffect(testEff8);
  test.ezAddEffect(testEff9);
  EXPECT_TRUE(test.getEffectsVector().size() == 10);



}

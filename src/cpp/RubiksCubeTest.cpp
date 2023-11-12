#include <gtest/gtest.h>
#include "Util.h"
#include "RubiksCube.h"

TEST(RubiksCube, rotate)
{
   RubiksCube cube;
   auto expected = RubiksCube(SOURCE_DIR "/test_data/0.txt");
   EXPECT_EQ(cube, expected);

   cube.rotate(Face::FRONT);
   expected = RubiksCube(SOURCE_DIR "/test_data/F.txt");
   EXPECT_EQ(cube, expected);

   cube.rotate(Face::FRONT, true);
   expected = RubiksCube(SOURCE_DIR "/test_data/FFF.txt");
   EXPECT_EQ(cube, expected);

   cube.rotate(Face::RIGHT);
   expected = RubiksCube(SOURCE_DIR "/test_data/FFFR.txt");
   EXPECT_EQ(cube, expected);

   cube.rotate(Face::BOTTOM, true);
   cube.rotate(Face::BOTTOM);
   expected = RubiksCube(SOURCE_DIR "/test_data/FFFRBBB.txt");
   EXPECT_EQ(cube, expected);
}

TEST(RubiksCube, toMatrix)
{
   RubiksCube cube;
   auto expected = RubiksCube(SOURCE_DIR "/test_data/0.txt");
   EXPECT_EQ(cube.toMatrix(), expected.toMatrix());
   cube.rotate(Face::FRONT);
   EXPECT_NE(cube.toMatrix(), expected.toMatrix());
}

TEST(RubiksCube, randomizedToMatrix)
{
   int numTests = 5000;
   for (int i = 0; i < numTests; i++)
   {
      RubiksCube cube1 = RubiksCube();
      RubiksCube cube2 = RubiksCube();
      cube1.scramble();
      cube2.scramble();
      if (!(cube1 == cube2))
         EXPECT_NE(cube1.toMatrix(), cube2.toMatrix());
      else
         EXPECT_EQ(cube1.toMatrix(), cube2.toMatrix());
   }
}

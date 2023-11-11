#include <gtest/gtest.h>
#include "RubiksCube.h"

/*
   T4
L2 F0 R3 B1
   B5

0 1 2
7 8 3
6 5 4
*/

TEST(RubiksCube, rotate)
{
    RubiksCube cube;
    auto expected = RubiksCube("../test_data/0.txt");
    EXPECT_EQ(cube, expected);

    cube.rotate(Face::FRONT);
    expected = RubiksCube("../test_data/F.txt");
    EXPECT_EQ(cube, expected);

    cube.rotate(Face::FRONT, true);
    expected = RubiksCube("../test_data/FFF.txt");
    EXPECT_EQ(cube, expected);

    cube.rotate(Face::RIGHT);
    expected = RubiksCube("../test_data/FFFR.txt");
    EXPECT_EQ(cube, expected);

    cube.rotate(Face::BOTTOM, true);
    cube.rotate(Face::BOTTOM);
    expected = RubiksCube("../test_data/FFFRBBB.txt");
    EXPECT_EQ(cube, expected);
}

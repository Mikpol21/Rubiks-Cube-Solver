#include "RubiksCube.h"

int main()
{
    RubiksCube cube("test_data/cube1.txt");
    std::cout << cube << std::endl;
    cube.rotate(Face::FRONT);
    std::cout << cube << std::endl;
}
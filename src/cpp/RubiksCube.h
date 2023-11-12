#pragma once

#include <vector>
#include <array>
#include <iostream>
#include <string>

enum Color
{
    WHITE,
    YELLOW,
    BLUE,
    GREEN,
    RED,
    ORANGE,
    INVALID_COLOR
};

std::ostream &operator<<(std::ostream &os, const Color &color);

enum Face
{
    FRONT,  // Red
    BACK,   // Orange
    LEFT,   // Green
    RIGHT,  // Blue
    TOP,    // White
    BOTTOM, // Yellow
    INVALID
};

std::ostream &operator<<(std::ostream &os, const Face &face);

template <unsigned int N, unsigned int M, typename T>
using Matrix = std::array<std::array<T, M>, N>;

struct Sticker
{
    Color color;
    Face face;
};

bool operator<(const Sticker &lhs, const Sticker &rhs);

template <unsigned int N>
struct Cublet
{
    Sticker stickers[N];
};

template <unsigned int N>
bool operator<(const Cublet<N> &lhs, const Cublet<N> &rhs);

class RubiksCube
{

    std::vector<Cublet<3>> corners;
    std::vector<Cublet<2>> middles;

    template <unsigned int N>
    const std::vector<Cublet<N>> &getCublets() const;
    template <unsigned int N>
    Color colorOnEdge(const std::vector<Face> &faces) const;
    template <unsigned int N>
    int cubletIndex(const std::vector<Face> &faces) const;

public:
    RubiksCube();

    RubiksCube(const RubiksCube &cube);

    RubiksCube(const Matrix<6, 9, Color> &matrix);

    RubiksCube(std::string cubeFile);

    RubiksCube(std::vector<Cublet<3>> corners, std::vector<Cublet<2>> middles);

    void rotate(Face face, bool twice = false);

    friend bool operator==(const RubiksCube &lhs, const RubiksCube &rhs);

    Matrix<20, 24, bool> toMatrix() const;

    /*
        0 1 2
        7 8 3
        6 5 4
    */
    Matrix<6, 9, Color> toColorMatrix() const;

    RubiksCube scramble(int numMoves = 20, int seed = 0);
    static RubiksCube scrambleCube(RubiksCube &cube, int numMoves = 20, int seed = 0);

    std::vector<RubiksCube> scrambleWithTrace(int numMoves = 20, int seed = 0);
    static std::vector<RubiksCube> scrambleCubeWithTrace(RubiksCube &cube, int numMoves = 20, int seed = 0);
    friend std::ostream &operator<<(std::ostream &os, const RubiksCube &cube);

    std::string toString() const;
};

void printCube(const RubiksCube &cube);
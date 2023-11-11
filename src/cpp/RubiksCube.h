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

struct Edge
{
    Color color;
    Face face;
};

template <unsigned int N>
struct Cublet
{
    Edge edges[N];
};

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

    friend std::ostream &operator<<(std::ostream &os, const RubiksCube &cube);
};
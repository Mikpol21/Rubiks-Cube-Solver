#include "RubiksCube.h"
#include "Util.h"
#include <optional>
#include <cassert>
#include <fstream>

namespace
{
    Face rotations[6][6] = {
        {FRONT, INVALID, TOP, BOTTOM, RIGHT, LEFT}, // Front
        {INVALID, BACK, BOTTOM, TOP, LEFT, RIGHT},  // Back
        {BOTTOM, TOP, LEFT, INVALID, FRONT, BACK},  // Left
        {TOP, BOTTOM, INVALID, RIGHT, BACK, FRONT}, // Right
        {LEFT, RIGHT, BACK, FRONT, TOP, INVALID},   // Top
        {RIGHT, LEFT, FRONT, BACK, INVALID, BOTTOM} // Bottom
    };

    std::pair<Face, Face> startingFaces[6] = {
        {TOP, LEFT},  // Front
        {TOP, RIGHT}, // Back
        {TOP, BACK},  // Left
        {TOP, FRONT}, // Right
        {BACK, LEFT}, // Top
        {FRONT, LEFT} // Bottom
    };

    int cubeFormat[9] = {0, 1, 2, 7, 8, 3, 6, 5, 4};

    template <unsigned int N>
    bool isRotated(Cublet<N> cublet, Face face)
    {
        for (int i = 0; i < N; i++)
            if (cublet.edges[i].face == face)
                return true;
        return false;
    }

    bool shareEdge(Face face1, Face face2)
    {
        if (face1 == FRONT && face2 == BACK)
            return false;
        if (face1 == BACK && face2 == FRONT)
            return false;
        if (face1 == LEFT && face2 == RIGHT)
            return false;
        if (face1 == RIGHT && face2 == LEFT)
            return false;
        if (face1 == TOP && face2 == BOTTOM)
            return false;
        if (face1 == BOTTOM && face2 == TOP)
            return false;
        return true;
    }

    Color fromChar(char c)
    {
        switch (c)
        {
        case 'R':
            return RED;
        case 'O':
            return ORANGE;
        case 'G':
            return GREEN;
        case 'B':
            return BLUE;
        case 'W':
            return WHITE;
        case 'Y':
            return YELLOW;
        case '0':
            return WHITE;
        case '1':
            return YELLOW;
        case '2':
            return BLUE;
        case '3':
            return GREEN;
        case '4':
            return RED;
        case '5':
            return ORANGE;
        default:
            return INVALID_COLOR;
        }
    }

    Matrix<6, 9, Color> fileToCubeMatrix(std::string cubeFilePath)
    {
        std::ifstream file;
        file.open(cubeFilePath.data());
        if (!file.is_open())
            throw std::runtime_error("Could not open file");
        char c;
        Matrix<6, 9, Color> matrix;
        for (int i = 0; i < 9; i++)
        {
            file >> c;
            matrix[TOP][cubeFormat[i]] = fromChar(c);
        }
        int faces[4] = {LEFT, FRONT, RIGHT, BACK};
        for (int line = 0; line < 3; line++)
            for (int face : faces)
                for (int i = 0; i < 3; i++)
                {
                    file >> c;
                    matrix[face][cubeFormat[3 * line + i]] = fromChar(c);
                }
        for (int i = 0; i < 9; i++)
        {
            file >> c;
            matrix[BOTTOM][cubeFormat[i]] = fromChar(c);
        }
        return std::move(matrix);
    }
}

RubiksCube::RubiksCube()
{
    // CornerCublets
    for (int color1 = 0; color1 < 6; color1++)
        for (int color2 = color1 + 1; color2 < 6; color2++)
            for (int color3 = color2 + 1; color3 < 6; color3++)
                if (shareEdge((Face)color1, (Face)color2) && shareEdge((Face)color2, (Face)color3) && shareEdge((Face)color3, (Face)color1))
                {
                    corners.push_back(Cublet<3>());
                    Cublet<3> &corner = corners.back();
                    corner.edges[0].color = (Color)color1;
                    corner.edges[1].color = (Color)color2;
                    corner.edges[2].color = (Color)color3;
                    corner.edges[0].face = (Face)(color1);
                    corner.edges[1].face = (Face)(color2);
                    corner.edges[2].face = (Face)(color3);
                }
    // MiddleCublets
    for (int color1 = 0; color1 < 6; color1++)
        for (int color2 = color1 + 1; color2 < 6; color2++)
            if (shareEdge((Face)color1, (Face)color2))
            {
                middles.push_back(Cublet<2>());
                Cublet<2> &middle = middles.back();
                middle.edges[0].color = (Color)color1;
                middle.edges[1].color = (Color)color2;
                middle.edges[0].face = (Face)(color1);
                middle.edges[1].face = (Face)(color2);
            }
    assert(corners.size() == 8);
    assert(middles.size() == 12);
}

RubiksCube::RubiksCube(std::vector<Cublet<3>> corners, std::vector<Cublet<2>> middles) : corners(corners), middles(middles) {}

RubiksCube::RubiksCube(const Matrix<6, 9, Color> &matrix) : RubiksCube()
{
    // CornerCublets
    for (int face = 0; face < 6; face++)
    {
        Face currentFace = startingFaces[face].first;
        Face previousFace = startingFaces[face].second;
        for (int i = 0; i < 4; i++)
        {
            Cublet<3> &cublet = corners[cubletIndex<3>({Face(face), currentFace, previousFace})];
            for (auto &edge : cublet.edges)
            {
                if (edge.face == face)
                    edge.color = matrix[face][2 * i];
            }
            Cublet<2> &middle = middles[cubletIndex<2>({Face(face), currentFace})];
            for (auto &edge : middle.edges)
            {
                if (edge.face == face)
                    edge.color = matrix[face][2 * i + 1];
            }
            previousFace = currentFace;
            currentFace = rotations[face][currentFace];
        }
    }
}

RubiksCube::RubiksCube(std::string cubeFilePath) : RubiksCube(fileToCubeMatrix(cubeFilePath)) {}

void RubiksCube::rotate(Face face, bool twice)
{
    for (auto &cublet : corners)
    {
        if (isRotated(cublet, face))
        {
            for (int i = 0; i < 3; i++)
                cublet.edges[i].face = rotations[face][cublet.edges[i].face];
        }
    }
    for (auto &cublet : middles)
    {
        if (isRotated(cublet, face))
        {
            for (int i = 0; i < 2; i++)
                cublet.edges[i].face = rotations[face][cublet.edges[i].face];
        }
    }

    if (twice)
        RubiksCube::rotate(face, false);
}

template <>
const std::vector<Cublet<3>> &RubiksCube::getCublets() const
{
    return corners;
}

template <>
const std::vector<Cublet<2>> &RubiksCube::getCublets() const
{
    return middles;
}

template <unsigned int N>
int RubiksCube::cubletIndex(const std::vector<Face> &faces) const
{
    const auto &cublets = getCublets<N>();
    for (int i = 0; i < cublets.size(); i++)
    {
        bool isMatch = true;
        for (Face face : faces)
            isMatch &= isRotated(cublets[i], face);
        if (isMatch)
            return i;
    }
    return -1;
}

template <unsigned int N>
Color RubiksCube::colorOnEdge(const std::vector<Face> &faces) const
{
    int cubletIndex = RubiksCube::cubletIndex<N>(faces);
    if (cubletIndex == -1)
        return INVALID_COLOR;
    const auto &cublet = getCublets<N>()[cubletIndex];
    for (int i = 0; i < N; i++)
        if (cublet.edges[i].face == faces[0])
            return cublet.edges[i].color;
    __builtin_unreachable();
}

Matrix<6, 9, Color> RubiksCube::toColorMatrix() const
{
    Matrix<6, 9, Color> matrix;
    for (int face = FRONT; face <= BOTTOM; face++)
    {
        matrix[face][8] = Color(face);
        Face currentFace = startingFaces[face].first;
        Face previousFace = startingFaces[face].second;
        for (int i = 0; i < 4; i++)
        {
            matrix[face][2 * i] = colorOnEdge<3>({Face(face), currentFace, previousFace});
            matrix[face][2 * i + 1] = colorOnEdge<2>({Face(face), currentFace});
            previousFace = currentFace;
            currentFace = rotations[face][currentFace];
        }
    }
    return std::move(matrix);
}

bool operator==(const RubiksCube &lhs, const RubiksCube &rhs)
{
    const auto lhsMatrix = lhs.toColorMatrix();
    const auto rhsMatrix = rhs.toColorMatrix();
    for (int face = FRONT; face <= BOTTOM; face++)
        for (int i = 0; i < 9; i++)
            if (lhsMatrix[face][i] != rhsMatrix[face][i])
                return false;
    return true;
}

std::ostream &operator<<(std::ostream &os, const Color &color)
{
    switch (color)
    {
    case RED:
        os << "\e[31mR\e[0m";
        break;
    case ORANGE:
        os << "\e[33mO\e[0m";
        break;
    case GREEN:
        os << "\e[32mG\e[0m";
        break;
    case BLUE:
        os << "\e[34mB\e[0m";
        break;
    case WHITE:
        os << "W";
        break;
    case YELLOW:
        os << "\e[93mY\e[0m";
        break;
    case INVALID_COLOR:
        os << "\e[13mX\e[0m";
        break;
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const Face &face)
{
    switch (face)
    {
    case FRONT:
        os << "FRONT";
        break;
    case BACK:
        os << "BACK";
        break;
    case LEFT:
        os << "LEFT";
        break;
    case RIGHT:
        os << "RIGHT";
        break;
    case TOP:
        os << "TOP";
        break;
    case BOTTOM:
        os << "BOTTOM";
        break;
    case INVALID:
        os << "INVALID";
        break;
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const RubiksCube &cube)
{
    const auto matrix = cube.toColorMatrix();
    os << "RubiksCube:" << std::endl;
    os << "      " << matrix[TOP][0] << " " << matrix[TOP][1] << " " << matrix[TOP][2] << std::endl;
    os << "      " << matrix[TOP][7] << " " << matrix[TOP][8] << " " << matrix[TOP][3] << std::endl;
    os << "      " << matrix[TOP][6] << " " << matrix[TOP][5] << " " << matrix[TOP][4] << std::endl;
    os << matrix[LEFT][0] << " " << matrix[LEFT][1] << " " << matrix[LEFT][2] << " ";
    os << matrix[FRONT][0] << " " << matrix[FRONT][1] << " " << matrix[FRONT][2] << " ";
    os << matrix[RIGHT][0] << " " << matrix[RIGHT][1] << " " << matrix[RIGHT][2] << " ";
    os << matrix[BACK][0] << " " << matrix[BACK][1] << " " << matrix[BACK][2] << std::endl;
    os << matrix[LEFT][7] << " " << matrix[LEFT][8] << " " << matrix[LEFT][3] << " ";
    os << matrix[FRONT][7] << " " << matrix[FRONT][8] << " " << matrix[FRONT][3] << " ";
    os << matrix[RIGHT][7] << " " << matrix[RIGHT][8] << " " << matrix[RIGHT][3] << " ";
    os << matrix[BACK][7] << " " << matrix[BACK][8] << " " << matrix[BACK][3] << std::endl;
    os << matrix[LEFT][6] << " " << matrix[LEFT][5] << " " << matrix[LEFT][4] << " ";
    os << matrix[FRONT][6] << " " << matrix[FRONT][5] << " " << matrix[FRONT][4] << " ";
    os << matrix[RIGHT][6] << " " << matrix[RIGHT][5] << " " << matrix[RIGHT][4] << " ";
    os << matrix[BACK][6] << " " << matrix[BACK][5] << " " << matrix[BACK][4] << std::endl;
    os << "      " << matrix[BOTTOM][0] << " " << matrix[BOTTOM][1] << " " << matrix[BOTTOM][2] << std::endl;
    os << "      " << matrix[BOTTOM][7] << " " << matrix[BOTTOM][8] << " " << matrix[BOTTOM][3] << std::endl;
    os << "      " << matrix[BOTTOM][6] << " " << matrix[BOTTOM][5] << " " << matrix[BOTTOM][4] << std::endl;
    return os;
}

Matrix<20, 24, bool> RubiksCube::toMatrix() const
{
    Matrix<20, 24, bool> matrix;
    return std::move(matrix);
}
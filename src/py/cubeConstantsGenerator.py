

# Face constants
FRONT = 0
BACK = 1
LEFT = 2
RIGHT = 3
TOP = 4
BOTTOM = 5

# Color constants
RED = 0
ORANGE = 1
YELLOW = 2
GREEN = 3
BLUE = 4
WHITE = 5

def share_edge(face1, face2):
    # Returns true if the two faces share an edge
    if face1 == face2:
        return False
    if face1 == FRONT and face2 == BACK:
        return False
    if face1 == BACK and face2 == FRONT:
        return False
    if face1 == LEFT and face2 == RIGHT:
        return False
    if face1 == RIGHT and face2 == LEFT:
        return False
    if face1 == TOP and face2 == BOTTOM:
        return False
    if face1 == BOTTOM and face2 == TOP:
        return False
    return True

cublets2 = []
cublets3 = []
for color1 in range(6):
    for color2 in range(color1 + 1, 6):
        if share_edge(color1, color2):
            cublets2.append([color1, color2])
            for color3 in range(color2 + 1, 6):
                if share_edge(color2, color3) and share_edge(color1, color3):
                    cublets3.append([color1, color2, color3])



def cublet3Id(color1, color2, color3):
    [color1, color2, color3] = sorted([color1, color2, color3])
    for i in range(len(cublets3)):
        if cublets3[i][0] == color1 and cublets3[i][1] == color2 and cublets3[i][2] == color3:
            return i
    return -1

def printCublets3Ids():
    print("constexpr int Cublets3Ids = {", end='')
    for color1 in range(6):
        print("{", end="")
        for color2 in range(6):
            print("{", end="")
            for color3 in range(6):
                print(cublet3Id(color1, color2, color3), end="")
                if color3 < 5:
                    print(",", end="")
            print("}", end="")
            if color2 < 5:
                print(",", end="")
        print("}")
        if color1 < 5:
            print(",", end="")
    print("};")


def cublet2Id(color1, color2):
    [color1, color2] = sorted([color1, color2])
    for i in range(len(cublets2)):
        if cublets2[i][0] == color1 and cublets2[i][1] == color2:
            return i
    return -1

def printCublets2Ids():
    print("constexpr int Cublets2Ids = {", end='')
    for color1 in range(6):
        print("{", end="")
        for color2 in range(6):
            print(cublet2Id(color1, color2), end="")
            if color2 < 5:
                print(",", end="")
        print("}", end="")
        if color1 < 5:
            print(",", end="")
    print("};")

faces3 = []
faces2 = []
for face1 in range(6):
    for face2 in range(6):
        if share_edge(face1, face2):
            faces2.append([face1, face2])
            for face3 in range(face2 + 1, 6):
                if share_edge(face2, face3) and share_edge(face1, face3):
                    faces3.append([face1, face2, face3])

def face3Id(face1, face2, face3):
    if face2 > face3:
        [face2, face3] = [face3, face2]
    for i in range(len(faces3)):
        if faces3[i][0] == face1 and faces3[i][1] == face2 and faces3[i][2] == face3:
            return i
    return -1

def printFaces3Ids():
    print("constexpr int Faces3Ids = {", end='')
    for face1 in range(6):
        print("{", end="")
        for face2 in range(6):
            print("{", end="")
            for face3 in range(6):
                print(face3Id(face1, face2, face3), end="")
                if face3 < 5:
                    print(",", end="")
            print("}", end="")
            if face2 < 5:
                print(",", end="")
        print("}")
        if face1 < 5:
            print(",", end="")
    print("};")

def face2Id(face1, face2):
    for i in range(len(faces2)):
        if faces2[i][0] == face1 and faces2[i][1] == face2:
            return i
    return -1

def printFaces2Ids():
    print("constexpr int Faces2Ids = {", end='')
    for face1 in range(6):
        print("{", end="")
        for face2 in range(6):
            print(face2Id(face1, face2), end="")
            if face2 < 5:
                print(",", end="")
        print("}", end="")
        if face1 < 5:
            print(",", end="")
    print("};")

print()
printCublets3Ids()
print()
printCublets2Ids()
print()
printFaces3Ids()
print()
printFaces2Ids()
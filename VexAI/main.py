from enum import Enum
import numpy as np


class Colour(Enum):
    RED = 1
    BlUE = 2

def get_ttt(match, square_size):
    red_score = 0
    blue_score = 0
    #check verticals
    for x in range(square_size):
        values = []
        for y in range(square_size):
            if any(match[x][y]):
                print(match[x][y])
                index = np.max(np.nonzero(match[x][y]))
                values.append(match[x][y][index])
            else:
                values.append(None)
        if len(set(values)) == 1:
            if values[0] == Colour.RED:
                red_score += 1
            elif  values[0] == Colour.BlUE:
                blue_score += 1
     #check Horisontals
    for y in range(square_size):
        values = []
        for x in range(square_size):
            if any(match[x][y]):
                index = np.max(np.nonzero(match[x][y]))
                values.append(match[x][y][index])
            else:
                values.append(None)
        if len(set(values)) == 1:
            if values[0] == Colour.RED:
                red_score += 1
            elif values[0] == Colour.BlUE:
                blue_score += 1
    #check diangonals
    values1 = []
    values2 = []
    for i in range(square_size):
        if any(match[i][i]):
            index1 = np.max(np.nonzero(match[i][i]))
            values1.append(match[i][i][index1])
        else:
            values1.append(None)
        if any(match[square_size-i-1][i]):
            index2 = np.max(np.nonzero(match[square_size-i-1][i]))
            values2.append(match[square_size-i-1][i][index2])
        else:
            values2.append(None)
    if len(set(values1)) == 1:
        if values1[0] == Colour.RED:
            red_score += 1
        elif values1[0] == Colour.BlUE:
            blue_score += 1

    if len(set(values2)) == 1:
        if values2[0] == Colour.RED:
            red_score += 1
        elif values2[0] == Colour.BlUE:
            blue_score += 1
    #retrun values
    return (blue_score * 6, red_score * 6)
        


def score_match(match, matchsize):
    blue_score = 0
    red_score = 0 

    for x in match:
        for y in x:
            for z in y:
                if z == Colour.RED:
                    red_score += 1
                if z == Colour.BlUE:
                    blue_score += 1
    
    blue_score_top, red_score_top = get_ttt(match, matchsize)
    blue_score += blue_score_top
    red_score += red_score_top
    return (blue_score, red_score)

def add_ball(team, x, y, ballsleft, match):
    match[x][y]

if __name__ == '__main__':
    matchtest = [[[None for x in range(3)] for y in range(3)] for x in range(3)]
    matchresult = score_match(matchtest, 3)
    print(matchresult)
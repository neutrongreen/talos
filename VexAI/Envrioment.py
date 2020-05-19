from enum import Enum
import numpy as np


class Colour(Enum):
    RED = 1
    BlUE = 2

class Match():
    def __init__(self, intial_balls = 16, match_size = 3):
        #create match array and set all to zero
        self.self.match_size = match_size
        self.env = [[[None for z in range(3)] for y in range(self.match_size)] for x in range(self.match_size)]
        #ball amout dictonary
        self.inital_balls = intial_balls
        self.balls = {Colour.BlUE: intial_balls, Colour.RED: intial_balls}
    #get tick tack toe of field
    def _get_ttt(self):
        red_score = 0
        blue_score = 0
        #check verticals
        for x in range(self.match_size):
            values = []
            for y in range(self.match_size):
                if any(self.env[x][y]):
                    print(self.env[x][y])
                    index = np.max(np.nonzero(self.env[x][y]))
                    values.append(self.env[x][y][index])
                else:
                    values.append(None)
            if len(set(values)) == 1:
                if values[0] == Colour.RED:
                    red_score += 1
                elif  values[0] == Colour.BlUE:
                    blue_score += 1
        #check Horisontals
        for y in range(self.match_size):
            values = []
            for x in range(self.match_size):
                if any(self.env[x][y]):
                    index = np.max(np.nonzero(self.env[x][y]))
                    values.append(self.env[x][y][index])
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
        for i in range(self.match_size):
            if any(self.env[i][i]):
                index1 = np.max(np.nonzero(self.env[i][i]))
                values1.append(self.env[i][i][index1])
            else:
                values1.append(None)
            if any(self.env[3-i-1][i]):
                index2 = np.max(np.nonzero(self.env[3-i-1][i]))
                values2.append(self.env[3-i-1][i][index2])
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
            


    def score_match(self):
        blue_score = 0
        red_score = 0 
        #count up balls
        for x in self.env:
            for y in x:
                for z in y:
                    if z == Colour.RED:
                        red_score += 1
                    if z == Colour.BlUE:
                        blue_score += 1
        #get ttt score
        blue_score_top, red_score_top = self._get_ttt(self.env, 3)
        blue_score += blue_score_top
        red_score += red_score_top
        #return resulting score
        return (blue_score, red_score)

    #add ball
    def add_ball(self, team, x, y):
        if self.balls[team] > 0:
            if not all(self.env[x][y]):
                env[x][y][np.count_nonzero(env[x][y])] = team
                self.balls[team] -= 1
    #remove ball and add back to total balls left   
    def remove_ball(self, x, y):
        if any(self.env[x][y]):
            ball = self.env[x][y].pop(0)
            self.env[x][y].append(None)
            self.balls[ball] += 1



    

if __name__ == '__main__':
    self.envtest = [[[None for x in range(3)] for y in range(3)] for x in range(3)]
    self.envresult = score_self.env(self.envtest, 3)
    print(self.envresult)
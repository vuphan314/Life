#!/usr/bin/python3

from typing import List, Set, Tuple

Cell = Tuple[int] # pair
GameState = List[List[bool]]

class Life:
    lower_survival = 2
    upper_survival = 3
    lower_birth = 3
    upper_birth = 3

    def __init__(self, edge=5):
        self.edge = edge

    def get_next_game_state(self, X: GameState) -> GameState:
        Y = []
        for i in range(self.edge):
            row = []
            for j in range(self.edge):
                C = i, j
                row.append(self.get_next_cell_state(X, C))
            Y.append(row)
        return Y

    def get_next_cell_state(self, X: GameState, C: Cell) -> bool:
        num_alive_neighbors = 0
        for neighbor in self.get_neighbors(C):
            if self.get_cur_cell_state(X, neighbor):
                num_alive_neighbors +=1
        cur_cell_state = self.get_cur_cell_state(X, C)
        if cur_cell_state:
            if is_within(
                num_alive_neighbors,
                self.lower_survival, self.upper_survival
            ):
                return True
            else:
                return False
        else:
            if is_within(
                num_alive_neighbors,
                self.lower_birth, self.upper_birth
            ):
                return True
            else:
                return False

    def get_cur_cell_state(self, X: GameState, C: Cell) -> bool:
        return X[C[0]][C[1]]

    def get_neighbors(self, C: Cell) -> Set[Cell]:
        D = {-1, 0, 1}
        rows = {self.wrap(C[0] + d) for d in D}
        cols = {self.wrap(C[1] + d) for d in D}
        cells = {(row, col) for row in rows for col in cols}
        cells.remove(C)
        return cells

    def wrap(self, val: int) -> int:
        return val % self.edge

def is_within(m: int, l: int, r: int):
    return l <= m and m <= r

l = Life()
n = 5
X = [[1 for i in range(n)] for j in range(n)]
p = l.get_next_game_state(X)
print(X, p)


def get_image_size(n: int) -> int:
    pass

def is_in_image(n: int, Y: GameState) -> bool:
    pass

def get_fiber(n: int, Y: GameState) -> Set[GameState]:
    pass

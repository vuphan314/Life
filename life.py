#!/usr/bin/python3

from itertools import product
from typing import List, Set, Tuple

GRID_EDGE = 2
SEQ_TYPES = {list, tuple} # itertools

Cell = Tuple[int] # pair
CellColl = Set[Cell]
CellState = bool
Row = List[CellState]
Grid = List[Row]
GridColl = Set[Grid]

class Life:
    lower_survival = 2
    upper_survival = 3
    lower_birth = 3
    upper_birth = 3

    def __init__(self, grid_edge=GRID_EDGE):
        self.grid_edge = grid_edge

    def get_image_size() -> int:
        pass

    def is_in_image(next_grid: Grid) -> bool:
        pass

    def get_fiber(next_grid: Grid) -> Set[Grid]:
        pass

    def get_image(self) -> GridColl:
        for grid in self.get_all_poss_grids():


    def get_all_poss_grids(self) -> GridColl:
        cell_states_sets = [{True, False}] * self.grid_edge
        all_poss_rows = set(product(*cell_states_sets))
        rows_sets = [all_poss_rows] * self.grid_edge
        return set(product(*rows_sets))

    def get_next_grid(self, grid: Grid) -> Grid:
        next_grid = []
        for i in range(self.grid_edge):
            row = []
            for j in range(self.grid_edge):
                cell = i, j
                row.append(self.get_next_cell_state(
                    grid, cell
                ))
            next_grid.append(row)
        return next_grid

    def get_next_cell_state(
        self, grid: Grid, cell: Cell
    ) -> CellState:
        num_alive_neighbors = 0
        for neighbor in self.get_neighbors(cell):
            if self.get_cell_state(grid, neighbor):
                num_alive_neighbors +=1
        cell_state = self.get_cell_state(grid, cell)
        if cell_state:
            return is_within(
                num_alive_neighbors,
                self.lower_survival, self.upper_survival
            )
        else:
            return is_within(
                num_alive_neighbors,
                self.lower_birth, self.upper_birth
            )

    def get_cell_state(
        self, grid: Grid, cell: Cell
    ) -> CellState:
        return grid[cell[0]][cell[1]]

    def get_neighbors(self, cell: Cell) -> CellColl:
        D = {-1, 0, 1}
        row_inds = {self.wrap(cell[0] + d) for d in D}
        col_inds = {self.wrap(cell[1] + d) for d in D}
        cells = {(row, col) for row in row_inds for col in col_inds}
        cells.remove(cell)
        return cells

    def wrap(self, val: int) -> int:
        return val % self.grid_edge

def is_within(m: int, l: int, r: int):
    return l <= m and m <= r

def printt(o: object) -> None: # pretty-print
    if type(o) in SEQ_TYPES and o and type(o[0]) in SEQ_TYPES:
    # isinstance(o, Grid)
        print('Grid: [')
        for row in o:
            print(row)
        print(']')
    elif isinstance(o, GridColl):
        print('GridColl: {')
        for grid in o:
            printt(grid)
        print('}')
    else:
        print(o)

l = Life()
p = l.get_all_poss_grids()
printt(p)

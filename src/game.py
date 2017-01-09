#!/usr/bin/python3

from itertools import product
from typing import Set, Tuple

GRID_EDGE = 2
ITER_TYPES = {set, tuple}

Cell = Tuple[int] # len: 2
CellColl = Set[Cell]
CellState = bool
Row = Tuple[CellState] # len: grid_edge
Grid = Tuple[Row] # len: grid_edge
GridColl = Set[Grid]

class Game:
    def __init__(self, grid_edge=GRID_EDGE) -> None:
        self.grid_edge = grid_edge
        self.lower_survival = 2
        self.upper_survival = 3
        self.lower_birth = 3
        self.upper_birth = 3

    ########################################################

    def get_image_size(self) -> int:
        """Research question 1 of 3."""
        return len(self.get_image())

    def get_image_prop(self) -> float:
        return self.get_image_size() / self.get_domain_size()

    def is_in_image(self, grid: Grid) -> bool:
        """Research question 2 of 3."""
        return grid in self.get_image()

    def get_a_prev_grid(self, grid: Grid) -> Grid:
        """Research question 3 of 3.

            May return None.
        """
        for poss_prev_grid in self.get_domain():
            if self.get_next_grid(poss_prev_grid) == grid:
                return poss_prev_grid

    # \Theta{(2 ^{n^2} * n^2)}
    def get_all_prev_grids(self, grid: Grid) -> GridColl:
        """Research question 3 of 3."""
        prev_grids = set()
        for poss_prev_grid in self.get_domain():
            if self.get_next_grid(poss_prev_grid) == grid:
                prev_grids.add(poss_prev_grid)
        return prev_grids

    ########################################################

    def get_image(self) -> GridColl:
        image = set()
        for grid in self.get_domain():
            next_grid = self.get_next_grid(grid)
            image.add(self.get_next_grid(grid))
        return image

    def get_domain_size(self) -> int:
        return 2 ** self.grid_edge**2

    def get_domain(self) -> GridColl:
        cell_states_sets = [{True, False}] * self.grid_edge
        all_poss_rows = set(product(*cell_states_sets))
        rows_sets = [all_poss_rows] * self.grid_edge
        return set(product(*rows_sets))

    # \Theta{(n^2)}
    def get_next_grid(self, grid: Grid) -> Grid:
        next_grid = ()
        for i in range(self.grid_edge):
            row = ()
            for j in range(self.grid_edge):
                cell = i, j
                row += self.get_next_cell_state(grid, cell),
            next_grid += row,
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

    def get_cell_state(self, grid: Grid, cell: Cell) -> CellState:
        return grid[cell[0]][cell[1]]

    def get_neighbors(self, cell: Cell) -> CellColl:
        D = {-1, 0, 1}
        row_inds = {self.get_wrapped_val(cell[0] + d) for d in D}
        col_inds = {self.get_wrapped_val(cell[1] + d) for d in D}
        cells = {(row, col) for row in row_inds for col in col_inds}
        cells.remove(cell)
        return cells

    def get_wrapped_val(self, val: int) -> int:
        return val % self.grid_edge

def is_within(m: int, l: int, r: int) -> bool:
    return l <= m and m <= r

def print_game(o: object) -> None:
    depth = get_depth(o)
    if depth == 2: # Grid
        print('Grid: (')
        for row in o:
            print(row)
        print(')')
    elif depth == 3: # GridColl
        print('GridColl: {')
        for grid in o:
            print_game(grid)
        print('}')
    else:
        print(o)

def get_depth(o: object) -> int:
    if type(o) in ITER_TYPES:
        depth = 1
        if o:
            if isinstance(o, set):
                el = o.pop()
            else:
                el = o[0]
            depth += get_depth(el)
        return depth
    else:
        return 0

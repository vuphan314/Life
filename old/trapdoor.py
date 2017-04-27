#!/usr/bin/python3

import game

def get_trapdoor_speedup(grid_edge: int) -> float:
    return (
        get_time_no_trapdoor(grid_edge) /
        get_time_trapdoor(grid_edge)
    )

def get_time_no_trapdoor(grid_edge: int) -> int:
    time = 0
    for poss_grid_edge in range(
        game.GRID_EDGE_MIN, grid_edge + 1
    ):
        time +=get_time_trapdoor(poss_grid_edge)
    return time

def get_time_trapdoor(grid_edge: int) -> int:
    return grid_edge**2 * 2**(grid_edge**2)

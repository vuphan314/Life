#!/usr/bin/python3

import time

from game import Game

GRID_EDGE_MIN = 0
GRID_EDGE_MAX = 4

def main():
    print_align('n', 'alpha', 'time (secs)')
    for grid_edge in range(GRID_EDGE_MIN, GRID_EDGE_MAX + 1):
        time_start = time.time()

        g = Game(grid_edge)
        prop = g.get_image_prop()

        time_taken = time.time() - time_start
        print_align(grid_edge, prop, time_taken)

def print_align(*args) -> None:
    field_width = 8
    aligned_args = []
    for arg in args:
        if isinstance(arg, str):
            arg = arg.ljust(field_width)
        elif isinstance(arg, int):
            arg = str(arg).rjust(field_width)
        elif isinstance(arg, float):
            arg = '{:8.4f}'.format(arg)
        aligned_args.append(arg)
    print(*aligned_args, sep='|')

if __name__ == '__main__':
    main()

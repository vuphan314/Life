#!/usr/bin/python3

import time

from game import Game

# inclusive:
GRID_EDGE_MIN = 0
GRID_EDGE_MAX = 4 # 5 crashed Vu's PC :(

def get_log_str() -> str:
    strs = [get_joined_str('n', 'alpha', 'time (secs)')]
    for grid_edge in range(GRID_EDGE_MIN, GRID_EDGE_MAX + 1):
        time_start = time.time()
        g = Game(grid_edge)
        prop = g.get_image_prop()
        time_taken = time.time() - time_start
        strs.append(get_joined_str(grid_edge, prop, time_taken))
    return '\n'.join(strs)

def get_joined_str(*args) -> str:
    field_width = 8
    strs = []
    for arg in args:
        if isinstance(arg, float):
            arg = '{:8.4f}'.format(arg)
        else:
            arg = str(arg)
            if isinstance(arg, int):
                arg = arg.rjust(field_width)
            else:
                arg = arg.ljust(field_width)
        strs.append(arg)
    return '|'.join(strs)

if __name__ == '__main__':
    str = get_log_str()
    print(str)

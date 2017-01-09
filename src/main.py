#!/usr/bin/python3

import time

from game import Game

# inclusive:
GRID_EDGE_MIN = 1
GRID_EDGE_MAX = 4 # 5 crashed Evgenii's pc

def get_log_str() -> str:
    sts = [get_aligned_str('n', 'alpha', 'time (secs)')]
    for grid_edge in range(GRID_EDGE_MIN, GRID_EDGE_MAX + 1):
        time_start = time.time()
        g = Game(grid_edge)
        prop = g.get_image_prop()
        time_taken = time.time() - time_start
        sts.append(get_aligned_str(grid_edge, prop, time_taken))
    return '\n'.join(sts)

def get_aligned_str(*args) -> str:
    field_width = 8
    sts = []
    for arg in args:
        if isinstance(arg, float):
            arg = '{:8.4f}'.format(arg)
        else:
            arg = str(arg)
            if isinstance(arg, int):
                arg = arg.rjust(field_width)
            else:
                arg = arg.ljust(field_width)
        sts.append(arg)
    return '|'.join(sts)

if __name__ == '__main__':
    log_path = 'log.txt'
    with open(log_path, 'a') as log_file:
        log_file.write(get_log_str() + '\n\n')
    print('Appended to `{}`.'.format(log_path))

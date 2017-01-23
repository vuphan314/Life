#!/usr/bin/python3

import time

import game
import trapdoor

GRID_EDGE_MAX_SPEEDUP = 200
GRID_EDGE_MAX_PROP = 5 # 5 crashes Vu-PC

def get_speedup_str() -> str:
    sts = [get_aligned_str('grid edge', 'speedup')]
    for grid_edge in range(game.GRID_EDGE_MIN, GRID_EDGE_MAX_SPEEDUP + 1):
        speedup = trapdoor.get_trapdoor_speedup(grid_edge)
        sts.append(get_aligned_str(grid_edge, speedup))
    return '\n'.join(sts)

def get_prop_str() -> str:
    sts = [get_aligned_str('grid edge', 'proportion', 'time (secs)')]
    for grid_edge in range(game.GRID_EDGE_MIN, GRID_EDGE_MAX_PROP + 1):
        time_start = time.time()
        g = game.Game(grid_edge)
        prop = g.get_image_prop()
        time_taken = time.time() - time_start
        sts.append(get_aligned_str(grid_edge, prop, time_taken))
    return '\n'.join(sts)

def get_aligned_str(*args) -> str:
    field_width = 15
    sts = []
    for arg in args:
        if isinstance(arg, float):
            arg = '{:15.10f}'.format(arg)
        else:
            arg = str(arg)
            if isinstance(arg, int):
                arg = arg.rjust(field_width)
            else:
                arg = arg.ljust(field_width)
        sts.append(arg)
    return '|'.join(sts)

############################################################

def main(st: str) -> None:
    log_path = 'log.txt'
    with open(log_path, 'a') as log_file:
        log_file.write(st + '\n\n')
    print('Appended to `{}`.'.format(log_path))

if __name__ == '__main__':
    # st = get_speedup_str()
    st = get_prop_str()
    main(st)

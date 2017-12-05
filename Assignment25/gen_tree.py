#!/usr/bin/python3

import random
import argparse
import sys

def gen_tree(size, fd):
    vertex_labels = list(range(size))
    random.shuffle(vertex_labels)

    print(size, file=fd)
    edges = []
    for i in range(1, size):
        a = vertex_labels[ i ]
        b = vertex_labels[ random.randint(0, i-1) ]
        e = sorted((a,b))
        edges.append("%d %d"%(e[0], e[1]))
    random.shuffle(edges)
    for e in edges:
        print(e, file=fd)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('size', nargs='?', metavar='size', type=int, default=10)
    args = parser.parse_args()
    gen_tree(args.size, sys.stdout)
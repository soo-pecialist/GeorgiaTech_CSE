from __future__ import print_function
import numpy
import time
import sys

min=0.0
max=100.0
seed=1
def rand_result(n):
    numpy.random.seed(seed)
    b = min + (max-min)*numpy.random.random(n)
    return numpy.around(b,4)


def write_rand_problem(n, name="score"):
    b = rand_result(n)
    fname = name + "_"+str(n)+".txt"
    thefile = open(fname, 'w')
    #thefile.write("%s\n" % n)
    for item in b:
		thefile.write("%s\n" % item)

def main():
    if len(sys.argv) < 2:
        print("Usage: python generate_input.py <n>")
        return
    n = int(sys.argv[1])
    write_rand_problem(n)


if __name__ == '__main__':
    main()

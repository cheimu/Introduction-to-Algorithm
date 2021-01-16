
# seed the pseudorandom number generator
import sys
from random import seed
import random as rm
# seed random number generator

# generate some random numbers
def gen_s(f, x, d):
  for i in range(x):
    f.write(str(round(rm.uniform(-100, 100), d)))
    f.write(" ")
    f.write(str(round(rm.uniform(-100, 100), d)))
    f.write('\n')


def main():
  f = open(sys.argv[1], "w")
  gen_s(f, int(sys.argv[2]), int(sys.argv[3]))

if __name__ == '__main__':
  main()

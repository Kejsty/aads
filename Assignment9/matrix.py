#!/usr/bin/python3

import argparse
import numpy
import timeit

class SlowImpl():
    """Implementation of a naive solution for assignment 9.

        Given an array A of size n and a square matrix B of size n*x,
        for all i<j fill B[i,j] with sum(A[i], A[i+1], .., A[j]).
        for i>=j, the value of B[i,j] is undefined.
        """

    def __init__(self, size, input_matrix=None):
        """set up array and matrix of size <size> and <size>x<size> respectively.
           if an input array is given in input_matrix, use that, otherwise
           create one filled with random integer values.
           """
        self.size = size
        if input_matrix is None:
            self.input_matrix = numpy.random.randint(100, size=size)
        else:
            if not input_matrix.ndim == 1: raise ValueError("Input array has wrong number of dimensions")
            if not size == input_matrix.shape[0]: raise ValueError("Input array size mismatch")
            self.input_matrix = input_matrix

        self.output_matrix = numpy.empty((size, size), dtype=numpy.int)

    def run(self):
        """Fill in the matrix B as required."""
        for i in range(0, self.size):
            for j in range(i+1, self.size):
                self.output_matrix[i, j] = sum(self.input_matrix[x] for x in range(i, j+1))

# Basic improvement to O(n^2)
class BasicSaneImpl(SlowImpl):
    """A hopefully smarter implementation of the problem from assignment 9"""
    def __init__(self, size, input_matrix=None):
        super().__init__(size, input_matrix)

    def run(self):
        for i in range(0, self.size):
            self.output_matrix[i, i] = self.input_matrix[i]
            for j in range(i+1, self.size):
                self.output_matrix[i,j] = self.output_matrix[i,j-1] + self.input_matrix[j]


# Improvement to O(n^2) with resoect to the cache
class SaneImpl(SlowImpl):
    """A hopefully smarter implementation of the problem from assignment 9"""
    def __init__(self, size, input_matrix=None):
        super().__init__(size, input_matrix)

    def run(self):
        self.output_matrix[0, 0]  = self.input_matrix[0]
        for i in range(1 , self.size):
            self.output_matrix[0, i]  = self.input_matrix[i] +  self.output_matrix[0, i - 1];

        for i in range(1, self.size):
            val = self.input_matrix[i - 1]
            for j in range(i, self.size):
                self.output_matrix[i, j] = self.output_matrix[i - 1, j] - val;



def compare_relevant(m1, m2):
    """Given two square matrixes of the same size, determine if
       that m1[i,j] == m[i,j] for all i>j.
       """
    if not m1.ndim == 2: raise ValueError("m1 has wrong number of dimensions")
    if not m2.ndim == 2: raise ValueError("m2 has wrong number of dimensions")
    if not m1.shape[0] == m1.shape[1]: raise ValueError("m1 is not square")
    if not m1.shape == m2.shape: raise ValueError("m1 is not same size as m2")

    size = m1.shape[0]
    for i in range(0, size):
        for j in range(i+1, size):
            if m1[i,j] != m2[i,j]: return False
    return True


if __name__ == "__main__":
    # The default set of sizes of the array/matrix.  This
    # can be overridden from the command line.  This default
    # set just gives a quick glimpse into the runtime behaviour.
    #
    # For actually plotting the run time, you will want many many
    # more samples from a wider range.
    default_sizes = [10 * 2**x for x in range(0,7)]

    parser = argparse.ArgumentParser()
    parser.add_argument('--impl', metavar='IMPL', choices=['naive', 'improved'],
      help='Pick implementation to run ("native" or "improved")')
    parser.add_argument('size', metavar='INPUTSIZE', type=int, nargs='*', default = default_sizes,
      help='List of input sizes')
    args = parser.parse_args()

    for size in args.size:
        if args.impl is not None:
            if args.impl == 'naive':
                i = SlowImpl(size)
            else:
                i = SaneImpl(size)

            t = timeit.Timer(i.run).timeit(number=1)
            print(size, ", ", t, sep='')
        else:
            i1 = SlowImpl(size)
            i2 = SaneImpl(size, input_matrix = i1.input_matrix)
            #i2 = BasicSaneImpl(size, input_matrix = i1.input_matrix) 

            t1 = timeit.Timer(i1.run).timeit(number=10)
            t2 = timeit.Timer(i2.run).timeit(number=10)
            assert(compare_relevant(i1.output_matrix, i2.output_matrix))
            print(size, ", ", t1, ", ", t2, sep='')

# Copyright (c) 2017 Peter Palfrader <peter@palfrader.org>
#
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
# LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
# OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
# WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
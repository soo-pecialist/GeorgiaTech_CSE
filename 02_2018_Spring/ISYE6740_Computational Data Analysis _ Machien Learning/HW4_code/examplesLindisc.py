"""
examples_lindisc.py
Jason Corso (jcorso@acm.org)

Example uses of the lindisc.py module in prpy.

Note that all of the training procedures in the lindisc.py module assume the 
class labels are -1 and 1 and not 0 and 1.

"""

import prpy as pr

import numpy as np
import scipy as sp
import scipy.io

# An example simple data set
X1 = pr.datatools.genData_gaussian([-5,0],[[1,0],[0,3]],20)
X2 = pr.datatools.genData_gaussian([5,0],[[1.4,0.71],[-0.71,1.4]],20)
Y1 = -1*np.ones(20,dtype=np.int16)
Y2 = np.ones(20,dtype=np.int16)
X = np.vstack((X1,X2))
Y = np.hstack((Y1,Y2))
pr.datatools.plotData(X,Y)

# Learn a linear discriminant with the batch perceptron
a = pr.lindisc.batchPerceptron(X,Y,np.random.rand(2),0.1)

# Single Sample Perceptron in a simple data set
Z = np.load('../data/2class_lindisc1.npz')
X = Z['X']
Y = Z['Y']
a = pr.lindisc.ssPerceptron(X,Y,np.random.rand(2),0.1)

# And some relaxation with a margin (of 0.1)
a = pr.lindisc.ssRelaxation(X,Y,np.random.rand(2),0.1,0.01)

# A least-squares soln
a = pr.lindisc.mse(X,Y,0.01)

# The Least-Squares can handle inseparable data too
Z = np.load('../data/2class_3.npz')
X = Z['X']
Y = Z['Y']
Y[Y==0] = -1  # align for the two-class setup
a = pr.lindisc.mse(X,Y,0.01)


# Create a linear discriminant object and classify a grid with it.
L = pr.lindisc.LinDisc(a)
pr.datatools.testGrid(L)



"""
examples_trees.py
Jason Corso (jcorso@acm.org)

Example uses of the trees.py module in prpy.

"""

import prpy as pr

import numpy as np
import scipy as sp
import scipy.io


# Using the axis-aligned tree learning procedure
Z = np.load('../data/2class_1.npz')
X = Z['X']; Y = Z['Y']
tree = pr.trees.trainDTree_AxisAligned(X,Y,pr.trees.impurity_entropy)

# Classify some points
print tree.classify([-10,-10])
print tree.classify([-10,10])
print tree.classify([10,-10])
print tree.classify([10,10])

# Show the decision regions
pr.datatools.testGrid(tree)

# compute the accuracy of the classifier on the data set
pr.datatools.accuracy(X,Y,tree)

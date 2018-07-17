"""
CSE 455/555 Homework 1 Random Forest on MNIST Digits Files
Jason Corso (jcorso@acm.org)

***This file is a hand-out for a homework assignment and leaves numerous places unfinished, intentionally.***

You are to use the prpy and examples code already provided as a base for this
functionality.

Make sure they are in your python path before you execute this file.

Refer to the assignment description for full details.  There are XXX places in this file
in which you are required to make changes and add code.  They are listed below.

A. Line 52 -- Complete the classification routine for the RandomForest.
B. Line 60 -- Complete the aggregation of probabilities over the RandomForest for a sample.
C. Line 170 -- Complete the randomized tree learning procedure.

Once the changes are complete, you should be able to just run the file (from with ipython,
you type run rfdigits, or on the command line just type python rfdigits).
And this should perform the full training and the classification.  (This is what we'll do as well...)

"""

import os
import PIL

import numpy as np
import scipy as sp
import scipy.io as sio

import matplotlib.pyplot as plt

import prpy as pr
from prpy.trees import *


# you need to change this to point to the location of the MNIST Data on your system
# On the CSE machines, it is at /projects/jcorso/CSE555/555_mnist_data/
kMNIST_PATH = './555_mnist_data/'


class RandomForest(object):
    ''' The RandomForest class that stores and aggregates a bunch of decision trees. '''

    def __init__(self, maxClassIndex):
        self.trees = []
        self.maxClassIndex = maxClassIndex

    def addTree(self, tree):
        ''' Add a DTree to the forest. '''
        self.trees.append(tree)

    def classify(self, sample):
        ''' Output a classification of the sample over the entire forest. '''

        ###################################
        ###  A.  Fill In Here (remove the pass statement above)
        ###################################

        p = self.probability(sample)
        return np.argmax(p)

    def probability(self, sample):
        ''' Output a probability density of the sample using the forest. '''

        ###################################
        ###  B.  Fill In Here (remove the pass statement above)
        ###################################

        ps = []
        for tree in self.trees:
            p = tree.probability(sample) # (NumClasses,)
            ps.append(p)
        ps = np.array(ps) # (NumTrees x NumClasses)
        return ps.mean(0) # (NumClasses,)


class DTQuery_KSum(pr.trees.DTQuery):
    '''
    A decision tree query class that applies a KSum feature and then thresholds it.

    You can use this Query class to store the queries at the decision tree nodes for
    the digits problem.

    '''

    def __init__(self, ksum, tau):
        self.ksum = ksum
        self.tau = tau

    def query(self, sample):
        ''' Apply the query. '''
        v = self.ksum.evaluate(sample)
        if v < self.tau:
            return 0
        return 1


class KSum_Feature(object):
    '''
    This is a simple feature for computing a response as a sum/difference of feature
    values.

    For the digit problem, it is suggested to set the weights to be either 1 or -1.
    '''

    def __init__(self, weights, indices):
        ''' Construct the KSum_Features on indices weighted by weights. '''
        assert(len(weights)==len(indices))
        self.weights = weights
        self.indices = indices

    def evaluate(self, sample):
        ''' Evaluate the KSum_Feature on the row-vector sample. '''
        return np.sum(sample[self.indices].astype(np.float32) * self.weights)

    def display(self,image):
        ''' For debugging, display the pixels being weighted in a KSum feature on image. '''

        plt.imshow(image)
        plt.gray()

        for i in range(len(self.indices)):
            (x,y) = np.unravel_index(self.indices[i], image.shape)
            if (self.weights[i] < 0):
                plt.plot(x,y,'ro')
            else:
                plt.plot(x,y,'b+')

        plt.axis(xmin=0, ymax=0, xmax=image.shape[1]-1, ymin=image.shape[0]-1)


def displayDigit(Xrow):
    ''' Display a digit by first reshaping it from the row-vector into the image.  '''
    plt.imshow(np.reshape(Xrow,(28,28)))
    plt.gray()


def loadData(prefix='train',num=1000):
    '''
    Loads all of the images into a data-array (for digits 0 through 5).

    The prefix is 'train' by default and can be set to 'test' to load
    the testing data into memory.

    The training data has 5000 images per digit and the testing data has 200,
    but loading that many images from the disk may take a while.  So, you can
    just use a subset of them, say 200 for training (otherwise it will take a
    long time to complete.

    Note that each image as a 28x28 grayscale image, loaded as an array and
    then reshaped into a single row-vector.

    Use the function displayDigit(row-vector) to visualize an image.

    '''

    print 'About to load the data in. This may take a while...'
    X = np.zeros((num*6,784),dtype=np.uint8)   #784=28*28
    Y = np.zeros(num*6,dtype=np.uint16)
    for d in xrange(6):
        print 'Reading digit %d' % d
        for i in range(num):
            pth = os.path.join(kMNIST_PATH,'%s%d'%(prefix,d),'%05d.pgm'%i)
            X[d*num+i,:] = np.asarray(PIL.Image.open(pth)).reshape(1,784);
            Y[d*num+i] = d

    print ''

    return X,Y


def trainDTree_randomized_prepRange(X, ksum):

    values = []
    for i in range(X.shape[0]):
        value = ksum.evaluate(X[i, :])
        values.append(value)

    dmin = np.min(values)
    dmax = np.max(values)

    dstep = np.abs((dmax - dmin) / np.double(pr.trees.kDT_LenRanges))

    if dstep < 1e-5:
        return np.zeros(pr.trees.kDT_LenRanges)

    return np.arange(dmin, dmax, dstep)


def trainDTree_randomized(X, Y, impurity, numClasses, numFeatures=100, depth=0):
    '''
    Randomized algorithm for training a decision tree using data set (X,Y)
    using combinations of pixel value features, randomly selected each training round.

    Convention on (X, Y) is that each row is a sample in the data set, X has the
    values of the data and Y has the class label.

    impurity is a function that takes X, Y and returns the impurity of the data set

    '''

    ###################################
    ###  C.  Fill In Here (remove the pass statement above)
    ###      This is substantially harder than A and B above.  You can use the
    ###      trainDTree_AxisAligned function as a start from pr.trees, but this version
    ###      needs to randomly select features when determining the query for each node.
    ###################################

    # calculate impurity at current node
    I = impurity(Y)

    # leaf node
    if I == 0 or depth == pr.trees.kDT_MaxDepth:
        leaf = DTLeafNode(numClasses)
        leaf.computeDensity(X, Y)
        return leaf

    # select a subset of features at random
    num, dim = X.shape
    randFeatures = np.random.permutation(dim)[:numFeatures]
    weights = np.ones(numFeatures)

    # create a feature from the random subset
    ksum = KSum_Feature(weights, randFeatures)

    # identify values of feature to evaluate node splits at
    value_range = trainDTree_randomized_prepRange(X, ksum)

    bestIg = 0

    bestL = None
    bestR = None

    # cycle through possible thresholds
    for t in value_range:

        Q = DTQuery_KSum(ksum, t)

        # calculate splits
        L = []
        R = []
        for i in range(num):
            if Q.query(X[i, :]) == 0:
                L.append(i)
            else:
                R.append(i)

        # calculate impurity gradient
        Lportion = len(L) / np.double(num)
        Limpurity = impurity(Y[L])
        Rportion = len(R) / np.double(num)
        Rimpurity = impurity(Y[R])

        Ig = I - Lportion * Limpurity - Rportion * Rimpurity

        # save current best split
        if Ig > bestIg:
            bestIg = Ig
            bestQ  = DTQuery_KSum(ksum, t)
            bestL  = L
            bestR  = R

    if bestL is None or bestR is None:
        leaf = DTLeafNode(numClasses)
        leaf.computeDensity(X, Y)
        return leaf

    # grow tree recursively
    leftChild = trainDTree_randomized(X[bestL, :], Y[bestL], impurity, numClasses, numFeatures, depth + 1)
    rightChild = trainDTree_randomized(X[bestR, :], Y[bestR], impurity, numClasses, numFeatures, depth + 1)

    # intermediate node
    if depth > 0:
        return DTNode(bestQ, leftChild, rightChild)

    # root node
    return DTree(DTNode(bestQ, leftChild, rightChild))


def trainRandomForest(X, Y, size=100, numFeatures=100):
    '''
    Train a random forest for with the KSum feature with a given size (number of trees).

    size is the number of trees to learn in the forest.
    numFeatures is the number of random features to explore when learning each node
    in each tree.

    '''

    numClasses = np.max(Y) + 1 # assuming class indices from 0 to np.max(Y)
    rf = RandomForest(numClasses)

    for t in range(size):
        if t % 10 == 0 or size < 20: print 'Training tree %d' % t
        tree = trainDTree_randomized(X, Y, pr.trees.impurity_entropy, numClasses, numFeatures)
        rf.addTree(tree)

    print ''

    return rf



########################################################################
if __name__ == '__main__':

    # When you are just testing out your code and getting started, then you
    # should
    # load only a subset of the data, say 50 samples from each class
    #   Xtrain,Ytrain = loadData(num=50)
    #
    # And you should use only a small forest and numFeatures
    #   forest = trainRandomForest(Xtrain,Ytrain,2,10)
    #
    # I don't recommend using all of the data for training on this assignment
    # as it would take a long time to train (and that's not the point of this
    # exercise).  You could train the trees in parallel, but that would still
    # take a while.
    #

    print 'Running rfdigits as a main script; will proceed for training and testing.\n'

    Xtrain, Ytrain = loadData(num=1000) # 100

    print 'Xtrain:', Xtrain.shape
    print 'Ytrain:', Ytrain.shape
    print ''

    pr.trees.kVerbose = False
    forest = trainRandomForest(Xtrain, Ytrain, 10, 28) # 5, 20

    print 'Accuracy on training data is %.4f\n' % pr.datatools.accuracy(Xtrain, Ytrain, forest)

    Xtest, Ytest = loadData('test', 200) # 200

    print 'Xtest:', Xtest.shape
    print 'Ytest', Ytest.shape
    print ''

    print 'Accuracy on testing data is %.4f\n' % pr.datatools.accuracy(Xtest, Ytest, forest)

    # results with size = 10, numFeatures = 28
    # trained on num_training = 1000 (per class) and
    # evaluated on num_test = 200 (per class):
    # accuracy_training = 0.7208
    # accuracy_testing  = 0.6642

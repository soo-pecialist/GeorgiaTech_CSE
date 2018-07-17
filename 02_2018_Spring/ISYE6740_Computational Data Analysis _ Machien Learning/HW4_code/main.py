#python2

import time

import numpy as np
import matplotlib.pyplot as plt

from matplotlib import cm

import prpy as pr
from rfdigits import loadData, trainRandomForest

#%% SECTION 1: DATA

#%% load images (if this is the first time, else see below)

Xtrain, Ytrain = loadData(num=1000)
Xtest, Ytest = loadData('test', num=200)

#%% save in numpy format for faster loading in future

with open('train1k.npz', 'wb') as f:
    np.savez(f, Xtrain=Xtrain, Ytrain=Ytrain)
    
with open('test.npz', 'wb') as f:
    np.savez(f, Xtest=Xtest, Ytest=Ytest)
    
#%% load data from npz files
    
trainfiles = np.load('train1k.npz')
print trainfiles['Xtrain'].shape
print trainfiles['Ytrain'].shape

testfiles = np.load('test.npz')
print testfiles['Xtest'].shape
print testfiles['Ytest'].shape

#%% load into X, Y

Xtrain, Ytrain = trainfiles['Xtrain'], trainfiles['Ytrain']
Xtest, Ytest = testfiles['Xtest'], testfiles['Ytest']

#%% print shapes

print 'Xtrain:', Xtrain.shape
print 'Ytrain:', Ytrain.shape
print ''

print 'Xtest:', Xtest.shape
print 'Ytest:', Ytest.shape
print ''

#%% visualize digits

N = Xtrain.shape[0]
rand = [int(r) for r in np.random.rand(12) * N]

fig = plt.figure(figsize=(18, 6))

for i in xrange(12):
    ax = fig.add_subplot(2, 6, i+1)
    ax.imshow(Xtrain[rand[i], :].reshape(28, 28), cmap='gray')
    ax.set_title(Ytrain[rand[i]])
    plt.axis('tight')

plt.show()

#%% SECTION 2: TRAIN AND TEST

#%% set verbose = False

pr.trees.kVerbose = False

#%% train on 1000 images per class

num_trees = 10
num_features = 28

tic = time.time()
forest = trainRandomForest(Xtrain, Ytrain, num_trees, num_features) 
toc = time.time()

print "elapsed time: %.2f mins" % ((toc - tic) / 60) # 6.69 mins

# This is too long to run several experiments with. Therefore for 
# further experiments we use 200 images per class.

#%% score

print 'Accuracy on training data is %.4f\n' % pr.datatools.accuracy(Xtrain, Ytrain, forest)
print 'Accuracy on testing data is %.4f\n' % pr.datatools.accuracy(Xtest, Ytest, forest)

# training accuracy = 0.7322
# testing accuracy  = 0.6708

#%% visualize some incorrectly classified digits

perm = np.random.permutation(Xtest.shape[0])

i = 0
r = 0

fig = plt.figure(figsize=(18, 6))

for r in xrange(len(perm)):
    x = Xtest[perm[r], :]
    y = Ytest[perm[r]]
    yhat = forest.classify(x)
    if yhat != y:        
        ax = fig.add_subplot(2, 6, i+1)
        ax.imshow(x.reshape(28, 28), cmap='gray')
        ax.set_title(yhat)
        plt.axis('tight')
        i += 1
    if i == 12:
        break

plt.show()

#%% SECTION 2B: VISUALIZING A TREE!

import graphviz as gv
from collections import deque

#%% a class for visualization nodes

class VizNode(object):
    def __init__(self, node, id, parent):
        self.node = node
        self.id = id
        self.parent = parent
        if node.isLeaf():
            self.label = ''
            for i, d in enumerate(node.density):
                self.label += '%.2f' % d
                if i == len(node.density) / 2 - 1:
                    self.label += '\n'
                elif i != len(node.density) - 1:
                    self.label += ' '
        else:
            self.label = str(node.q.ksum.indices[:4]) + '\n' + \
                         str(round(node.q.tau, 2))
        
    def get(self):
        return (str(self.id), {'label': self.label})

#%% a class for visualization trees
        
class TreeViz(object):
    def __init__(self, format='png'):
        self.graph = gv.Digraph(format=format)
        
    def add_node(self, node):
        if isinstance(node, tuple):
            self.graph.node(node[0], **node[1])
        else:
            self.graph.node(node)
                
    def add_edge(self, edge):
        if isinstance(edge[0], tuple):
            self.graph.edge(*edge[0], **edge[1])
        else:
            self.graph.edge(*edge)
            
    def render(self, name):
        self.graph.render(name)

#%% function for creating tree visualization

def visualize_tree(tree, name='tree'):
    print 'creating tree visualization...'
    viz = TreeViz()
    i = 0
    root = VizNode(tree.root, 0, -1)
    vnodes = deque()
    vnodes.append(root)
    while(vnodes):
        vnode = vnodes.popleft()
        viz.add_node(vnode.get())
        if vnode.parent != -1:
            viz.add_edge((str(vnode.parent), str(vnode.id)))
        if not vnode.node.isLeaf():
            i += 1
            lchild = VizNode(vnode.node.child[0], i, vnode.id)
            vnodes.append(lchild)
            i += 1
            rchild = VizNode(vnode.node.child[1], i, vnode.id)
            vnodes.append(rchild)
    viz.render(name)

#%% visualize one tree

# note: please set kDT_MaxDepth <= 3 for creating the visualization
# otherwise the tree is too big to fit into a reasonably small image

pr.trees.kDT_MaxDepth = 3    
toy_forest = trainRandomForest(Xtrain, Ytrain, 2, 10)
pr.trees.kDT_MaxDepth = 5

for i, tree in enumerate(toy_forest.trees):
    visualize_tree(tree, 'tree%d' % i)

#%% SECTION 3: EXPLORE EFFECT of FOREST SIZE and NUMBER of FEATURES

#%% create new training set with n = 200 images per class

n = 200

Xtr = np.zeros((n*6, 784), dtype=np.uint8)
Ytr = np.zeros(n*6, dtype=np.uint8)

for i in xrange(6):
    Xtr[i*n: (i+1)*n, :] = Xtrain[i*1000: i*1000+n, :]
    Ytr[i*n: (i+1)*n] = Ytrain[i*1000: i*1000+n]
    
print 'Xtr:', Xtr.shape
print 'Ytr:', Ytr.shape
print ''

#%% train on n images per class

num_trees = 10
num_features = 28

tic = time.time()
forest = trainRandomForest(Xtr, Ytr, num_trees, num_features) 
toc = time.time()

print "elapsed time: %.2f mins" % ((toc - tic) / 60) # 1.48 mins

# This is too long to run several experiments with. Therefore for 
# further experiments we use 200 images per class.

#%% score

print 'Accuracy on training data is %.4f\n' % pr.datatools.accuracy(Xtr, Ytr, forest)
print 'Accuracy on testing data is %.4f\n' % pr.datatools.accuracy(Xtest, Ytest, forest)

# training accuracy = 0.7992
# testing accuracy  = 0.6275

#%% evaluate forests on a grid of num_trees x num_features

num_trees = [10, 50, 100]
num_features = [10, 28, 64, 128]

acc_tr = np.zeros((len(num_trees), len(num_features)))
acc_te = np.zeros((len(num_trees), len(num_features)))

tic = time.time()

for j, M in enumerate(num_trees):
    for k, K in enumerate(num_features):
        print M, K
        randforest = trainRandomForest(Xtr, Ytr, M, K) 
        acc_tr[j, k] = pr.datatools.accuracy(Xtr, Ytr, randforest)
        acc_te[j, k] = pr.datatools.accuracy(Xtest, Ytest, randforest)
        
toc = time.time()

print "elapsed time: %.2f mins" % ((toc - tic) / 60) # 98.02 mins


#%%
        
print acc_tr
print acc_te

#%%

fig = plt.figure(figsize=(9.0, 18.0))

ax = fig.add_subplot(211)
res = ax.imshow(acc_tr, vmin=1/6., vmax=6/6., cmap=cm.jet, interpolation='nearest')

cb = fig.colorbar(res, orientation='horizontal')

ax.set_yticks(range(3))
ax.set_yticklabels(num_trees)
ax.set_xticks(range(4))
ax.set_xticklabels(num_features)

ax.set_ylabel('number of trees')
ax.set_xlabel('number of features')
ax.set_title('training accuracy')

plt.axis('tight')

for j in xrange(len(num_trees)):
    for k in xrange(len(num_features)):
        ax.annotate('%.4g' % acc_tr[j][k], xy=(k, j), 
                    horizontalalignment='center',
                    verticalalignment='center')
   
ax = fig.add_subplot(212)
res = ax.imshow(acc_te, vmin=1/6., vmax=6/6., cmap=cm.jet, interpolation='nearest')
plt.axis('tight')

ax.set_yticks(range(3))
ax.set_yticklabels(num_trees)
ax.set_xticks(range(4))
ax.set_xticklabels(num_features)

ax.set_ylabel('number of trees')
ax.set_xlabel('number of features')
ax.set_title('test accuracy')

plt.axis('tight')

for j in xrange(len(num_trees)):
    for k in xrange(len(num_features)):
        ax.annotate('%.4g' % acc_te[j][k], xy=(k, j), 
                    horizontalalignment='center',
                    verticalalignment='center')


cb = fig.colorbar(res, orientation='horizontal')

plt.show()

#%% EOF
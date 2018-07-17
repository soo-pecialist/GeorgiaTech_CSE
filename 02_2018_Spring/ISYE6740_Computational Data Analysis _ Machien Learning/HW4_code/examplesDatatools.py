"""
examples_datatools.py
Jason Corso (jcorso@acm.org)

Example uses of the datatools.py module in prpy.

"""

import prpy as pr

import numpy as np
import scipy as sp
import scipy.io


# Example 1.  Generating a data set with a Gaussian distribution and plotting it

X1 = pr.datatools.genData_gaussian([0,0],[[1,0],[0,3]],1000)
pr.datatools.plotData_one(X1)




# Example 2.  Plotting two classes of data alongside each other

X2 = pr.datatools.genData_gaussian([5,0],[[1.4,0.71],[-0.71,1.4]],1000)
pr.datatools.plotData_twoClass(X1,X2)




# Example 3.  Generating a multiclass 2D data set interactively

(X,Y) = pr.datatools.genData_interactive(2,10)
close('all')
pr.datatools.plotData(X,Y)




# Example 4.  Saving to disk and reading from disk (use NumPy directly)
np.save('/tmp/X.npy',X)             # save one array at a time
np.savez('/tmp/data.npz',X=X,Y=Y)   # save many arrays into a package

Z = np.load('/tmp/data.npz')
Z.keys()
pr.datatools.plotData(Z['X'],Z['Y'])

X = Z['X']
Y = Z['Y']




# Example 5. Saving for Matlab and Reading from Matlab
sp.io.savemat('/tmp/data.mat',{'X':X,'Y':Y})

M = sp.io.loadmat('/tmp/data.mat')
pr.datatools.plotData(M['X'],M['Y'])


#!/usr/bin/env python
import numpy as np
import matplotlib.pyplot as plt
import pylab as pl
import sys
import os

filename = sys.argv[1]

data=np.loadtxt(filename).T
plotfontsize=18
fig = plt.figure()
x=y=np.linspace(-10.0,10.0,201)
X,Y=np.meshgrid(x,y)
ax = fig.add_subplot(111)
ax.set_ylabel(r'$y$ (fm)', {'fontsize': plotfontsize})
ax.set_xlabel(r'$x$ (fm)', {'fontsize': plotfontsize})
ax.set_title(r'Initial conditions for %(filename)s' % {"filename": filename})
#print max(data.flatten()), np.asarray(np.linspace(0,1,11)[1:])
fmt = '%r %%'
data *= 100./max(data.flatten())
#cs = plt.contour(X,Y,data, levels=(max(data.flatten())*(np.linspace(0,1,11)[1:]-0.05)))
cm = plt.cm.gnuplot
plt.imshow(data[75:125,75:125], cmap=cm, origin='lower', interpolation='bicubic', extent=[-10.0,10.0,-10.0,10.0])
ax.set_aspect('equal')
#plt.clabel(cs, cs.levels, inline=True, fmt=fmt, fontsize=10)
direc=os.path.dirname(filename)
plt.show()
#outfilename=direc + '/contours2.pdf'
#print 'Saved as', outfilename
#plt.savefig(outfilename, format='pdf')

# End of file

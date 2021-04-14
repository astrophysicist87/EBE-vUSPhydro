#!/usr/bin/env python
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
import scipy.interpolate
#from scipy.interpolate import RegularGridInterpolator as rgi
import sys, os



#====================================================
if __name__ == "__main__":
	# Set KT and Kphi grids
	(KTpts, KTwts) = np.loadtxt('KT.dat').T
	(Kphipts, Kphiwts) = np.loadtxt('KPHI36.dat').T
	nKT = len(KTpts)
	nKphi = len(Kphipts)
	
	filename = sys.argv[1]
	R2ij = [ R2s, R2o, R2os, R2l, R2sl, R2ol ] \
		= [ radsq.reshape([nKT, nKphi]) for radsq in np.loadtxt(filename)[:,2:].T ]

	newKTpts = np.linspace(0.01, 1.01, 101)
	f = scipy.interpolate.interp1d(KTpts, R2ij, kind='cubic', axis=1)
	interpR2ij = np.array(f(newKTpts))

	nMax = 3	
	R2ijCos = np.array([np.dot( interpR2ij * np.cos(n*Kphipts), Kphiwts )
	                    / (2.0*np.pi) for n in range(nMax+1)])
	R2ijSin = np.array([np.dot( interpR2ij * np.sin(n*Kphipts), Kphiwts )
	                    / (2.0*np.pi) for n in range(nMax+1)])
	
	#print R2ijCos.shape
	#print R2ijSin.shape
		
	# form other (zero-indexed) columns explicitly
	col0 = np.repeat(newKTpts,nMax+1)
	col1 = np.tile(range(nMax+1), len(newKTpts))
	R2ijCos = (R2ijCos.transpose((1,2,0))).reshape([6, (nMax+1)*len(newKTpts)])
        R2ijSin = (R2ijSin.transpose((1,2,0))).reshape([6, (nMax+1)*len(newKTpts)])
	
	# split it back up to save
	[ R2sCos, R2oCos, R2osCos, R2lCos, R2slCos, R2olCos ] = R2ijCos
	[ R2sSin, R2oSin, R2osSin, R2lSin, R2slSin, R2olSin ] = R2ijSin
	
	#print np.c_[ col0, col1, R2sCos, R2sSin, R2oCos, R2oSin, R2osCos, R2osSin, \
	#                         R2lCos, R2lSin, R2slCos, R2slSin, R2olCos, R2olSin ]
	#	
        results = np.c_[ col0, col1, R2sCos, R2sSin, R2oCos, R2oSin, R2osCos, R2osSin, \
	                             R2lCos, R2lSin, R2slCos, R2slSin, R2olCos, R2olSin ]
	outfilename = os.path.dirname(filename) + '/R2ij_GF_cfs.dat'
	print('Saving to', outfilename)
	np.savetxt( outfilename, results, fmt='%1.2f %d'+(' %1.6f'*12) )
	
	
	


# End of file

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
	(Kphipts, Kphiwts) = np.loadtxt('KPHI.dat').T
	nKT = len(KTpts)
	nKphi = len(Kphipts)
	
	filename = sys.argv[1]
	SVs = [ S, xs, xo, xl, t, \
			xs_t, xo_t, xl_t, xo_xs, xl_xs, xo_xl, \
			xs2, xo2, xl2, t2 ] \
		= [ SV.reshape([nKT, nKphi]) for SV in np.loadtxt(filename)[:,2:].T ]

        # normalize
	SVs = np.asarray(SVs) / S

	# split back up into individual source variances
	[ S, xs, xo, xl, t, \
		xs_t, xo_t, xl_t, xo_xs, xl_xs, xo_xl, \
		xs2, xo2, xl2, t2 ] = SVs

        SVs = [ xs2 - xs**2, xo2 - xo**2, xo_t - xo*t, t2 - t**2, xl2 - xl**2 ]

	newKTpts = np.linspace(0.01, 1.01, 101)
	f = scipy.interpolate.interp1d(KTpts, SVs, kind='cubic', axis=1)
	SVs = np.dot( f(newKTpts), Kphiwts ) / (2.0*np.pi)
	
	outfilename = os.path.dirname(filename) + '/SV_cfs.dat'
	print('Saving to', outfilename)
	np.savetxt( outfilename, np.c_[ newKTpts, SVs[0], SVs[1], SVs[2], SVs[3], SVs[4] ], fmt='%1.6f' )
	
	
	


# End of file

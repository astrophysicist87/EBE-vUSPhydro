#!/usr/bin/env python
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
import glob

#====================================================
GeVtoMeV = 1000.0
showInsteadOfSave = False

#====================================================
def pause():
    programPause = raw_input("Press ENTER to continue...")
#====================================================
    
ppCentralities = ['0_0.001', '0_0.01', '0_0.1', '0_1', '0_10', '10_20', '20_30', '30_40', '40_50', '50_60', '60_70', '70_80', '80_90', '90_100'][::-1]
pPbCentralities = ['0_10', '10_20', '20_30', '30_40', '40_50', '50_60', '60_70', '70_80', '80_90', '90_100'][::-1]
PbPbCentralities = ['0_10', '10_20', '20_30', '30_40', '40_50', '50_60', '60_70', '70_80', '80_90', '90_100'][::-1]
centralities = {'pp': ppCentralities, 'pPb': pPbCentralities, 'PbPb': PbPbCentralities}

#====================================================
#====================================================
def make_surface_plot(collisionSystem):
	f, (ax1, ax2) = plt.subplots(1, 2, sharey = True, figsize=(10,5))
	f.subplots_adjust(wspace=0)
	lw = 5

	surfaceXfiles = ['RESULTS_EA_' + collisionSystem + '_C' + cen + '_wHBT/job-1/event-1/SurfaceX.dat' for cen in centralities[collisionSystem]]
	surfaceYfiles = ['RESULTS_EA_' + collisionSystem + '_C' + cen + '_wHBT/job-1/event-1/SurfaceY.dat' for cen in centralities[collisionSystem]]

	# make color scale normalized to number of pp surfaces
	colors = plt.cm.jet(np.linspace(1.0/len(surfaceXfiles), 1-1.0/len(surfaceXfiles), len(surfaceXfiles)))
	#colors = plt.cm.jet(np.linspace(1.0/len(ppCentralities), 1-1.0/len(ppCentralities), len(ppCentralities)))

	i=0
	for surfaceXfile in surfaceXfiles:
		surfaceX = np.loadtxt(surfaceXfile)
		tau0 = np.amin(surfaceX[:,0])
		surfaceX = np.c_[ surfaceX, np.arctan2(surfaceX[:,3], surfaceX[:,0]-tau0) ]
		surfaceX = surfaceX[surfaceX[:,-1].argsort()]
		surfaceX = surfaceX[np.where(surfaceX[:,3] >= 0.0)]
		ax1.plot(surfaceX[:,3], surfaceX[:,0]-tau0, linewidth=lw, color=colors[i])
		i+=1

	i=0
	for surfaceYfile in surfaceYfiles:
		surfaceY = np.loadtxt(surfaceYfile)
		tau0 = np.amin(surfaceY[:,0])
		surfaceY = np.c_[ surfaceY, np.arctan2(surfaceY[:,5], surfaceY[:,0]-tau0) ]
		surfaceY = surfaceY[surfaceY[:,-1].argsort()]
		surfaceY = surfaceY[np.where(surfaceY[:,5] >= 0.0)]
		ax2.plot(surfaceY[:,5], surfaceY[:,0]-tau0, linewidth=lw, color=colors[i])
		i+=1

	#xLimits = {'pp': [0.0, 11.9], 'pPb': [0.0, 11.9], 'PbPb': [0.0, 11.9]}[collisionSystem]
	xLimits = [0.0, 11.9]
	yLimits = [0.0, 15.9]
	ax1.set_xlim(xLimits)
	ax2.set_xlim(xLimits)
	#ax1.set_ylim(bottom=0.0)
	#ax2.set_ylim(bottom=0.0)
	ax1.set_ylim(yLimits)
	#plt.gca().set_xlim(left=0.0)
	#plt.gca().set_xlim(right=4.0)
	#plt.gca().set_ylim(bottom=0.0)
	#plt.gca().set_ylim(top=4.0)
	ax1.set_xlabel(r'$x$ (fm)', fontsize=20)
	ax2.set_xlabel(r'$y$ (fm)', fontsize=20)
	ax1.set_ylabel(r'$\tau-\tau_0$', fontsize=20)
	
	if showInsteadOfSave:
		plt.show(block=False)
	else:
		plt.savefig('./RESULTS_EA_' + collisionSystem + '_FOslices.pdf', bbox_inches='tight')



#====================================================
#====================================================
def make_surface_plots(frame):
	f, axs = plt.subplots(2, 3, sharex = True, sharey = True, figsize=(15,10))
	f.subplots_adjust(hspace=0, wspace=0)
	lw = 5

	lines=[]
	names=[]

	col=0
	for collisionSystem in ['PbPb', 'pPb', 'pp']:
		surfaceXfiles = ['RESULTS_EA_' + collisionSystem + '_C' + cen + '_wHBT/job-1/event-1/SurfaceX.dat' for cen in centralities[collisionSystem]][:frame]
		surfaceYfiles = ['RESULTS_EA_' + collisionSystem + '_C' + cen + '_wHBT/job-1/event-1/SurfaceY.dat' for cen in centralities[collisionSystem]][:frame]

		# make color scale normalized to number of pp surfaces
		#colors = plt.cm.jet(np.linspace(1.0/len(surfaceXfiles), 1-1.0/len(surfaceXfiles), len(surfaceXfiles)))
		colors = plt.cm.jet(np.linspace(1.0/len(ppCentralities), 1-1.0/len(ppCentralities), len(ppCentralities)))

		i=0
		for surfaceXfile in surfaceXfiles:
			surfaceX = np.loadtxt(surfaceXfile)
			tau0 = np.amin(surfaceX[:,0])
			surfaceX = np.c_[ surfaceX, np.arctan2(surfaceX[:,3], surfaceX[:,0]-tau0) ]
			surfaceX = surfaceX[surfaceX[:,-1].argsort()]
			surfaceX = surfaceX[np.where(surfaceX[:,3] >= 0.0)]
			if col==2:
				lines+=axs[0,col].plot(surfaceX[:,3], surfaceX[:,0]-tau0, linewidth=lw, color=colors[i])
				names+=[(centralities[collisionSystem][i].replace('_','-')+'%')]
			else:
				axs[0,col].plot(surfaceX[:,3], surfaceX[:,0]-tau0, linewidth=lw, color=colors[i])
			i+=1

		i=0
		for surfaceYfile in surfaceYfiles:
			surfaceY = np.loadtxt(surfaceYfile)
			tau0 = np.amin(surfaceY[:,0])
			surfaceY = np.c_[ surfaceY, np.arctan2(surfaceY[:,5], surfaceY[:,0]-tau0) ]
			surfaceY = surfaceY[surfaceY[:,-1].argsort()]
			surfaceY = surfaceY[np.where(surfaceY[:,5] >= 0.0)]
			axs[1,col].plot(surfaceY[:,5], surfaceY[:,0]-tau0, linewidth=lw, color=colors[i])
			i+=1

		#xLimits = {'pp': [0.0, 11.9], 'pPb': [0.0, 11.9], 'PbPb': [0.0, 11.9]}[collisionSystem]
		xLimits = [0.0, 11.9]
		yLimits = [0.0, 15.9]
		axs[0,col].set_xlim(xLimits)
		axs[1,col].set_xlim(xLimits)
		axs[0,col].set_ylim(yLimits)
		axs[0,col].set_xlabel(r'$x$ (fm)', fontsize=20)
		axs[0,col].xaxis.set_label_position('top') 
		axs[1,col].set_xlabel(r'$y$ (fm)', fontsize=20)
		
		col+=1

	axs[0,0].set_ylabel(r'$\tau-\tau_0$ (fm/$c$)', fontsize=20)
	axs[1,0].set_ylabel(r'$\tau-\tau_0$ (fm/$c$)', fontsize=20)

	axs[0,1].legend(lines[:10], names[:10], loc='best')
	axs[0,2].legend(lines[10:], names[10:], loc='best')

	axs[1,0].text(0.84, 0.1, r'$\mathrm{Pb+Pb}$', {'color': 'black', 'fontsize': 20},
         horizontalalignment='center', verticalalignment='center', transform=axs[1,0].transAxes)
	axs[1,1].text(0.84, 0.1, r'$\mathrm{p+Pb}$', {'color': 'black', 'fontsize': 20},
         horizontalalignment='center', verticalalignment='center', transform=axs[1,1].transAxes)
	axs[1,2].text(0.84, 0.1, r'$\mathrm{p+p}$', {'color': 'black', 'fontsize': 20},
         horizontalalignment='center', verticalalignment='center', transform=axs[1,2].transAxes)
	
	if showInsteadOfSave:
		plt.show(block=False)
	else:
		outfilename = './Figures/RESULTS_EA_FOslices_frame' + str(frame) + '.pdf'
		print 'Saving to', outfilename
		plt.savefig(outfilename, bbox_inches='tight')



#====================================================
#====================================================
def plot_radii_vs_dNchdeta(ppKT, pPbKT, PbPbKT):
	# Figure settings
	f, axs = plt.subplots(1, 3, sharey = True, figsize=(15,5))
	f.subplots_adjust(wspace=0)
	lw = 3
	ms = 8

	lines0=lines1=[]

	# Plot Pb+Pb
	datafile = 'HBTradii_vs_dNchdeta_PbPb_kt' + PbPbKT + 'MeV.dat'
	data = np.loadtxt(datafile)

	axs[0].plot(data[:,0]**(1.0/3.0), np.sqrt(data[:,1]), 'r^--', linewidth=lw, markersize=ms)
	lines1 += axs[1].plot(data[:,0]**(1.0/3.0), np.sqrt(data[:,2]), 'r^--', linewidth=lw, markersize=ms, label=r'$\mathrm{Pb+Pb}$')
	axs[2].plot(data[:,0]**(1.0/3.0), np.sqrt(data[:,3]), 'r^--', linewidth=lw, markersize=ms)

	# Plot p+Pb
	datafile = 'HBTradii_vs_dNchdeta_pPb_kt' + pPbKT + 'MeV.dat'
	data = np.loadtxt(datafile)

	axs[0].plot(data[:,0]**(1.0/3.0), np.sqrt(data[:,1]), '*--', color='purple', linewidth=lw, markersize=ms)
	lines1 += axs[1].plot(data[:,0]**(1.0/3.0), np.sqrt(data[:,2]), '*--', color='purple', linewidth=lw, markersize=ms, label=r'$\mathrm{p+Pb}$')
	axs[2].plot(data[:,0]**(1.0/3.0), np.sqrt(data[:,3]), '*--', color='purple', linewidth=lw, markersize=ms)

	# Plot p+p
	datafile = 'HBTradii_vs_dNchdeta_pp_kt' + ppKT + 'MeV.dat'
	data = np.loadtxt(datafile)

	lines0 += axs[0].plot(data[:,0]**(1.0/3.0), np.sqrt(data[:,1]), 'go--', linewidth=lw, markersize=ms, label=r'$\mathrm{p+p}$ (with quarks)')
	axs[1].plot(data[:,0]**(1.0/3.0), np.sqrt(data[:,2]), 'go--', linewidth=lw, markersize=ms)
	axs[2].plot(data[:,0]**(1.0/3.0), np.sqrt(data[:,3]), 'go--', linewidth=lw, markersize=ms)

	# Plot settings
	axs[2].set_ylim(top=9.0)
	axs[0].set_xlabel(r'$\left(dN_{\mathrm{ch}}/d\eta\right)^{1/3}$', fontsize=20)
	axs[1].set_xlabel(r'$\left(dN_{\mathrm{ch}}/d\eta\right)^{1/3}$', fontsize=20)
	axs[2].set_xlabel(r'$\left(dN_{\mathrm{ch}}/d\eta\right)^{1/3}$', fontsize=20)
	axs[0].set_ylabel(r'fm', fontsize=20)

	plt.text(0.8, 0.125, r'$R_{\mathrm{side}}$', {'color': 'black', 'fontsize': 24},
         horizontalalignment='center', verticalalignment='center', transform=axs[0].transAxes)
	plt.text(0.8, 0.125, r'$R_{\mathrm{out}}$', {'color': 'black', 'fontsize': 24},
         horizontalalignment='center', verticalalignment='center', transform=axs[1].transAxes)
	plt.text(0.8, 0.125, r'$R_{\mathrm{long}}$', {'color': 'black', 'fontsize': 24},
         horizontalalignment='center', verticalalignment='center', transform=axs[2].transAxes)

        plt.text(0.5, 0.85, r'$K_{T,pp} = $' + ppKT + r'$\,\mathrm{MeV}$', {'color': 'black', 'fontsize': 24},
         horizontalalignment='center', verticalalignment='center', transform=axs[2].transAxes)

	axs[0].legend(loc='upper left', fontsize=18)
	axs[1].legend(loc='upper left', fontsize=18)

	if showInsteadOfSave:
		plt.show(block=False)
	else:
		stem = ''
		plt.savefig('./Figures/Ri_vs_dNchdeta' + stem \
				+ '_ppKT' + ppKT + 'MeV.pdf', \
				bbox_inches='tight')





#====================================================
#====================================================
def plot_radii_vs_KT(collisionSystem):
	f, axs = plt.subplots(2, 2, sharex = True, sharey = False, figsize=(10,10))
	f.subplots_adjust(wspace=0,hspace=0)
	lw = 3
	ms = 8

	radiiFiles = ['RESULTS_EA_' + collisionSystem + '_C' + cen + '_wHBT/job-1/event-1/HBTradii_GF_cfs_no_df.dat' for cen in centralities[collisionSystem]]

	colors = plt.cm.jet(np.linspace(1.0/len(radiiFiles),1-1.0/len(radiiFiles),len(radiiFiles)))

	lines0 = []
	lines1 = []
	names0 = []
	names1 = []

	icutoff = {'pp': 5, 'pPb': 4, 'PbPb': 4}[collisionSystem]

	i=0
	for radiiFile in radiiFiles:
		data = np.loadtxt(radiiFile)
		data = data[np.where((data[:,1]==0) & (data[:,0]>=0.1) & (data[:,0]<=1.0))][:,[0,2,4,8]]

		line=axs[0,0].plot(data[:,0], np.sqrt(data[:,1]), linestyle='-', color=colors[i], linewidth=lw, markersize=ms)
		if i<=icutoff:
			lines0+=line
			names0+=[(centralities[collisionSystem][i].replace('_','-')+'%')]
		axs[0,1].plot(data[:,0], np.sqrt(data[:,2]), linestyle='-', color=colors[i], linewidth=lw, markersize=ms)
		line=axs[1,0].plot(data[:,0], np.sqrt(data[:,3]), linestyle='-', color=colors[i], linewidth=lw, markersize=ms)
		if i>icutoff:
			lines1+=line
			names1+=[(centralities[collisionSystem][i].replace('_','-')+'%')]
		#axs[3].plot(data[:,0], np.sqrt(data[:,2]/data[:,1]), linestyle='-', color=colors[i], linewidth=lw, markersize=ms)

		i+=1

	i=0
	for radiiFile in radiiFiles[::-1]:
		data = np.loadtxt(radiiFile)
		data = data[np.where((data[:,1]==0) & (data[:,0]>=0.1) & (data[:,0]<=1.0))][:,[0,2,4,8]]

		if i==0:
			axs[1,1].plot(data[:,0], 1.0+0.0*data[:,0], linestyle='-', color='black', linewidth=2)
		axs[1,1].plot(data[:,0], np.sqrt(data[:,2]/data[:,1]), linestyle='-', color=colors[::-1][i], linewidth=lw, markersize=ms)
		i+=1

	#axs[0].set_xlabel(r'$K_T$ (GeV)', fontsize=20)
	#axs[1].set_xlabel(r'$K_T$ (GeV)', fontsize=20)
	axs[1,0].set_xlabel(r'$K_T$ (GeV)', fontsize=20)
	axs[1,1].set_xlabel(r'$K_T$ (GeV)', fontsize=20)
	axs[0,0].set_ylabel(r'$R_{\mathrm{side}}$ (fm)', fontsize=20)
	axs[0,1].set_ylabel(r'$R_{\mathrm{out}}$ (fm)', fontsize=20)
	axs[0,1].yaxis.tick_right()
	axs[0,1].yaxis.set_label_position("right")
	axs[1,0].set_ylabel(r'$R_{\mathrm{long}}$ (fm)', fontsize=20)
	axs[1,1].set_ylabel(r'$R_{\mathrm{out}}/R_{\mathrm{side}}$', fontsize=20)
	axs[1,1].yaxis.tick_right()
	axs[1,1].yaxis.set_label_position("right")

	for ax in axs.reshape(4):
		ax.set_xlim([0.11, 0.99])
	axs[0,0].set_ylim({'pp': [0.51,2.6], 'pPb': [0.51,2.6], 'PbPb': [0.51,6.0]}[collisionSystem])
	axs[0,1].set_ylim({'pp': [0.51,2.6], 'pPb': [0.51,2.8], 'PbPb': [0.51,7.0]}[collisionSystem])
	if collisionSystem == 'PbPb':
		axs[1,0].set_ylim([1.0,13.0])

	axs[0,0].legend(lines0, names0, loc='best')
	if collisionSystem=='pp':
		axs[1,0].legend(lines1, names1, loc='best', ncol=2)
	else:
		axs[1,0].legend(lines1, names1, loc='best')

	if showInsteadOfSave:
		plt.show(block=False)
	else:
		outfilename = './Figures/' + collisionSystem + '_Ri_vs_KT.pdf'
		print 'Saving to', outfilename
		plt.savefig(outfilename, bbox_inches='tight')




#====================================================
#====================================================
def plot_radii_vs_KT_and_dNchdeta(collisionSystem):
	f, axs = plt.subplots(2, 2, sharex = True, sharey = False, figsize=(10,10))
	f.subplots_adjust(wspace=0, hspace=0)
	lw = 3
	ms = 8

	# KT == 250 MeV
	datafile = 'HBTradii_vs_dNchdeta_' + collisionSystem + '_kt' + '250' + 'MeV.dat'
	data = np.loadtxt(datafile)

	axs[0,0].plot(data[:,0]**(1.0/3.0), np.sqrt(data[:,1]), 'rs--', linewidth=lw, markersize=ms, label=r'$K_T = 250\,\mathrm{MeV}$')
	axs[0,1].plot(data[:,0]**(1.0/3.0), np.sqrt(data[:,2]), 'rs--', linewidth=lw, markersize=ms)
	axs[1,0].plot(data[:,0]**(1.0/3.0), np.sqrt(data[:,3]), 'rs--', linewidth=lw, markersize=ms)
	axs[1,1].plot(data[:,0]**(1.0/3.0), np.sqrt(data[:,2]/data[:,1]), 'rs--', linewidth=lw, markersize=ms)

	# KT == 350 MeV
	datafile = 'HBTradii_vs_dNchdeta_' + collisionSystem + '_kt' + '350' + 'MeV.dat'
	data = np.loadtxt(datafile)

	axs[0,0].plot(data[:,0]**(1.0/3.0), np.sqrt(data[:,1]), 'g^--', linewidth=lw, markersize=ms, label=r'$K_T = 350\,\mathrm{MeV}$')
	axs[0,1].plot(data[:,0]**(1.0/3.0), np.sqrt(data[:,2]), 'g^--', linewidth=lw, markersize=ms)
	axs[1,0].plot(data[:,0]**(1.0/3.0), np.sqrt(data[:,3]), 'g^--', linewidth=lw, markersize=ms)
	axs[1,1].plot(data[:,0]**(1.0/3.0), np.sqrt(data[:,2]/data[:,1]), 'g^--', linewidth=lw, markersize=ms)

	# KT == 450 MeV
	datafile = 'HBTradii_vs_dNchdeta_' + collisionSystem + '_kt' + '450' + 'MeV.dat'
	data = np.loadtxt(datafile)

	axs[0,0].plot(data[:,0]**(1.0/3.0), np.sqrt(data[:,1]), 'yo--', linewidth=lw, markersize=ms)
	axs[0,1].plot(data[:,0]**(1.0/3.0), np.sqrt(data[:,2]), 'yo--', linewidth=lw, markersize=ms)
	axs[1,0].plot(data[:,0]**(1.0/3.0), np.sqrt(data[:,3]), 'yo--', linewidth=lw, markersize=ms, label=r'$K_T = 450\,\mathrm{MeV}$')
	axs[1,1].plot(data[:,0]**(1.0/3.0), np.sqrt(data[:,2]/data[:,1]), 'yo--', linewidth=lw, markersize=ms)

	# KT == 550 MeV
	datafile = 'HBTradii_vs_dNchdeta_' + collisionSystem + '_kt' + '550' + 'MeV.dat'
	data = np.loadtxt(datafile)

	axs[0,0].plot(data[:,0]**(1.0/3.0), np.sqrt(data[:,1]), '*--', color='purple', linewidth=lw, markersize=ms)
	axs[0,1].plot(data[:,0]**(1.0/3.0), np.sqrt(data[:,2]), '*--', color='purple', linewidth=lw, markersize=ms)
	axs[1,0].plot(data[:,0]**(1.0/3.0), np.sqrt(data[:,3]), '*--', color='purple', linewidth=lw, markersize=ms, label=r'$K_T = 550\,\mathrm{MeV}$')
	axs[1,1].plot(data[:,0]**(1.0/3.0), np.sqrt(data[:,2]/data[:,1]), '*--', color='purple', linewidth=lw, markersize=ms)

	#axs[0].set_xlabel(r'$\left(dN_{\mathrm{ch}}/d\eta\right)^{1/3}$', fontsize=20)
	#axs[1].set_xlabel(r'$\left(dN_{\mathrm{ch}}/d\eta\right)^{1/3}$', fontsize=20)
	axs[1,0].set_xlabel(r'$\left(dN_{\mathrm{ch}}/d\eta\right)^{1/3}$', fontsize=20)
	axs[1,1].set_xlabel(r'$\left(dN_{\mathrm{ch}}/d\eta\right)^{1/3}$', fontsize=20)
	axs[0,0].set_ylabel(r'$R_{\mathrm{side}}$ (fm)', fontsize=20)
	axs[0,1].set_ylabel(r'$R_{\mathrm{out}}$ (fm)', fontsize=20)
	axs[0,1].yaxis.tick_right()
	axs[0,1].yaxis.set_label_position("right")
	axs[1,0].set_ylabel(r'$R_{\mathrm{long}}$ (fm)', fontsize=20)
	axs[1,1].set_ylabel(r'$R_{\mathrm{out}}/R_{\mathrm{side}}$', fontsize=20)
	axs[1,1].yaxis.tick_right()
	axs[1,1].yaxis.set_label_position("right")

	#for ax in axs.reshape(4):
	#	ax.set_xlim([0.11, 0.99])
	#axs[0,0].set_ylim({'pp': [0.61,2.6], 'pp_SoE2': [0.61,2.6]}[collisionSystem])
	axs[0,0].set_ylim(bottom={'pp': 0.75, 'pp_SoE2': 0.55}[collisionSystem])
	axs[0,1].set_ylim(bottom={'pp': 1.01, 'pp_SoE2': 0.75}[collisionSystem])
	axs[1,0].set_ylim(top=3.9)
	axs[1,1].set_ylim(top={'pp': 1.6, 'pp_SoE2': 1.75}[collisionSystem])
	axs[1,1].set_ylim(bottom={'pp': 0.81, 'pp_SoE2': 0.95}[collisionSystem])
	axs[1,1].set_xlim(left={'pp': 1.01, 'pp_SoE2': 0.51}[collisionSystem])
	axs[1,1].set_xlim(right={'pp': 3.99, 'pp_SoE2': 3.49}[collisionSystem])

	axs[0,0].legend(loc='best')
	axs[1,0].legend(loc='best')

	f.suptitle( {'pp': r'$\mathrm{p+p}$ (with quarks)', \
				'pp_SoE2': r'$\mathrm{p+p}$ (without quarks)'}[collisionSystem], \
				fontsize=24 )

	if showInsteadOfSave:
		plt.show(block=False)
	else:
		outfilename = './Figures/' + collisionSystem + '_Ri_vs_KT_and_dNchdeta.pdf'
		print 'Saving to', outfilename
		plt.savefig(outfilename, bbox_inches='tight')


#====================================================
#====================================================
def plot_SVs_vs_KT_and_dNchdeta():
	f, axs = plt.subplots(1, 1, figsize=(5,5))
	lw = 3
	ms = 8
	
	# KT == 400 MeV
	datafile = 'SV_cfs_vs_dNchdeta_pp_kt400MeV.dat'
	data = np.loadtxt(datafile)

	axs.plot(data[:,0], data[:,2], 'r-', linewidth=lw, markersize=ms, label=r'$\left< \tilde{x}_s^2 \!\right>$')
	axs.plot(data[:,0], data[:,3], 'g-', linewidth=lw, markersize=ms, label=r'$\left< \tilde{x}_o^2 \!\right>$')
	axs.plot(data[:,0], data[:,4], 'b-', linewidth=lw, markersize=ms, label=r'$\left< \tilde{x}_o \tilde{t} \right>$')
	axs.plot(data[:,0], data[:,5], '-', color='purple', linewidth=lw, markersize=ms)
	axs.plot(data[:,0], data[:,6], 'y-', linewidth=lw, markersize=ms)
	axs.plot(np.linspace(-1.0,100.0,2), 0.0*np.linspace(-1.0,100.0,2), '-', color='black', linewidth=1)

	axs.set_xlabel(r'$dN_{\mathrm{ch}}/d\eta$', fontsize=20)
	#axs.set_ylabel(r'$\left(\mathrm{fm}^2\right)$', fontsize=20)
	axs.set_ylabel(r'$\mathrm{fm}^2$', fontsize=20)

	axs.set_xlim([0.0, 50.0])
	axs.set_ylim([-0.5, 8.0])
	axs.legend(loc='best')
	plt.text(0.5, 0.85, r'$\mathrm{p+p}$', {'color': 'black', 'fontsize': 20},
         horizontalalignment='center', verticalalignment='center', transform=axs.transAxes)

	if showInsteadOfSave:
		plt.show(block=False)
	else:
		outfilename = './Figures/pp_SV_scaling_kt400MeV.pdf'
		print 'Saving to', outfilename
		plt.savefig(outfilename, bbox_inches='tight')
		plt.close()

	f, axs = plt.subplots(1, 1, figsize=(5,5))

	# KT == 400 MeV
	datafile = 'SV_cfs_vs_dNchdeta_PbPb_kt450MeV.dat'
	data = np.loadtxt(datafile)

	axs.plot(data[:,0], data[:,2], 'r-', linewidth=lw, markersize=ms)
	axs.plot(data[:,0], data[:,3], 'g-', linewidth=lw, markersize=ms)
	axs.plot(data[:,0], data[:,4], 'b-', linewidth=lw, markersize=ms)
	axs.plot(data[:,0], data[:,5], '-', color='purple', linewidth=lw, markersize=ms, label=r'$\left< \tilde{t}^2 \!\right>$')
	axs.plot(data[:,0], data[:,6], 'y-', linewidth=lw, markersize=ms, label=r'$\left< \tilde{x}_l^2 \!\right>$')
	axs.plot(np.linspace(-1.0,1000.0,2), 0.0*np.linspace(-1.0,1000.0,2), '-', color='black', linewidth=1)

	axs.set_xlabel(r'$dN_{\mathrm{ch}}/d\eta$', fontsize=20)
	#axs.set_ylabel(r'$\left(\mathrm{fm}^2\right)$', fontsize=20)
	axs.set_ylabel(r'$\mathrm{fm}^2$', fontsize=20)

	axs.set_xlim([0.0, 625.0])
	#axs.set_ylim([-3.0, 12.0])
	axs.legend(loc='best')
	plt.text(0.5, 0.85, r'$\mathrm{Pb+Pb}$', {'color': 'black', 'fontsize': 20},
         horizontalalignment='center', verticalalignment='center', transform=axs.transAxes)

	if showInsteadOfSave:
		plt.show(block=False)
	else:
		outfilename = './Figures/PbPb_SV_scaling_kt450MeV.pdf'
		print 'Saving to', outfilename
		plt.savefig(outfilename, bbox_inches='tight')
		plt.close()







#====================================================
if __name__ == "__main__":

	# Make surface plots
	'''make_surface_plot('pp')
	make_surface_plot('pp_SoE2')
	make_surface_plot('3pp')
	make_surface_plot('pPb')
	make_surface_plot('PbPb')'''
	#plot_radii_vs_dNchdeta()
	#plot_radii_vs_KT('pp')
	#plot_radii_vs_KT('pPb')
	#plot_radii_vs_KT('PbPb')
	#plot_radii_vs_KT_and_dNchdeta('pp')
	#plot_radii_vs_KT_and_dNchdeta('pp_SoE2')
	#plot_SVs_vs_KT_and_dNchdeta()
	#for frame in xrange(1,len(ppCentralities)+1):
	#	make_surface_plots(frame)
	plot_radii_vs_dNchdeta('250','250','250')
	plot_radii_vs_dNchdeta('400','400','400')
	plot_radii_vs_dNchdeta('450','450','450')
	if showInsteadOfSave:
		pause()



# End of file

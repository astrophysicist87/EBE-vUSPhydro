#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <vector>
#include <map>

#include "SPH.h"
#include "int.h"
#include "spectra.h"
#include "tables.h"

using namespace std;



int main (int argc, char *argv[])
{
	_inputIC ics;
	
	if (argv[1])
	{
	ics.man= argv[1];
	
	
	
	if (argv[3])
	{
	stringstream s,s1;
	s << argv[3];
	s >> ics.start;
	
	ics.end=ics.start;
	ics.on=1;
	}
	else {	
	ics.on=0;
	}
	
	}
	else
	{
	ics.man="input.dat";
	ics.on=0;
	}

	


	cout << "read1" <<endl;


	SPH<2,3> sph;
	sph.readin(ics);
	cout << "read2" << endl;
	int qmst=0;
	if ((sph.typ==1)||(sph.typ==3) )
  	{
  		sph.flist();	
  	}

        cout << sph.typ << endl;
        cout << argv[2] << endl;
	cout << argv[4] << endl;
	cout <<	argv[5] << endl;


	
  	if (argv[2])
	{
	  cout << "here " << endl;
	  stringstream s2a;
        s2a << argv[2];
        s2a >> sph.rnum;
	//sph.rnum=argv[2];
	cout << sph.rnum << endl;
	
	}
	if (argv[4])
	{
	  cout << "here" << endl;
	sph.neg=argv[4];
	cout << sph.neg << endl;
	}
	if (argv[5])
	{
	  cout << "here" << endl;
	stringstream s2;
	s2 << argv[5];
	s2 >> qmst;
	cout << qmst << endl;
	
	}

	
	cout << "readin done" <<endl;
  	
  	
	const bool set_spacetime_moments = true;
  	vector<int> HBTparticleIDs;
	HBTparticleIDs.push_back( 211 );
	HBTparticleIDs.push_back( 321 );
	std::map<int,string> HBTparticleNames;
	HBTparticleNames[211] = "pion_+";
	HBTparticleNames[321] = "kaon_+";
  	
  	list l(sph.pt.size(),sph.phi.size());
  	l.setup(sph.pt,sph.phi);
  	
  	// set up calculation of correlation function
	const double deltaQ = 0.075; // GeV
	const int nQX = 1, nQY = 1, nQZ = 1;	// should all be odd to guarantee Q=0 included
	vector<double> QXpts(nQX), QYpts(nQY), QZpts(nQZ);
	for (int iQX = 0; iQX < nQX; iQX++) QXpts[iQX] = -0.5*(nQX-1)*deltaQ + deltaQ*iQX;
	for (int iQY = 0; iQY < nQY; iQY++) QYpts[iQY] = -0.5*(nQY-1)*deltaQ + deltaQ*iQY;
	for (int iQZ = 0; iQZ < nQZ; iQZ++) QZpts[iQZ] = -0.5*(nQZ-1)*deltaQ + deltaQ*iQZ;
  	FTlist FTl(sph.pt.size(), sph.phi.size(), nQX, nQY, nQZ);
  	FTl.setup(sph.pt, sph.phi, QXpts, QYpts, QZpts);
  	
  	
	cout <<"setup done" << endl;
	
	string ofolder="out/"+sph.folder;
	string zero="0";
	if (argv[6]&&argv[6]!=zero)  {
           string newout=argv[6];
           ofolder=ofolder+"/"+newout;
        }
	if (qmst==1) ofolder=ofolder+"/qm";
	const double pisub=pow(2*PI,3);
	 //switches end result into 1/[GeV]^2
	
	cout << ofolder << endl;

	if (sph.typ==1) //bulk
	{
		for (int i=0;i<sph.NHAD;i++) // sets up bulk coefficients for each hadron
		{
		sph.had[i].vfac=sph.had[i].deg/pisub;
		sph.setcoef(i);
		sph.calcsPI(i);
		sph.had[i].form3=0.25*pow(sph.had[i].mass,3);
		sph.had[i].halm2=0.5*pow(sph.had[i].mass,2);
		}
	}
	else if (sph.typ==2) //shear
	{
	for (int i=0;i<sph.NHAD;i++) // sets up bulk coefficients for each hadron
		{
		sph.had[i].form3=0.25*pow(sph.had[i].mass,3);
		sph.had[i].halm2=0.5*pow(sph.had[i].mass,2);
		sph.had[i].svfac=sph.had[i].deg/pisub*sph.scale;
		}
	}
	else if (sph.typ==3) //bulk+shear
	{
		for (int i=0;i<sph.NHAD;i++) // sets up bulk coefficients for each hadron
		{
		sph.had[i].vfac=sph.had[i].deg/pisub;
		sph.had[i].svfac=sph.had[i].vfac*sph.scale;
		sph.had[i].form3=0.25*pow(sph.had[i].mass,3);
		sph.had[i].halm2=0.5*pow(sph.had[i].mass,2);
		sph.setcoef(i);
		sph.calcsPI(i);
		}	
	}
	else {
		for (int i=0;i<sph.NHAD;i++) // sets up bulk coefficients for each hadron
		{
		sph.had[i].vfac=sph.had[i].deg/pisub;
		}
	}
	
	sph.printrun(ofolder);
  	cout << ofolder << endl;
	
	
	ofstream OUT;
	for (int ev=sph.start;ev<=sph.end;ev++) // runs over all the events
	{	
	
	  
	sph.readin2(ev);

	
	//multiply shear spectra prefactor by entropy
//	if (sph.typ>=2){
//	if (ev==sph.start){
//		for (int i=0;i<sph.NHAD;i++) 
//		{
//		sph.had[i].svfac/=sph.s;
//		}
//	}
//	}
	
	string conev=sph.convertInt(ev);
	string out1,out2;
	printstart<2,3>(ofolder,conev,out1,out2,sph);
	
	
	sph.checknu();
	
	
	
	for (int h=0;h<sph.NHAD;h++) // runs over the number of hadrons
	{
		
		const bool do_HBT_on_this_particle
					= bool( std::count( HBTparticleIDs.begin(),
										HBTparticleIDs.end(),
										sph.had[h].id ) > 0 );
		
	  //		cout << "\r" << sph.had[h].id << " " << h << "= Hadron #"  <<flush;

	        if (sph.had[h].null!=2){

		if (sph.typ==0){
		for (int ps=0;ps<l.pTmax;ps++)
		{
			for(int i=0;i<l.phimax;i++)
			{
				l.dNdpdphi.x[ps][i]
					= sph.dNdpdphi( l.pt.x[ps][i],l.phi.x[ps][i],sph.had[h],
									do_HBT_on_this_particle and set_spacetime_moments);
				if ( do_HBT_on_this_particle and set_spacetime_moments )
				{						
					l.stm_S.x[ps][i]   = sph.ST_out[0];
					l.stm_xS.x[ps][i]  = sph.ST_out[1];
					l.stm_yS.x[ps][i]  = sph.ST_out[2];
					l.stm_zS.x[ps][i]  = sph.ST_out[3];
					l.stm_tS.x[ps][i]  = sph.ST_out[4];
					l.stm_x2S.x[ps][i] = sph.ST_out[5];
					l.stm_xyS.x[ps][i] = sph.ST_out[6];
					l.stm_y2S.x[ps][i] = sph.ST_out[7];
					l.stm_xzS.x[ps][i] = sph.ST_out[8];
					l.stm_yzS.x[ps][i] = sph.ST_out[9];
					l.stm_z2S.x[ps][i] = sph.ST_out[10];
					l.stm_xtS.x[ps][i] = sph.ST_out[11];
					l.stm_ytS.x[ps][i] = sph.ST_out[12];
					l.stm_ztS.x[ps][i] = sph.ST_out[13];
					l.stm_t2S.x[ps][i] = sph.ST_out[14];

					// switch from XYZ --> OSL
					const double ckp     = cos(l.phi.x[ps][i]),
								 skp     = sin(l.phi.x[ps][i]);
					const double xSloc   = l.stm_xS.x[ps][i],  ySloc  = l.stm_xS.x[ps][i];
					const double x2Sloc  = l.stm_x2S.x[ps][i], xySloc = l.stm_xyS.x[ps][i],
								 y2Sloc  = l.stm_y2S.x[ps][i];
					const double xtSloc  = l.stm_xtS.x[ps][i], ytSloc = l.stm_ytS.x[ps][i],
								 xzSloc  = l.stm_xtS.x[ps][i], yzSloc = l.stm_ytS.x[ps][i];
					l.stm_xoS.x[ps][i]   = ckp*xSloc+skp*ySloc;
					l.stm_xsS.x[ps][i]   = ckp*ySloc-skp*xSloc;
					l.stm_xlS.x[ps][i]   = l.stm_zS.x[ps][i];
					l.stm_xo2S.x[ps][i]  = ckp*ckp*x2Sloc+skp*skp*y2Sloc+2.0*ckp*skp*xySloc;
					l.stm_xs2S.x[ps][i]  = ckp*ckp*y2Sloc+skp*skp*x2Sloc-2.0*ckp*skp*xySloc;
					l.stm_xl2S.x[ps][i]  = l.stm_z2S.x[ps][i];
					l.stm_xoxsS.x[ps][i] = (ckp*ckp*-skp*skp)*xySloc+ckp*skp*(y2Sloc-x2Sloc);
					l.stm_xotS.x[ps][i]  = ckp*xtSloc+skp*ytSloc;
					l.stm_xstS.x[ps][i]  = ckp*ytSloc-skp*xtSloc;
					l.stm_xltS.x[ps][i]  = l.stm_ztS.x[ps][i];
					l.stm_xoxlS.x[ps][i] = ckp*xzSloc+skp*yzSloc;
					l.stm_xsxlS.x[ps][i] = ckp*yzSloc-skp*xzSloc;
				}
			}

	 	}
	 	}
	 	else{
	 	for (int ps=0;ps<l.pTmax;ps++)
		{
			for(int i=0;i<l.phimax;i++) {
			l.dNdpdphi.x[ps][i]
				= sph.dNdpdphi(l.pt.x[ps][i],l.phi.x[ps][i],sph.had[h],
										do_HBT_on_this_particle and set_spacetime_moments );
			l.dNdpdphic.x[ps][i]=sph.outc;
					if ( do_HBT_on_this_particle and set_spacetime_moments )
					{						
						l.stm_S.x[ps][i]    = sph.ST_out[0];
						l.stm_xS.x[ps][i]   = sph.ST_out[1];
						l.stm_yS.x[ps][i]   = sph.ST_out[2];
						l.stm_zS.x[ps][i]   = sph.ST_out[3];
						l.stm_tS.x[ps][i]   = sph.ST_out[4];
						l.stm_x2S.x[ps][i]  = sph.ST_out[5];
						l.stm_xyS.x[ps][i]  = sph.ST_out[6];
						l.stm_y2S.x[ps][i]  = sph.ST_out[7];
						l.stm_xzS.x[ps][i]  = sph.ST_out[8];
						l.stm_yzS.x[ps][i]  = sph.ST_out[9];
						l.stm_z2S.x[ps][i]  = sph.ST_out[10];
						l.stm_xtS.x[ps][i]  = sph.ST_out[11];
						l.stm_ytS.x[ps][i]  = sph.ST_out[12];
						l.stm_ztS.x[ps][i]  = sph.ST_out[13];
						l.stm_t2S.x[ps][i]  = sph.ST_out[14];
	
						l.stm_Sc.x[ps][i]   = sph.ST_outc[0];
						l.stm_xSc.x[ps][i]  = sph.ST_outc[1];
						l.stm_ySc.x[ps][i]  = sph.ST_outc[2];
						l.stm_zSc.x[ps][i]  = sph.ST_outc[3];
						l.stm_tSc.x[ps][i]  = sph.ST_outc[4];
						l.stm_x2Sc.x[ps][i] = sph.ST_outc[5];
						l.stm_xySc.x[ps][i] = sph.ST_outc[6];
						l.stm_y2Sc.x[ps][i] = sph.ST_outc[7];
						l.stm_xzSc.x[ps][i] = sph.ST_outc[8];
						l.stm_yzSc.x[ps][i] = sph.ST_outc[9];
						l.stm_z2Sc.x[ps][i] = sph.ST_outc[10];
						l.stm_xtSc.x[ps][i] = sph.ST_outc[11];
						l.stm_ytSc.x[ps][i] = sph.ST_outc[12];
						l.stm_ztSc.x[ps][i] = sph.ST_outc[13];
						l.stm_t2Sc.x[ps][i] = sph.ST_outc[14];
	
						// switch from XYZ --> OSL
						const double ckp     = cos(l.phi.x[ps][i]),
									 skp     = sin(l.phi.x[ps][i]);
						const double xSloc   = l.stm_xS.x[ps][i],  ySloc  = l.stm_xS.x[ps][i];
						const double x2Sloc  = l.stm_x2S.x[ps][i], xySloc = l.stm_xyS.x[ps][i],
									 y2Sloc  = l.stm_y2S.x[ps][i];
						const double xtSloc  = l.stm_xtS.x[ps][i], ytSloc = l.stm_ytS.x[ps][i],
									 xzSloc  = l.stm_xtS.x[ps][i], yzSloc = l.stm_ytS.x[ps][i];
						l.stm_xoS.x[ps][i]   = ckp*xSloc+skp*ySloc;
						l.stm_xsS.x[ps][i]   = ckp*ySloc-skp*xSloc;
						l.stm_xlS.x[ps][i]   = l.stm_zS.x[ps][i];
						l.stm_xo2S.x[ps][i]  = ckp*ckp*x2Sloc+skp*skp*y2Sloc+2.0*ckp*skp*xySloc;
						l.stm_xs2S.x[ps][i]  = ckp*ckp*y2Sloc+skp*skp*x2Sloc-2.0*ckp*skp*xySloc;
						l.stm_xl2S.x[ps][i]  = l.stm_z2S.x[ps][i];
						l.stm_xoxsS.x[ps][i] = (ckp*ckp*-skp*skp)*xySloc+ckp*skp*(y2Sloc-x2Sloc);
						l.stm_xotS.x[ps][i]  = ckp*xtSloc+skp*ytSloc;
						l.stm_xstS.x[ps][i]  = ckp*ytSloc-skp*xtSloc;
						l.stm_xltS.x[ps][i]  = l.stm_ztS.x[ps][i];
						l.stm_xoxlS.x[ps][i] = ckp*xzSloc+skp*yzSloc;
						l.stm_xsxlS.x[ps][i] = ckp*yzSloc-skp*xzSloc;

						const double xScloc   = l.stm_xSc.x[ps][i],  yScloc  = l.stm_xSc.x[ps][i];
						const double x2Scloc  = l.stm_x2Sc.x[ps][i], xyScloc = l.stm_xySc.x[ps][i],
									 y2Scloc  = l.stm_y2Sc.x[ps][i];
						const double xtScloc  = l.stm_xtSc.x[ps][i], ytScloc = l.stm_ytSc.x[ps][i],
									 xzScloc  = l.stm_xtSc.x[ps][i], yzScloc = l.stm_ytSc.x[ps][i];
						l.stm_xoSc.x[ps][i]   = ckp*xScloc+skp*yScloc;
						l.stm_xsSc.x[ps][i]   = ckp*yScloc-skp*xScloc;
						l.stm_xlSc.x[ps][i]   = l.stm_zSc.x[ps][i];
						l.stm_xo2Sc.x[ps][i]  = ckp*ckp*x2Scloc+skp*skp*y2Scloc+2.0*ckp*skp*xyScloc;
						l.stm_xs2Sc.x[ps][i]  = ckp*ckp*y2Scloc+skp*skp*x2Scloc-2.0*ckp*skp*xyScloc;
						l.stm_xl2Sc.x[ps][i]  = l.stm_z2Sc.x[ps][i];
						l.stm_xoxsSc.x[ps][i] = (ckp*ckp*-skp*skp)*xyScloc+ckp*skp*(y2Scloc-x2Scloc);
						l.stm_xotSc.x[ps][i]  = ckp*xtScloc+skp*ytScloc;
						l.stm_xstSc.x[ps][i]  = ckp*ytScloc-skp*xtScloc;
						l.stm_xltSc.x[ps][i]  = l.stm_ztSc.x[ps][i];
						l.stm_xoxlSc.x[ps][i] = ckp*xzScloc+skp*yzScloc;
						l.stm_xsxlSc.x[ps][i] = ckp*yzScloc-skp*xzScloc;

						/*cout << "HBT (ideal): " << l.pt.x[ps][i] << "   " << l.phi.x[ps][i]
								<< "   " << l.dNdpdphi.x[ps][i];
						for (int ii = 0; ii < 15; ii++) cout << "   " << sph.ST_out[ii];
						cout << endl << "HBT (viscous): " << l.pt.x[ps][i] << "   " << l.phi.x[ps][i]
								<< "   " << l.dNdpdphic.x[ps][i];
						for (int ii = 0; ii < 15; ii++) cout << "   " << sph.ST_outc[ii];
						cout << endl;*/
	
					}

					/*cout << "Check spectra: "
						<< sph.dNdpdphi(l.pt.x[ps][i],l.phi.x[ps][i],sph.had[h]) << endl;
					cout << "Check FT spectra: "
						<< sph.dNdpdphi_FT(l.pt.x[ps][i],l.phi.x[ps][i],0.0,sph.had[h],
											0.0, 0.0, 0.0, 0.0) << endl;*/

					// if this is one of the HBT particles, do HBT!
					if ( do_HBT_on_this_particle )
					{
						// set grid.params file as well
						string particleName = HBTparticleNames[sph.had[h].id];
						string gridParamsFilename = ofolder + "/" + particleName
													+ "_grid.params";
						ofstream gridParams( gridParamsFilename.c_str(), ios::out );
						gridParams << l.pTmax << std::endl;
						gridParams << l.phimax << std::endl;
						gridParams << nQX << std::endl;
						gridParams << nQY << std::endl;
						gridParams << nQZ << std::endl;
						gridParams.close();

						string outfilename = ofolder + "/" + particleName
												+ "_HBT_correlation_function.dat";
						ofstream outHBT( outfilename.c_str(), ios::out | ios::app );

						const double mass  = sph.had[h].mass;
						const double KT    = l.pt.x[ps][i];
						const double Kphi  = l.phi.x[ps][i];
						const double KYrap = 0.0;
						const double ckp   = cos(Kphi),
									 skp   = sin(Kphi);

						// use HBT cache to speed things up
						sph.NQZ = nQZ;
						sph.set_HBT_CACHE();
		
						for ( int iQX = 0; iQX < nQX; iQX++ )
						for ( int iQY = 0; iQY < nQY; iQY++ )
						for ( int iQZ = 0; iQZ < nQZ; iQZ++ )
						{
							sph.iQZ = iQZ;
							const double xi2  = mass*mass + KT*KT
												+ 0.25*( QXpts[iQX]*QXpts[iQX]
														+ QYpts[iQY]*QYpts[iQY]
														+ QZpts[iQZ]*QZpts[iQZ] );
						
							const double QOUT = ckp * QXpts[iQX] + skp * QYpts[iQY];
							const double Q0   = sqrt(xi2 + QOUT*KT) - sqrt(xi2 - QOUT*KT);
		
							// use built-in indexer
							const int FTl_index
								= FTl.FTdNdpdphi.index5D(ps, i, iQX, iQY, iQZ);
							FTl.FTdNdpdphi.x[ FTl_index ]
								= sph.dNdpdphi_FT( KT, Kphi, KYrap, sph.had[h],
													Q0, QXpts[iQX], QYpts[iQY], QZpts[iQZ] );
							FTl.FTdNdpdphic.x[ FTl_index ] = sph.outc_comp;
	//cout << "Check all spectra: " << l.pt.x[ps][i] << "   " << l.phi.x[ps][i] << "   "
	//		<< l.dNdpdphi.x[ps][i] << "   " << l.dNdpdphic.x[ps][i] << "   "
	//		<< FTl.FTdNdpdphi.x[ FTl_index ] << "   " << FTl.FTdNdpdphic.x[ FTl_index ] << endl;

							complex<double> ratio = FTl.FTdNdpdphi.x[ FTl_index ]
													/ l.dNdpdphi.x[ps][i];
							complex<double> ratioc = FTl.FTdNdpdphic.x[ FTl_index ]
													/ l.dNdpdphic.x[ps][i];
							const double CF = 1.0 + abs(ratio)*abs(ratio);
							const double CFc = 1.0 + abs(ratioc)*abs(ratioc);

							// finally, output everything to appropriate files
							outHBT << l.pt.x[ps][i] << "   "
								   << l.phi.x[ps][i] << "   "
								   << QXpts[iQX] << "   "
								   << QYpts[iQY] << "   "
								   << QZpts[iQZ] << "   "
								   << l.dNdpdphi.x[ps][i] << "   "
								   << l.dNdpdphic.x[ps][i] << "   "
								   << FTl.FTdNdpdphi.x[ FTl_index ].real() << "   "
								   << FTl.FTdNdpdphi.x[ FTl_index ].imag() << "   "
								   << FTl.FTdNdpdphic.x[ FTl_index ].real() << "   "
								   << FTl.FTdNdpdphic.x[ FTl_index ].imag() << "   "
								   << CF << "   " << CFc << endl;

						}	// end of Q loops

					 	outHBT.close();
					}		// end of if condition (test HBT particle)

//			if (l.dNdpdphi.x[ps][i]<0||isnan(l.dNdpdphi.x[ps][i])) l.dNdpdphi.x[ps][i]=0;
//			if (l.dNdpdphic.x[ps][i]<0||isnan(l.dNdpdphic.x[ps][i])) l.dNdpdphic.x[ps][i]=0;
			if (isnan(l.dNdpdphi.x[ps][i])) l.dNdpdphi.x[ps][i]=0;
			if (isnan(l.dNdpdphic.x[ps][i])) l.dNdpdphic.x[ps][i]=0;
			}
	 	}
	 	
	 	}
		

		if ( do_HBT_on_this_particle and set_spacetime_moments )
		{
			// set this using source variances defined above
			l.compute_HBT_radii( sph.had[h].mass );
	
			// then print to file
			string particleName = HBTparticleNames[sph.had[h].id];
			string outHBTfilename = ofolder + "/" + particleName + "_HBT_radii_SV.dat";
			ofstream outHBT( outHBTfilename.c_str(), ios::out );

			for (int ps=0;ps<l.pTmax;ps++)
			for (int i=0;i<l.phimax;i++)
				outHBT
					<< l.pt.x[ps][i]     << "   " << l.phi.x[ps][i]    << "   "
					<< l.R2o.x[ps][i]    << "   " << l.R2s.x[ps][i]    << "   " << l.R2l.x[ps][i]    << "   "
					<< l.R2os.x[ps][i]   << "   " << l.R2ol.x[ps][i]   << "   " << l.R2sl.x[ps][i]   << "   "
					<< l.R2o_c.x[ps][i]  << "   " << l.R2s_c.x[ps][i]  << "   " << l.R2l_c.x[ps][i]  << "   "
					<< l.R2os_c.x[ps][i] << "   " << l.R2ol_c.x[ps][i] << "   " << l.R2sl_c.x[ps][i] << std::endl;

			outHBT.close();
		}
		
		//print off spectra
		if (sph.typ>0) printc(out2,h,l,sph);
//		else print(out1,h,l,sph);
 		print(out1,h,l,sph);
		
		if (sph.had[h].anti==1){
		  int nsub=-sph.had[h].id;
		  if (sph.typ>0) nprintc(out2,h,l,sph,nsub);
		  else nprint(out1,h,l,sph,nsub);
		  
		}

                if (sph.had[h].null==1){
		  int ssid=sph.had[h].sids.size();
		  for (int ss=0;ss<ssid;ss++){
		    int hsub=sph.had[h].sids[ss];
		    if (sph.typ>0) nprintc(out2,h,l,sph,sph.had[hsub].id);
		    else nprint(out1,h,l,sph,sph.had[hsub].id);
		    
		    if (sph.had[hsub].anti==1){
		      if (sph.typ>0) nprintc(out2,h,l,sph,-sph.had[hsub].id);
		      else nprint(out1,h,l,sph,-sph.had[hsub].id);
		    }
		  }
		}


		
		}
		
	 	
		
		
		
	}
	
	
	delete [] sph.par;
	delete [] sph.qv;
	}
	
	if (sph.typ>0){
	 l.destroyc();
	}
	else l.destroy();
	
	
	if (sph.typ>0) delete [] sph.hlist;
	 

 return 0;
}

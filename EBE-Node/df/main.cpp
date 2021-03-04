#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <vector>

#include "SPH.h"
#include "int.h"
#include "spectra.h"
#include "tables.h"

using namespace std;



int main (int argc, char *argv[])
{
	_inputIC ics;
	
	if (argc>1)
	{
	ics.man= argv[1];
	
	
	
	if (argc>3)
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

	





	SPH<2,3> sph;
	sph.readin(ics);
	int qmst=0;
	if ((sph.typ==1)||(sph.typ==3) )
  	{
  		sph.flist();	
  	}
  	
  	if (argc>2)
	{
	sph.rnum=argv[2];
	
	}
	if (argc>4)
	{
	sph.neg=argv[4];
        cout << sph.neg << endl;
	}
	if (argc>5)
	{
	stringstream s2;
	s2 << argv[5];
	s2 >> qmst;
	
	}

	

  	
  	
  	list l(sph.pt.size(),sph.phi.size());
  	l.setup(sph.pt,sph.phi);
  	
  	
	const int nQX = 7, nQY = 7, nQZ = 7;
  	FTlist FTl(sph.pt.size(), sph.phi.size(), nQX, nQY, nQZ);
  	FTl.setup()

	
	string ofolder="out/"+sph.folder;
	string zero="0";
	if (argc>6&&argv[6]!=zero)  {
           string newout=argv[6];
           ofolder=ofolder+"/"+newout;
        }
	if (qmst==1) ofolder=ofolder+"/qm";
	const double pisub=pow(2*PI,3);
	 //switches end result into 1/[GeV]^2
	
       

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
		sph.had[i].svfac=sph.had[i].deg/pisub/pow(sph.T,3)*sph.scale;
		}
	}
	else if (sph.typ==3) //bulk+shear
	{
		for (int i=0;i<sph.NHAD;i++) // sets up bulk coefficients for each hadron
		{
		sph.had[i].vfac=sph.had[i].deg/pisub;
		sph.had[i].svfac=sph.had[i].vfac/pow(sph.T,3)*sph.scale;
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
		
		
	  //		cout << "\r" << sph.had[h].id << " " << h << "= Hadron #"  <<flush;

	        if (sph.had[h].null!=2){

		if (sph.typ==0){
		for (int ps=0;ps<l.pTmax;ps++)
		{
			for(int i=0;i<l.phimax;i++)
			{
				l.dNdpdphi.x[ps][i]=sph.dNdpdphi(l.pt.x[ps][i], l.phi.x[ps][i],
											sph.had[h], set_spacetime_moments );
				if ( set_spacetime_moments )
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
				}

				/*cout << "Check spectra: "
					<< sph.dNdpdphi(l.pt.x[ps][i],l.phi.x[ps][i],sph.had[h]) << endl;
				cout << "Check FT spectra: "
					<< sph.dNdpdphi_FT(l.pt.x[ps][i],l.phi.x[ps][i],0.0,sph.had[h],
										0.0, 0.0, 0.0, 0.0) << endl;*/
			}
	 	}
	 	}
	 	else{
	 	for (int ps=0;ps<l.pTmax;ps++)
		{
			for(int i=0;i<l.phimax;i++)
			{
				l.dNdpdphi.x[ps][i]=sph.dNdpdphi(l.pt.x[ps][i],l.phi.x[ps][i],sph.had[h]);
				l.dNdpdphic.x[ps][i]=sph.outc;
				if ( set_spacetime_moments )
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
				}
				/*cout << "Check spectra: "
					<< sph.dNdpdphi(l.pt.x[ps][i],l.phi.x[ps][i],sph.had[h]) << endl;
				cout << "Check FT spectra: "
					<< sph.dNdpdphi_FT(l.pt.x[ps][i],l.phi.x[ps][i],0.0,sph.had[h],
										0.0, 0.0, 0.0, 0.0) << endl;*/
				double tmplocal
						= sph.dNdpdphi_FT( l.pt.x[ps][i], l.phi.x[ps][i], 0.0,
											sph.had[h], 0.0, 0.0, 0.0, 0.0 );
	//			if (l.dNdpdphi.x[ps][i]<0||isnan(l.dNdpdphi.x[ps][i])) l.dNdpdphi.x[ps][i]=0;
	//			if (l.dNdpdphic.x[ps][i]<0||isnan(l.dNdpdphic.x[ps][i])) l.dNdpdphic.x[ps][i]=0;
				if (isnan(l.dNdpdphi.x[ps][i])) l.dNdpdphi.x[ps][i]=0;
				if (isnan(l.dNdpdphic.x[ps][i])) l.dNdpdphic.x[ps][i]=0;
			}
	 	}
	 	
	 	}
		
		
		//print off spectra
		if (sph.typ>0) printc(out2,h,l,sph);
		else print(out1,h,l,sph);

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

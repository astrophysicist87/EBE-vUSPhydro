#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <vector>
# include "newspline.hpp"

#include "SPH.h"
#include "int.h"
#include "global.h"

using namespace std;

vector<vector<SPEC> > spec;

template <int D,int DD>
void vnint(double ptmin, double ptmax,double dpt,  SPH<D,DD> &sph);



int main (int argc, char *argv[])
{
	
	
	int ev0,evf,lhc=0;
	string hadtyp,vnfile;
	if (argv[1]&&argv[2]&&argv[3])
	{
	vnfile=argv[1];
	
		
	stringstream s,s1;
	s << argv[2];
	s >> ev0;
	
	s1 << argv[3];
	s1 >> evf;
	
	if (argv[4])
	{
	hadtyp=argv[4];
	}
	
	
	
	
	}
	else 
	{
	cout << " Error: missing intput of first and last event!" << endl;
	exit(1);
	}
	
	if (argv[5])
	{
	string nlhc="lhc";
	if (nlhc==argv[5]) lhc=1;
	else lhc=0;
	
	}
	

	SPH<2,3> sph;
	sph.readin(ev0,evf,hadtyp,vnfile);
  	
  	if (argv[6])
	{
	stringstream s3;
	s3 << argv[6];
	s3 >> sph.decays;
	}
  	
  	

	//g.fixptab(sph.pt_s,sph.pt_e,sph.pt_step); //sets up number of steps in pt
	string ofolder="out/"+sph.folder;
	//const double pisub=pow(2*PI,3);
	 //switches end result into 1/[GeV]^2
	string viname;
	sph.setvns(spec.size() ); // sets up the list of v_n's that will be computed
	
	
	
	viname="int_"+sph.hadname+".dat";
	if (sph.decays==0) viname="int_"+sph.hadname+"ND.dat";
	if (sph.decays==2) viname="int_"+sph.hadname+"negND.dat";
	
	
	p_vintstart(ofolder,viname); // sets up file for integrated v_n yields
	sph.ide=new VNVAR[sph.vnmax];  //initializes n (of v_n) dependent terms for the ideal df
	sph.Ia=new double[spec.size()];
	
  	
	
	
	ofstream OUT;
	for (int ev=sph.start;ev<=sph.end;ev++) // runs over all the events
	{	
	
	
	sph.readin2(ev);
	
	string conev=sph.convertInt(ev);
	string out1,out2;
	printstart<2,3>(ofolder,conev,out1,out2,sph);
	
	
		
	
	
		int ptmax=spec.size();
		
		sph.restart(ptmax); 
		
		int ptmin=0;
		
			
	
		
		//for(int j=0;j<sph.vnmax;j++) 
		
		ofstream OUT2;
		OUT2.open(out2.c_str(), ios::out | ios::app );
		
		for (int ps=ptmin;ps<ptmax;ps++)
		{
		int phimax=spec[ps].size();
		
		for(int j=0;j<sph.vnmax;j++)
		{
		sph.ide[j].I.x[0]=0;
		sph.ide[j].I.x[1]=0;}
	
		for(int i=0;i<phimax;i++)
		{	
		double multi=spec[ps][i].dNdpt*spec[ps][i].wphi;
		
		sph.Ia[ps]+=multi;	
		for(int j=0;j<sph.vnmax;j++)
		{
		double theta=sph.ntab[j]*spec[ps][i].phi;
	
		sph.ide[j].I.x[0]+=cos(theta)*multi;
		sph.ide[j].I.x[1]+=sin(theta)*multi;
	
		}}
		
		
		
		
		
		OUT2 <<  spec[ps][0].pt ;
		
		for(int j=0;j<sph.vnmax;j++)
		{
		
		sph.ide[j].Q=spec[ps][0].pt*sph.ide[j].I;
		sph.ide[j].psi=atan2(sph.ide[j].Q.x[1],sph.ide[j].Q.x[0])/sph.ntab[j];
		
		
		for(int i=0;i<phimax;i++)
		{
		sph.vout[j][ps]+=spec[ps][i].dNdpt*spec[ps][i].wphi*cos(sph.ntab[j]*(spec[ps][i].phi-sph.ide[j].psi));
		}
		
		OUT2  << "  " << sph.vout[j][ps]/sph.Ia[ps]  << "  " << sph.ide[j].psi ;
		}
		OUT2 << endl;
	
		}
		
		
		OUT2.close();

		
		
		
		
//		double interb=0;
//		for (int ps=ptmin;ps<ptmax;ps++) interb+=spec[ps][0].pt*spec[ps][0].wpt*sph.Ia[ps];
		//calculates final v_n's
		
		 //redo both
		//cout << sph.ide[1].intert << " " << interb<<  endl;
		//for(int j=0;j<sph.vnmax;j++) sph.ide[j].intert/=interb;

		
	
		
		
		
		
		
	
	
	
	}
	
	sph.destroyvout(spec.size() );
	
	delete [] sph.ide;
	delete [] sph.Ia;
	
	
	 

 return 0;
}


template <int D,int DD>
void vnint(double ptmin, double ptmax,double dpt,  SPH<D,DD> &sph){
 
	int pmax=spec.size();
	int min,max;
	int lmin=0,lmax=0;
	for (int ps=ptmin;ps<pmax;ps++)
	{
		if ((lmin==0)&&(spec[ps][0].pt>=ptmin)){
		lmin=1;
		 min=ps;}
		if ((lmax==0)&&(spec[ps][0].pt>=ptmax)){
		lmax=1;
		max=ps;}
	
	
	}
	if (lmax==0) max=pmax;
	
	
	for (int ps=min;ps<max;ps++)
	{
		spec[ps][0].wpt=spec[ps][0].pt-spec[ps-1][0].pt;
		
	
	}
	
	
	
	for (int ps=min;ps<max;ps++)
	{
		gausslegendrequad<2,3>(ps,sph);
	 	double p2=spec[ps][0].pt*spec[ps][0].wpt;
	 	for (int j=0;j<sph.vnmax;j++) sph.ide[j].Q+=p2*sph.ide[j].I;
	 }	
	
		
	for(int j=0;j<sph.vnmax;j++) sph.ide[j].psi=atan2(sph.ide[j].Q.x[1],sph.ide[j].Q.x[0])/sph.ntab[j];
		
	for (int ps=min;ps<max;ps++)
	{
		int phimax=spec[ps].size();
		for(int i=0;i<phimax;i++)
		{
		for(int j=0;j<sph.vnmax;j++)
		{
		sph.vout[j][ps]+=spec[ps][i].dNdpt*spec[ps][i].wphi*cos(sph.ntab[j]*(spec[ps][i].phi-sph.ide[j].psi));
		}
		}
	
	
		for(int j=0;j<sph.vnmax;j++)
		{
		sph.ide[j].intert+=spec[ps][0].pt*spec[ps][0].wpt*sph.vout[j][ps];
		}
	}
	
	double interb=0;
	for (int ps=min;ps<max;ps++) interb+=spec[ps][0].pt*spec[ps][0].wpt*sph.Ia[ps];
		
		
		
	for(int j=0;j<sph.vnmax;j++) sph.ide[j].intert/=interb;
	
	
	
}




//template <int D,int DD>
//void vnint(double ptmin, double ptmax,double dpt,  SPH<D,DD> &sph){

//	int max=15;
//	sph.restart(max);
//	
//	//for (int ps=0;ps<15;ps++) cout << spec[ps][0].pt << " " <<  spec[ps][1].dNdpt << endl;

//	double pt=ptmin;
//	int pi=0;
//	while (pt<=ptmax)
//	{
//	
//	
//	
//	double dpts=dpt;
//	if ((pi==0)||(pt>(ptmax-0.5*dpt))) dpts=0.5*dpt; 
// 	glint<2,3>(pt,dpts,pi,sph); 	
//	double p2=pt*dpts;
//	for (int j=0;j<sph.vnmax;j++) sph.ide[j].Q+=p2*sph.ide[j].I;
//	spec[pi][0].pt=pt;
//	pt+=dpt;
//	pi++;
//	}
//	max=pi;	

//	for(int j=0;j<sph.vnmax;j++) sph.ide[j].psi=atan2(sph.ide[j].Q.x[1],sph.ide[j].Q.x[0])/sph.ntab[j];
//	cout << sph.ide[1].psi << endl;
//		
//	for (int ps=0;ps<max;ps++)
//	{
//	double dpts=dpt;
//	if ((ps==0)||(ps==max)) dpts=0.5*dpt;
//	
//	
//	int phimax=spec[ps].size();
//	for(int i=0;i<phimax;i++)
//	{
//	for(int j=0;j<sph.vnmax;j++)
//	{
//		sph.vout[j][ps]+=lin(spec[ps][0].pt,i)*spec[0][i].wphi*cos(sph.ntab[j]*(spec[0][i].phi-sph.ide[j].psi));
//	}
//	}
//	
//	
//	for(int j=0;j<sph.vnmax;j++)
//	{
//		sph.ide[j].intert+=spec[ps][0].pt*dpts*sph.vout[j][ps];
//	}
//	}
//		
//	double interb=0;
//	for (int ps=0;ps<max;ps++) {
//	double dpts=dpt;
//	if ((ps==0)||(ps==max)) dpts=0.5*dpt;
//	interb+=spec[ps][0].pt*dpts*sph.Ia[ps];}
//	
//	//cout << sph.ide[1].intert << " " << interb<<  endl;
//	for(int j=0;j<sph.vnmax;j++) sph.ide[j].intert/=interb;
//	
//	
//	
////	cout << sph.ide[1].intert << endl;
////	exit(1);
//	


//}









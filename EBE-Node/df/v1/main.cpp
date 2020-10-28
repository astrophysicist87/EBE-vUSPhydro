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
void vnint(double ptmin, double ptmax,double dpt,  SPH<D,DD> &sph,double rpt);

struct vall
{
  int flag,cen;
  double npart;
  double M,mpt,mpt2,mptvar,dNdpt[30][30];
};



int main (int argc, char *argv[])
{
	
	
	int ev0,evf;
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
	
	double pmin22=0.2;
	double pmax2=1.0;
	if (argv[5]&&argv[6])
	{
	stringstream s2,s3;
	s2 << argv[5];
	s2 >> pmin22;
	
	s3 << argv[6];
	s3 >> pmax2;
	}
	

	SPH<2,3> sph;
	sph.readin(ev0,evf,hadtyp,vnfile);
  	
  	if (argv[7])
	{
	stringstream s3;
	s3 << argv[7];
	s3 >> sph.decays;
	}
  	

	

  	string npfile;
  	//sorting Npart file
  	if (argv[8])
	{
	npfile=argv[8];
	}
  	
	cout << pmin22 << " " << pmax2 << " " << npfile << endl;

	int ptvn=0;//0=<pt>, 1=<vn>
	if (argv[9])
	  {
	  	stringstream s4;
		s4 << argv[9];
		s4 >> ptvn;
	  }

	//g.fixptab(sph.pt_s,sph.pt_e,sph.pt_step); //sets up number of steps in pt
	string ofolder="out/"+sph.folder;
	//const double pisub=pow(2*PI,3);
	 //switches end result into 1/[GeV]^2
	string viname;
	sph.setvns(spec.size() ); // sets up the list of v_n's that will be computed
	
	
	if (ptvn==1){// sets up integrated vn files, only if vn's "on"
	viname="int_"+sph.hadname+"_pt"+argv[5]+"-"+argv[6]+".dat";
	if (sph.decays==0) viname="int_"+sph.hadname+"_pt"+argv[5]+"-"+argv[6]+"ND.dat";
	if (sph.decays==2) viname="int_"+sph.hadname+"_pt"+argv[5]+"-"+argv[6]+"negND.dat";
	if (sph.decays==3) viname="int_"+sph.hadname+"_pt"+argv[5]+"-"+argv[6]+"nodf.dat";
	
	
	
	p_vintstart(ofolder,viname); // sets up file for integrated v_n yields
	}
	
	sph.ide=new VNVAR[sph.vnmax];  //initializes n (of v_n) dependent terms for the ideal df
	sph.Ia=new double[spec.size()];
	
  	
	
	
	ofstream OUT;

	vector<vall> vv;
        vv.resize(sph.end+1);
	
	cout << "reading events" << endl;
	int ptmin=0,ptmax,phimax;
	if (ptvn==0){
	for (int ev=sph.start;ev<=sph.end;ev++) // runs over all the events
	{	
	

	
	vv[ev].flag=sph.readin2(ev);
	if (vv[ev].flag==1){
          cout << "event not read in " << ev << endl;
	  continue;}
	

	if (ev==sph.start){
        ptmax=spec.size();
        phimax=spec[0].size();
        }
        
	sph.restart(ptmax); 
	
	//	for (int ps=0;ps<ptmax;ps++) {
	// for(int i=0;i<phimax;i++)        cout << spec[ps][i].pt << " " << spec[ps][i].phi << " " <<  spec[ps][i].dNdpt << endl;
	//}

	double meanpt=0,botmean=0,meanpt2=0;
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
	    if (ps>0) spec[ps][0].wpt=spec[ps][0].pt-spec[ps-1][0].pt;
	    else spec[0][0].wpt=spec[0][0].pt;


	  }


        for (int ps=min;ps<max;ps++)
	  {
	  	double multi=0;
	  	for(int i=0;i<phimax;i++)	 multi+=spec[ps][i].dNdpt*spec[ps][i].wphi;
	    	 meanpt2+=pow(spec[ps][0].pt,3)*spec[ps][0].wpt*multi;
		 meanpt+=spec[ps][0].pt*spec[ps][0].pt*spec[ps][0].wpt*multi;
		 botmean+=spec[ps][0].pt*spec[ps][0].wpt*multi;
	  }
	
	
//			for (int ps=0;ps<ptmax;ps++)
//		{
//		
//		double multi=0;
//		for(int i=0;i<phimax;i++){
//			
//		
//		 multi+=spec[ps][i].dNdpt*spec[ps][i].wphi;
//		 vv[ev].dNdpt[ps][i]=spec[ps][i].dNdpt;
//		}
//		
//		// mean pt
//		
//                

//		meanpt2+=pow(spec[ps][0].pt,3)*spec[ps][0].wpt*multi;
//		 meanpt+=spec[ps][0].pt*spec[ps][0].pt*spec[ps][0].wpt*multi;
//		 botmean+=spec[ps][0].pt*spec[ps][0].wpt*multi;
//		// mean pt
//	       


	vv[ev].mpt= meanpt/botmean;
	vv[ev].mpt2=meanpt2/botmean;
	vv[ev].M=botmean;
	if (vv[ev].M<=0||vv[ev].M>10000) vv[ev].flag=1;
	}// saves spectra, pt and angles
	}
	else{
	
		for (int ev=sph.start;ev<=sph.end;ev++) // runs over all the events
		{	
	
		vv[ev].flag=sph.readin2(ev);
		if (vv[ev].flag==1) continue;
		
		if (ev==sph.start){
		ptmax=spec.size();
		phimax=spec[0].size();
		}
		
		sph.restart(ptmax);
		
		for (int ps=0;ps<ptmax;ps++)
		{
		for(int i=0;i<phimax;i++){
		 vv[ev].dNdpt[ps][i]=spec[ps][i].dNdpt;
		}}
	
		}// end run over events
		
		string snam="out/"+sph.folder+"/mpt"+sph.hadname+".dat";
		ifstream inputpt(snam.c_str());
		if (!inputpt.is_open())
	 	{
	 	cout << "Can't open " << snam << endl;
	 	exit(1);
	 	}
	 	
	 	for (int ev=sph.start;ev<=sph.end;ev++) // runs over all the events
		{	
		
		string line2;
		getline(inputpt,line2);
		std::vector<std::string> x = split(line2, ' ');
		vector<double> y(5,0);
		
		for (int k=0;k<5;k++){
		stringstream sk;
		sk << x[k];
		sk >> y[k];
		}
		
		vv[ev].npart= y[0];
		vv[ev].mpt= y[1];
		vv[ev].mpt2=y[2];
		vv[ev].M=y[3];
		vv[ev].flag=y[4];
		
		}
		inputpt.close();
	}	
		
	std::string str2 ("/");
  	std::size_t found = sph.folder.find(str2);
  	found=sph.folder.find(str2,found+1);
  	string minifol=sph.folder.substr (0,found);
  	
  	std::size_t last = sph.folder.rfind(str2);
	if (ptvn==0){
  	int siz=sph.folder.size();
  	stringstream s;
  	int num;
  	s << sph.folder.substr (last+1,siz);
  	s >> num;
	string nameb="out/"+minifol+"/npart.dat";
	cout << "npart start" << endl;
	
	ifstream inputb(nameb.c_str());
	if (!inputb.is_open())
 	{
 	cout << "Can't open " << nameb << endl;
 	exit(1);
 	}

	string line;
	
	vector<double> npmin;
	
	int cn=0,pl=0,first=0;
	while (getline(inputb,line)) {
	std::vector<std::string> x = split(line, ' ');
	if (x.size()<2) std::vector<std::string> x = split(line, '\t');


	// CHECK IF NPART FILE HAS TWO COLUMNS OR NOT!!!
	
	stringstream s;
	double snp=0;
	s << x[pl];
	s >> snp;
	
	if (snp==1&&first==0){
	  pl=1;
          stringstream ss;
          ss << x[pl];
          ss >> snp;
          first=1;
	}
	
	
	npmin.push_back(snp);
	cn++;
	}

	inputb.close();
	//done with npart
	
	for (int ev=sph.start;ev<=sph.end;ev++){
		int spot=num*1000+ev;
	    	vv[ev].npart=npmin[spot];
	}
	
	// outputs mean pTonly and exits
	string snam="out/"+sph.folder+"/mpt"+sph.hadname+".dat";
	
	ofstream OUT3;
	OUT3.open(snam.c_str());
	if (!OUT3.is_open())
	{
		cout << "Error: cannot open out2 file!" << endl;
		exit(1);
	}
	
	for (int ev=sph.start;ev<=sph.end;ev++)
	{
		OUT3 <<  vv[ev].npart << "  " <<  vv[ev].mpt << "  " << vv[ev].mpt2  << " " << vv[ev].M << " " << vv[ev].flag<<  endl;		
	}
	OUT3.close();
	exit(0);
	}
	
	
	
		
	// read in centralities
	vector<double> nph,npl;
	vector<double> cmax,cmin;
	string named="out/sorts/"+npfile+".dat";
	ifstream inputd(named.c_str());
	if (!inputd.is_open())
 	{
 	cout << "Can't open " << named << endl;
 	exit(1);
 	}
 	string line;
	while (getline(inputd,line)) {
	
	std::vector<double> y (2,0) ;
	std::vector<double> y2 (4,0) ;
	std::vector<std::string> x = split(line, ' ');
	if (x.size()<4) std::vector<std::string> x = split(line, '\t');
	
	

	for(int j=0;j<2;j++)
	{
	stringstream s;
	s << x[j];
	s >> y[j];
	}
	for(int j=2;j<4;j++)
	{
	stringstream s;
	s << x[j];
	s >> y2[j];
	}
	
	nph.push_back(y2[2]);
	npl.push_back(y2[3]);
	cmax.push_back(y[1]);
	cmin.push_back(y[0]);
	
	
	}
	inputd.close();
	// done read in centralities
	cout << "read in centralities" << endl;	
		
		
	int ctot=cmax.size();
	int atot=sph.end;
	
	int sep=ctot/10+2;

	vector<double>rpt;
	rpt.resize(ctot);
	string napt="out/"+sph.folder.substr (0,last+1)+"cpT_"+sph.hadname+".dat";
	ifstream inner(napt.c_str());
	if (!inner.is_open())
 	{
 	cout << "Can't open " << napt << endl;
 	exit(1);
 	}
 	int c=0;
	while (getline(inner,line)) {
	std::vector<std::string> x = split(line, ' ');	

	double sub;
	stringstream s;
	s << x[1];
	s >> sub;
	rpt[c]=sub;	
	c++;
	}
	inner.close();
		
		
	for (int ev=sph.start;ev<=sph.end;ev++) // runs over all the events
	{	
		string out1,out2;
		string conev=sph.convertInt(ev);
		printstart<2,3>(ofolder,conev,out1,out2,sph);
		if (vv[ev].flag==1) {
		  p_vint(viname,ev,sph);
		  printnull<2,3>(out2,sph,ptmin);
		  continue;
		 }
		 
  		 sph.restart(ptmax); 
  		 
  		 for (int ps=ptmin;ps<ptmax;ps++){
  		 for(int i=0;i<phimax;i++) spec[ps][i].dNdpt=vv[ev].dNdpt[ps][i];
		}
		
		 //rpt[vv[ev].cen]=0.856984;
		double ptvar=0;
		for (int ps=ptmin;ps<ptmax;ps++)
		{
		gausslegendrequad<2,3>(ps,sph);	
		
		ptvar+=pow(spec[ps][0].pt*spec[ps][0].pt*spec[ps][0].wpt*sph.Ia[ps]-vv[ev].mpt,2);
              
              	
                for(int j=0;j<sph.vnmax;j++){
                vector <double> Q;
                Q.resize(2);
                if (j==0){
                	Q[0]=spec[ps][0].pt*spec[ps][0].pt*spec[ps][0].wpt*sph.ide[j].I.x[0];
                	Q[1]=spec[ps][0].pt*spec[ps][0].pt*spec[ps][0].wpt*sph.ide[j].I.x[1];
                }
                else {
                	Q[0]=spec[ps][0].pt*spec[ps][0].pt*spec[ps][0].wpt*sph.ide[j].I.x[0];
                	Q[1]=spec[ps][0].pt*spec[ps][0].pt*spec[ps][0].wpt*sph.ide[j].I.x[1];
                }
                spec[ps][j].psi=atan2(Q[1],Q[0])/sph.ntab[j];
                }
                
		
		for(int i=0;i<phimax;i++)
		{
		for(int j=0;j<sph.vnmax;j++)
		{
		sph.vout[j][ps]+=spec[ps][i].dNdpt*spec[ps][i].wphi*cos(sph.ntab[j]*(spec[ps][i].phi-spec[ps][j].psi));
		}
		}
		}
		
		vv[ev].mptvar=ptvar/vv[ev].M;
		
		// print off pT dependent yields
		print2<2,3>(out2,sph,ptmin);
		
			
		vnint(pmin22, pmax2,0.1, sph,rpt[vv[ev].cen]);
		

		
		p_vint(viname,ev, sph,vv[ev].mpt,vv[ev].M,vv[ev].mpt2,vv[ev].mptvar);
		 
		 
	
	
	
	}
		
	
	
	sph.destroyvout(spec.size() );
	
	delete [] sph.ide;
	delete [] sph.Ia;
	
	
	 

 return 0;
}


template <int D,int DD>
void vnint(double ptmin, double ptmax,double dpt,  SPH<D,DD> &sph,double rpt){
 
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
		if (ps>0) spec[ps][0].wpt=spec[ps][0].pt-spec[ps-1][0].pt;
		else spec[0][0].wpt=spec[0][0].pt;
		
	
	}
	
	
	for (int ps=min;ps<max;ps++)
	{
		gausslegendrequad<2,3>(ps,sph);
	 	double p2=spec[ps][0].pt*spec[ps][0].wpt;
		 sph.ide[0].Q=(spec[ps][0].pt-rpt)*p2*sph.ide[0].I;
	 	for (int j=1;j<sph.vnmax;j++) sph.ide[j].Q+=p2*sph.ide[j].I;
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
	
	        //sph.ide[0].intert+=(spec[ps][0].pt-rpt)*spec[ps][0].pt*spec[ps][0].wpt*sph.vout[0][ps];
		for(int j=0;j<sph.vnmax;j++)
		{
		sph.ide[j].intert+=spec[ps][0].pt*spec[ps][0].wpt*sph.vout[j][ps];
		}
	}
	
	double interb=0,v1interb=0;
	for (int ps=min;ps<max;ps++) interb+=spec[ps][0].pt*spec[ps][0].wpt*sph.Ia[ps];
	//for (int ps=min;ps<max;ps++) v1interb+=(spec[ps][0].pt-rpt)*spec[ps][0].pt*spec[ps][0].wpt*sph.Ia[ps];
	
	
	//sph.ide[0].intert/=v1interb;				
	for(int j=0;j<sph.vnmax;j++) sph.ide[j].intert/=interb;
	
	
	
}













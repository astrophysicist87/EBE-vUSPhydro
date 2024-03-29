#ifndef ENTERIC_H_ 
#define ENTERIC_H_

#include <iostream>
#include <iomanip> 
#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>
 
#include "mathdef.h"
#include "vector.h"
#include "tables.h"
#include "particle.h"
#include "LinkList.h"
#include "eostables.h"
#include "eos.h"
#include <stdlib.h>
#include <vector>

using namespace std;

void readEOStable();
void readEOS_lowS(string &firstry);


void readEOS_T(string &firstry);

void readEOS_p(string &firstry);

void open_dir(string eventfolder, string *&filelist,int &count);

string convertInt(int number);

std::vector<std::string> split(const std::string &s, char delim);
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);

// 2 dimension functions

void readICs_gen(string &firstry, string &firstry2,int &_Ntable3, Particle<2> *&_p,double const& efcheck, int & numpart);

void readICs_gen(string &firstry, int &_Ntable3, Particle<2> *&_p,double const& efcheck, int & numpart);
void readICs_genUU(string &firstry,  int &_Ntable3,Particle<2> *&_p,double const& efcheck, int & numpart);
void readICs_ebe(string &firstry,  int &_Ntable3,Particle<2> *&_p,double factor,double const& efcheck, int & numpart);
void readICs_gebe(string &firstry,  int &_Ntable3,Particle<2> *&_p,double factor,double const& efcheck, int & numpart); // for giorgio
void readICs_nebe(string &firstry,  int &_Ntable3,Particle<2> *&_p,double factor,double const& efcheck, int & numpart); // for nexus with flow
double readICs_gl(string &firstry,  int &_Ntable3,Particle<2> *&_p,double factor,double const& efcheck, int & numpart) ;// ipglasma
void readICs_glno(string &firstry,  int &_Ntable3,Particle<2> *&_p,double factor,double const& efcheck, int & numpart) ; //ipglasma no flow
void readICs_tnt(string &firstry,  int &_Ntable3,Particle<2> *&_p,double factor,double const& efcheck, int & numpart, eos EOS); //trento (entropy density)

// 3 dimension functions

void readICs_gen(string &firstry,  string &firstry2, int &_Ntable3,Particle<3> *&_p,double const& efcheck, int & numpart);
void readICs_gen(string &firstry,   int &_Ntable3, Particle<3> *&_p,double const& efcheck, int & numpart);
void readICs_genUU(string &firstry,  int &_Ntable3,Particle<3> *&_p,double const& efcheck, int & numpart);
void readICs_ebe(string &firstry,  int &_Ntable3,Particle<3> *&_p,double factor,double const& efcheck, int & numpart);
void readICs_gebe(string &firstry,  int &_Ntable3,Particle<3> *&_p,double factor,double const& efcheck, int & numpart); // for giorgio
void readICs_nebe(string &firstry,  int &_Ntable3,Particle<3> *&_p,double factor,double const& efcheck, int & numpart); // for nexus with flow
double readICs_gl(string &firstry,  int &_Ntable3,Particle<3> *&_p,double factor,double const& efcheck, int & numpart); //ipglasma
void readICs_glno(string &firstry,  int &_Ntable3,Particle<3> *&_p,double factor,double const& efcheck, int & numpart); //ipglasma no flow
void readICs_tnt(string &firstry,  int &_Ntable3,Particle<3> *&_p,double factor,double const& efcheck, int & numpart, eos EOS); //trento (entropy density)



template <int D>
void manualenter(_inputIC &ics, LinkList<D> &linklist)
{
  
   
   
   string manf = ifolder+ics.man;
   istringstream manis (manf.c_str());
   //manenter:
   FILE * openmanf = fopen (manf.c_str(),"r");	
    double h,factor;
   double it0;
   int start,end; 
   string eos_s,eos_p,ic,outf,ictype,ic2,eostype,fvisc,low_switch;
   string table ("table");
   string tableuu ("tableuu");
   string separate ("separate");
   string weight ("weight");
   string bjorken ("bjorken");
   string eventbyevent ("eventbyevent");
   string giorgio ("giorgio");
   string nexus ("nexus");
   string average ("average");
   string smooth ("smooth");
   string glasma ("glasma");
   string glasmanoflow ("glasmanoflow");
   string hglasma ("hglasma");
   string hglasmanoflow ("hglasmanoflow");
   string hgiorgio ("hgiorgio");
   string ideal ("ideal");
   string trento ("trento");
   int df;
  	if ((!manf.empty())&&(openmanf!= NULL)){
  	   fscanf(openmanf,"%*s %lf	%*s   	%lf	    \n",&h,&ics.dt);
  	   char charin[100],charin2[100];
  	   fscanf(openmanf,"%*s %s %*s  %s \n",charin2,charin);  
  	   fvisc=charin2;   
  	   eostype=charin;
  	   if (eostype==table)
  	   {
  	   	fscanf(openmanf,"%s \n",charin);  	   
  	   	eos_s=charin;
  	   	fscanf(openmanf,"%s \n",charin);  	   
  	   	eos_p=charin;
  	   	char charin3[100];
  	   	fscanf(openmanf,"%*s %s\n",charin3);  	   
  	   	low_switch=charin3;
  	   }
  	   if (fvisc!=ideal){ //if not ideal, define eta/s and zeta/s format
  	   fscanf(openmanf,"%*s %i \n",&linklist.etaconst); //if 1, eta/s const, else temp dependent
  	   if (linklist.etaconst!=1) fscanf(openmanf,"%*s %lf  %lf  %lf \n",&linklist.zwidth,&linklist.sTc,&linklist.zTc); // guassian width zeta/s, eta/s T_c, zeta/s T_c
	   fscanf(openmanf,"%*s %lf %lf \n",&linklist.bvf,&linklist.svf);  } // constant multiplying factor for zeta/s
	   //	   cout << linklist.bvf << " " << linklist.svf <<endl;
	   fscanf(openmanf,"%*s %lf \n",&it0);  
	   fscanf(openmanf,"%*s %lf \n",&freezeoutT); 		    
	   freezeoutT/=197.3; 
	   
  	   fscanf(openmanf,"%s \n",charin);  	   
  	   ictype=charin;
  	   
  	   
  	   if (ictype==hgiorgio){
  	   ictype=giorgio;
  	   hcor=1;}
  	   else if (ictype==hglasma){
  	   ictype=glasma;
  	   fscanf(openmanf,"%i ",&hcor);
  	   }
  	   else if (ictype==hglasmanoflow){
  	   ictype=glasmanoflow;
  	   fscanf(openmanf,"%i ",&hcor);}
  	   else hcor=0;
  	   
  	
  	  
  	   
  	   if (ictype!=bjorken)
  	   {
  	   if (ictype==average||ictype==eventbyevent||ictype==trento||ictype==giorgio||ictype==nexus||ictype==smooth||ictype==glasma||ictype==glasmanoflow)
  	   {
  	   	fscanf(openmanf,"%lf \n",&factor);  
  	   	linklist.factor=factor;
  	   	
           }
  	   
  	  
  	   
  	   fscanf(openmanf,"%*s %s \n",charin);  	   
  	   ic=charin;
  	    int qmin;
  	   fscanf(openmanf,"%*s %i \n",&qmin);  
  	   linklist.qmf=qmin;  
  	   
  	   fscanf(openmanf,"%*s %s \n",charin);  	   
  	   outf=charin;
  	   
  	   if (ictype==eventbyevent||ictype==trento||ictype==giorgio||ictype==nexus||ictype==glasma||ictype==glasmanoflow)
  	   {
  	   	fscanf(openmanf,"%d \n",&start);  
  	   	fscanf(openmanf,"%d \n",&end); 
  	   	
  	   	
  	   	if (ics.on==0){
  	   	linklist.start=start; 
  	   	linklist.end=end;}
  	   	else
  	   	{
  	   	linklist.start=ics.start; 
  	   	linklist.end=ics.end;
  	   	}
  	   	cout << "Event # " << linklist.start << " with fac="<< factor << endl;
           }
           
	   //           cout << "Event # " << linklist.start << " with fac="<< factor << endl;           
  	   
  	   if (ictype==separate)
  	   {
  	   	fscanf(openmanf,"%s \n",charin);  	   
  	   	ic2=charin;
           }
           }
           fscanf(openmanf,"%*s %i \n",&df);  
           if (df!=0) // for df, analytical solution - no decays
  	   {
  	   	fscanf(openmanf,"%*s %s \n",charin);
  	   	string dfpre,dffile=charin;

        // Added by Christopher Plumberg - 04/14/2021
        // If df < 0, assume df is one level higher in directory structure
        string directoryPrefix = (df < 0) ? "../" : "";
  	   	if (abs(df)==1) dfpre="df2/input/";  
  	   	else if (abs(df)==2) dfpre="sampling/input/"; 
  	   	else cout << "Error: undefined df calcualtion type " << df <<  endl;
  	   	string dfout=directoryPrefix+dfpre+dffile;
  	   	
  	   	
  	   	
  	   	ofstream DFO;
  	   	DFO.open(dfout.c_str());
  	   	if (!DFO.is_open())
		{cout << "Error: cannot open "<< dfout << " file!" << endl;
		getchar();}
		
		DFO << "typeofequations: " << fvisc <<  endl;
		DFO << "folder: " << outf << endl;
		DFO << "range(pt,phi): input/gl15.dat input/gq20.dat" << endl;
		DFO << "decays: 1" << endl;
		DFO << "rangeofevents: " << linklist.start << " " << linklist.end << endl;
  	   	DFO << "freezeouttemp: " << freezeoutT*0.1973 << endl;
  	   	
  	   	
  	   	
//  	   	string sampofol;
//  	   	if (df==2) {
//  	   	 	   
//  	  	sampofol=outf;
//  	   //	DFO << "output_folder: " << sampofol << endl;
//  	   	}
  	   	
  	   	while (fgets (charin, 100 ,openmanf)!= NULL )  DFO << charin ;
  	   	
  	   	DFO.close();
  	   	
  	   	
//  	   	string after=".dat";
//  	   	string ideal ("ideal");
//   		string bulk ("bulk");
//   		string shearbulk ("shear+bulk");
//   		string bulkshear ("bulk+shear");
//  	   	string before;
//  	   	if (fvisc==ideal) before="freezeout_ev";
//		else if (fvisc==bulk) before="bvfreezeout_ev";
//		else if ((fvisc==shearbulk)||(fvisc==bulkshear)) before="sbvfreezeout_ev";
//  	   	ofstream RUN;
//  	   	string runname="run"+ics.rnum+ ".sh";
//  	   	RUN.open(runname.c_str() );
//  	   	RUN << "if [ ! -d \"" << dfpre << outf <<"\" ]; then" << endl;
//  	   	RUN << "  mkdir " <<  dfpre << outf << endl;
//  	   	RUN << "fi" << endl;
//  	   	RUN << "for ((i=" <<  linklist.start << "; i<=" << linklist.end << "; i++)) " << endl;
//  	   	RUN << "do " << endl;
//  	   	RUN << "   mv outputfiles/" << outf << "/"<< before << "\"$i\"" << after << " " << dfpre << outf << "/"<< before << "\"$i\"" << after << endl;
//  	   	RUN << "done " << endl;

//  	   	
//  	   	if (df==1){
//  	   	RUN << "cd df" << endl;
//  	   	RUN << "if [ ! -d \"out/" <<  outf <<"\" ]; then" << endl;
//  	   	RUN << "  mkdir out/" << outf << endl;
//  	   	RUN << "fi" << endl;
//  	   	RUN << "make" << endl;
//  	   	RUN << "for ((j=" <<  linklist.start << "; j<=" << linklist.end << "; j++)) " << endl;
//		RUN << "do" << endl;
//  	   	RUN << "./fo " << dffile << " " << ics.rnum<<    " \"$j\"" << endl;
//  	   	RUN << "done" << endl;
//  	   	RUN << "./run\"$1\".sh   " <<  linklist.start << " " << linklist.end << endl;
//  	   	}
//  	   	else if (df==2){
//  	   	RUN << "cd sampling" << endl;
//  	   	RUN << "make" << endl;
//  	   	RUN << "for ((j=" <<  linklist.start << "; j<=" << linklist.end << "; j++)) " << endl;
//		RUN << "do" << endl;
//  	   	RUN << "./fo " << dffile <<  " " << ics.rnum << " \"$j\"" << endl;
//  	   	RUN << "done" << endl;	
//  	   	RUN << "./" << runname <<  " " << ics.rnum << " " << linklist.start << "  " << linklist.end <<  endl;
//  	   	}
//  	   	RUN.close();
  	   	
  	   	
           }
          
         
        
  	   fclose(openmanf);
	   cout << manf.c_str() << ": Input sucessful!\n";
  	}
  	else {
  		
  	
  		cout << "Error: " << manf.c_str() << " does not exist.  Please enter new a file name that includes h,timestep,dimensions and file names for EOS/IC's \n";
		
		exit(1); 	
  	}


	
	
  	
  	linklist.setv(fvisc);
  	linklist.eost=eostype;
  	linklist.cevent=0;
//    cout << "h=" << h << " timestep=" << timestep << " dimensions=" << D << " dt=" << dt << " Number of Loops=" << Nloop << " Output loops=" << Outnum << "\n";
    cout << fvisc << " hydro, h=" << h <<  " dimensions=" << D << " dt=" << ics.dt << " QM fluc:  " <<  linklist.qmf << "\n";
    
    if (eostype==table)
    {
    	cout << "Using Equation of State table from: " << eos_s << " and " << eos_p << "\n";
    	
    	//       Start reading EoS table           //
    	string on ("on");   	
		
    	readEOS_T(eos_s);  
    	readEOS_p(eos_p);
    	
    	if (low_switch==on)
    	{
    		
    		linklist.lowT=1;
    		string lowsT ("lowsT.dat");
    		readEOS_lowS(lowsT);
    	}
    	else
    		linklist.lowT=0;
    }
    else
    {
    	cout << "Using " << eostype << " Equation of State." << endl;
    }
   
   
    eos EOS;
    EOS.eosin(eostype);   
    double efcheck=EOS.efreeze();
    double sfcheck=EOS.sfreeze();
  
    linklist.efcheck=efcheck;    
    linklist.sfcheck=sfcheck; 
    linklist.fcount=0;    
    linklist.average=0;
    //       Start reading ICs          //    
    Particle<D> *_p;
    int numpart,_Ntable3;
    
  //  cout << "setting up SPH" << endl;
       
    
    linklist.gtyp=0;
    if (ictype==weight)
    {
    	
    	readICs_gen(ic,  _Ntable3, _p,efcheck,numpart);  
    	
    	_p[0].start(eostype);
    	
    	linklist.setup(it0,_Ntable3,h,_p,ics.dt,numpart);
    	
    	
	cout << "number of sph particles=" << _Ntable3 << endl;
    	
    }
    else if (ictype==separate)
   {
 	readICs_gen(ic, ic2, _Ntable3, _p,efcheck,numpart);
 	linklist.setup(it0,_Ntable3,h,_p,ics.dt,numpart);
   }
   else if (ictype==bjorken)
   {
   	if ((fvisc=="shear")||(fvisc=="shear+bulk")||(fvisc=="bulk+shear"))
		linklist.gubsershear(h);
   	else
   		linklist.gubser(h);
   	
   }
   else if (ictype==tableuu)
   {
   	readICs_genUU(ic,  _Ntable3, _p,efcheck,numpart);
    	linklist.setup(it0,_Ntable3,h,_p,ics.dt,numpart);
   }
   else if (ictype==eventbyevent)
   {
   	
   	int count=linklist.end-linklist.start+1;
   	linklist.ebe_folder=outf;
   	string *filelist;
   	filelist=new string[count];
   	
   	int j=0;
   	for (int i=linklist.start;i<=linklist.end;i++)
   	{
   	filelist[j]= ic+"/ic"+convertInt(i)+".dat";
   	j++;
   	}
   	linklist.filenames=filelist;
   	linklist.fcount=count;
   	linklist.fnum=linklist.start;
   
   	
   	readICs_ebe(linklist.filenames[0],  _Ntable3, _p,factor,efcheck,  numpart);
   	
   	_p[0].start(eostype);
    	linklist.setup(it0,_Ntable3,h,_p,ics.dt,numpart);
    	/// compare linklist.gubser
    	
    	cout << "number of sph particles=" << _Ntable3 << endl;
    	
    	
   }
   else if (ictype==trento)
     {
       
       int count=linklist.end-linklist.start+1;
       linklist.ebe_folder=outf;
       string *filelist;
       filelist=new string[count];
       
       int j=0;
       for (int i=linklist.start;i<=linklist.end;i++)
	 {
	   filelist[j]= ic+"/ic"+convertInt(i)+".dat";
	   j++;
	 }
       linklist.filenames=filelist;
       linklist.fcount=count;
       linklist.fnum=linklist.start;
   
       
       readICs_tnt(linklist.filenames[0],  _Ntable3, _p,factor,sfcheck,  numpart, EOS);
       
       _p[0].start(eostype);
       linklist.setup(it0,_Ntable3,h,_p,ics.dt,numpart);
       /// compare linklist.gubser
       
       cout << "number of sph particles=" << _Ntable3 << endl;
       linklist.gtyp=5;
       
     }
   else if (ictype==giorgio)
   {
   	
   	
   	int count=linklist.end-linklist.start+1;
   	linklist.ebe_folder=outf;
   	string *filelist;
   	filelist=new string[count];
   	
   	int j=0;
   	for (int i=linklist.start;i<=linklist.end;i++)
   	{
   	filelist[j]= ic+"/ic"+convertInt(i)+".dat";
   	j++;
   	}
   	linklist.filenames=filelist;
   	linklist.fcount=count;
   	linklist.fnum=linklist.start;
   
   	
   	readICs_gebe(linklist.filenames[0],  _Ntable3, _p,factor,efcheck,  numpart);   	
   	
    	linklist.setup(it0,_Ntable3,h,_p,ics.dt,numpart);
    	
    	/// compare linklist.gubser
    	
    	cout << "number of sph particles=" << _Ntable3 << endl;
    	linklist.gtyp=1;
    	
    	
   }
   else if (ictype==nexus)
   {
   	
   	
   	int count=linklist.end-linklist.start+1;
   	linklist.ebe_folder=outf;
   	string *filelist;
   	filelist=new string[count];
   	
   	int j=0;
   	for (int i=linklist.start;i<=linklist.end;i++)
   	{
   	filelist[j]= ic+"/ic"+convertInt(i)+".dat";
   	j++;
   	}
   	linklist.filenames=filelist;
   	linklist.fcount=count;
   	linklist.fnum=linklist.start;
   
   	
   	readICs_nebe(linklist.filenames[0],  _Ntable3, _p,factor,efcheck,  numpart);   	
   	
    	linklist.setup(it0,_Ntable3,h,_p,ics.dt,numpart);
    	
    	/// compare linklist.gubser
    	
    	cout << "number of sph particles=" << _Ntable3 << endl;
    	linklist.gtyp=2;
    	
    	
   }
   else if (ictype==glasma)
   {
   	
   	
   	int count=linklist.end-linklist.start+1;
   	linklist.ebe_folder=outf;
   	string *filelist;
   	filelist=new string[count];
   	
   	int j=0;
   	for (int i=linklist.start;i<=linklist.end;i++)
   	{
   	filelist[j]= ic+"/ic"+convertInt(i)+".dat";
   	j++;
   	}
   	linklist.filenames=filelist;
   	linklist.fcount=count;
   	linklist.fnum=linklist.start;
   
   	
   	linklist.gd2=readICs_gl(linklist.filenames[0],  _Ntable3, _p,factor,efcheck,  numpart);   	
   	
    	linklist.setup(it0,_Ntable3,h,_p,ics.dt,numpart);
    	
    	/// compare linklist.gubser
    	
    	cout << "number of sph particles=" << _Ntable3 << endl;
    	linklist.gtyp=3;
    	
    	
   }
   else if (ictype==glasmanoflow)
   {
   	
   	
   	int count=linklist.end-linklist.start+1;
   	linklist.ebe_folder=outf;
   	string *filelist;
   	filelist=new string[count];
   	
   	int j=0;
   	for (int i=linklist.start;i<=linklist.end;i++)
   	{
   	filelist[j]= ic+"/ic"+convertInt(i)+".dat";
   	j++;
   	}
   	linklist.filenames=filelist;
   	linklist.fcount=count;
   	linklist.fnum=linklist.start;
   
   	
   	readICs_glno(linklist.filenames[0],  _Ntable3, _p,factor,efcheck,  numpart);   	
   	
    	linklist.setup(it0,_Ntable3,h,_p,ics.dt,numpart);
    	
    	/// compare linklist.gubser
    	
    	cout << "number of sph particles=" << _Ntable3 << endl;
    	linklist.gtyp=4;
    }
   else if (ictype==average)
   {
   	   	
   	linklist.average=1;
   	linklist.ebe_folder=outf;
   	string filelist=ic+("/dens2.dat");
   	linklist.filenames=new string[1];
   	linklist.filenames[0]=filelist;
   	linklist.fcount=0;
   
   	
   	readICs_ebe(linklist.filenames[0],  _Ntable3, _p,factor,efcheck,numpart);
    	linklist.setup(it0,_Ntable3,h,_p,ics.dt,numpart);
    	
    	//cout << "averaged ICs" << endl;
    	//cout << "number of sph particles=" << _Ntable3 << endl;
    	
    	
   }
   else if (ictype==smooth)
   {
   	   	
   	linklist.average=1;
   	linklist.ebe_folder=outf;
   	linklist.filenames=new string[1];
   	linklist.filenames[0]=ic;
   	linklist.fcount=0;
   
   	
   	readICs_ebe(linklist.filenames[0],  _Ntable3, _p,factor,efcheck,numpart);
    	linklist.setup(it0,_Ntable3,h,_p,ics.dt,numpart);
    	
    	//cout << "averaged ICs" << endl;
    	//cout << "number of sph particles=" << _Ntable3 << endl;
    	
   }

    string name3=ofolder+linklist.ebe_folder+"/temp_ic"+convertInt(linklist.start)+".dat";
    ofstream PRINT(name3.c_str());
    if (!PRINT.is_open())
      {
	cout << "Can't open " << name3 << endl;
	exit(1);
      }
    
    PRINT << _Ntable3-numpart << endl;
    
    PRINT.close();

   
 // cout << "Setting up EOS" << endl;
    
   // sets up EOS
	for (int i=0;i<linklist.n();i++)
	{	
		linklist._p[i].start(eostype);
	}
	
	if (ictype==weight)
   	{

   		linklist.initiate();
   		double tmax=0;
   		int imax,k;
		Vector<double,D> rmax;
		for (int j=0;j<D;j++)
		{
			rmax.x[j]=0;
		}
		for (int i=0;i<linklist.n();i++)
		{
		linklist._p[i].gamma=linklist._p[i].gamcalc();
		}
		
		k=linklist.n();
		
		for (int i=0;i<linklist.n();i++)
		{
		
			//linklist._p[i].u.x[0]=0.5*_p[i].r.x[0]/linklist.t0;  
			//linklist._p[i].u.x[1]=_p[i].r.x[1]/linklist.t0; 
			linklist._p[i].u.x[0]=0;  
			linklist._p[i].u.x[1]=0; 
			linklist._p[i].gamma=linklist._p[i].gamcalc();
			//_p[i].Bulk=0.05;

			
			linklist.optimization(i);
			
			linklist._p[i].EOS.update_s(linklist._p[i].sigma/linklist._p[i].gamma/linklist.t0);

			linklist._p[i].shv.x[1][1]= 0 ; 
			linklist._p[i].shv.x[2][2]=0 ; 
			linklist._p[i].shv.x[1][2]=0;
			linklist._p[i].shv.x[2][1]=0;
		//	linklist._p[i].shv.x[1][2]=0.1*svf*0.08*linklist._p[i].EOS.s()/(3*linklist.t0); 
			//linklist._p[i].shv.x[2][1]=0.1*svf*0.08*linklist._p[i].EOS.s()/(3*linklist.t0);  
				
				
			if (linklist._p[i].EOS.T()>tmax)
			{
			tmax=linklist._p[i].EOS.T();
			rmax=linklist._p[i].r;
			imax=i;
			}
			
			numpart=0;
			if (linklist._p[i].EOS.T()>freezeoutT)	
			{
			 linklist._p[i].Freeze=0;
			}
			else 
			{
			 linklist._p[i].Freeze=4;
			 numpart++;
			 k--;
			}
			
		}
		
		
		linklist.number_part=numpart;
		cout << "largest temperature" << endl;
		cout << tmax*197.3 << " " << std::setprecision(20) << rmax << " " << linklist._p[imax].EOS.e() << endl;
		cout << "pre-n=" << linklist.n() << " post-n=" << k << endl;
		
    }
    
   
	if ((ictype==bjorken)||(ictype==eventbyevent)||(ictype==trento)||(ictype==average)||(ictype==smooth)||(ictype==giorgio)||(ictype==nexus)||(ictype==glasma)||(ictype==glasmanoflow))
   {
   	
   	linklist.updateIC();
//cout << "optimization done" << endl;
   }

   
   linklist.freezeset();

   
   if ((fvisc=="bulk")&&ictype!=weight){
	linklist.prints( );
     linklist.etas_set();
     }
   else if ((fvisc=="bulk+shear")||(fvisc=="shear+bulk"))  
   {
	   if (ictype!=bjorken) linklist.sv_set();
	   else linklist.sv_setb();
   }
    
    linklist.first=1;
    

    
}




template <int D>
void nextevent(int i, LinkList<D> &linklist)
{

	
   	

	int numpart, _Ntable3;
	Particle<D> *_p;
	if (linklist.gtyp==0) readICs_ebe(linklist.filenames[i],  _Ntable3, _p,linklist.factor,linklist.efcheck,numpart);
	else if (linklist.gtyp==1) readICs_gebe(linklist.filenames[i],  _Ntable3, _p,linklist.factor,linklist.efcheck,numpart);
	else if (linklist.gtyp==2) readICs_nebe(linklist.filenames[i],  _Ntable3, _p,linklist.factor,linklist.efcheck,numpart);
	else if (linklist.gtyp==3) linklist.gd2=readICs_gl(linklist.filenames[i],  _Ntable3, _p,linklist.factor,linklist.efcheck,numpart);
	else if (linklist.gtyp==4) readICs_glno(linklist.filenames[i],  _Ntable3, _p,linklist.factor,linklist.efcheck,numpart);
        else if (linklist.gtyp==5) readICs_tnt(linklist.filenames[i],  _Ntable3, _p,linklist.factor,linklist.sfcheck,numpart,linklist._p[0].EOS);
    	linklist.setupnext(_Ntable3,_p,numpart);
    	
    		
	for (int i=0;i<linklist.n();i++)
	{
		linklist._p[i].start(linklist.eost);
	}
	
    
	linklist.updateIC();
	

  	 if (linklist.visc==1) linklist.etas_set();
  	 
  	 
  	 linklist.first=1;
  	 linklist.fnum+=1;
	
}


#endif

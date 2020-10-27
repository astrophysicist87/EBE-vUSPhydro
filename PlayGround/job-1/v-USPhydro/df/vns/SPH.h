#ifndef _SPH_H_
#define _SPH_H_

#include <string>
#include <iostream>
#include <fstream> 
#include <cmath>
#include <vector>
#include "vector.h"
#include "vnvar.h"
#include "bessel.h"
#include "global.h"
	
using namespace std;

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {    	
        if (!item.empty()) elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

template <int D,int DD>
class SPH {
private:
	
	
	double facc;
	
	
	//double I1c,I2c,I1sc,I2sc;
	int h1tot;
	
	//double nuprod( int nsph) {return par[nsph].n&par[nsph].u;   }
	
	void countpoints();
	 static const string bulk;
	 static const string ideal ;	
	 static const string shear ;
	 static const string shearbulk;	
	 static const string bulkshear;
	
	
	
public:
	static const double scale=0.1973;
	static const double sc3=0.1973*0.1973*0.1973;
	double T,s;
	double m3;
	int N,NHAD;
	int tots2;
	int vnmax;
	int ntab[7];
	double ** vout;
    
        double ptout(int cev);	 
	
	int all;
	
	vector <int> hl;

	int evn,evncor;  // number of sph particles, file number of event
	
	VNVAR *ide;
	
	int decays;
	double outc;
	int typ;
	
	string before,after;
	int start, end;
	string folder;
	double *Ia;
	
	string hadname;
	
	double *v;
	double *vc;
	
	
	SPH<D,DD>();
	~SPH<D,DD>();
	void readin(int sin,int fin,string resolist, string vnfile);
	int readin2(int cev);
	string convertInt(int number);
	void restart(int pTmax);
	void setvns(int ptmax );
	void destroyvout(int pTmax) ;
	
	void readweights(string ptpoints, string phipoints);
};

        template <int D,int DD> const string SPH<D,DD>::bulk="bulk";
	template <int D,int DD> const string SPH<D,DD>::ideal="ideal";	
	template <int D,int DD> const string SPH<D,DD>::shear="shear";
	template <int D,int DD> const string SPH<D,DD>::shearbulk="shear+bulk";	
	template <int D,int DD> const string SPH<D,DD>::bulkshear="bulk+shear";

template <int D,int DD>
SPH<D,DD>::SPH()
{
	facc=1/(2*PI*PI);
	
}

template <int D,int DD>
SPH<D,DD>::~SPH()
{
	
}


// reads in the basic information from "input.dat" such at the number of events, events folder, particles to observe etc.
template <int D,int DD>
void SPH<D,DD>::readin(int sin,int fin, string resolist, string vnfile)
{

	start=sin;
	end=fin;
	string sall ("all");

	vnfile="input/"+vnfile;
        FILE * myfile = fopen (vnfile.c_str(),"r");
        if(myfile== NULL)
        {
  		cout << "Error: input.dat does not exist. \n";
		exit(1);	
  	}	
		
           //fscanf(myfile,"%*s  %i %i \n",&start,&end); // range of events e.g. start=0 end=199
           char charin[150];
           fscanf(myfile,"%*s %s \n",charin); // type of equations (ideal, bulk etc)
           string type=charin;
           fscanf(myfile,"%*s %s \n",charin); // folder that contains the events
           folder=charin;
           fscanf(myfile,"%*s  %s",charin);  // file that contains pt grid
           string ptpoints=charin;
           fscanf(myfile,"%s",charin);  // file that contains phi grid
           string phipoints=charin;
           fscanf(myfile,"%*s  %i",&decays);  //decays=1 post decays, decays=0 no decays
           fgets(charin, 150, myfile);
           fgets(charin, 150, myfile);
           fgets(charin, 150, myfile);
           fgets(charin, 150, myfile);
           fgets(charin, 150, myfile);
           fscanf(myfile,"%*s  %s",charin); 
           string reso=charin;          
  	fclose(myfile);
	cout << "input.dat: Input sucessful!\n";
	
	
	
  	
  	if (resolist==sall) all=1;
        else all=0;
  	
  	
	 
	 
           if (all==0){
            hadname=resolist.substr(0,resolist.length()-4);
           resolist="input/"+resolist;
           
	   FILE * myfile3 = fopen (resolist.c_str(),"r");
	   if(myfile3== NULL)
	   {
	  		cout << "Error: "<< resolist << " does not exist. \n";
			exit(1);	
	   }	
           
           
          
           cout << resolist << " " << hadname << endl;
           int sub;
           while(fscanf(myfile3,"%i",&sub)==1)
           {  
           	hl.push_back(sub);
           }
           fclose(myfile3); 
           }
           else {
           hadname="all";
           
            ifstream input(reso.c_str());
        
        	cout << reso << endl;
            
            
            string line;
//            getline(input,line);
//            getline(input,line);
	    while (getline(input,line)){
	     std::vector<std::string> x = split(line, '\t');
	     int sm=x.size();
	     if (sm<10) continue;
	     
	     stringstream ss,ss2,ss3;
	     int sub1,br,bar;
	     ss << x[10];
	     ss >> sub1;
	     
	     ss2 << x[11];
	     ss2 >> br;
	     
	     ss3 << x[5];
	     ss3 >> bar;
	     
	     if ((abs(sub1)>0)&&(br==1)&&(bar>=0)) {
	     stringstream sq;
	     int subid;
	     sq << x[0];
	     sq >> subid;
	     
	     cout << subid << endl;
	     hl.push_back(subid);
	     if (bar>0){ hl.push_back(-subid);
	     cout << -subid << endl;}
	     
	     
	     }
	    
	    }
           
           }
           
          
           
	
	if (type==ideal)
		typ=0;
	else if (type==bulk)
		typ=1;
	else if (type==shear)
		typ=2;
	else if ((type==shearbulk)||(type==bulkshear))
		typ=3;
		
	if (typ==0) before="freezeout_ev";
  	else if (typ>1) before="sbvfreezeout_ev";
  	else if (typ==1) before="bvfreezeout_ev";
  	after=".dat";

	int bcor=0;
	if (typ>1) bcor=1;

	N=end-start+1; // determines the total number of events
	
	
		
	readweights(ptpoints,phipoints);
	
	
	ofstream OUT3;
	string inname="out/in.dat";
	OUT3.open(inname.c_str() );
	if (!OUT3.is_open())
	{
		cout << "Error: cannot open out2 file!" << endl;
		exit(1);
	}
	
	
	
	OUT3 <<  "visc:  " << type << endl ;
	OUT3 <<  "folder:  " << folder << endl ;
	OUT3 <<  "hadrontype(s):  " << hadname << endl ;
	OUT3 <<  "bulkcorrection:  " << bcor << endl ;
	OUT3 <<  "ptsize:  " << spec.size() << endl ;
	

	
	OUT3.close();
	
	
	
}

// reads in the basic information from "input.dat" such at the number of events, events folder, particles to observe etc.
template <int D,int DD>
int SPH<D,DD>::readin2(int cev)
{

	for(int ps=0; ps<spec.size(); ps++) 
		for(int i=0; i<spec[0].size(); i++) spec[ps][i].dNdpt=0;


	string ty;
	if (decays==1){
	if (typ==0) ty="di";
	else if (typ==1) ty="dbvc";
	else if (typ==3) ty="dsbvc";
	}
	else if (decays==3){
	if (typ==0) ty="di";
	else if (typ==1) ty="dbv";
	else if (typ==3) ty="dsbv";
	}
	else{
	if (typ==0) ty="i";
	else if (typ==1) ty="bvc";
	else if (typ==3) ty="sbvc";
	}


	string event="out/"+folder+"/ev"+convertInt(cev)+ty+"_dNdphidpp.dat";
	if (decays==2) event="out/"+folder+"/ev"+convertInt(cev)+ty+"_dNdphidpp_neg.dat";
	//cout << event << endl;
	ifstream myfile(event.c_str() );
	if (!myfile.is_open()) 
	{
		event="out/"+folder+"/ev"+convertInt(cev)+"_dNdphidpp.dat";
	
		cout << cev << " " ;
		return 1;
	
	}
	
//	PAR * par2=new PAR [800];
//	int i=0;
//	
	int nid;
	
	int nhad=0;
	while (myfile >> nid ){
	
	
	nhad++;
	int on=0;
	
	
	for (int chl=0; chl<hl.size();chl++) if (nid==hl[chl]) on=1;
	
	
	
//	if (on==1) cout << nid << endl;
//	else {cout << "nope " << nid << endl;
//	getchar();}
	
	
	
	for(int i=0;i<spec[0].size();i++)
	{
	for(int ps=0;ps<spec.size();ps++)//reads in spectra
	{
	double spcsub;
	myfile >> spcsub ;
//	if (spcsub<0) spcsub=0;
	
	if (isnan(spcsub)==1) {spcsub=0;
	cout << "nan input from spectra in event: " <<  cev << endl;
	
	getchar();}
	
	if (on==1){
//	if (spcsub<0) spcsub=0;
	   cout << "spcsub" << endl;
	spec[ps][i].dNdpt+=spcsub;}
	}}
	
	
	
	}
	
	//if (nhad!=144) cout << "Event " << cev << " missing resonances" << endl;
	
	myfile.close();
	
	
	
	// not sure if it is needed?
	evncor=cev;
//	cout << "Event "<< cev << endl;
	
	
	// ensures that if negative contributions show up that the entire spectra goes to zero over phi
//	for(int ps=0;ps<spec.size();ps++)
//	{
//	int neg=0;
//	for(int i=0;i<spec[0].size();i++)
//	{
//	if (spec[ps][i].dNdpt<=0) neg=1;
//	}
//	
//	if (neg==1){
//	
//	for(int i=0;i<spec[0].size();i++) spec[ps][i].dNdpt=0;
//	
//	}
//	
//	
//	}
	
	
	return 0;
	
}





template <int D,int DD>
  double SPH<D,DD>::ptout(int cev)
{


  string ty;
  if (decays==1){
    if (typ==0) ty="di";
    else if (typ==1) ty="dbvc";
    else if (typ==3) ty="dsbvc";
  }
  else if (decays==3){
    if (typ==0) ty="di";
    else if (typ==1) ty="dbv";
    else if (typ==3) ty="dsbv";
  }
  else{
    if (typ==0) ty="i";
    else if (typ==1) ty="bvc";
    else if (typ==3) ty="sbvc";
  }


  string event="out/"+folder+"/ev"+convertInt(cev)+ty+"_dNdphidpp.dat";
  if (decays==2) event="out/"+folder+"/ev"+convertInt(cev)+ty+"_dNdphidpp_neg.dat";
  ifstream myfile(event.c_str() );
  if (!myfile.is_open()) 
    {
      event="out/"+folder+"/ev"+convertInt(cev)+"_dNdphidpp.dat";
      
      cout << event << endl;
      return 1;
      
    }
  

  for(int ps=0; ps<spec.size(); ps++) 
    for(int i=0; i<spec[0].size(); i++) spec[ps][i].dNdpt=0;

  int nid;
  
  int nhad=0;
  while (myfile >> nid ){
    
    
    nhad++;
    int on=0;
    
    
    for (int chl=0; chl<hl.size();chl++) if (nid==hl[chl]) on=1;

    
    for(int i=0;i<spec[0].size();i++)
      {
	for(int ps=0;ps<spec.size();ps++)//reads in spectra
	  {
	    double spcsub;
	    myfile >> spcsub ;
	    
	    if (on==1){

	      spec[ps][i].dNdpt+=spcsub;}
	  }}
    
  }
  myfile.close();
  
  double meanpt=0;
  double botmean=0;
  for (int ps=0;ps<spec.size();ps++){
    double IA=0;
    for(int i=0;i<spec[0].size();i++) IA+=spec[ps][i].dNdpt*spec[ps][i].wphi;
    meanpt+=spec[ps][0].pt*spec[ps][0].pt*spec[ps][0].wpt*IA;
    botmean+=spec[ps][0].pt*spec[ps][0].wpt*IA;
  }
  double mpt= meanpt/botmean;
  if (botmean<0) mpt=0;
  if (botmean>10000) mpt=0;

  
  return mpt;
  
}



template <int D,int DD>
void SPH<D,DD>::restart(int pTmax)
{

	for (int ps=0;ps<pTmax;ps++)
	{
		Ia[ps]=0;
	}
	
	for(int i=0;i<vnmax;i++)
	{
		ide[i].restart();
		for (int j=0;j<pTmax;j++)
		{
		vout[i][j]=0;
		}
	}

}








template <int D,int DD>
string SPH<D,DD>::convertInt(int number)
{
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}





template <int D,int DD>
void SPH<D,DD>::readweights(string ptpoints, string phipoints)
{


	vector<double> pt,phi,wpt,wphi;
	FILE * myfile = fopen (ptpoints.c_str(),"r");
	
	if (myfile==NULL) 
	{
	cout << "Error: Can't open Event " << ptpoints << endl;
	exit(1);
	}
	
	
	
	double ptsub,wsub;
	while(fscanf(myfile,"%lf %lf", &ptsub,&wsub)==2)
           {
           	  
           	pt.push_back(ptsub);
           	wpt.push_back(wsub);
           }
        fclose(myfile);
        
        cout << "finished pt"  << endl;
        
	FILE * myfile2 = fopen (phipoints.c_str(),"r");
	
	if (myfile2==NULL) 
	{
	cout << "Error: Can't open Event " << phipoints << endl;
	exit(1);
	}
	
	double phisub,pwsub;
	while(fscanf(myfile2,"%lf %lf", &phisub,&pwsub)==2)
           {
           	  
           	phi.push_back(phisub);
           	wphi.push_back(pwsub);
           }
        fclose(myfile2);

	int ptmax=pt.size();
	int phimax=phi.size();
	spec.resize(ptmax);
	for (int lpt=0;lpt<ptmax; lpt++){
		spec[lpt].resize(phimax);
		for (int lphi=0;lphi<phimax; lphi++){
			spec[lpt][lphi].pt=pt[lpt];
			spec[lpt][lphi].phi=phi[lphi];
			spec[lpt][lphi].wpt=wpt[lpt];
			spec[lpt][lphi].wphi=wphi[lphi];
			
		}
	}
	
		

}


template <int D,int DD>
void SPH<D,DD>::setvns(int ptmax )
{
	vnmax=7;
	ntab[0]=1;
	ntab[1]=2;
  	ntab[2]=3;
  	ntab[3]=4;
  	ntab[4]=5;
  	ntab[5]=6;
        ntab[6]=7;
  	
  	
  	
  	vout = new double*[vnmax];
	for(int i=0; i<vnmax; i++)
		vout[i] = new double[ ptmax];


	for(int j=0; j< ptmax; j++)
		for(int i=0; i<vnmax; i++)
			vout[i][j] = 0;
	
}


template <int D,int DD>
void SPH<D,DD>::destroyvout(int pTmax) {
	for(int i=0; i<vnmax; i++)
	{
		delete [] vout[i];
	}
	delete [] vout;
}



#endif

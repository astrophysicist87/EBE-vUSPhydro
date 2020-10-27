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


using namespace std;


string convertInt(int number)
{
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

int readin2(int cev);
void readin(string vnfile);

struct vall
{
  int flag,cen;
  double npart;
  double dNdpt[30][30];
  vector<double> M,mpt,mpt2;
};

struct SPEC{
double pt;
double phi;
double psi;
double dNdpt;
double wpt;
double wphi;
};

vector<vector<SPEC> > spec; 
string folder;
vector<vall> vv;
int decays;
string type;
string bulk="bulk",ideal="ideal",shear="shear",shearbulk="shear+bulk",bulkshear="bulk+shear";
int typ2;

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

	}
	else 
	{
	cout << " Error: missing intput of first and last event!" << endl;
	exit(1);
	}
	
	

	readin(vnfile);
  	
 	decays=1;
  		
	
	string ofolder="out/"+folder;
	//const double pisub=pow(2*PI,3);
	 //switches end result into 1/[GeV]^2
	string viname;

	
	ofstream OUT;

	
        vv.resize(evf+1);
	
	cout << "reading events" << endl;
	int ptmin=0,ptmax,phimax;
	
	for (int ev=ev0;ev<=evf;ev++) // runs over all the events
	{		
		vv[ev].flag=readin2(ev);
		if (vv[ev].flag==1){
		  cout << "event not read in " << ev << endl;
		  continue;}
	
	}// saves spectra, pt and angles
	
		
	std::string str2 ("/");
  	std::size_t found = folder.find(str2);
  	found=folder.find(str2,found+1);
  	string minifol=folder.substr (0,found);
  	
  	std::size_t last = folder.rfind(str2);

  	int siz=folder.size();
  	stringstream s;
  	int num;
  	s << folder.substr (last+1,siz);
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
	
	cout << "npart closed" << endl;
	
	for (int ev=ev0;ev<=evf;ev++){
		int spot=num*1000+ev;
	    	vv[ev].npart=npmin[spot];
	}
	
	// outputs mean pTonly and exits
	string snam="out/"+folder+"/allpt.dat";
	
	ofstream OUT3;
	OUT3.open(snam.c_str());
	if (!OUT3.is_open())
	{
		cout << "Error: cannot open out2 file!" << endl;
		exit(1);
	}
	
	cout << "printing" << endl;
	
	for (int ev=ev0;ev<=evf;ev++)
	{
		OUT3 <<  vv[ev].flag << " " << vv[ev].npart << "  " ;
		int numid=vv[ev].mpt.size();
		for (int i=0;i<numid;i++) OUT3 << vv[ev].mpt[i] << "  " << vv[ev].M[i]*vv[ev].mpt2[i]  <<  " " ;	
		OUT3 << endl;	
	}
	OUT3.close();
	exit(0);
	
	
	
	
	
	 

 return 0;
}


void readin( string vnfile)
{

	
	

	vnfile="input/"+vnfile;
        FILE * myfile = fopen (vnfile.c_str(),"r");
        if(myfile== NULL)
        {
  		cout << "Error: input.dat does not exist. \n";
		exit(1);	
  	}	
		
           char charin[150];
           fscanf(myfile,"%*s %s \n",charin); // type of equations (ideal, bulk etc)
          type=charin;
           fscanf(myfile,"%*s %s \n",charin); // folder that contains the events
           folder=charin;
	   cout << folder << endl;
           fscanf(myfile,"%*s  %s",charin);  // file that contains pt grid
           string ptpoints=charin;
           fscanf(myfile,"%s",charin);  // file that contains phi grid
           string phipoints=charin;     
  	fclose(myfile);
	cout << "input.dat: Input sucessful!\n";
	
 
          
           
	
	if (type==ideal)
		typ2=0;
	else if (type==bulk)
		typ2=1;
	else if (type==shear)
		typ2=2;
	else if ((type==shearbulk)||(type==bulkshear))
		typ2=3;
		
//	if (typ2==0) before="freezeout_ev";
//  	else if (typ2>1) before="sbvfreezeout_ev";
//  	else if (typ2==1) before="bvfreezeout_ev";
//  	after=".dat";

	int bcor=0;
	if (typ2>1) bcor=1;

	
	
	
	// read in pt spectra
	vector<double> pt,phi,wpt,wphi;
	FILE * myfile12 = fopen (ptpoints.c_str(),"r");
	
	if (myfile12==NULL) 
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
        fclose(myfile12);
        
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



int readin2(int cev)
{


	string ty;
	if (decays==1){
	if (typ2==0) ty="di";
	else if (typ2==1) ty="dbvc";
	else if (typ2==3) ty="dsbvc";
	}
	else if (decays==3){
	if (typ2==0) ty="di";
	else if (typ2==1) ty="dbv";
	else if (typ2==3) ty="dsbv";
	}
	else{
	if (typ2==0) ty="i";
	else if (typ2==1) ty="bvc";
	else if (typ2==3) ty="sbvc";
	}


	string event="out/"+folder+"/ev"+convertInt(cev)+ty+"_dNdphidpp.dat";
	if (decays==2) event="out/"+folder+"/ev"+convertInt(cev)+ty+"_dNdphidpp_neg.dat";
	//cout << event << endl;
	ifstream myfile(event.c_str() );
	if (!myfile.is_open()) 
	{
		event="out/"+folder+"/ev"+convertInt(cev)+"_dNdphidpp.dat";
                cout << event << endl;
	
		cout << cev << " " ;
		return 1;
	
	}


	
	int nhad=0;
	string enter;
	int ptmax=spec.size(),phimax=spec[0].size();

	while (myfile >> enter ){
		int nid;

		//read in spectra
		stringstream s;
		s << enter;
		s >> nid;
	  
		for(int ps=0; ps<ptmax; ps++) 
			for(int i=0; i<phimax; i++) spec[ps][i].dNdpt=0;
	
		for(int i=0;i<phimax;i++)
		{
		for(int ps=0;ps<ptmax;ps++)//reads in spectra
		{
		double spcsub;
		string subsp;
		myfile >> subsp ;

		if (isnan(spcsub)==1){
		     cout << "nan for nid " << nid << endl;
		    exit(1);
		}
		else{
		stringstream ss;
		ss << subsp;
		ss >> spcsub;
		}
	
		spec[ps][i].dNdpt=spcsub;}}
		
		//calculation <pT> and <pT^2>
	
		double meanpt=0,botmean=0,meanpt2=0;

		for (int ps=0;ps<ptmax;ps++)
		{
	
		double multi=0;
		for(int i=0;i<phimax;i++){
		 multi+=spec[ps][i].dNdpt*spec[ps][i].wphi;
		}
		

		meanpt2+=pow(spec[ps][0].pt,3)*spec[ps][0].wpt*multi;
		 meanpt+=spec[ps][0].pt*spec[ps][0].pt*spec[ps][0].wpt*multi;
		 botmean+=spec[ps][0].pt*spec[ps][0].wpt*multi;
	       }


		vv[cev].mpt.push_back( meanpt/botmean);
		vv[cev].mpt2.push_back(meanpt2/botmean);
		vv[cev].M.push_back(botmean);
	
	}
	

       
	myfile.close();
	
	

	
	return 0;
	
}





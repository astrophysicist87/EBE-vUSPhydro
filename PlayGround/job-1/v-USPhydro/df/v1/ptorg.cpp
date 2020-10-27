#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <vector>

using namespace std;

struct ev
{
  int flag,cen;
  double npart;
  double M,mpt,mpt2;
};

string convertInt(int number);

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

	int fol0,folf;
	string hadtyp,folder,npfile;
	if (argv[1]&&argv[2]&&argv[3])
	{
	folder=argv[1];
	
	stringstream s,s1;
	s << argv[2];
	s >> fol0;
	
	s1 << argv[3];
	s1 >> folf;
	
	if (argv[4])
	{
	hadtyp=argv[4];
	}
  	
  	if (argv[5])
	{
	npfile=argv[5];
	}
	
	}
	else 
	{
	cout << " Error: missing intput of first and last event!" << endl;
	exit(1);
	}
	
  	vector<ev> vv;
  	int tot=1000*(folf-fol0+1);
  	vv.resize(tot);
  	
  	int cn=0;
  	for(int f=fol0;f<=folf;f++){
  	
  	string nameb=folder+"/"+convertInt(f)+"/mpt"+hadtyp+".dat";
	cout << nameb << endl;
  	ifstream inputb(nameb.c_str());
	if (!inputb.is_open())
 	{
 	cout << "Can't open " << nameb << endl;
 	exit(1);
 	}

	string line;
	while (getline(inputb,line)) {
	
		std::vector<std::string> x = split(line, ' ');
		vector<double> y(5,0);
		
		for (int k=0;k<5;k++){
		stringstream sk;
		sk << x[k];
		sk >> y[k];
		}
		
		vv[cn].npart= y[0];
		vv[cn].mpt= y[1];
		vv[cn].mpt2=y[2];
		vv[cn].M=y[3];
		vv[cn].flag=y[4];
		cn++;
	}
	inputb.close();
  	}
  	
  	cout << "read in cens" << endl;
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

	//	cout << "start" << endl;

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
	//	getchar();
	
	
	int ctot=cmax.size();
	int atot=tot;
	
	int sep=ctot/10+2;
	vector < double > mpt,mpt2;
	vector<int> ncen;
	mpt.resize(ctot,0);
	mpt2.resize(ctot,0);
	ncen.resize(ctot,0);
       
       
       	// sort centralities, calculate mean pt
	for (int i=0;i<atot;i++){
		for (int c=0;c<ctot;c++){
		  
		       
			if (vv[i].npart<=nph[c]&&vv[i].npart>npl[c]&&vv[i].flag!=1) {
			mpt[c]+=vv[i].mpt;
			mpt2[c]+=vv[i].mpt2;
			vv[i].cen=c;
			ncen[c]++;
			}
		}
		if (vv[i].npart>nph[0]&&vv[i].flag!=1) {
			mpt[0]+=vv[i].mpt;
			mpt2[0]+=vv[i].mpt2;
			vv[i].cen=0;
			ncen[0]++;
			}
		if (vv[i].npart==npl[ctot-1]&&vv[i].flag!=1)
		{
			mpt[ctot-1]+=vv[i].mpt;
			mpt2[ctot-1]+=vv[i].mpt2;
			vv[i].cen=ctot-1;
			ncen[ctot-1]++;
			} 
		else vv[i].cen=ctot;
	}
	
	
	ofstream OUT3;
	string snam=folder+"/cpT_"+hadtyp+".dat";
	OUT3.open(snam.c_str());
	if (!OUT3.is_open())
	{
		cout << "Error: cannot open out2 file!" << endl;
		exit(1);
	}
	
	vector<double>rpt;
	rpt.resize(ctot);
	for (int c=0;c<ctot;c++) 
	{
		rpt[c]=mpt2[c]/mpt[c];
		OUT3 <<  c << "  " <<  rpt[c] <<  endl;		
	}
	OUT3.close();
	 

 return 0;
}

string convertInt(int number)
{
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}


#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <string.h>
#include <vector>



using namespace std;

struct var{

  double rx,ry,e,T;

};

double tdis( vector <var> list, double & etotout);
string convertInt(int number);
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);


int main (int argc, char *argv[]) //execute with ./a.out start_event# end_event#
{

	string infol;
	int ev1,ev2;
	if (!argv[1]||!argv[2])
	{
		cout << "Please include folder name and event numbers" << endl;
		exit(1);
	}
	else
	{
		infol=argv[1];
		
		stringstream s;
		s << argv[2];
		s >> ev1;
		
		if (argv[3]){
		stringstream s1;
		s1 << argv[3];
		s1 >> ev2;
		}
		else ev2=ev1;
		
	}
	
	for (int ev=ev1;ev<=ev2;ev++){
	  vector<double> templist,taulist,elist;
	for (int t=1;t<=200;t++){
	string name=infol+"/sveprofile"+convertInt(t)+"_ev"+convertInt(ev)+".dat";
	ifstream input(name.c_str());
	if (!input.is_open())
 	{
 	break;
 	}

	
	string line;
	getline(input,line);
	std::vector<std::string> xx = split(line, ' ');
	stringstream ss;
	double b;
	ss << xx[0];
	ss >> b;
	
	
	
	taulist.push_back(b);
	
	
	
	vector< var > list;
	while (getline(input,line)){
	
	std::vector<double> y (4,0) ;
	std::vector<std::string> x = split(line, ' ');

	
	
	for(int j=0;j<4;j++)
	{
	stringstream s;
	s << x[j];
	s >> y[j];
	}
	
	var sub;
	sub.rx=y[0];
	sub.ry=y[1];
	sub.e=y[2];
	sub.T=y[3];
	
		
	list.push_back(sub);
	
	}
	input.close();

	double eout;
	double ttsub=tdis(list,eout);
	templist.push_back(ttsub);
	elist.push_back(eout);
	}

	int tend=taulist.size();
	if (tend>0){
	string name2=infol+"/temps/ev"+convertInt(ev)+".dat";
	ofstream OUT2(name2.c_str());
	if (!OUT2.is_open())
 	{
 	cout << "Can't open " << name2 << endl;
 	}
	
	
	for(int t=0;t<tend;t++){
	  OUT2 <<  taulist[t] << " " << templist[t] << " " << taulist[t]*elist[t] << endl;
	}
	
	OUT2.close();
	}
	}

}

double tdis(vector <var> list, double & etotout){

	

	int max=list.size();
	
	double xcm=0,ycm=0,etot=0,Ttot=0;
	for (int s=0;s<max;s++){
	xcm+=list[s].rx*list[s].T;
	ycm+=list[s].ry*list[s].T;
	Ttot+=list[s].T;
	etot+=list[s].e;
	}
	xcm/=Ttot;
	ycm/=Ttot;

	etotout=etot/max;
	
	
	vector<double> r2;
	r2.resize(max);
	double rb=0,rv=0;
	for (int s=0;s<max;s++){
	double xsub=(list[s].rx-xcm);
	double ysub=(list[s].ry-ycm);
	r2[s]=sqrt(xsub*xsub+ysub*ysub);
	
	rv+=list[s].T*r2[s];
	
	
	rb+=r2[s];
	}
		
	return rv/rb*1000;

}

string convertInt(int number)
{
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

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

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

string  convertInt(int number)
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


int main (int argc, char *argv[])
{

  string infile=argv[1];

  string bin=argv[2];

  string ev0=argv[3];

  string evf=argv[4];

  string name="inputfiles/"+infile+".dat";



	ifstream input(name.c_str());
	if (!input.is_open())
 	{
 	cout << "Can't open " << name << endl;
 	exit(1);
 	}

  string line,chk="output_folder:";
	while (getline(input,line))
  {

	std::vector<std::string> x = split(line, ' ');

  if (x[0]==chk) {
    std::vector<std::string> xslash = split(x[1], '/');
    int xsize=xslash.size();
    string fol;
    for (int i=3;i<xsize;i++) {
      if (i==3) fol=fol+xslash[i];
      else fol=fol+"/"+xslash[i];
    }

    int iclength=xslash[0].size();

    string in="input";
    std::size_t found = infile.find(in);
    string runID=infile.substr (found+5+iclength);


    ofstream OUT;
    string name2="run"+runID+"_"+ev0+"_"+evf+".sh";
  	OUT.open(name2.c_str());

  	if (!OUT.is_open())
  	{
  		cout << "Error: Can't open" << name2 << endl;
  		exit(1);
  	}


    OUT << "#!/bin/bash " <<endl;

    OUT << "#PBS -d /projects/jnorhos/v-USPhydro2" <<endl;
    OUT << "#PBS -l walltime=30:00:00" <<endl;
    OUT << "#PBS -N vUSPtests" <<endl;
    OUT << "#PBS -l nodes=1:ppn=1" <<endl;
    OUT << "#PBS -q secondary" <<endl;
    OUT << "#PBS -j oe" <<endl;

    OUT << "cd /projects/jnorhos/v-USPhydro2/" <<endl;

    OUT << "rm $PBS_O_WORKDIR/logs/*" <<endl;
    OUT << "rm $PBS_O_WORKDIR/runs/*" <<endl; 

    OUT << "for (( i=" << ev0 << "; i<=" << evf << "; i++))" <<endl;
    OUT << "    do" <<endl;
    OUT << "    $PBS_O_WORKDIR/charm.sh " << runID << " \"$i\" \"$i\" " << xslash[2] << " " << fol << " " << xslash[0] << " " << xslash[1] << " " <<endl;
    OUT << "    done" <<endl;
    OUT << "  wait" <<endl;



    OUT.close();

    break;
  }
}




}

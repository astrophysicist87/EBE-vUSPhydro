#ifndef GLOBAL_H_ 
#define GLOBAL_H_

#include <cmath>
#include <vector>
#include <stdio.h>
#include <iostream>

using namespace std;

struct SPEC{
double pt;
double phi;
double psi;
double dNdpt;
double wpt;
double wphi;
};

extern vector<vector<SPEC> > spec; 


#endif

#include<iostream>
#include<iomanip>
#include<fstream>
#include<string>
#include<sstream>
#include<math.h>
#include<sys/time.h>
#include<algorithm>

#include "ParameterReader.h"
#include "Arsenal.h"
#include "fit.h"

using namespace std;


int main(int argc, char *argv[])
{
	cout << endl << "********************************************************************************" << endl;
	cout << endl
		<< "            Correlation functions fitting            " << endl
		<< endl
		<< "  Ver 1.0   ----- Author: Christopher Plumberg   " << endl;
	cout << endl << "********************************************************************************" << endl << endl;
	
	// Loop over multiple directories at once
	for ( int iArg = 1; iArg < argc; iArg++)
	{
		// use non-linear chi^2-minimization GSL fit
		fitCF::Get_GF_HBTradii( std::string(argv[iArg]), false );
	
		// use log fit
		fitCF::Get_GF_HBTradii( std::string(argv[iArg]), true );
	}

	return 0;
}

//End of file

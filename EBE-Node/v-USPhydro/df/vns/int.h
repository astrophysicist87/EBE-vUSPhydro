#ifndef _INT_H_
#define _INT_H_


#include "SPH.h"
#include "global.h"


template <int D,int DD>
void gausslegendrequad(int ps,  SPH<D,DD> &sph)
{
	for(int j=0;j<sph.vnmax;j++)
	{
	sph.ide[j].I.x[0]=0;
	sph.ide[j].I.x[1]=0;}
	
	
	
	
	int phimax=spec[0].size();
	
	for(int i=0;i<phimax;i++)
	{	
	double multi=spec[ps][i].dNdpt*spec[ps][i].wphi;
		
	sph.Ia[ps]+=multi;	
	for(int j=0;j<sph.vnmax;j++)
	{
	double theta=sph.ntab[j]*spec[ps][i].phi;
	
	sph.ide[j].I.x[0]+=cos(theta)*multi;
	sph.ide[j].I.x[1]+=sin(theta)*multi;
	
	}
	}
	
	
}

double lin(double p, int i){

int low=0;
int ps=0;
int ptmax=spec.size();

while ((low==0)&&(ps<ptmax)){
if (p<spec[ps][i].pt) low=1;
else ps++;
}

double out;
if ((ps>0)&&(ps<ptmax)) {
out=spec[ps-1][i].dNdpt+ (spec[ps][i].dNdpt-spec[ps-1][i].dNdpt)*(p-spec[ps-1][i].pt)/(spec[ps][i].pt-spec[ps-1][i].pt);
}
else{
cout << "Error: pT out of interpolation range!" << endl;
cout << p << " " << ps <<  endl;
exit(1);
}

return out;

}

template <int D,int DD>
void glint(double p, double dpt, int pi,  SPH<D,DD> &sph)
{
	for(int j=0;j<sph.vnmax;j++)
	{
	sph.ide[j].I.x[0]=0;
	sph.ide[j].I.x[1]=0;}
	
	
	
	
	int phimax=spec[0].size();
	
	for(int i=0;i<phimax;i++)
	{	
	double multi=lin(p,i)*spec[0][i].wphi;
	
		
	sph.Ia[pi]+=multi;	
	for(int j=0;j<sph.vnmax;j++)
	{
	double theta=sph.ntab[j]*spec[0][i].phi;
	
	sph.ide[j].I.x[0]+=cos(theta)*multi;
	sph.ide[j].I.x[1]+=sin(theta)*multi;
	
	}
	}
	
	
}




template <int D,int DD>
void printstart(string ofolder,string ev,string &out1, string &out2,  SPH<D,DD> &sph)
{
	
	string vtyp;
	if (sph.decays!=3){
	if (sph.typ==0) vtyp="vn"; 
	else if (sph.typ==1) vtyp="bvnc"; 
	else if (sph.typ==2) vtyp="svnc"; 
	else if (sph.typ==3) vtyp="sbvnc"; 
	}
	else{
	if (sph.typ==0) vtyp="vn"; 
	else if (sph.typ==1) vtyp="bvn"; 
	else if (sph.typ==2) vtyp="svn"; 
	else if (sph.typ==3) vtyp="sbvn"; 
	
	}
	
	
	
	
	
	out2=ofolder+"/ev"+ev+"_"+vtyp+".dat";
	ofstream OUT2;	
	OUT2.open(out2.c_str());
	if (!OUT2.is_open())
	{
		cout << "Error: cannot open spectra file!" << endl;
		exit(1);
	}
	
	
	OUT2 << "p [GeV]    dN/(pT dpT) [pT MeV]";

	for(int j=0;j<sph.vnmax;j++)
	{
	OUT2 << " v" << sph.ntab[j] << "   "  ;
	}
	OUT2 << endl;
	
	OUT2.close();
	
	

}


void p_vintstart(string ofolder,string &name)
{
	name=ofolder+"/"+name;
		
	ofstream OUT3;
	OUT3.open(name.c_str());
	if (!OUT3.is_open())
	{
		cout << "Error: cannot open " << name << " file!" << endl;
		exit(1);
	}
	
	OUT3.close();
	
	

}

template <int D,int DD>
void p_vint(string name, int ev, SPH<D,DD> sph ,double mpt,double bot)
{

	
	ofstream OUT3;
	OUT3.open(name.c_str(), ios::out | ios::app );
	if (!OUT3.is_open())
	{
		cout << "Error: cannot open out2 file!" << endl;
		exit(1);
	}
	
	
	
	OUT3 << ev << "  " ;
	
	for(int j=0;j<sph.vnmax;j++)
	{
		OUT3 <<  sph.ide[j].intert << "  " <<  sph.ide[j].psi  << " " ;		
	}
	OUT3 << mpt  << " " << bot << endl;
	
	OUT3.close();
	
	

}




template <int D,int DD>
  void p_vint(string name, int ev,SPH<D,DD> sph )
{

  
  ofstream OUT3;
  OUT3.open(name.c_str(), ios::out | ios::app );
  if (!OUT3.is_open())
    {
      cout << "Error: cannot open out2 file!" << endl;
      exit(1);
    }
  
  
  
  OUT3 << ev << "  " ;
  
  for(int j=0;j<sph.vnmax;j++)
    {
      OUT3 << 0 << "  " <<  0  << " " ;
    }
  OUT3 << 0  << " " << 0 << endl;
  
  OUT3.close();
  
  

}

template <int D,int DD>
  void printnull(string out2,SPH<D,DD> sph , int ptmin)
{

  ofstream OUT2;
  OUT2.open(out2.c_str(), ios::out | ios::app );
  if (!OUT2.is_open())
    {
      cout << "Error: cannot open " << out2 << " file!" << endl;
      exit(1);
    }
  
  for (int ps=ptmin;ps<spec.size();ps++)
    {
      OUT2 <<  spec[ps][0].pt << "  " << 0 ;

      for(int j=0;j<sph.vnmax;j++)
	{
	  OUT2  << "  " << 0 << " " << 0 ;
	}
      OUT2 << endl;
    }
  
  OUT2.close();


}







template <int D,int DD>
void print(string out1, SPH<D,DD> sph, int ptmin)
{
	
	ofstream OUT;
	OUT.open(out1.c_str(), ios::out | ios::app );
	if (!OUT.is_open())
	{
		cout << "Error: cannot open print file!" << endl;
		exit(1);
	}
	
	
	for(int ps=ptmin;ps<spec.size();ps++)
	{
	
	for(int i=0;i<spec[0].size();i++)
	{
		
	OUT << spec[ps][i].pt << "  "<< spec[ps][i].phi << "  " << spec[ps][i].dNdpt << endl;
	}
	}
	
	OUT.close();

}



template <int D,int DD>
void print2(string out2,  SPH<D,DD> sph, int ptmin)
{

	ofstream OUT2;
	OUT2.open(out2.c_str(), ios::out | ios::app );
	if (!OUT2.is_open())
	{
		cout << "Error: cannot open " << out2 << " file!" << endl;
		exit(1);
	}
	
	for (int ps=ptmin;ps<spec.size();ps++)
	{
	OUT2 <<  spec[ps][0].pt << "  " << sph.Ia[ps] ;

	for(int j=0;j<sph.vnmax;j++)
	{
	OUT2  << "  " << sph.vout[j][ps]/sph.Ia[ps] << " " << spec[ps][j].psi ;
	}
	OUT2 << endl;
	}
	
	OUT2.close();


}





#endif

#ifndef _GLOB_H_
#define _GLOB_H_

class GLOB
{
public:
	int ngl;
	double *w,*z,*v;	
	int ev,h;
	double p,phi,dNdpdphi;
	double *ptab;
	int n;
	double N;
	int vnmax;
	int steps;
	int *ntab;
	double *subp;
	void in();
	void vn();
	int typ;
	void fixptab(double pstart,double pend,double dp);
	
	void destroy() 
	{
		delete [] w; 
		delete [] z; 
		delete [] ntab;
	}

};

void GLOB::in ()
{
	w=new double[ngl];
	z=new double[ngl];

}


void GLOB::vn ()
{
	vnmax=6;
  	ntab=new int [vnmax];
	ntab[0]=1;
	ntab[1]=2;
  	ntab[2]=3;
  	ntab[3]=4;
  	ntab[4]=5;
  	ntab[5]=6;


}

void GLOB::fixptab(double pstart,double pend,double dp)
{
	steps=ceil((pend-pstart)/dp+1);
	ptab=new double [steps];
	ptab[0]=pstart;
	for (int i=1;i<steps;i++)
	{
		ptab[i]=ptab[i-1]+dp;
	}
	

}

#endif

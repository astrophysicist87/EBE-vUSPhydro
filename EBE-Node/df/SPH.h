#ifndef _SPH_H_
#define _SPH_H_

#include <string>
#include <iostream>
#include <vector>
#include "vector.h"
#include "vnvar.h"
#include "spectra.h"
#include "bessel.h"
#include "tables.h"
#include <complex>

#include "bessf.h"

template <int D,int DD>
class SPH {
private:
	
	
	double facc;
	static const int Nmax=10;
	
	double I1c,I2c,I1sc,I2sc;
	complex<double> I1c_comp, I2c_comp, I1sc_comp, I2sc_comp;
	int h1tot;

	// vectors to hold space-time moments w.r.t. I1, I2
	vector<double> ST_out1, ST_out2, ST_out1c, ST_out2c;
	vector<double> ST_I1, ST_I2, ST_I1c, ST_I2c, ST_I1sc, ST_I2sc;
	
	double Eperp(double p, double m) {return sqrt(p*p+m*m);}
	double pperp(double p, double phi, Vector<double,DD> u) {return p*(u.x[1]*cos(phi)+u.x[2]*sin(phi));}
	//double nuprod( int nsph) {return par[nsph].n&par[nsph].u;   }
	
	
	 static const string bulk;
	 static const string ideal ;	
	 static const string shear ;
	 static const string shearbulk;	
	 static const string bulkshear;
	
	
	
public:
	static const double scale=0.1973;
	static const double sc3=0.1973*0.1973*0.1973;
	double T;
	double m3;
	int N,NHAD;
	int spectra;
	int vnmax;
	int ngl;
	double *w,*z;
	int ntab[6];
	double ** vout, ** voutc;
	 
	
	struct PAR
	{
	Vector<double,DD> u,n,r;
	double vol,bulk,bulkpi,nu,tau,s;
	double pi00,pi11,pi22,pi33,pi12;
	double spec;
	}; // structure that contains the basic info for each SPH particle e.g. normal vecotrs, four-velocity etc
	
	struct HAD
	{
	double mass,deg,theta,spar;
	double form3,halm2;
	double E0,B0,D0;
	double vfac,svfac;
	int id,baryon;
	double C00,mT;
	  int anti,null;
	  vector<int> sids;
	}; // structure that is set up for each observed hadron. 

	struct HLIST
	{
	double E0,B0,D0;
	double C00,mT;
	int T;
	}; // structure that is set up for each observed hadron. 
	
	HLIST *hlist;

	int evn,evncor;  // number of sph particles, file number of event
	PAR *par;
	VNVAR *ide,*cor;
	Vector<double,DD> *qv;
	
	
	double outc;
	int typ;
	vector<HAD> had,nmes;
	string before,after,rnum;
	int start, end;
	string folder;
	string flist2;
	double *Ia,*Iac;

	// space-time moments
	vector<double> ST_out, ST_outc, ST_outsc;
	
	string neg;
	
	double pt_s,pt_e,pt_step;
	double *v;
	double *vc;
	vector<double> pt,phi;
	
	
	SPH<D,DD>();
	~SPH<D,DD>();
	void readin(_inputIC ics);
	void readin2(int cev);
	double dNdpdphi(double p, double phi, HAD cur, bool set_spacetime_moments = false);
	double dNdpdphi_FT( double p, double phi, double pRap, HAD cur,
						double Q0, double QX, double QY, double QZ );
	void Iout(double &I1, double &I2, double p, double phi, HAD cur,int nsph, bool set_spacetime_moments = false);
	void IoutFT(complex<double> &I1_comp, complex<double> &I2_comp, double pT, double phi, double pRap,
				HAD cur,int nsph, double Q0, double QX, double QY, double QZ);
	string convertInt(int number);
	void flist();
	void calcF2(HAD cur, int nsph,double pd,double &F0,double &F1, double &F2);
	void checknu( );
	void setcoef(int i);
	void calcsPI(int h);
	double totmul(int ht);
	void delavg(double avg, int &n, PAR * par2);
	void restart(int pTmax);
	void restartc(int pTmax);
	void setvns(int ptmax );
	void destroyvout(int pTmax) ;
	void destroyvoutc(int pTmax);
	void printrun(string ofolder);
	
	void readweights(string grid,string grid2);
	string grid,grid2;
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
	
	// resize vectors to hold space-time moments
	ST_out1.resize(15);
	ST_out2.resize(15);
	ST_out1c.resize(15);
	ST_out2c.resize(15);
	ST_I1.resize(15);
	ST_I2.resize(15);
	ST_I1c.resize(15);
	ST_I2c.resize(15);
	ST_I1sc.resize(15);
	ST_I2sc.resize(15);
	ST_out.resize(15);
	ST_outc.resize(15);
	ST_outsc.resize(15);
}

template <int D,int DD>
SPH<D,DD>::~SPH()
{
	
}


// reads in the basic information from "input.dat" such at the number of events, events folder, particles to observe etc.
template <int D,int DD>
void SPH<D,DD>::readin(_inputIC ics)
{

	string infile="input/"+ics.man;
	string resolist,hadclist;
        FILE * myfile = fopen (infile.c_str(),"r");
        if(myfile== NULL)
        {
  		cout << "Error: "<< infile << " does not exist. \n";
		exit(1);	
  	}	
		
		
	   char charin[100];
           fscanf(myfile,"%*s %s \n",charin); // type of equations (ideal, bulk etc)
           string type=charin;
           fscanf(myfile,"%*s  %s \n",charin); // folder that contains the events
           folder=charin; 
           fscanf(myfile,"%*s  %s ",charin); // reads in pt points
           grid=charin;
           fscanf(myfile,"  %s ",charin);  //reads in phi points
           grid2=charin;
           fscanf(myfile,"%*s  %*s \n");
           fscanf(myfile,"%*s  %i %i \n",&start,&end); // range of events e.g. start=0 end=199
           if (ics.on==1){
  	   	start=ics.start; 
  	   	end=ics.end;
  	   }
           fscanf(myfile,"%*s %lf \n",&T); 
           
                   
  	   fscanf(myfile,"%*s %s \n",charin); // file that contains df corrections
           flist2=charin;
           fscanf(myfile,"%*s  %*s \n");  // file that contains phi/pt grid
          
           fscanf(myfile,"%*s  %s",charin);  // file that contains list of hadrons and decays
           resolist=charin;
            fscanf(myfile,"%*s  %s",charin);  // file that contains list of hadrons to actually compute
           hadclist=charin;
           fclose(myfile);
           
           
          FILE * myfile3 = fopen (hadclist.c_str(),"r");
        if(myfile3== NULL)
        {
  		cout << "Error: decay/input/numbers.dat does not exist. \n";
		cout << "Error: " << hadclist.c_str() << " does not exist. \n";
		exit(1);	
  	}	
  		
	 
           vector <double> hcheck;
           int subc;
           while(fscanf(myfile3,"%i", &subc)==1)
           {
           	  
           	hcheck.push_back(subc);
           }
           fclose(myfile3); 
           
           
        // reads in the resonances and their properties
	FILE * myfile2 = fopen (resolist.c_str(),"r");
        if(myfile2== NULL)
        {
  		cout << "Error: decay/input/"<< resolist<< " does not exist. \n";
		exit(1);	
  	}	
	 
           int decays;
           HAD had2;
           vector <HAD> had3;
           //while(fscanf(myfile2,"%i %*s %lf %*f%*i %i %*i%*i%*i %lf %*i %i %*f%*f%*f", &had2.id, &had2.mass,&had2.baryon, &had2.deg,&decays)==5)
           while(fscanf(myfile2,"%i %*s %lf %*f %lf %i %*i%*i%*i %*f %*i %i ", &had2.id, &had2.mass,&had2.deg, &had2.baryon, &decays)==5)
           {
           	if (had2.baryon==0) had2.theta=-1;
           	else had2.theta=1;
           	int hc=0,hsize=hcheck.size();
           	while (hc<hsize){
           	if (had2.id==hcheck[hc])
           	{
           		
           		if (had2.baryon<0) had3.push_back(had2);
           		else if (had2.id<0&&had2.baryon==0) nmes.push_back(had2);
			else {
			  had2.anti=0;
			  had2.null=0;
			  had.push_back(had2);
			}
           		
           		
           		break;
           	}
           	else hc++;
           	}
           	
           	
           	
           	for (int l=0;l<decays;l++)
           	{
           	fscanf(myfile2,"%*i%*i%*f%*i%*i%*i%*i%*i");
           	}
           	
           	
           	
//           	if (had2.baryon==1){//creates anti-particle for baryons
//           	
//          		
////           	j++;
////           	HAD had3;
////           	had3.id=-had2.id;
////           	had3.mass=had2.mass;
////           	had3.baryon=-had2.baryon;
////           	had3.deg= had2.deg;
////           	had3.theta=1;
////           	had.push_back(had3);
//           	}
           	
           }
           cout << "size " <<  had.size() << endl;
           fclose(myfile2);
	//cout << "Resonances read in!\n"; 
	
           NHAD=had.size();
        
           for (int ch=0;ch<NHAD;ch++){
	     if (had[ch].baryon==0) {
	       int nmessiz=nmes.size();
	       for (int nm=0;nm<nmessiz;nm++){
		 if (had[ch].id==abs(nmes[nm].id)){
		   had[ch].anti=1;
		   //  cout << had[ch].id << endl;
		   break;
		 }
	       }        
	     }

             for (int nu=0;nu<NHAD;nu++){
	       if (had[ch].null<=1){
		 if ((ch!=nu)&&(had[ch].mass==had[nu].mass)&&(had[ch].deg==had[nu].deg)&&(had[ch].baryon==had[nu].baryon)){
		   had[ch].null=1;
		   had[nu].null=2;
		   had[ch].sids.push_back(nu);
		   had[nu].sids.push_back(ch);
		 }}
	       
	     }



           }
           
//        ofstream OUT4;
//	string inname2="input/numbers2.dat";
//	OUT4.open(inname2.c_str() );
//	if (!OUT4.is_open())
//	{
//		cout << "Error: cannot open out4 file!" << endl;
//		exit(1);
//	}
//	
//	for (int k=0;k<NHAD;k++) OUT4 <<  had[k].id << endl ;
//	OUT4.close();
//	exit(1);
           
      	  
	
	if (type==ideal){
		typ=0;
		before="freezeout_ev";}
	else if (type==bulk){
		typ=1;
		before="bvfreezeout_ev";}
	else if (type==shear){
		typ=2;
		before="svfreezeout_ev";}
	else if ((type==shearbulk)||(type==bulkshear)){
		typ=3;
		before="sbvfreezeout_ev";}

        after=".dat";
        N=end-start+1; // determines the total number of events
        
       
        
        readweights(grid,grid2);
        
//        ofstream OUT3;
//	string inname="input/vn_input.dat";
//	OUT3.open(inname.c_str() );
//	if (!OUT3.is_open())
//	{
//		cout << "Error: cannot open out2 file!" << endl;
//		exit(1);
//	}
//	
//	
//	
//	OUT3 <<  "typeofequations:  " << type << endl ;
//	OUT3 <<  folder << endl ;
//	OUT3 <<  "range(pt,phi):  input/gl15.dat input/gq20.dat" << endl ;
//	OUT3 <<  "decays:  1"<< endl ;
//	
//	OUT3.close();
		
	
}

// reads in the basic information from "input.dat" such at the number of events, events folder, particles to observe etc.
template <int D,int DD>
void SPH<D,DD>::readin2(int cev)
{
	string event="input/"+folder+"/"+before;
	event+=convertInt(cev)+after;
	FILE * myfile = fopen (event.c_str(),"r");
	if (myfile==NULL) 
	{
	cout << "Error: Can't open Event " << cev << endl;
	cout << event<< endl;
	exit(1);
	}
	
	PAR * par2=new PAR [200000];
	int i=0;
	int j=0;
	double avgnu=0;
	
	if (typ==0) //ideal
	{
//	fscanf(myfile,"%lf\n",&s); 
	while (!feof(myfile))   // runs over all the SPH particles in the event
	{
	fscanf(myfile,"%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf  %lf\n",&par2[i].n.x[0],&par2[i].n.x[1],&par2[i].n.x[2],&par2[i].u.x[0],&par2[i].u.x[1],&par2[i].u.x[2],&par2[i].vol,&par2[i].tau,&par2[i].r.x[0],&par2[i].r.x[1],&par2[i].s); 
	//par2[i].vol/=sc3;
//	// old version starts here		
//	s=1.87229;
//	while (!feof(myfile))   
//	{
//	fscanf(myfile,"%lf %lf %lf %lf %lf %lf %lf %*f \n",&par2[i].n.x[0],&par2[i].n.x[1],&par2[i].n.x[2],&par2[i].u.x[0],&par2[i].u.x[1],&par2[i].u.x[2],&par2[i].vol);
////	// old version ends here
	par2[i].vol/=sc3;		
	
	
	par2[i].nu=par2[i].n&par2[i].u;
	
	if (par2[i].nu<0) j++;
	else
	{
		avgnu+=par2[i].nu;
		i++;
	}

	}
	
	}
	else if (typ==1) // bulk
	{	
	
////	// old version starts here
//	s=1.87229;
//	while (!feof(myfile))   
//	{
//	fscanf(myfile,"%lf %lf %lf %lf %lf %lf %lf %*f %lf \n",&par2[i].n.x[0],&par2[i].n.x[1],&par2[i].n.x[2],&par2[i].u.x[0],&par2[i].u.x[1],&par2[i].u.x[2],&par2[i].vol,&par2[i].bulkpi);  
////	// old version ends here
	
	
	
	
	while (!feof(myfile))   // runs over all the SPH particles in the event
	{
	fscanf(myfile,"%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf  %lf\n",&par2[i].n.x[0],&par2[i].n.x[1],&par2[i].n.x[2],&par2[i].u.x[0],&par2[i].u.x[1],&par2[i].u.x[2],&par2[i].vol,&par2[i].bulkpi,&par2[i].tau,&par2[i].r.x[0],&par2[i].r.x[1],&par2[i].s); 
	par2[i].vol/=sc3;
	
	par2[i].nu=par2[i].n&par2[i].u;
	if (par2[i].nu<0)
	{
		
		j++;
	}
	else
	{
		
		avgnu+=par2[i].nu;
		i++;
	}
	
	
	}
	}
	else if (typ==2) //shear
	{
	
	while (!feof(myfile))   // runs over all the SPH particles in the event
	{
	fscanf(myfile,"%lf %lf %lf %lf %lf %lf %lf %lf %lf  %lf %lf %lf %lf  %lf %lf  %lf \n",&par2[i].n.x[0],&par2[i].n.x[1],&par2[i].n.x[2],&par2[i].u.x[0],&par2[i].u.x[1],&par2[i].u.x[2],&par2[i].vol,&par2[i].pi00,&par2[i].pi11,&par2[i].pi22,&par2[i].pi33,&par2[i].pi12,&par2[i].tau,&par2[i].r.x[0],&par2[i].r.x[1],&par2[i].s); 
	par2[i].vol/=sc3;
	par2[i].nu=par2[i].n&par2[i].u;
	if (par2[i].nu<0)
	{
		
		j++;
	}
	else
	{
		
		avgnu+=par2[i].nu;
		i++;
	}
	
	
	}
	}
	else if (typ==3) //bulk+shear
	{
	
	while (!feof(myfile))   // runs over all the SPH particles in the event
	{
	//cout << "Reading in " << myfile << ": dimension = " << par2[i].n.dim() << "   " << par2[i].u.dim() << "   " << par2[i].r.dim() << endl;
	fscanf(myfile,"%lf %lf %lf %lf %lf %lf %lf %lf %lf  %lf %lf %lf %lf %lf %lf %lf  %lf \n",&par2[i].n.x[0],&par2[i].n.x[1],&par2[i].n.x[2],&par2[i].u.x[0],&par2[i].u.x[1],&par2[i].u.x[2],&par2[i].vol,&par2[i].bulkpi,&par2[i].pi00,&par2[i].pi11,&par2[i].pi22,&par2[i].pi33,&par2[i].pi12,&par2[i].tau,&par2[i].r.x[0],&par2[i].r.x[1],&par2[i].s); 
	par2[i].vol/=sc3;
	par2[i].pi33*=par2[i].tau*par2[i].tau;
	par2[i].nu=par2[i].n&par2[i].u;
	if (par2[i].nu<0)
	{
		
		j++;
	}
	else
	{
		
		avgnu+=par2[i].nu;
		i++;
	}
	
	
	}
	}
	fclose(myfile);
	
	avgnu/=i;
	delavg(avgnu,i,par2);
	delete [] par2;
	
	// not sure if it is needed?
	evn=i;
	evncor=cev;
	//cout << evn << endl;
	cout << "% of negative SPH particles=" << (j*100.)/(i+j) << "%" << endl;
	cout << "Pions=" << totmul(0) <<  endl;
	//cout << "Pions=" << totmul(0) <<  " Kaons=" << totmul(1) <<  " Protons=" << totmul(2) <<  endl;
	cout << "Event "<< cev << endl;
	
	
}

template <int D,int DD>
void SPH<D,DD>::delavg(double avg, int &n, PAR * par2)
{
	
	
	int *list=new int [n];
	
	int nl=0;
	double avgp=avg/1000;
	for(int i=0;i<n;i++)
	{
		if (par2[i].nu<avgp)
		{
			list[i]=1;
			//cout << i << endl;
		
		}
		else 
		{
			list[i]=0;
			nl++;
		}
	
	}
	
	par=new PAR [nl];
	
	int nc=0;
	for(int i=0;i<n;i++)
	{
		if (list[i]==0)
		{
			par[nc]=par2[i];
			nc++;
		}
	}
	delete [] list;
	cout << "SPH with too small nu= " << n-nl << endl;
	n=nl;
	
	


	
}

template <int D,int DD>
double SPH<D,DD>::totmul(int ht)
{

	double tot=0,tot2=0;
	double pre,inner;
	pre=had[ht].deg/(2*PI*PI)*had[ht].mass*had[ht].mass*T;
	
	inner=had[ht].mass/T;
	
	for(int j=0;j<10;j++)
	{
		double add=j+1;
		double sub=add*inner;
		Bessel bes;
		tot2+=pow(-had[ht].theta,j)/add*bes.Kn(2,sub);
	
	}
	tot2*=pre;
	//cout << "rho_pi=" << tot2 << endl;
	
	
	for(int i=0;i<evn;i++)
	{
		tot+=tot2*par[i].vol;
	
	}
	return tot;

}


template <int D,int DD>
void SPH<D,DD>::printrun(string ofolder){

	ofstream OUT;
	
	string vtyp;
	
	
	
	
	if (typ==0) vtyp="i"; 
	if (typ==1) vtyp="bvc"; 
	else if (typ==2) vtyp="svc"; 
	else if (typ==3) vtyp="sbvc"; 
	
	
	string pre=ofolder+"/ev";
	string post=vtyp+"_dNdphidpp.dat";
	
	
//	string runname="run"+rnum+ ".sh";
//  	OUT.open(runname.c_str() );
//  	if (!OUT.is_open())
//	{
//		
//		cout << "Error: cannot open " << runname << endl;
//		getchar();
//		exit(1);
//	}
//	
//	OUT << "#!/bin/bash " << endl;
//	OUT << "cd decays " << endl;
//	OUT << "make -f makefile reso " << endl;
//	OUT << "cd .. " << endl;
//	OUT << "for (( j=$1; j<=$2; j++ ))" << endl;
//	OUT << "do" << endl;
//	OUT << "   ./decays/reso  " << pre << "\"$j\"" <<  post  << " " <<  pre << "\"$j\"d" <<  post  << endl;
//	OUT << "done " << endl;
//	
//	
//	
//	
//	

//  	OUT.close();
//  	cout << "printed run.sh" << endl;
  	
//  	if (typ>0){
//	if (typ==1) vtyp="bv"; 
//	else if (typ==2) vtyp="sv"; 
//	else if (typ==3) vtyp="sbv"; }
//	
	
	
  	
  	}




template <int D,int DD>
void SPH<D,DD>::flist()
{

	
	
	ifstream input(flist2.c_str());
	
	if (!input.is_open())
 	{
 	cout << "Can't open " << flist2 << endl;
 	exit(1);
 	}

	
	string line;
	
	h1tot=101;
	getline(input,line);
	
	hlist=new HLIST [h1tot];
	int i=0;
	while (input >> hlist[i].T >>  hlist[i].E0 >> hlist[i].D0 >> hlist[i].B0)   
	{ 	
	
	i++;
	if (i>h1tot) break;
	}
	input.close();
	
	h1tot=i;
	
	double Tnew=T*1000;
	for (int ru=0;ru<h1tot;ru++){
	
	if ((hlist[ru].T<(Tnew+0.01))&&(hlist[ru].T>(Tnew-0.01))) {
	h1tot=1;
	hlist[0].E0=hlist[ru].E0;
	hlist[0].D0=hlist[ru].D0;
	hlist[0].B0=hlist[ru].B0;
	//cout <<  hlist[ru].T << " " << hlist[ru].B0 << endl;
	break;
	}
	
	}
	
	
	
	
	
	
}

template <int D,int DD>
void SPH<D,DD>::setcoef(int i)
{
	

	
  if (h1tot==1)
  {
	had[i].E0=hlist[0].E0;
  	had[i].D0=hlist[0].D0;
  	had[i].B0=hlist[0].B0;
  }
  else{
	double mT=had[i].mass/T;
	if (mT<hlist[0].mT) cout << "Error: mass is smaller than the pion!" << endl;

  	for (int j=1;j<h1tot;j++)
  	{
  	if (mT<hlist[j].mT) 
  	{
  		double mfac=(mT-hlist[j-1].mT)/(hlist[j].mT-hlist[j-1].mT);
  		had[i].E0=hlist[j-1].E0+mfac*(hlist[j].E0-hlist[j-1].E0);
  		had[i].D0=hlist[j-1].D0+mfac*(hlist[j].D0-hlist[j-1].D0);
  		had[i].B0=hlist[j-1].B0+mfac*(hlist[j].B0-hlist[j-1].B0);
  		cout << "constants" << endl;
  		cout << had[i].E0 << " " << had[i].D0 << " "  << had[i].B0 << endl;
  		break;
  	
  	}
  
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
double SPH<D,DD>::dNdpdphi(double p, double phi, HAD cur, bool set_spacetime_moments /*= false*/)
{
	
	double vfac=cur.vfac;
	double out=0,outsc=0;
	outc=0;
	string negc="neg";
	
	std::fill( ST_out.begin(), ST_out.end(), 0.0 );
	std::fill( ST_outc.begin(), ST_outc.end(), 0.0 );
	std::fill( ST_outsc.begin(), ST_outsc.end(), 0.0 );
	
	for (int i=0;i<evn;i++)
	{
		double I1,I2;
		
		std::fill( ST_I1.begin(), ST_I1.end(), 0.0 );
		std::fill( ST_I2.begin(), ST_I2.end(), 0.0 );

		Iout(I1,I2,p,phi,cur,i, set_spacetime_moments);
		//cout << "Check Iout: " << I1 << "   " << I2 << endl;
		
		
		double qp=p*cos(phi)*qv[i].x[1]+p*sin(phi)*qv[i].x[2];
		
		double qtot=qv[i].x[0]+qp;
		
		double sub=qv[i].x[0]*I1+qp*I2;
		if (neg!=negc){ if ((sub<0)||qtot<0) sub=0;}
		out+=sub;  
		
		if ( set_spacetime_moments )
		{
			for (int ii = 0; ii < 15; ii++)
			{
				double sub_ii = qv[i].x[0]*ST_I1[ii]+qp*ST_I2[ii];
				if (neg!=negc)
				{
					if ((sub_ii<0)||qtot<0) sub_ii=0;
				}
				ST_out[ii] += sub_ii;
//if (ii==0) cout << "Check here(0): " << sub << "   " << sub_ii << "   " << out << "   " << ST_out[ii] << endl;
			}
		}

		// if bulk		 
		if ((typ==1)||(typ==3))
		{
			double sub2=qv[i].x[0]*I1c+qp*I2c;
			if (isnan(sub2)) sub2=0;
			if (neg!=negc)
			{
				if ((sub2<0)||qtot<0||isnan(sub2)||sub2>100) sub2=0;
			}
			outc+=sub2;

			if ( set_spacetime_moments )
			{
				for (int ii = 0; ii < 15; ii++)
				{
					double sub2_ii=qv[i].x[0]*ST_I1c[ii]+qp*ST_I2c[ii];
					if (isnan(sub2_ii)) sub2_ii=0;
					if (neg!=negc)
					{
						if ((sub2_ii<0)||qtot<0||isnan(sub2_ii)||sub2_ii>100) sub2_ii=0;
					}
					ST_outc[ii] += sub2_ii;
				}
			}
		}

//cout << "Check here(1): " << out << "   " << ST_out[0] << endl;

		// if shear
		if (typ>1)
		{
			double sub3=(qv[i].x[0]*I1sc+qp*I2sc)/par[i].s;
			if (isnan(sub3)) sub3=0; 
			if (neg!=negc)
			{
				if ((sub3<0)||qtot<0||isnan(sub3)||sub3>100) sub3=0;
			}
			outsc+=sub3;

			if ( set_spacetime_moments )
			{
				for (int ii = 0; ii < 15; ii++)
				{
					double sub3_ii = (qv[i].x[0]*ST_I1sc[ii]+qp*ST_I2sc[ii])/par[i].s;
					if (isnan(sub3_ii)) sub3_ii=0; 
					if (neg!=negc)
					{
						if ((sub3_ii<0)||qtot<0||isnan(sub3_ii)||sub3_ii>100) sub3_ii=0;
					}
					ST_outsc[ii] += sub3_ii;
				}
			}
		}
	}

//cout << "Check here(2): " << out << "   " << ST_out[0] << endl;

	if (isnan(out)==1) cout << out << endl;
	
	if (typ==1)
	{
		outc*=vfac;
		if ( set_spacetime_moments )
			for (int ii = 0; ii < 15; ii++)
				ST_outc[ii] *= vfac;
	}
	else if (typ==2)
	{
		outc=vfac*out+cur.svfac*outsc;
		if ( set_spacetime_moments )
			for (int ii = 0; ii < 15; ii++)
				ST_outc[ii] = vfac*ST_out[ii]+cur.svfac*ST_outsc[ii];
	}
	else if (typ==3)
	{
		outc=vfac*outc+cur.svfac*outsc;
		if ( set_spacetime_moments )
			for (int ii = 0; ii < 15; ii++)
				ST_outc[ii] = vfac*ST_outc[ii]+cur.svfac*ST_outsc[ii];
	}

//cout << "Check here(3): " << out << "   " << ST_out[0] << endl;


	if ( set_spacetime_moments )
		for (int ii = 0; ii < 15; ii++)
		{
			ST_out[ii] *= vfac;
			if ( ii > 0 ) ST_out[ii] /= ST_out[0];
		}

	return out*=vfac;
}



template <int D,int DD>
double SPH<D,DD>::dNdpdphi_FT( double p, double phi, double pRap, HAD cur,
								double Q0, double QX, double QY, double QZ )
{
	double vfac=cur.vfac;
	double out=0,outsc=0;
	outc=0;
	string negc="neg";

	for (int i=0;i<evn;i++)
	{		
		complex<double> I1_comp, I2_comp;
		double I1, I2;
		double pRap = 0.0;	// take y = 0 for right now
		double Q0 = 0.0, QX = 0.0, QY = 0.0, QZ = 0.0;	// also Q = 0
		IoutFT(I1_comp,I2_comp,p,phi,pRap,cur,i, Q0, QX, QY, QZ);
		//cout << "Check IoutFT: " << I1_comp << "   " << I2_comp << endl;
		
		double qp=p*cos(phi)*qv[i].x[1]+p*sin(phi)*qv[i].x[2];
		
		double qtot=qv[i].x[0]+qp;
		
		double sub=qv[i].x[0]*I1+qp*I2;
		if (neg!=negc)
		{
			if ((sub<0)||qtot<0) sub=0;
		}
		out+=sub;  
		 
		if ((typ==1)||(typ==3))
		{
			double sub2=qv[i].x[0]*I1c+qp*I2c;
			if (isnan(sub2)) sub2=0;
			if (neg!=negc)
			{
				if ((sub2<0)||qtot<0||isnan(sub2)||sub2>100) sub2=0;
			}
			outc+=sub2;
		}
		if (typ>1)
		{
			double sub3=(qv[i].x[0]*I1sc+qp*I2sc)/par[i].s;
			if (isnan(sub3)) sub3=0; 
			if (neg!=negc)
			{
				if ((sub3<0)||qtot<0||isnan(sub3)||sub3>100) sub3=0;
			}
			outsc+=sub3;
		}
	}

	if (isnan(out)==1) cout << out << endl;	

	if (typ==1)  outc*=vfac;
	else if (typ==2) outc=vfac*out+cur.svfac*outsc;
	else if (typ==3) outc=vfac*outc+cur.svfac*outsc;
	
	return out*=vfac;
}



template <int D,int DD>
void SPH<D,DD>::checknu( )
{
	qv=new Vector<double,DD> [evn];
	for (int i=0;i< evn;i++)
	{
	qv[i]=(par[i].vol/par[i].nu)*par[i].n;
	}
}



template <int D,int DD>
void SPH<D,DD>::Iout(double &I1, double &I2, double p, double phi, HAD cur,
					 int nsph, bool set_spacetime_moments /*= false*/)
{
	double out1=0,out2=0;
	double out1c=0,out2c=0;
	double pd=pperp(p,phi,par[nsph].u);
	double b0,b1,b2,bsub,fac,pre;
	double g=par[nsph].u.x[0];
	double eperp=Eperp(p,cur.mass);
	double f0s,f1s,f2s;
	double F0c,F1c,F2c;
	double px=p*cos(phi);
	double py=p*sin(phi);
	double px2=px*px,py2=py*py,pxy=2*px*py;
	double ep2=eperp*eperp;
	double ep3=ep2*eperp/4.;
	double TG=T/g;
	double bfac=eperp/TG;
	
	std::fill( ST_out1.begin(), ST_out1.end(), 0.0 );
	std::fill( ST_out2.begin(), ST_out2.end(), 0.0 );
	std::fill( ST_out1c.begin(), ST_out1c.end(), 0.0 );
	std::fill( ST_out2c.begin(), ST_out2c.end(), 0.0 );

	if ((typ==1)||(typ==3)) calcF2(cur,nsph,pd,f0s,f1s,f2s);
	if (typ>1)
	{
		I1sc=0;
		I2sc=0;
		std::fill( ST_I1sc.begin(), ST_I1sc.end(), 0.0 );
		std::fill( ST_I2sc.begin(), ST_I2sc.end(), 0.0 );
	}

	double expT=exp(pd/T);
		
	for (int nn=0;nn<=Nmax;nn++)
	{
		
		Bessel bes;
		double add=(nn+1);
		bsub=add*bfac;
		b0=bes.K0(bsub);
		b1=bes.K1(bsub);
		
		// N.B. - Christopher Plumberg:
		// define my own Bessel function evaluations so
		// they don't get confused with Jaki's
		// (new version of Kn to avoid recalculating K0, K1)
		double STb0 = b0;
		double STb1 = b1;
		double STb2 = bes.Kn(2, b0, b1, bsub);
		double STb3 = bes.Kn(3, b0, b1, bsub);
		double STb4 = bes.Kn(4, b0, b1, bsub);
		double STb5 = bes.Kn(5, b0, b1, bsub);

		
		pre=pow(-cur.theta,nn)*pow(expT,add);
		double preb1=pre*b1;
		out1+=preb1;
		double preb0=pre*b0;
		out2+=preb0;
		
		
		if ( set_spacetime_moments )
		{
			//double tau_SPH     = sqrt( abs( par[nsph].r.x[0]*par[nsph].r.x[0]
			//					     - par[nsph].r.x[3]*par[nsph].r.x[3] ) ); 
			//double x_SPH       = par[nsph].r.x[1];
			//double y_SPH       = par[nsph].r.x[2];
			double tau_SPH     = par[nsph].tau; 
			double x_SPH       = par[nsph].r.x[0];
			double y_SPH       = par[nsph].r.x[1];

			// moments from first term
			ST_out1[0] += pre * STb1 * 1.0;									// S
			ST_out1[1] += pre * STb1 * x_SPH;								// x S
			ST_out1[2] += pre * STb1 * y_SPH;								// y S
			//ST_out1[3] += 0.0;											// z S
			ST_out1[4] += pre * 0.5*(STb0+STb2) * tau_SPH;					// t S
			ST_out1[5] += pre * STb1 * x_SPH * x_SPH;						// x^2 S
			ST_out1[6] += pre * STb1 * x_SPH * y_SPH;						// x y S
			ST_out1[7] += pre * STb1 * y_SPH * y_SPH;						// y^2 S
			//ST_out1[8] += 0.0;											// x z S
			//ST_out1[9] += 0.0;											// y z S
			ST_out1[10] += pre * 0.25*(STb3-STb0) * tau_SPH * tau_SPH;		// z^2 S
			ST_out1[11] += pre * 0.5*(STb0+STb2) * x_SPH * tau_SPH;			// x t S
			ST_out1[12] += pre * 0.5*(STb0+STb2) * y_SPH * tau_SPH;			// y t S
			//ST_out1[13] += 0.0;											// z t S
			ST_out1[14] += pre * 0.25*(STb3+3.0*STb0) * tau_SPH * tau_SPH;	// t^2 S

			// moments from second term
			ST_out2[0] += pre * STb0 * 1.0;									// S
			ST_out2[1] += pre * STb0 * x_SPH;								// x S
			ST_out2[2] += pre * STb0 * y_SPH;								// y S
			//ST_out2[3] += 0.0;											// z S
			ST_out2[4] += pre * STb1 * tau_SPH;								// t S
			ST_out2[5] += pre * STb0 * x_SPH * x_SPH;						// x^2 S
			ST_out2[6] += pre * STb0 * x_SPH * y_SPH;						// x y S
			ST_out2[7] += pre * STb0 * y_SPH * y_SPH;						// y^2 S
			//ST_out2[8] += 0.0;											// x z S
			//ST_out2[9] += 0.0;											// y z S
			ST_out2[10] += pre * 0.5*(STb2-STb0) * tau_SPH * tau_SPH;		// z^2 S
			ST_out2[11] += pre * STb1 * x_SPH * tau_SPH;					// x t S
			ST_out2[12] += pre * STb1 * y_SPH * tau_SPH;					// y t S
			//ST_out2[13] += 0.0;											// z t S
			ST_out2[14] += pre * 0.5*(STb2+STb0) * tau_SPH * tau_SPH;		// t^2 S
		}
		
		
		
		if ((typ==1)||(typ==3))
		{
			fac=TG/add;
			b2=bes.Kn(2,bsub);
			F0c=1+add*f0s;
			F1c=add*f1s;
			F2c=add*f2s;
			
			double prep=preb1*eperp;
			double facF2=fac*F2c;
			double F0F2=F0c+F2c*ep2;
			
			
			out1c+=ep2*(F1c*preb0+pre*facF2*b2)+prep*(F0F2+fac*F1c);		
			out2c+=preb0*F0F2+prep*(F1c+facF2);

			if ( set_spacetime_moments )
			{
				double K0coeff = pre*ep2*F1c;
				double K1coeff = pre*eperp*(F0F2+fac*F1c);
				double K2coeff = pre*ep2*facF2;

			//double tau_SPH     = sqrt( abs( par[nsph].r.x[0]*par[nsph].r.x[0]
			//					     - par[nsph].r.x[3]*par[nsph].r.x[3] ) ); 
			//double x_SPH       = par[nsph].r.x[1];
			//double y_SPH       = par[nsph].r.x[2];
			double tau_SPH     = par[nsph].tau; 
			double x_SPH       = par[nsph].r.x[0];
			double y_SPH       = par[nsph].r.x[1];
	
				// moments from first term
				ST_out1c[0] += (K0coeff*STb0+K1coeff*STb1+K2coeff*STb2) * 1.0;				// S
				ST_out1c[1] += (K0coeff*STb0+K1coeff*STb1+K2coeff*STb2) * x_SPH;			// x S
				ST_out1c[2] += (K0coeff*STb0+K1coeff*STb1+K2coeff*STb2) * y_SPH;			// y S
				//ST_out1c[3] += 0.0;														// z S
				ST_out1c[4] += (K0coeff*STb1
								+K1coeff*0.5*(STb2+STb0)
								+K2coeff*0.5*(STb3+STb1)) * tau_SPH;						// t S
				ST_out1c[5] += (K0coeff*STb0+K1coeff*STb1+K2coeff*STb2) * x_SPH * x_SPH;	// x^2 S
				ST_out1c[6] += (K0coeff*STb0+K1coeff*STb1+K2coeff*STb2) * x_SPH * y_SPH;	// x y S
				ST_out1c[7] += (K0coeff*STb0+K1coeff*STb1+K2coeff*STb2) * y_SPH * y_SPH;	// y^2 S
				//ST_out1c[8] += 0.0;														// x z S
				//ST_out1c[9] += 0.0;														// y z S
				ST_out1c[10] += (K0coeff*0.5*(STb2-STb0)
									+ K1coeff*0.25*(STb3-STb0)
									+ K2coeff*0.25*(STb4-2.0*STb2+STb0))
								* tau_SPH * tau_SPH;										// z^2 S
				ST_out1c[11] += (K0coeff*STb1
									+ K1coeff*0.5*(STb2+STb0)
									+ K2coeff*0.5*(STb3+STb1)) * x_SPH * tau_SPH;			// x t S
				ST_out1c[12] += (K0coeff*STb1
									+ K1coeff*0.5*(STb2+STb0)
									+ K2coeff*0.5*(STb3+STb1)) * y_SPH * tau_SPH;			// y t S
				//ST_out1c[13] += 0.0;														// z t S
				ST_out1c[14] += (K0coeff*0.5*(STb2+STb0)
									+ K1coeff*0.25*(STb3+3.0*STb0)
									+ K2coeff*0.25*(STb4+2.0*STb2+STb0))
								* tau_SPH * tau_SPH;	// t^2 S
	

				// reset Bessel function coefficients
				K0coeff = pre*F0F2;
				K1coeff = pre*eperp*(F1c+facF2);

				// moments from second term
				ST_out2c[0] += (K0coeff*STb0+K1coeff*STb1) * 1.0;							// S
				ST_out2c[1] += (K0coeff*STb0+K1coeff*STb1) * x_SPH;							// x S
				ST_out2c[2] += (K0coeff*STb0+K1coeff*STb1) * y_SPH;							// y S
				//ST_out2c[3] += 0.0;														// z S
				ST_out2c[4] += (K0coeff*STb1+K1coeff*0.5*(STb2+STb0)) * tau_SPH;			// t S
				ST_out2c[5] += (K0coeff*STb0+K1coeff*STb1) * x_SPH * x_SPH;					// x^2 S
				ST_out2c[6] += (K0coeff*STb0+K1coeff*STb1) * x_SPH * y_SPH;					// x y S
				ST_out2c[7] += (K0coeff*STb0+K1coeff*STb1) * y_SPH * y_SPH;					// y^2 S
				//ST_out2c[8] += 0.0;														// x z S
				//ST_out2c[9] += 0.0;														// y z S
				ST_out2c[10] += (K0coeff*0.5*(STb2-STb0)
								+K1coeff*0.25*(STb3-STb0)) * tau_SPH * tau_SPH;				// z^2 S
				ST_out2c[11] += (K0coeff*STb1+K1coeff*0.5*(STb2+STb0)) * x_SPH * tau_SPH;	// x t S
				ST_out2c[12] += (K0coeff*STb1+K1coeff*0.5*(STb2+STb0)) * y_SPH * tau_SPH;	// y t S
				//ST_out2c[13] += 0.0;														// z t S
				ST_out2c[14] += (K0coeff*0.5*(STb2+STb0)
								+K1coeff*0.25*(STb3+3.0*STb0)) * tau_SPH * tau_SPH;			// t^2 S
			}
		}
		if (typ>1)
		{
			if (typ==2) b2=bes.Kn(2,bsub);
			double pred=pre*add;
			
			double spi1=par[nsph].pi00+par[nsph].pi33;
			double spi3=px2*par[nsph].pi11 +py2*par[nsph].pi22 +pxy* par[nsph].pi12;
			
			
			I1sc+=pred*( ep3*spi1*bes.Kn(3,bsub)
							+ ( ep3*(3*par[nsph].pi00 - par[nsph].pi33) + eperp*spi3 )*b1
						);
			I2sc+=pred*( 0.5*ep2*spi1*b2+(0.5*ep2*(par[nsph].pi00- par[nsph].pi33)+spi3)*b0);
		
			if ( set_spacetime_moments )
			{
				double K3coeff = pred*ep3*spi1;
				double K1coeff = pred*(ep3*(3.0*par[nsph].pi00-par[nsph].pi33)+eperp*spi3);
				
				//double tau_SPH     = sqrt( abs( par[nsph].r.x[0]*par[nsph].r.x[0]
				//					     - par[nsph].r.x[3]*par[nsph].r.x[3] ) ); 
				//double x_SPH       = par[nsph].r.x[1];
				//double y_SPH       = par[nsph].r.x[2];
				double tau_SPH     = par[nsph].tau; 
				double x_SPH       = par[nsph].r.x[0];
				double y_SPH       = par[nsph].r.x[1];
	
				// moments from I1sc
				ST_I1sc[0] += (K3coeff*STb3 + K1coeff*STb1) * 1.0;						// S
				ST_I1sc[1] += (K3coeff*STb3 + K1coeff*STb1) * x_SPH;					// x S
				ST_I1sc[2] += (K3coeff*STb3 + K1coeff*STb1) * y_SPH;					// y S
				//ST_I1sc[3] += 0.0;													// z S
				ST_I1sc[4] += (K3coeff*0.5*(STb4-STb2)
								+ K1coeff*0.5*(STb2+STb0)) * tau_SPH;					// t S
				ST_I1sc[5] += (K3coeff*STb3 + K1coeff*STb1) * x_SPH * x_SPH;			// x^2 S
				ST_I1sc[6] += (K3coeff*STb3 + K1coeff*STb1) * x_SPH * y_SPH;			// x y S
				ST_I1sc[7] += (K3coeff*STb3 + K1coeff*STb1) * y_SPH * y_SPH;			// y^2 S
				//ST_I1sc[8] += 0.0;													// x z S
				//ST_I1sc[9] += 0.0;													// y z S
				ST_I1sc[10] += (K3coeff*0.25*(STb5-2.0*STb3+STb1)
								+ K1coeff*0.25*(STb3-STb0)) * tau_SPH * tau_SPH;		// z^2 S
				ST_I1sc[11] += (K3coeff*0.5*(STb4-STb2)
								+ K1coeff*0.5*(STb2+STb0)) * x_SPH * tau_SPH;			// x t S
				ST_I1sc[12] += (K3coeff*0.5*(STb4-STb2)
								+ K1coeff*0.5*(STb2+STb0)) * y_SPH * tau_SPH;			// y t S
				//ST_I1sc[13] += 0.0;													// z t S
				ST_I1sc[14] += (K3coeff*0.25*(STb5+2.0*STb3+STb1)
								+ K1coeff*0.25*(STb3+3.0*STb0)) * tau_SPH * tau_SPH;	// t^2 S
	
				// set needed Bessel function coefficients
				double K2coeff = 0.5*pred*ep2*spi1;
				double K0coeff = pred*(0.5*ep2*(par[nsph].pi00-par[nsph].pi33)+spi3);

				// moments from I2sc
				ST_I2sc[0] += (K2coeff*STb2 + K0coeff*STb0) * 1.0;				// S
				ST_I2sc[1] += (K2coeff*STb2 + K0coeff*STb0) * x_SPH;			// x S
				ST_I2sc[2] += (K2coeff*STb2 + K0coeff*STb0) * y_SPH;			// y S
				//ST_I2sc[3] += 0.0;											// z S
				ST_I2sc[4] += (K2coeff*0.5*(STb3+STb1)
								+ K0coeff*STb1) * tau_SPH;						// t S
				ST_I2sc[5] += (K2coeff*STb2 + K0coeff*STb0) * x_SPH * x_SPH;	// x^2 S
				ST_I2sc[6] += (K2coeff*STb2 + K0coeff*STb0) * x_SPH * y_SPH;	// x y S
				ST_I2sc[7] += (K2coeff*STb2 + K0coeff*STb0) * y_SPH * y_SPH;	// y^2 S
				//ST_I2sc[8] += 0.0;											// x z S
				//ST_I2sc[9] += 0.0;											// y z S
				ST_I2sc[10] += (K2coeff*0.25*(STb4-2.0*STb2+STb0)
								+ K0coeff*0.5*(STb2-STb0)) * tau_SPH * tau_SPH;	// z^2 S
				ST_I2sc[11] += (K2coeff*0.5*(STb3+STb1)
								+ K0coeff*STb1) * x_SPH * tau_SPH;				// x t S
				ST_I2sc[12] += (K2coeff*0.5*(STb3+STb1)
								+ K0coeff*STb1) * y_SPH * tau_SPH;				// y t S
				//ST_I2sc[13] += 0.0;											// z t S
				ST_I2sc[14] += (K2coeff*0.25*(STb4+2.0*STb2+STb0)
								+ K0coeff*0.5*(STb2+STb0)) * tau_SPH * tau_SPH;	// t^2 S
			}
		}
	}

	


	I1=2*out1*eperp;
	I2=2*out2;
	
	I1c=2*out1c;
	I2c=2*out2c;
	
	if ( set_spacetime_moments )
	{
		for (int ii = 0; ii < 15; ii++)
		{
			ST_I1[ii] = 2*ST_out1[ii]*eperp;
			ST_I2[ii] = 2*ST_out2[ii];
			ST_I1c[ii] = 2*ST_out1c[ii];
			ST_I2c[ii] = 2*ST_out2c[ii];
		}
	}

	cout << "Check real: " << I1 << "   " << I2 << "   "
		<< I1c << "   " << I2c << "   "
		<< I1sc << "   " << I2sc << "   "
		<< ST_I1[0] << "   " << ST_I2[0] << "   "
		<< ST_I1c[0] << "   " << ST_I2c[0] << "   "
		<< ST_I1sc[0] << "   " << ST_I2sc[0] << endl;
	
	// momentum too large at this temperature to give meaningful contribution;
	// ergo, zero everything
	if ((pd/T)>64)
	{
		I1=0; I2=0; I1c=0; I2c=0;
		fill(ST_I1.begin(), ST_I1.end(), 0.0);
		fill(ST_I2.begin(), ST_I2.end(), 0.0);
		fill(ST_I1c.begin(), ST_I1c.end(), 0.0);
		fill(ST_I2c.begin(), ST_I2c.end(), 0.0);
	}
	
	

	
}




template <int D,int DD>
void SPH<D,DD>::IoutFT( complex<double> &I1_comp, complex<double> &I2_comp,
						double pT, double phi, double pRap, HAD cur,int nsph,
						double Q0, double QX, double QY, double QZ )
{
	// eventually define this globally
	const double hbarc_local = 0.19733;
	const complex<double> iComplex(0.0, 1.0);

	// set space-time and momentum info for SPH particle
	//double tau_SPH     = sqrt( abs( par[nsph].r.x[0]*par[nsph].r.x[0]
	//					     - par[nsph].r.x[3]*par[nsph].r.x[3] ) ); 
	//double eta_SPH     = 0.5*log( abs(par[nsph].r.x[0]+par[nsph].r.x[3])
    //                             /(abs(par[nsph].r.x[0]-par[nsph].r.x[3])+1e-100) ); 
	//double x_SPH       = par[nsph].r.x[1];
	//double y_SPH       = par[nsph].r.x[2];
	double tau_SPH     = par[nsph].tau; 
	double x_SPH       = par[nsph].r.x[0];
	double y_SPH       = par[nsph].r.x[1];
	double chy         = cosh(pRap),
           shy         = sinh(pRap);
	complex<double> beta_tilde( tau_SPH*(Q0*chy - QZ*shy)/hbarc_local, 0.0);
	complex<double> gamma_tilde( tau_SPH*(Q0*shy - QZ*chy)/hbarc_local, 0.0);

	complex<double> trans_phase = exp(-iComplex*(QX*x_SPH + QY*y_SPH)/hbarc_local);

	// continue calculation as Jaki's Iout (some variables renamed or made complex)
	complex<double> out1=0, out2=0;
	complex<double> out1c=0, out2c=0;
	double pd=pperp(pT,phi,par[nsph].u);
	double mT=Eperp(pT,cur.mass);
	double gamma=par[nsph].u.x[0];
	double px=pT*cos(phi);
	double py=pT*sin(phi);
	double px2=px*px, py2=py*py, pxy=2*px*py;
	double mT2=mT*mT;
	double mT3=mT2*mT/4.;
	double T_over_gamma=T/gamma;
	double bfac=mT/T_over_gamma;

	complex<double> b0, b1, b2;
	double bsub, fac, pre;
	double f0s, f1s, f2s;
	double F0c, F1c, F2c;

	// Evaluate functions entering bulk df corrections
	if ((typ==1)||(typ==3)) calcF2(cur,nsph,pd,f0s,f1s,f2s);

	if (typ>1)
	{
		I1sc_comp=0;
		I2sc_comp=0;
	}

	double expT=exp(pd/T);
	
	for (int nn=0;nn<=Nmax;nn++)
	{
		double add=(nn+1);
		bsub=add*bfac;
		complex<double> ci0  = 0.0, ci1  = 0.0, ck0  = 0.0, ck1  = 0.0;
		complex<double> ci0p = 0.0, ci1p = 0.0, ck0p = 0.0, ck1p = 0.0;
		complex<double> at_m_i_bt = complex<double>(bsub, 0.0) - iComplex*beta_tilde;
		//complex<double> at_m_i_bt = complex<double>(bsub, -beta_tilde);
		complex<double> z = sqrt( at_m_i_bt*at_m_i_bt + gamma_tilde*gamma_tilde );
		int success = BesselFunction::cbessik01( z, ci0,  ci1,  ck0,  ck1,
													ci0p, ci1p, ck0p, ck1p );
		complex<double> z2 = z*z;
		complex<double> z3 = z2*z;
		complex<double> z5 = z2*z3;

		// set K0 and K1 evaluations; no need to compute K2 or K3
		b0 = ck0;
		b1 = ck1;

		/*cout << "Check Bessel: "
				<< bsub << "   " << beta_tilde << "   " << gamma_tilde << "   "
				<< at_m_i_bt << "   " << z << "   " << b0 << "   " << b1 << endl;*/

		complex<double> I0_CP = 2.0*b0;
		complex<double> I1_CP = 2.0*at_m_i_bt*b1/z;
		complex<double> I2_CP = 2.0*( at_m_i_bt*at_m_i_bt*b0/z2
									  + (z2-2.0*gamma_tilde*gamma_tilde)*b1/z3);
		complex<double> I3_CP = 2.0*(at_m_i_bt/z5)
								* ( z*(z2-4.0*gamma_tilde*gamma_tilde)*b0
									+ ( z2*(z2+2.0)+gamma_tilde*gamma_tilde*(8.0-z2) )*b1 );
		
		pre=pow(-cur.theta,nn)*pow(expT,add);
		complex<double> preb1=2.0*pre*mT*b1;
		out1+=preb1;
		complex<double> preb0=2.0*pre*b0;
		out2+=preb0;
		
		if ((typ==1)||(typ==3))	// if including bulk
		{
			fac=T_over_gamma/add;
			double G0bulk = add*f0s;
			double G1bulk = add*f1s;
			double G2bulk = add*f2s;

			out1c += pre * mT * trans_phase
					 * ( I1_CP * (1.0+G0bulk) + I2_CP * G1bulk + I3_CP * G2bulk );
			out2c += pre * trans_phase
					 * ( I0_CP * (1.0+G0bulk) + I1_CP * G1bulk + I2_CP * G2bulk );
		}
		if (typ>1)				// if including shear
		{
			//double pred=pre*add;
			double spi1=par[nsph].pi00 + par[nsph].pi33;
			double spi3=px2*par[nsph].pi11 + py2*par[nsph].pi22 + pxy*par[nsph].pi12;

			complex<double> G0shear = 0.5*add*( spi3 - mT*mT*par[nsph].pi33 );
			complex<double> G1shear = 0.0;
			complex<double> G2shear = 0.5*add*mT*mT*spi1;
			
			I1sc_comp += pre * mT * trans_phase
						 * ( G0shear * I1_CP + G1shear * I2_CP + G2shear * I3_CP );
			I2sc_comp += pre * trans_phase
						 * ( G0shear * I0_CP + G1shear * I1_CP + G2shear * I2_CP );
		}
	}

	I1_comp=out1;
	I2_comp=out2;
	
	I1c_comp=out1c;
	I2c_comp=out2c;

	//cout << "Check complex: " << I1_comp << "   " << I2_comp << "   "
	//	<< I1c_comp << "   " << I2c_comp << "   "
	//	<< I1sc_comp << "   " << I2sc_comp << endl;
	
	if ((pd/T)>64)
	{
		I1_comp=0;
		I2_comp=0;
		I1c_comp=0;
		I2c_comp=0;	
	}

	return;
}




template <int D,int DD>
void SPH<D,DD>::calcsPI(int h)
{
   
   m3=pow(had[h].mass,3);
   double bfac=had[h].mass/T;
   double bsum=0;
   for(int i=1;i<10;i++)
   {
   double bsub2=i*bfac;
   Bessel bes;
   bsum+=bes.Kn(3,bsub2)/i;
   }
   
   
   had[h].spar=had[h].deg*m3*facc*bsum/sc3;
 
   
}

template <int D,int DD>
void SPH<D,DD>::calcF2(HAD cur, int nsph, double pd,double &F0,double &F1, double &F2)
{
	F0=par[nsph].bulkpi*(cur.E0 - cur.D0*pd+cur.B0*pd*pd);
	F1=par[nsph].bulkpi*par[nsph].u.x[0]*(cur.D0-2*cur.B0*pd);
	F2=par[nsph].bulkpi*par[nsph].u.x[0]*par[nsph].u.x[0]*cur.B0;
}

template <int D,int DD>
void SPH<D,DD>::readweights(string ptpoints, string phipoints)
{

	//cout << "start" << endl;

	FILE * myfile2 = fopen (phipoints.c_str(),"r");
	
	cout << phipoints << endl;
	double sub;
	while (fscanf(myfile2,"%lf %*f",&sub)==1)
	{
	phi.push_back(sub);
	
	}
	
	fclose(myfile2);



	FILE * myfile = fopen (ptpoints.c_str(),"r");
	 

	cout << ptpoints << "\n";	
	while (fscanf(myfile,"%lf %*f",&sub)==1)
	{
	pt.push_back(sub); 
	
	}
	
	fclose(myfile);

	

}



#endif

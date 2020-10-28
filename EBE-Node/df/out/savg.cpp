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

struct evs{

	int flag;
	double ntot;
        double npart;
	double ivn[6];
	double ipsi[6];
	vector<double>  spec;
  vector < vector<double> > vn, psin,vrms;
	
	};
	
struct outs{

	double ntot,M2;
	vector<double> vint;
	vector <double> spec,m2,mm4,mm2;
  vector < vector<double> > vn,c2,c4, vrms;
	vector < vector<double> > r2,r3,r4;
	vector <double> SCM32,SCM42,SCM43;

	};

outs cen(vector<evs> & list, double p[],int psize, int & nlist);

void comb(outs & a, string name2, double p[],int psize);

void rnout(outs & a, string name2, double p[], int psize);

void scmout(outs & a, string name2, double p[], int psize);

void cum(outs & a, string name2, double p[], int psize);

void rms(outs & a, string name2, double p[], int psize);

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
	
	
  int folend,evf;
	string vnfile,b,npfile,intfile;
	int cor,decays;
	if (argv[1]&&argv[2])
	{	
	vnfile=argv[1];
	
	
	stringstream s,s1;
	
	s1 << argv[2];
	s1 >> folend;
	
	if (argv[3])
	{
	b=argv[3];  //hadrons watched
	}
	
	if (argv[4])
	{
	npfile=argv[4];  //npart
	}
	

	if (argv[5]){
	  intfile=argv[5];
	}

	cor=1;
	decays=1;
	
	
	}
	else 
	{
	cout << " Error: missing intput of first and last event!" << endl;
	exit(1);
	}
	
        cout << "starting" << endl;	
	
	evf=(folend+1)*1000;

	FILE * myfile1;
//	string input;
//	input="in.dat";
	vnfile="input/"+vnfile;
	myfile1 = fopen (vnfile.c_str(),"r");
	
        	
	
	string visc,folder,ptn;
	char charin[100];
	int pts;
	double p0,pe,ps;
	
	
           fscanf(myfile1,"%*s %s \n",charin); // type of equations (ideal, bulk etc)
           visc=charin;
           fscanf(myfile1,"%*s %s \n",charin); // folder that contains the events
           folder=charin;
           fscanf(myfile1,"%*s  %*s %*s");  // file that contains pt/phi grid, ignore
           fscanf(myfile1,"%*s  %i",&decays);  //decays=1 post decays, decays=0 no decays
           fscanf(myfile1,"%*s  %*s");
           fscanf(myfile1,"%*s  %*s");
           fscanf(myfile1,"%*s  %*s");
           fscanf(myfile1,"%*s %*s  %s",charin); // pt file for number           
           ptn=charin;
  	fclose(myfile1);
  
        cout << "read input" << endl;

	std::vector<std::string> minfol = split(folder, '/');
        
	int afol=minfol.size();
	string sfol;
        sfol=minfol[0];
	for (int j=1;j<(afol-1);j++){
	  sfol=sfol+"/"+minfol[j];
	}
        cout << sfol << endl;
	string npfol=minfol[0]+"/"+minfol[1];
       

	
  	
  	FILE * ptfile;
	ptfile = fopen (ptn.c_str(),"r");
  	fscanf(ptfile,"%*s  %i",&pts);
  	fclose(ptfile);
	
	// need to get pts still!!! 


	vector <evs> list;

	//read in npart
	
	string nameb="out/"+npfol+"/npart.dat";
	ifstream inputb(nameb.c_str());
	if (!inputb.is_open())
 	{
 	cout << "Can't open " << nameb << endl;
 	exit(1);
 	}

	string line;

	int first=0;
	int pl=0;
	int cou=0;
	while (getline(inputb,line)&&(cou<evf)) {
	std::vector<std::string> x = split(line, ' ');
	if (x.size()<2) std::vector<std::string> x = split(line, '\t');


	// CHECK IF NPART FILE HAS TWO COLUMNS OR NOT!!!
	evs y;
	stringstream s;
	s << x[pl];
	s >> y.npart;
	y.flag=0;

	if (y.npart==1&&first==0){
	  pl=1;
	  stringstream ss;
	  ss << x[pl];
	  ss >> y.npart;
	  y.flag=0;
	  first=1;
	}
	
	//cout << y.npart << endl;
	list.push_back(y);
	cou++;
	}

       
	inputb.close();
	//done with npart


        cout << "read npart" << endl;

	
	int count=0;
	int nev=0;
	
	string bname;
	string allname="all";
	if (b!=allname) bname=b.substr(0,b.length()-4);
	else bname=allname;
	
	
	double p[pts],v1avg[pts],v2avg[pts],v3avg[pts],v4avg[pts],v5avg[pts],v6avg[pts],spectraavg[pts];
	string name,event,vnin;
	if (cor!=0){
	if (visc=="shear+bulk"||visc=="bulk+shear") vnin="sbvnc";
	else if (visc=="bulk") vnin="bvnc";
	else vnin="vn";
	}
	else{
	if (visc=="shear+bulk"||visc=="bulk+shear") vnin="sbvn";
	else if (visc=="bulk") vnin="bvn";
	else vnin="vn";
	
	}
	name="out/"+folder+"/ev";
	
	//name3=folder+"/"+b+"int_"+vnin+".dat";
	
	
	
//	read in integrated vn's
	string name3="out/"+sfol+"/"+intfile+".dat";


	ifstream input(name3.c_str());
	if (!input.is_open())
 	{
 	cout << "Can't open " << name3 << endl;
 	exit(1);
 	}

	int cnt=0;
	while (getline(input,line)&&(cnt<evf)) {

	  if (cnt>list.size() ){
	    cout << "npart file too short" << endl;
	    exit(1);
	  }
	
	std::vector<double> y (17,0) ;
	std::vector<std::string> x = split(line, ' ');

             

	if (x.size()<17) x = split(line, '\t');
	
	if (x.size()<17){
	  cout <<"string size too short" << endl;
	  exit(1);
	}

	int k=0;
	//	cout << x.size() << endl;
	for(int j=1;j<13;j=j+2)
	{
	stringstream s;
	double sub;
	s << x[j];
	s >> sub;

	list[cnt].ivn[k]=sub;
	
	//k=0 is v1
	
	

	if (k>0){
	if (list[cnt].flag==0){
	if ((list[cnt].ivn[k]<=0)||isnan(list[cnt].ivn[k])||(list[cnt].ivn[k]>0.24)) {
	  list[cnt].flag=1;
	  cout << "flagged vn " << cnt << " " << x.size() <<    endl;
	}}}
	k++;
	}
	
    

	
	k=0;
        if (list[cnt].flag==0){
	for(int j=2;j<14;j=j+2)
	{

	stringstream s;
	double sub;
	s << x[j];
	s >> sub;
	list[cnt].ipsi[k]=sub;
	
	k++;
	}

	stringstream s;
	s << x[16];
	s >> list[cnt].ntot;
	
	if ((list[cnt].ntot<=0)||isnan(list[cnt].ntot)||(list[cnt].ntot>10000)) {
          list[cnt].flag=1;
          cout << "flagged ntot " << cnt <<  endl;
        }}
	

	cnt++;
	
	//cout << x[0] << endl;
	}
	input.close();
//	done read in integrated vn's
	

	
	
	cout << "read intergrated" << endl;
	


	
	int psize;
        
	for (int nfol=0;nfol<=folend;nfol++) // runs over all the events to be read in 
	{

	  
	  for (int ev=0;ev<=999;ev++){
	    //int i=ev-1;
	    int i=nfol*1000+ev;

	    //    cout << i << endl;
	if (list[i].flag!=1){
		int on=0;
		nev++;
		
		event="out/"+sfol+"/"+convertInt(nfol)+"/ev"+convertInt(ev)+"_"+vnin+".dat";
		

	       
		

		ifstream myfile(event.c_str());
		if (!myfile.is_open())
		{
		  cout << ev  << " " ;
			list[i].flag=1;
                        continue;
		}
                
                

		list[i].vn.resize(6);
		list[i].psin.resize(6);


		getline(myfile,line);
		int t=0;
		while (getline(myfile,line)&&list[i].flag==0) {
	
			std::vector<double> y (18,0) ;
			std::vector<std::string> x = split(line, ' ');
	
			if (x.size()<14) {
			  x = split(line, '\t');
			  cout << "split issues " << i <<  endl;
                          list[i].flag=1;
			  break;
			}
			
			

			for(int j=0;j<14;j++)
			{
			stringstream sb;
			sb << x[j];
			sb >> y[j];
			}
		  
		  	p[t]=y[0];
			

			
			list[i].spec.push_back(y[1]);
		  	if (list[i].spec[t]<0){
			  list[i].flag=1;
			  cout << "spec flag " << i <<  endl;
			}
			       
		  	for (int j=2;j<13;j=j+2){
		  		int nj=j/2-1;
				
		  		list[i].vn[nj].push_back(abs(y[j]));
		  		list[i].psin[nj].push_back(y[j+1]);
		  		//if ((nj>0)&&(list[i].vn[nj][t]>0.24) ) list[i].flag=1;
		  	}
		  	t++;	
		  	psize=t;		
		}
		
		
		myfile.close();
	}
	}
	}

  
	
	cout << "read in vn's " << endl;
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
	
	//	cout << y[0] << " " << y[1]<< " cens " <<  y2[2] << " " << y2[3] << endl;
	}
	inputd.close();
	// done read in centralities
	cout << "read in centralities" << endl;
	
	int ctot=cmax.size();
	int atot=evf;
	
	int sep=8+20;
	vector < vector <evs> > csep;
	csep.resize(ctot);
       
	vector<double> mmean;
	mmean.resize(ctot,0);
	for (int i=0;i<atot;i++){

	  

		for (int c=0;c<ctot;c++){
			//if (list[i].flag!=1&&list[i].npart<=nph[c*10]&&list[i].npart>npl[c*10+9]) csep[c].push_back(list[i]);
		  
		       
		  if (list[i].flag!=1&&list[i].npart<=nph[c]&&list[i].npart>npl[c]) {
		    csep[c].push_back(list[i]);
		    mmean[c]+=list[i].ntot;
		  }
		}
		if (list[i].flag!=1&&list[i].npart>nph[0]) { 
		  csep[0].push_back(list[i]);
		  mmean[0]+=list[i].ntot;
		}
		if (list[i].flag!=1&&list[i].npart==npl[ctot-1]) {
		  csep[ctot-1].push_back(list[i]);
		  mmean[ctot-1]+=list[i].ntot;
		}
	}


	for (int c=0;c<ctot;c++) {
	  mmean[c]/=csep[c].size();
	  //	  cout << c << " " <<  mmean[c] << endl; 
	    }

	for (int c=0;c<ctot;c++){
	  for (int i=0;i<csep[c].size();i++){
	    if (csep[c][i].ntot>(1.1*mmean[c])||csep[c][i].ntot<(0.9*mmean[c])) csep[c].erase( csep[c].begin() +i );
	  }
	}
	
      



        cout << "done cen" << endl;

	
//	for (int ch=0;ch<csep[30].size();ch++) {
//	cout << csep[30][ch].npart << " " << csep[30][ch].ivn[2] << endl;
//	getchar();
//	}
	
	vector <outs> a;
	a.resize(ctot);
	vector <int> nlist;
	nlist.resize(ctot);
	for (int c=0;c<ctot;c++){  
	  int nlsub;
	  a[c]=cen(csep[c],p,psize,nlsub);
	  nlist[c]=nlsub;
	}
        
	
	cout << "almost printing" << endl;
	
	for (int c=0;c<sep;c++) {
	
		int cbot,ctop;
		if (c==0){
		cbot=0;
		ctop=9;
		}
		else if (c==1){
		cbot=10;
		ctop=39;
		}
		else if (c==2){
		cbot=40;
		ctop=79;
		}
		else if (c==3){
		cbot=0;
		ctop=79;
		}
		else if (c==4){
		cbot=10;
		ctop=19;
		}
		else if (c==5){
		cbot=20;
		ctop=39;
		}
		else if (c==6){
		cbot=39;
		ctop=59;
		}
		else if (c==7){
		cbot=59;
		ctop=79;
		}
	        else {
		  cbot=(c-8)*5;
		  ctop=(c-8)*5+4;
		}
	
		//		cout << c << " " << cbot << " " << ctop << endl;		
		string cnam=convertInt(cmin[cbot])+convertInt(cmax[ctop]);
		string name2="out/"+sfol+"/"+bname+"avg_"+cnam+vnin+".dat";
		
		if (decays==0) name2="out/"+sfol+"/"+bname+"avg_"+cnam+vnin+"ND.dat";
		if (decays==3) name2="out/"+sfol+"/"+bname+"avg_"+cnam+vnin+"nodf.dat";
		if (decays==2) name2="out/"+sfol+"/"+bname+"avg_"+cnam+vnin+"negND.dat";
		
		string namrn="out/"+sfol+"/"+bname+"rn_"+cnam+vnin+".dat";
		
		if (decays==0) namrn="out/"+sfol+"/"+bname+"rn_"+cnam+vnin+"ND.dat";
		if (decays==3) namrn="out/"+sfol+"/"+bname+"rn_"+cnam+vnin+"nodf.dat";
		if (decays==2) namrn="out/"+sfol+"/"+bname+"rn_"+cnam+vnin+"negND.dat";
		
		string namsc="out/"+sfol+"/"+bname+"scm_"+cnam+vnin+".dat";
		
		if (decays==0) namsc="out/"+sfol+"/"+bname+"scm_"+cnam+vnin+"ND.dat";
		if (decays==3) namsc="out/"+sfol+"/"+bname+"scm_"+cnam+vnin+"nodf.dat";
		if (decays==2) namsc="out/"+sfol+"/"+bname+"scm_"+cnam+vnin+"negND.dat";
		
		string namcum="out/"+sfol+"/"+bname+"cum_"+cnam+vnin+".dat";
		
		if (decays==0) namcum="out/"+sfol+"/"+bname+"cum_"+cnam+vnin+"ND.dat";
		if (decays==3) namcum="out/"+sfol+"/"+bname+"cum_"+cnam+vnin+"nodf.dat";
		if (decays==2) namcum="out/"+sfol+"/"+bname+"cum_"+cnam+vnin+"negND.dat";
		
		string namrms="out/"+sfol+"/"+bname+"rms_"+cnam+vnin+".dat";

                if (decays==0) namrms="out/"+sfol+"/"+bname+"cum_"+cnam+vnin+"ND.dat";
                if (decays==3) namrms="out/"+sfol+"/"+bname+"cum_"+cnam+vnin+"nodf.dat";
                if (decays==2) namrms="out/"+sfol+"/"+bname+"cum_"+cnam+vnin+"negND.dat";
		
		outs print;
		double M=0,vnn[6]={0};
		vector<double> m,mm2,mm4;
		m.resize(pts,0);
		mm4.resize(pts,0);
		mm2.resize(pts,0);
		print.r2.resize(pts);
		print.r3.resize(pts);
		print.r4.resize(pts);
		print.c2.resize(6);
		print.c4.resize(6);
		print.SCM32.resize(pts,0);
		print.SCM42.resize(pts,0);
		print.SCM43.resize(pts,0);
		print.vrms.resize(6);
		for (int t=0;t<pts;t++){	     
			print.r2[t].resize(t+1,0);
			print.r3[t].resize(t+1,0);
			print.r4[t].resize(t+1,0);
		}
		vector < vector<double> > vpt,vrms;
		vpt.resize(6);	    
		vrms.resize(6);
		for (int i=0;i<6;i++) {
			vpt[i].resize(pts,0);
			print.c2[i].resize(pts,0);
			print.c4[i].resize(pts,0);
			print.vrms[i].resize(pts,0);
			vrms[i].resize(pts,0);
		}
		
		
		print.spec.resize(pts);


		cout << "spec set "<< endl;
		int ncentot=0;
		for (int ci=cbot;ci<=ctop;ci++){
			M+=a[ci].M2;
			//	cout << a[ci].M2 << " " << a[ci].m2[0] << endl;
			//getchar();
			ncentot+=nlist[ci];
			//  cout << ci << " " << ncentot << " " << csep[ci].size() << endl;
			for (int t=0;t<pts;t++) {
			m[t]+=a[ci].m2[t];
			mm4[t]+=a[ci].mm4[t];
			mm2[t]+=a[ci].mm2[t];
			print.spec[t]+=a[ci].spec[t];
			print.SCM32[t]+=a[ci].mm4[t]*a[ci].SCM32[t];
			print.SCM42[t]+=a[ci].mm4[t]*a[ci].SCM42[t];
			print.SCM43[t]+=a[ci].mm4[t]*a[ci].SCM43[t];
			for (int t2=0;t2<=t;t2++){
					print.r2[t][t2]+=a[ci].r2[t][t2];
					print.r3[t][t2]+=a[ci].r3[t][t2];
					print.r4[t][t2]+=a[ci].r4[t][t2];
					//if (t==10&&t2==9) cout << ci << " " <<   a[ci].r2[t][t2] << endl;
			}
			
			}
			for (int i=0;i<6;i++) {
			vnn[i]+=a[ci].M2*a[ci].vint[i];
				for (int t=0;t<pts;t++) {				        
					vpt[i][t]+=a[ci].m2[t]*a[ci].vn[i][t];
					vrms[i][t]+=a[ci].vrms[i][t];
					print.c2[i][t]+=a[ci].mm2[t]*a[ci].c2[i][t];
					print.c4[i][t]+=a[ci].mm4[t]*a[ci].c4[i][t];
				}
			}
			
		}

		
		print.vn.resize(6);
		for (int i=0;i<6;i++){
		  for (int t=0;t<pts;t++) {
			  //			  if (i==1){ cout << vpt[i][t] << " " << m[t] << " " << sqrt(vnn[i]/M) << " " << M << " " << vpt[i][t]/m[t]/sqrt(vnn[i]/M) <<  endl;
		  //	  getchar();}
			  print.vrms[i][t]=sqrt(vrms[i][t]/ncentot);
				print.vn[i].push_back(vpt[i][t]/m[t]/sqrt(vnn[i]/M));
				print.c2[i][t]/=mm2[t];
				print.c4[i][t]/=mm4[t];
				}

		}
		for (int t=0;t<pts;t++) {
			print.spec[t]/=ncentot;
			print.SCM32[t]/=mm4[t];
			print.SCM42[t]/=mm4[t];
			print.SCM43[t]/=mm4[t];
			for (int t2=0;t2<=t;t2++){
				print.r2[t][t2]/=(ctop-cbot+1);
				print.r3[t][t2]/=(ctop-cbot+1);
				print.r4[t][t2]/=(ctop-cbot+1);
			}
                }
                
//                cout << print.r2[10][9] << endl;
//                getchar();

		//cout << "printer" << endl;
		comb(print, name2,p,psize);
		cout << "name2" << endl;
		rnout(print, namrn,p,psize);
		cout << namrn << endl;
		scmout(print, namsc,p,psize);
		cum(print, namcum,p,psize);
		rms(print, namrms,p,psize);
		//cout << "done "<< endl;
	}
	cout << "really done" << endl;

 return 0;
}

outs cen(vector<evs> & list, double p[], int psize, int & nlout)
{

	outs a,sub;
	vector<double> vv2,vv3,vv4;
	vector< vector<double> > subc2,subc4;
	a.vn.resize(6);
	a.vint.resize(6,0);
	int asize=list.size();
	a.spec.resize(psize); /// change name!!!!
	a.r2.resize(psize);
	a.r3.resize(psize);
	a.r4.resize(psize);
	subc4.resize(6);
	subc2.resize(6);
	sub.r2.resize(psize);
	sub.r3.resize(psize);
	sub.r4.resize(psize);
	vv2.resize(psize,0);
	vv3.resize(psize,0);
	vv4.resize(psize,0);
	a.SCM32.resize(psize,0);
	a.SCM42.resize(psize,0);
	a.SCM43.resize(psize,0);
	a.mm4.resize(psize,0);
	a.mm2.resize(psize,0);
	a.c4.resize(6);
	a.c2.resize(6);
	a.vrms.resize(6);
	for (int i=0;i<6;i++) {
	  a.vn[i].resize(psize,0);
	  a.c4[i].resize(psize,0);
	  subc4[i].resize(psize,0);
	  a.c2[i].resize(psize,0);
	  subc2[i].resize(psize,0);
		a.vrms[i].resize(psize,0);
	}
	for (int i=0;i<psize;i++) {
		a.r2[i].resize(i+1,0);
		a.r3[i].resize(i+1,0);
		a.r4[i].resize(i+1,0);
		sub.r2[i].resize(i+1,0);
		sub.r3[i].resize(i+1,0);
		sub.r4[i].resize(i+1,0);
		
	}
	
	int nsize=asize;
	double vitot[6];
	a.M2=0;
	//cout << asize << endl;
	
	for (int i=0;i<6;i++) vitot[i]=0;
	

	for (int cnt=0;cnt<asize;cnt++) {
	  //cout << cnt << " " << list[cnt].flag << endl;

		if (list[cnt].flag==1) nsize--;
		else {
			a.M2+=list[cnt].ntot*list[cnt].ntot;
			//		cout << "M2" <<  list[cnt].ntot << endl;
			for (int ns=0;ns<6;ns++) { vitot[ns]+=list[cnt].ntot*list[cnt].ntot*list[cnt].ivn[ns]*list[cnt].ivn[ns];
			  for (int t=0;t<psize;t++) a.vrms[ns][t]+=list[cnt].vn[ns][t]*list[cnt].vn[ns][t];
			}
			
		}
	}
	for (int ns=0;ns<6;ns++) a.vint[ns]=vitot[ns]/a.M2;
       
	
	

	
	vector <double> ep;
	ep.resize(psize,0);
	a.m2.resize(psize,0);
	for (int cnt=0;cnt<asize;cnt++) {
		if (list[cnt].flag!=1){
			for (int t=0;t<psize;t++){
			  //double m4sub=list[cnt].ntot*(list[cnt].ntot-1.)*(list[cnt].ntot-2.)*(list[cnt].ntot-3.);
			  //	double m2sub=list[cnt].ntot*(list[cnt].ntot-1.);

				double m4sub=list[cnt].spec[t]*(list[cnt].spec[t]-1.)*(list[cnt].spec[t]-2.)*(list[cnt].spec[t]-3.);
				double m2sub=list[cnt].spec[t]*(list[cnt].spec[t]-1.);
				

				a.spec[t]+=list[cnt].spec[t];
				a.m2[t]+=list[cnt].ntot*list[cnt].spec[t];
				a.mm4[t]+=m4sub;
				a.mm2[t]+=m2sub;
				vv2[t]+=m2sub*pow(list[cnt].vn[1][t],2);
				vv3[t]+=m2sub*pow(list[cnt].vn[2][t],2);
				vv4[t]+=m2sub*pow(list[cnt].vn[3][t],2);
				a.SCM32[t]+=m4sub*pow(list[cnt].vn[1][t],2)*pow(list[cnt].vn[2][t],2);
				a.SCM42[t]+=m4sub*pow(list[cnt].vn[1][t],2)*pow(list[cnt].vn[3][t],2);
				a.SCM43[t]+=m4sub*pow(list[cnt].vn[3][t],2)*pow(list[cnt].vn[2][t],2);
				ep[t]+=list[cnt].ivn[1]*list[cnt].vn[1][t]*cos(2*(list[cnt].ipsi[1]-list[cnt].psin[1][t]));

				for (int ns=0;ns<6;ns++) {

				  double VV2=list[cnt].ntot*list[cnt].spec[t]*list[cnt].ivn[ns]*list[cnt].vn[ns][t]*cos((ns+1)*(list[cnt].ipsi[ns]-list[cnt].psin[ns][t]));
				  double VV4=list[cnt].ntot*(list[cnt].ntot-1)*(list[cnt].ntot-2)*list[cnt].spec[t]*pow(list[cnt].ivn[ns],3)*list[cnt].vn[ns][t]*cos((ns+1)*(list[cnt].ipsi[ns]-list[cnt].psin[ns][t]));
				  a.vn[ns][t]+=VV2;
					
				  subc4[ns][t]+=m4sub*pow(list[cnt].vn[ns][t],4);
				  subc2[ns][t]+=m2sub*pow(list[cnt].vn[ns][t],2);
					}
				for (int t2=0;t2<=t;t2++){
				 sub.r2[t][t2]+=list[cnt].vn[1][t]*list[cnt].vn[1][t2]*abs(cos(2.*(list[cnt].psin[1][t2]-list[cnt].psin[1][t])));
				 sub.r3[t][t2]+=list[cnt].vn[2][t]*list[cnt].vn[2][t2]*cos(3.*(list[cnt].psin[2][t2]-list[cnt].psin[2][t]));
				 sub.r4[t][t2]+=list[cnt].vn[3][t]*list[cnt].vn[3][t2]*cos(4.*(list[cnt].psin[3][t2]-list[cnt].psin[3][t]));				 
				}
				
//				if (t==psize-3) {
//				
//				for (int t2=0;t2<=t;t2++) cout << p[t]-p[t2] << " " << sub.r2[t][t2]/sqrt(sub.r2[t][t]*sub.r2[t2][t2]) << endl;
//				getchar();
//				}
			}
		}
	}
	

	
	for (int t=0;t<psize;t++) {
		//a.spec[t]/=nsize;
	  

		a.SCM32[t]=a.SCM32[t]/a.mm4[t]/(vv2[t]/a.mm2[t]*vv3[t]/a.mm2[t])-1.;
		a.SCM42[t]=a.SCM42[t]/a.mm4[t]/(vv2[t]/a.mm2[t]*vv4[t]/a.mm2[t])-1.;
		a.SCM43[t]=a.SCM43[t]/a.mm4[t]/(vv4[t]/a.mm2[t]*vv3[t]/a.mm2[t])-1.;
                if (nsize==0) a.spec[t]=0;
		for (int ns=0;ns<6;ns++) {
			a.vn[ns][t]=a.vn[ns][t]/a.m2[t];
			a.c2[ns][t]=sqrt(subc2[ns][t]/a.mm2[t]);
			a.c4[ns][t]=2.*pow(subc2[ns][t]/a.mm2[t],2.)-subc4[ns][t]/a.mm4[t];
			}
		for (int t2=0;t2<=t;t2++){
			a.r2[t][t2]=sub.r2[t][t2]/sqrt(sub.r2[t][t]*sub.r2[t2][t2]);
			a.r3[t][t2]=sub.r3[t][t2]/sqrt(sub.r3[t][t]*sub.r3[t2][t2]);
			a.r4[t][t2]=sub.r4[t][t2]/sqrt(sub.r4[t][t]*sub.r4[t2][t2]);
		}
		//		cout << pow(a.c4[1][t],0.25)/a.c2[1][t] << endl;
	}
	

	nlout=nsize;

	return a;
}

void comb(outs & a, string name2, double p[], int psize){


	ofstream OUT;
	OUT.open(name2.c_str());
	
	//cout << name2 << endl;
	
	if (!OUT.is_open())
	{
		cout << "Error: Can't open" << name2 << endl;
		exit(1);
	} 


	
	for (int i=0;i<psize;i++) // runs over all pts 
	{
	  //	  cout << a.vn[1][i] << endl;
	  OUT << p[i] <<  "  " << a.spec[i]/(2*3.14159265359) << "  " ;
		for (int v=0;v<6;v++) OUT << a.vn[v][i] << " " ;
		for (int pp=0;pp<=i;pp++) OUT << a.r2[i][pp] << " " ;
		OUT << endl;
		
	}
	OUT.close();
	

}

void rnout(outs & a, string name2, double p[], int psize){


	ofstream OUT;
	OUT.open(name2.c_str());
	
	//cout << name2 << endl;
	
	if (!OUT.is_open())
	{
		cout << "Error: Can't open" << name2 << endl;
		exit(1);
	} 


	OUT <<  "r2  " << endl ;
	for (int i=0;i<psize;i++) // runs over all pts 
	{
		
		OUT << p[i] <<  "  " ;
		for (int pp=0;pp<=i;pp++) OUT << a.r2[i][pp] << " " ;
		OUT << endl;
		
	}
	
	OUT <<  "r3  " << endl ;
	for (int i=0;i<psize;i++) // runs over all pts 
	{
		
		
		OUT << p[i] <<  "  " ;
		for (int pp=0;pp<=i;pp++) OUT << a.r3[i][pp] << " " ;
		OUT << endl;
		
	}
	
	OUT <<  "r4  " << endl ;
	for (int i=0;i<psize;i++) // runs over all pts 
	{
		
		
		OUT << p[i] <<  "  " ;
		for (int pp=0;pp<=i;pp++) OUT << a.r4[i][pp] << " " ;
		OUT << endl;
		
	}
	OUT.close();
	

}

void scmout(outs & a, string name2, double p[], int psize){


	ofstream OUT;
	OUT.open(name2.c_str());
	
	//cout << name2 << endl;
	
	if (!OUT.is_open())
	{
		cout << "Error: Can't open" << name2 << endl;
		exit(1);
	} 


	
	for (int i=0;i<psize;i++) // runs over all pts 
	{
		
		OUT << p[i] <<  "  " << a.SCM32[i]  <<  "  " << a.SCM42[i]   <<  "  " << a.SCM43[i]  << "  "  << endl;
		
	}
	OUT.close();
	

}

void cum(outs & a, string name2, double p[], int psize){


	ofstream OUT;
	OUT.open(name2.c_str());
	
	//cout << name2 << endl;
	
	if (!OUT.is_open())
	{
		cout << "Error: Can't open" << name2 << endl;
		exit(1);
	} 


	
	for (int i=0;i<psize;i++) // runs over all pts 
	{
		
		OUT << p[i] <<  "  " ;
		for (int v=0;v<6;v++) OUT << pow(a.c4[v][i],0.25)/a.c2[v][i] << " ";
		OUT << endl;
		
	}
	OUT.close();
	

}

void rms(outs & a, string name2, double p[], int psize){


  ofstream OUT;
  OUT.open(name2.c_str());

  //cout << name2 << endl;                                                                                                                                         

  if (!OUT.is_open())
    {
      cout << "Error: Can't open" << name2 << endl;
      exit(1);
    }



  for (int i=0;i<psize;i++) // runs over all pts                                                                                                                   
    {

      OUT << p[i] <<  "  " ;
      for (int v=0;v<6;v++) OUT << a.vrms[v][i]  << " ";
      OUT << endl;

    }
  OUT.close();


}

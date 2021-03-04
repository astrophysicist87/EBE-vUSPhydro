#ifndef _SPECTRA_H_
#define _SPECTRA_H_

#include <vector>

using namespace std;


class spectra {
public:
	double ** x;
	int pTmax,phimax;
	spectra& operator=(spectra a);
	spectra& operator=(double a);
	void setup(int pT,int phi);
	void setpT(int i_pT,double a);
	void setphi(int i_phi,double a);
	void destroy();
	void destroyc();
	
	
};


class list {
public:
	int pTmax,phimax;
	spectra pt,phi,dNdpdphi,dNdpdphic;
	list(int npT,int nphi);
	void setup(vector<double> pt,vector<double> phi);
	void destroy();
	void destroyc();
};


class FTspectra
{
	private:
		inline int index5D(int ipT, int iphi, int iQX, int iQY, int iQZ)
		{
			return ( ( ( ( ipT * phimax + iphi ) * QXmax + iQX )
							   * QYmax + iQY )   * QZmax + iQZ );
		}
	public:
		vector<double> x;	// collapse 5d vector
		int pTmax, phimax, QXmax, QYmax, QZmax;
		FTspectra& operator=(FTspectra a);
		FTspectra& operator=(double a);
		void setup(int pT, int phi, int QX, int QY, int QZ);
		void setpT(int i_pT, double a);
		void setphi(int i_phi, double a);
		void setQX(int i_QX, double a);
		void setQY(int i_QY, double a);
		void setQZ(int i_QZ, double a);
		void destroy();
		void destroyc();
};


class FTlist {
public:
	int pTmax, phimax, QXmax, QYmax, QZmax;
	FTspectra pt, phi, QX, QY, QZ, FTdNdpdphi, FTdNdpdphic;
	FTlist(int npT, int nphi, int nQX, int nQY, int nQZ);
	void setup( vector<double> & pt, vector<double> & phi,
				vector<double> & QX, vector<double> & QY, vector<double> & QZ );
	void destroy();
	void destroyc();
};


list::list(int npT,int nphi) {
                      
        pTmax=npT;
        phimax=nphi;
        pt.setup(pTmax,phimax);
        phi.setup(pTmax,phimax);
        dNdpdphi.setup(pTmax,phimax);
        dNdpdphic.setup(pTmax,phimax);
}

void list::destroy() {
                      
        pt.destroy();
        phi.destroy();
        dNdpdphi.destroy();
        
}

void list::destroyc() {
                      
        pt.destroy();
        phi.destroy();
        dNdpdphi.destroy();
        dNdpdphic.destroy();
        
}

void list::setup(vector<double> ptp,vector<double> phip) {
            
            
            int si=ptp.size();
        for (int i=0;i<si;i++){
       
  		pt.setpT(i,ptp[i]);
  	}
  	si=phip.size();
  	for (int i=0;i<si;i++){
  		phi.setphi(i,phip[i]);
  	}
}




void spectra::setup(int pT,int phi) {
                      
        pTmax=pT;
        phimax=phi;
	
	x = new double*[pT];
	for(int i=0; i<pT; i++)
		x[i] = new double[phi];

	// initialization
	for(int j=0; j<phi; j++)
		for(int i=0; i<pT; i++)
			x[i][j] = 0;
}

void spectra::destroy() {
	for(int i=0; i<pTmax; i++)
		delete [] x[i];

	delete [] x;
}


spectra& spectra::operator=(spectra a) {
             
	for(int i=0; i<pTmax; i++) {
	for(int j=0; j<phimax; j++) {
	x[i][j]=a.x[i][j];
	} }
	
	return *this;
	
}


spectra& spectra::operator=(double a) {
             
	for(int i=0; i<pTmax; i++) {
	for(int j=0; j<phimax; j++) {
	 x[i][j]=a;
	 }}
	
	return *this;
	
}


void spectra::setpT(int i_pT,double a) {
                      
	
	for(int j=0; j<phimax; j++) {
	x[i_pT][j]=a;
	}	
}


void spectra::setphi(int i_phi,double a) {
                      
	
	for(int j=0; j<pTmax; j++) {
	x[j][i_phi]=a;
	}	
}



// Christopher Plumberg changes - 02/26/2021
// Add FTlist methods to store Fourier-transformed (FT) spectra
FTlist::FTlist(int npT,int nphi)
{       
        pTmax=npT;
        phimax=nphi;
        pt.setup(pTmax,phimax);
        phi.setup(pTmax,phimax);
        dNdpdphi.setup(pTmax,phimax);
        dNdpdphic.setup(pTmax,phimax);
}

void FTlist::destroy() {
                      
        pt.destroy();
        phi.destroy();
        dNdpdphi.destroy();
        
}

void FTlist::destroyc() {
                      
        pt.destroy();
        phi.destroy();
        dNdpdphi.destroy();
        dNdpdphic.destroy();
        
}

void FTlist::setup( vector<double> & ptp, vector<double> & phip,
					vector<double> & QXp, vector<double> & QYp, vector<double> & QZp )
{
	int si=ptp.size();
    for (int i=0;i<si;i++) pt.setpT(i,ptp[i]);
  	si=phip.size();
  	for (int i=0;i<si;i++) phi.setphi(i,phip[i]);
  	si=QXp.size();
  	for (int i=0;i<si;i++) QX.setQX(i,QXp[i]);
  	si=QYp.size();
  	for (int i=0;i<si;i++) QY.setQY(i,QYp[i]);
  	si=QZp.size();
  	for (int i=0;i<si;i++) QZ.setQZ(i,QZp[i]);
}



void FTspectra::setup(int pT, int phi, int QX, int QY, int QZ)
{                      
	pTmax=pT; phimax=phi;
	QXmax=QX; QYmax=QY; QZmax=QZ;
	
	x.resize(pTmax*phimax*QXmax*QYmax*QZmax, 0.0);
	return;
}

void FTspectra::destroy()
{
	return;
}


FTspectra& FTspectra::operator=(FTspectra a)
{
	x = a.x;
	return *this;
}


FTspectra& FTspectra::operator=(double a)
{
	std::fill(x.begin(), x.end(), a);
	return *this;	
}


void FTspectra::setpT(int i_pT, double a)
{
	for(int iphi=0; iphi<phimax; iphi++)
	for(int iQX=0; iQX<QXmax; iQX++)
	for(int iQY=0; iQY<QYmax; iQY++)
	for(int iQZ=0; iQZ<QZmax; iQZ++)
		x[index5D(i_pT,iphi,iQX,iQY,iQZ)] = a;
	return;
}


void FTspectra::setphi(int i_phi,double a)
{
	for(int ipT=0; ipT<pTmax; ipT++)
	for(int iQX=0; iQX<QXmax; iQX++)
	for(int iQY=0; iQY<QYmax; iQY++)
	for(int iQZ=0; iQZ<QZmax; iQZ++)
		x[index5D(ipT,i_phi,iQX,iQY,iQZ)] = a;
	return;
}


void FTspectra::setQX(int i_QX,double a)
{
	for(int ipT=0; ipT<pTmax; ipT++)
	for(int iphi=0; iphi<phimax; iphi++)
	for(int iQY=0; iQY<QYmax; iQY++)
	for(int iQZ=0; iQZ<QZmax; iQZ++)
		x[index5D(ipT,iphi,i_QX,iQY,iQZ)] = a;
	return;
}


void FTspectra::setQY(int i_QY,double a)
{
	for(int ipT=0; ipT<pTmax; ipT++)
	for(int iphi=0; iphi<phimax; iphi++)
	for(int iQX=0; iQX<QXmax; iQX++)
	for(int iQZ=0; iQZ<QZmax; iQZ++)
		x[index5D(ipT,iphi,iQX,i_QY,iQZ)] = a;
	return;
}


void FTspectra::setQZ(int i_QZ,double a)
{
	for(int ipT=0; ipT<pTmax; ipT++)
	for(int iphi=0; iphi<phimax; iphi++)
	for(int iQX=0; iQX<QXmax; iQX++)
	for(int iQY=0; iQY<QYmax; iQY++)
		x[index5D(ipT,iphi,iQX,iQY,i_QZ)] = a;
	return;
}


#endif

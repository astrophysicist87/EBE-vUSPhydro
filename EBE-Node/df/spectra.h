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
	int pTmax, phimax;
	spectra pt, phi, dNdpdphi, dNdpdphic;
	// direct space-time moments
	spectra stm_S,   stm_xS,  stm_yS,  stm_zS,  stm_tS, 
			stm_x2S, stm_y2S, stm_z2S, stm_t2S, 
			stm_xyS, stm_xzS, stm_yzS, stm_xtS, stm_ytS, stm_ztS;
	// space-time moments with viscous corrections included
	spectra stm_Sc,   stm_xSc,  stm_ySc,  stm_zSc, stm_tSc, 
			stm_x2Sc, stm_y2Sc, stm_z2Sc, stm_t2Sc, 
			stm_xySc, stm_xzSc, stm_yzSc, stm_xtSc, stm_ytSc, stm_ztSc;
	list(int npT, int nphi);
	void setup(vector<double> & pt, vector<double> & phi);
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


list::list(int npT,int nphi)
{
    pTmax=npT;
    phimax=nphi;
    pt.setup(pTmax,phimax);
    phi.setup(pTmax,phimax);
    dNdpdphi.setup(pTmax,phimax);
    dNdpdphic.setup(pTmax,phimax);
	stm_S.setup(pTmax,phimax);
	stm_xS.setup(pTmax,phimax);
	stm_yS.setup(pTmax,phimax);
	stm_zS.setup(pTmax,phimax);
	stm_tS.setup(pTmax,phimax); 
	stm_x2S.setup(pTmax,phimax);
	stm_y2S.setup(pTmax,phimax);
	stm_z2S.setup(pTmax,phimax);
	stm_t2S.setup(pTmax,phimax);
	stm_xyS.setup(pTmax,phimax);
	stm_xzS.setup(pTmax,phimax);
	stm_yzS.setup(pTmax,phimax);
	stm_xtS.setup(pTmax,phimax);
	stm_ytS.setup(pTmax,phimax);
	stm_ztS.setup(pTmax,phimax);
	stm_Sc.setup(pTmax,phimax);
	stm_xSc.setup(pTmax,phimax);
	stm_ySc.setup(pTmax,phimax);
	stm_zSc.setup(pTmax,phimax);
	stm_tSc.setup(pTmax,phimax); 
	stm_x2Sc.setup(pTmax,phimax);
	stm_y2Sc.setup(pTmax,phimax);
	stm_z2Sc.setup(pTmax,phimax);
	stm_t2Sc.setup(pTmax,phimax);
	stm_xySc.setup(pTmax,phimax);
	stm_xzSc.setup(pTmax,phimax);
	stm_yzSc.setup(pTmax,phimax);
	stm_xtSc.setup(pTmax,phimax);
	stm_ytSc.setup(pTmax,phimax);
	stm_ztSc.setup(pTmax,phimax);
}

void list::destroy()
{
    pt.destroy();
    phi.destroy();
    dNdpdphi.destroy();
	stm_S.destroy();
	stm_xS.destroy();
	stm_yS.destroy();
	stm_zS.destroy();
	stm_tS.destroy();
	stm_x2S.destroy();
	stm_y2S.destroy();
	stm_z2S.destroy();
	stm_t2S.destroy();
	stm_xyS.destroy();
	stm_xzS.destroy();
	stm_yzS.destroy();
	stm_xtS.destroy();
	stm_ytS.destroy();
	stm_ztS.destroy();
}

void list::destroyc()
{                      
    pt.destroy();
    phi.destroy();
    dNdpdphi.destroy();
    dNdpdphic.destroy();
	stm_S.destroy();
	stm_xS.destroy();
	stm_yS.destroy();
	stm_zS.destroy();
	stm_tS.destroy();
	stm_x2S.destroy();
	stm_y2S.destroy();
	stm_z2S.destroy();
	stm_t2S.destroy();
	stm_xyS.destroy();
	stm_xzS.destroy();
	stm_yzS.destroy();
	stm_xtS.destroy();
	stm_ytS.destroy();
	stm_ztS.destroy();
	stm_Sc.destroy();
	stm_xSc.destroy();
	stm_ySc.destroy();
	stm_zSc.destroy();
	stm_tSc.destroy();
	stm_x2Sc.destroy();
	stm_y2Sc.destroy();
	stm_z2Sc.destroy();
	stm_t2Sc.destroy();
	stm_xySc.destroy();
	stm_xzSc.destroy();
	stm_yzSc.destroy();
	stm_xtSc.destroy();
	stm_ytSc.destroy();
	stm_ztSc.destroy();        
}

void list::setup(vector<double> & ptp,vector<double> & phip)
{
            
            
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
FTlist::FTlist(int npT, int nphi, int nQX, int nQY, int nQZ)
{       
    pTmax  = npT;
    phimax = nphi;
	QXmax  = nQX;
	QYmax  = nQY;
	QZmax  = nQZ;
    pt.setup( pTmax, phimax, QXmax, QYmax, QZmax );
    phi.setup( pTmax, phimax, QXmax, QYmax, QZmax );
    FTdNdpdphi.setup( pTmax, phimax, QXmax, QYmax, QZmax );
    FTdNdpdphic.setup( pTmax, phimax, QXmax, QYmax, QZmax );
}

void FTlist::destroy()
{
    pt.destroy();
    phi.destroy();
    FTdNdpdphi.destroy();
}

void FTlist::destroyc()
{
    pt.destroy();
    phi.destroy();
    FTdNdpdphi.destroy();
    FTdNdpdphic.destroy();
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

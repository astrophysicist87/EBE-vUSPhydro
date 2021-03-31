#ifndef _SPECTRA_H_
#define _SPECTRA_H_

#include <vector>
#include <complex>

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
	spectra stm_xoS, stm_xsS, stm_xlS, stm_xo2S, stm_xs2S, stm_xl2S,
			stm_xoxsS, stm_xoxlS, stm_xsxlS, stm_xotS, stm_xstS, stm_xltS;
	// direct space-time moments with viscous corrections included
	spectra stm_Sc,   stm_xSc,  stm_ySc,  stm_zSc, stm_tSc, 
			stm_x2Sc, stm_y2Sc, stm_z2Sc, stm_t2Sc, 
			stm_xySc, stm_xzSc, stm_yzSc, stm_xtSc, stm_ytSc, stm_ztSc;
	spectra stm_xoSc, stm_xsSc, stm_xlSc, stm_xo2Sc, stm_xs2Sc, stm_xl2Sc,
			stm_xoxsSc, stm_xoxlSc, stm_xsxlSc, stm_xotSc, stm_xstSc, stm_xltSc;

	// not really spectra, but use the class anyway...
	spectra R2o, R2s, R2l, R2os, R2sl, R2ol;
	spectra R2o_c, R2s_c, R2l_c, R2os_c, R2sl_c, R2ol_c;

	list(int npT, int nphi);
	void setup(vector<double> & pt, vector<double> & phi);
	void compute_HBT_radii( double mass );
	void destroy();
	void destroyc();
};


class FTspectra
{
	public:
		vector<complex<double> > x;	// collapse 5d vector
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
		inline int index5D(int ipT, int iphi, int iQX, int iQY, int iQZ)
		{
			return ( ( ( ( ipT * phimax + iphi ) * QXmax + iQX )
							   * QYmax + iQY )   * QZmax + iQZ );
		}
};


class FTlist
{
	public:
		int pTmax, phimax, QXmax, QYmax, QZmax;
		FTspectra pt, phi, QX, QY, QZ, FTdNdpdphi, FTdNdpdphic;
		FTlist(int npT, int nphi, int nQX, int nQY, int nQZ);
		void setup( vector<double> & pt, vector<double> & phi,
					vector<double> & QX, vector<double> & QY, vector<double> & QZ );
		void destroy();
		void destroyc();
		/*inline int index5D(int ipT, int iphi, int iQX, int iQY, int iQZ)
		{
			return ( ( ( ( ipT * phimax + iphi ) * QXmax + iQX )
							   * QYmax + iQY )   * QZmax + iQZ );
		}*/
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

	stm_xoS.setup(pTmax,phimax);
	stm_xsS.setup(pTmax,phimax);
	stm_xlS.setup(pTmax,phimax);
	stm_xo2S.setup(pTmax,phimax);
	stm_xs2S.setup(pTmax,phimax);
	stm_xl2S.setup(pTmax,phimax);
	stm_xoxsS.setup(pTmax,phimax);
	stm_xoxlS.setup(pTmax,phimax);
	stm_xsxlS.setup(pTmax,phimax);
	stm_xotS.setup(pTmax,phimax);
	stm_xstS.setup(pTmax,phimax);
	stm_xltS.setup(pTmax,phimax);

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

	stm_xoSc.setup(pTmax,phimax);
	stm_xsSc.setup(pTmax,phimax);
	stm_xlSc.setup(pTmax,phimax);
	stm_xo2Sc.setup(pTmax,phimax);
	stm_xs2Sc.setup(pTmax,phimax);
	stm_xl2Sc.setup(pTmax,phimax);
	stm_xoxsSc.setup(pTmax,phimax);
	stm_xoxlSc.setup(pTmax,phimax);
	stm_xsxlSc.setup(pTmax,phimax);
	stm_xotSc.setup(pTmax,phimax);
	stm_xstSc.setup(pTmax,phimax);
	stm_xltSc.setup(pTmax,phimax);

	R2o.setup(pTmax,phimax);
	R2s.setup(pTmax,phimax);
	R2l.setup(pTmax,phimax);
	R2os.setup(pTmax,phimax);
	R2ol.setup(pTmax,phimax);
	R2sl.setup(pTmax,phimax);

	R2o_c.setup(pTmax,phimax);
	R2s_c.setup(pTmax,phimax);
	R2l_c.setup(pTmax,phimax);
	R2os_c.setup(pTmax,phimax);
	R2ol_c.setup(pTmax,phimax);
	R2sl_c.setup(pTmax,phimax);

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

	stm_xoS.destroy();
	stm_xsS.destroy();
	stm_xlS.destroy();
	stm_xo2S.destroy();
	stm_xs2S.destroy();
	stm_xl2S.destroy();
	stm_xoxsS.destroy();
	stm_xoxlS.destroy();
	stm_xsxlS.destroy();
	stm_xotS.destroy();
	stm_xstS.destroy();
	stm_xltS.destroy();

	R2o.destroy();
	R2s.destroy();
	R2l.destroy();
	R2os.destroy();
	R2ol.destroy();
	R2sl.destroy();

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

	stm_xoS.destroy();
	stm_xsS.destroy();
	stm_xlS.destroy();
	stm_xo2S.destroy();
	stm_xs2S.destroy();
	stm_xl2S.destroy();
	stm_xoxsS.destroy();
	stm_xoxlS.destroy();
	stm_xsxlS.destroy();
	stm_xotS.destroy();
	stm_xstS.destroy();
	stm_xltS.destroy();

	stm_xoSc.destroy();
	stm_xsSc.destroy();
	stm_xlSc.destroy();
	stm_xo2Sc.destroy();
	stm_xs2Sc.destroy();
	stm_xl2Sc.destroy();
	stm_xoxsSc.destroy();
	stm_xoxlSc.destroy();
	stm_xsxlSc.destroy();
	stm_xotSc.destroy();
	stm_xstSc.destroy();
	stm_xltSc.destroy();

	R2o.destroy();
	R2s.destroy();
	R2l.destroy();
	R2os.destroy();
	R2ol.destroy();
	R2sl.destroy();

	R2o_c.destroy();
	R2s_c.destroy();
	R2l_c.destroy();
	R2os_c.destroy();
	R2ol_c.destroy();
	R2sl_c.destroy();
}

void list::setup(vector<double> & ptp,vector<double> & phip)
{
            
    int si=ptp.size();
    for (int i=0;i<si;i++)
	{   
  		pt.setpT(i,ptp[i]);
  	}
  	si=phip.size();
  	for (int i=0;i<si;i++){
  		phi.setphi(i,phip[i]);
  	}
}


void list::compute_HBT_radii( double mass )
{
	// first normalize all source moments
	/*for ( int ipT  = 0; ipT  < pTmax;  ipT++ )
	for ( int iphi = 0; iphi < phimax; iphi++ )
	{
		double Slocal = stm_S.x[ipT][iphi];
		stm_xS.x[ipT][iphi] /= Slocal;
		stm_yS.x[ipT][iphi] /= Slocal;
		stm_zS.x[ipT][iphi] /= Slocal;
		stm_tS.x[ipT][iphi] /= Slocal;
		stm_x2S.x[ipT][iphi] /= Slocal;
		stm_y2S.x[ipT][iphi] /= Slocal;
		stm_z2S.x[ipT][iphi] /= Slocal;
		stm_t2S.x[ipT][iphi] /= Slocal;
		stm_xyS.x[ipT][iphi] /= Slocal;
		stm_xzS.x[ipT][iphi] /= Slocal;
		stm_yzS.x[ipT][iphi] /= Slocal;
		stm_xtS.x[ipT][iphi] /= Slocal;
		stm_ytS.x[ipT][iphi] /= Slocal;
		stm_ztS.x[ipT][iphi] /= Slocal;
	
		double Sclocal = stm_Sc.x[ipT][iphi];
		stm_xSc.x[ipT][iphi] /= Sclocal;
		stm_ySc.x[ipT][iphi] /= Sclocal;
		stm_zSc.x[ipT][iphi] /= Sclocal;
		stm_tSc.x[ipT][iphi] /= Sclocal;
		stm_x2Sc.x[ipT][iphi] /= Sclocal;
		stm_y2Sc.x[ipT][iphi] /= Sclocal;
		stm_z2Sc.x[ipT][iphi] /= Sclocal;
		stm_t2Sc.x[ipT][iphi] /= Sclocal;
		stm_xySc.x[ipT][iphi] /= Sclocal;
		stm_xzSc.x[ipT][iphi] /= Sclocal;
		stm_yzSc.x[ipT][iphi] /= Sclocal;
		stm_xtSc.x[ipT][iphi] /= Sclocal;
		stm_ytSc.x[ipT][iphi] /= Sclocal;
		stm_ztSc.x[ipT][iphi] /= Sclocal;
	
		stm_xoS.x[ipT][iphi] /= Slocal;
		stm_xsS.x[ipT][iphi] /= Slocal;
		stm_xlS.x[ipT][iphi] /= Slocal;
		stm_xo2S.x[ipT][iphi] /= Slocal;
		stm_xs2S.x[ipT][iphi] /= Slocal;
		stm_xl2S.x[ipT][iphi] /= Slocal;
		stm_xoxsS.x[ipT][iphi] /= Slocal;
		stm_xoxlS.x[ipT][iphi] /= Slocal;
		stm_xsxlS.x[ipT][iphi] /= Slocal;
		stm_xotS.x[ipT][iphi] /= Slocal;
		stm_xstS.x[ipT][iphi] /= Slocal;
		stm_xltS.x[ipT][iphi] /= Slocal;
	
		stm_xoSc.x[ipT][iphi] /= Sclocal;
		stm_xsSc.x[ipT][iphi] /= Sclocal;
		stm_xlSc.x[ipT][iphi] /= Sclocal;
		stm_xo2Sc.x[ipT][iphi] /= Sclocal;
		stm_xs2Sc.x[ipT][iphi] /= Sclocal;
		stm_xl2Sc.x[ipT][iphi] /= Sclocal;
		stm_xoxsSc.x[ipT][iphi] /= Sclocal;
		stm_xoxlSc.x[ipT][iphi] /= Sclocal;
		stm_xsxlSc.x[ipT][iphi] /= Sclocal;
		stm_xotSc.x[ipT][iphi] /= Sclocal;
		stm_xstSc.x[ipT][iphi] /= Sclocal;
		stm_xltSc.x[ipT][iphi] /= Sclocal;
	}*/

	// then get the radii (ideal)
	for ( int ipT  = 0; ipT  < pTmax;  ipT++ )
	for ( int iphi = 0; iphi < phimax; iphi++ )
	{
		const double KT = pt.x[ipT][iphi], Kphi = phi.x[ipT][iphi], KYrap = 0.0;
		const double betaT = KT / sqrt(mass*mass + KT*KT), betaL = 0.0;

		double xo   = stm_xoS.x[ipT][iphi],   xs   = stm_xsS.x[ipT][iphi],
			   xl   = stm_xlS.x[ipT][iphi],   ta   = stm_tS.x[ipT][iphi];
		double xo2  = stm_xo2S.x[ipT][iphi],  xs2  = stm_xs2S.x[ipT][iphi],
			   xl2  = stm_xl2S.x[ipT][iphi],  t2   = stm_t2S.x[ipT][iphi],
			   xoxs = stm_xoxsS.x[ipT][iphi], xoxl = stm_xoxlS.x[ipT][iphi],
			   xsxl = stm_xsxlS.x[ipT][iphi], xot  = stm_xotS.x[ipT][iphi],
			   xst  = stm_xstS.x[ipT][iphi],  xlt  = stm_xltS.x[ipT][iphi];
		R2o.x[ipT][iphi]  = (xo2-xo*xo) - 2.0*betaT*(xot-xo*ta) + betaT*betaT*(t2-ta*ta);
		R2s.x[ipT][iphi]  = (xs2-xs*xs);
		R2l.x[ipT][iphi]  = (xl2-xl*xl) - 2.0*betaL*(xlt-xl*ta) + betaL*betaL*(t2-ta*ta);
		R2os.x[ipT][iphi] = (xoxs-xo*xs) - betaT*(xst-xs*ta);
		R2ol.x[ipT][iphi] = (xoxl-xo*xl) - betaT*(xlt-xl*ta) - betaL*(xot-xo*ta) + betaT*betaL*(t2-ta*ta);
		R2sl.x[ipT][iphi] = (xsxl-xs*xl) - betaL*(xst-xs*ta);
	}

	// finally get the radii (ideal+viscous)
	for ( int ipT  = 0; ipT  < pTmax;  ipT++ )
	for ( int iphi = 0; iphi < phimax; iphi++ )
	{
		const double KT = pt.x[ipT][iphi], Kphi = phi.x[ipT][iphi], KYrap = 0.0;
		const double betaT = KT / sqrt(mass*mass + KT*KT), betaL = 0.0;

		double xo   = stm_xoSc.x[ipT][iphi],   xs   = stm_xsSc.x[ipT][iphi],
			   xl   = stm_xlSc.x[ipT][iphi],   ta   = stm_tSc.x[ipT][iphi];
		double xo2  = stm_xo2Sc.x[ipT][iphi],  xs2  = stm_xs2Sc.x[ipT][iphi],
			   xl2  = stm_xl2Sc.x[ipT][iphi],  t2   = stm_t2Sc.x[ipT][iphi],
			   xoxs = stm_xoxsSc.x[ipT][iphi], xoxl = stm_xoxlSc.x[ipT][iphi],
			   xsxl = stm_xsxlSc.x[ipT][iphi], xot  = stm_xotSc.x[ipT][iphi],
			   xst  = stm_xstSc.x[ipT][iphi],  xlt  = stm_xltSc.x[ipT][iphi];
		R2o_c.x[ipT][iphi]  = (xo2-xo*xo) - 2.0*betaT*(xot-xo*ta) + betaT*betaT*(t2-ta*ta);
		R2s_c.x[ipT][iphi]  = (xs2-xs*xs);
		R2l_c.x[ipT][iphi]  = (xl2-xl*xl) - 2.0*betaL*(xlt-xl*ta) + betaL*betaL*(t2-ta*ta);
		R2os_c.x[ipT][iphi] = (xoxs-xo*xs) - betaT*(xst-xs*ta);
		R2ol_c.x[ipT][iphi] = (xoxl-xo*xl) - betaT*(xlt-xl*ta) - betaL*(xot-xo*ta) + betaT*betaL*(t2-ta*ta);
		R2sl_c.x[ipT][iphi] = (xsxl-xs*xl) - betaL*(xst-xs*ta);
	}

	return;
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

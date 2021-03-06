#ifndef _SPECTRA_H_
#define _SPECTRA_H_


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
	spectra pt,phi,dNdpdphi;
	list(int npT,int nphi);
	void setup(double pt_start,double pt_end, double pt_step,double * philist);
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

void list::setup(double pt_start,double pt_end, double pt_step,double * philist) {
            
            double psub=pt_start;
            
        for (int i=0;i<pTmax;i++){
       
  		pt.setpT(i,psub);
  		psub+=pt_step;
  	}
  	for (int i=0;i<phimax;i++){
  		phi.setphi(i,philist[i]);
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



#endif

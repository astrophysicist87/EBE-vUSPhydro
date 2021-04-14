#ifndef EOS_H_ 
#define EOS_H_

#include <string>
#include <iostream>

//#include "fadiff.h"





class eos {
private:
	double energy,pressure,temp,entropy,Aout,eppcur;
	
	double scheck;		
	void pressure_ideal();
	void energy_ideal();
	void temp_ideal();
	void pressure_tab();
	void energy_tab();
	void temp_tab();
	void tablow();
	double dwds_low();
	int repeat,low;
	
	
	double Aideal();
	double Atable();
	
	
	double cp,cp2;
	double e_ptable(double s);
	double h_calc(double x);
	double dervs(double (eos::*f)(double) ,double x, double h,double & err);
	double dervsprint(double (eos::*f)(double) ,double x, double h,double & err);
	double dervssub(double (eos::*f)(double) ,double x, double h);
	void  epp() {eppcur=energy+pressure;}
	double dwds_tab();
	double dwds_ideal();
	double e_plow(double s);
	double estable(double s);
	double tempcutoff,es1,es2,ps1,ds1,fourthirds,BIG,onethird,gg,echeck;
	int pos;
	
        static constexpr double  mpi=0.736231, cPI=3.14159265358979;
	constexpr int static NTAB=10;
public:
	std::string ideal,table;
	std::string typerem;
	double c,cs;
	eos();
	void eosin(std::string type);
	
	~eos();
	void update_s(double sin);
	double s_out(double ein);
        double e_out(double sIC);
	
	void outpos();
	double e();
	double p();
	double s();
	double T();
	double A();
	double w() {return eppcur;}
	double dwds();
	double tout(double a, double e0) {return pow(a*e0/(3*c),0.25);}
	double efreeze();
        double sfreeze();
	double s_terms_T(double Tt);
	double cs2out(double Tt);
	double wfz(double Tt);
	
	
	
};



#endif

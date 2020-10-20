#pragma once

#include "../splinter/include/bspline.h"

#include <fstream>
#include <string>

using std::string;

using namespace SPLINTER;

class eos {

    public:
        eos();
        //quantityFile must be in dimensionless quantities and must be formatted "T  muB  muQ  muS  p  entr  b  s  q  e  cs2"
        //derivFile should be formatted "T  muB  muQ  muS  d2p_db2  d2p_dq2  d2p_ds2  d2p_dbdq  d2p_dbds d2p_dqds  d2p_dtdb  d2p_dtdq  d2p_dtds  d2p_dt2"
        eos(string quantityFile, string derivFile, int degree);

        void init(string quantityFile, string derivFile, int degree);

        //getter functions for the quantities of interest at the current tbs/tbqs
        double p();     //pressure
        double entr();  //entropy density
        double b();     //baryon density
        double s();     //strangeness density
        double q();     //charge density
        double e();     //energy density
        double cs2();   //speed of sound
        double T();     //temperature
        double muB();   //baryon chemical potential
        double muQ();   //charge chemical potential
        double muS();   //strangeness chemical potential

        double d2p_db2();
        double d2p_dq2();
        double d2p_ds2();
        double d2p_dbdq();
        double d2p_dbds();
        double d2p_dqds();      //derivative functions
        double d2p_dtdb();
        double d2p_dtdq();
        double d2p_dtds();
        double d2p_dt2();

        //These functions are placeholders for the sake of compilation. They do nothing and should not be used in their current state
        void eosin(std::string type);
        void update_s(double sin);
        double w();
        double A();
        // double s();
        double dwds();
        double s_out(double ein);
        double s_terms_T(double Tt);
        double efreeze();
        double sfreeze();
        double cs2out(double Tt);
        double wfz(double Tt);

        //these functions change the position in the grid to (T,muB,muQ,muS)
        void tbqs(double setT, double setmuB, double setmuQ, double setmuS);

        bool rootfinder4D(double entrGiven, double eGiven, double rhoBGiven, double rhoSGiven, double rhoQgiven, double error, size_t steps);
        bool quant_rootfinder4D(double quantGiven, string quantType, int whichIndepVar, double error, size_t steps);

    private:

        //interpolation for each quantity
        BSpline pSpline, entrSpline, bSpline, sSpline, qSpline, eSpline, cs2Spline;
        BSpline db2Spline, dq2Spline, ds2Spline, dt2Spline, dbdqSpline, dbdsSpline, dtdbSpline, dqdsSpline, dtdqSpline, dtdsSpline;

        //value of each quantity at the current tbsPosition
        double pVal, entrVal, bVal, sVal, qVal, eVal, cs2Val;
        double db2Val, ds2Val, dq2Val, dt2Val, dbdqVal, dbdsVal, dqdsVal, dtdbVal, dtdsVal, dtdqVal;

        double maxMuB, minMuB, maxMuS, minMuS, maxMuQ, minMuQ, maxT, minT;

        //the current position in (T, muB, muQ, muS)
        DenseVector tbqsPosition;

};
#ifndef FIT_H
#define FIT_H

#include<iostream>
#include<sstream>
#include<string>
#include<fstream>
#include<cmath>
#include<iomanip>
#include<vector>
#include<stdio.h>

#include <gsl/gsl_errno.h>
#include <gsl/gsl_rng.h>	    // gsl random number generators
#include <gsl/gsl_randist.h>	    // gsl random number distributions
#include <gsl/gsl_vector.h>	    // gsl vector and matrix definitions
#include <gsl/gsl_blas.h>	    // gsl linear algebra stuff
#include <gsl/gsl_multifit_nlin.h>  // gsl multidimensional fitting
#include <gsl/gsl_multifit.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_math.h>

#include "ParameterReader.h"
#include "Arsenal.h"
#include "gauss_quadrature.h"

namespace fitCF
{
	
	struct Correlationfunction3D_data
	{
		size_t data_length;
		vector<double> q_o;
		vector<double> q_s;
		vector<double> q_l;
		vector<double> y;
		vector<double> sigma;
	};
	
	int Fittarget_correlfun3D_f (const gsl_vector *xvec_ptr, void *params_ptr,
									gsl_vector *f_ptr);
	int Fittarget_correlfun3D_df (const gsl_vector *xvec_ptr, void *params_ptr,
									gsl_matrix *Jacobian_ptr);
	int Fittarget_correlfun3D_fdf (const gsl_vector* xvec_ptr, void *params_ptr,
									gsl_vector* f_ptr, gsl_matrix* Jacobian_ptr);
	int Fittarget_correlfun3D_f_withlambda (const gsl_vector *xvec_ptr, void *params_ptr,
									gsl_vector *f_ptr);
	int Fittarget_correlfun3D_df_withlambda (const gsl_vector *xvec_ptr, void *params_ptr,
									gsl_matrix *Jacobian_ptr);
	int Fittarget_correlfun3D_fdf_withlambda (const gsl_vector* xvec_ptr, void *params_ptr,
									gsl_vector* f_ptr, gsl_matrix* Jacobian_ptr);
	
	const double hbarC = 0.197327053;
	
	const bool USE_LAMBDA = true;
	//const bool USE_LOG_FIT = true;
	extern bool USE_LOG_FIT;
	const int VERBOSE = 0;
	
	/*const int n_KT_pts = 15;
	const int n_Kphi_pts = 36;
	const int nqxpts = 7;
	const int nqypts = 7;
	const int nqzpts = 7;*/
	// initialize but allow them to be changed
	int n_KT_pts = 15;
	int n_Kphi_pts = 36;
	int nqxpts = 7;
	int nqypts = 7;
	int nqzpts = 7;
	
	const int n_order = 4;
	
	const int nKT = 101;
	const int nKphi = 48;
	const double KT_min = 0.01;
	const double KT_max = 1.01;
	
	
	const double fit_tolerance = 1e-6;
	const int fit_max_iterations = 100;
	
	inline int indexer_KT_Kphi( int iKT, int iKphi )
	{
		return ( iKT * n_Kphi_pts + iKphi );
	}
	
	inline int indexer_qx_qy_qz( int iqx, int iqy, int iqz )
	{
		return ( (iqx * nqypts + iqy) * nqzpts + iqz );
	}
	
	inline double get_fit_results(int i, gsl_multifit_fdfsolver * solver_ptr)
	{
		return gsl_vector_get (solver_ptr->x, i);
	}
	
	inline double get_fit_err (int i, gsl_matrix * covariance_ptr)
	{
		return sqrt (gsl_matrix_get (covariance_ptr, i, i));
	}
	
	/*vector<double> KT_pts(n_KT_pts), Kphi_pts(n_Kphi_pts);
	vector<double> qx_pts(nqxpts), qy_pts(nqypts), qz_pts(nqzpts);
	
	vector<vector<double> > CFvals = vector<vector<double> >( n_KT_pts * n_Kphi_pts,
									 vector<double> ( nqxpts * nqypts * nqzpts ) );*/
	
	extern vector<double> KT_pts, Kphi_pts, qx_pts, qy_pts, qz_pts;
	
	extern vector<vector<double> > CFvals;
	
	
	void Set_grid_parameters( string grid_parameter_file, int & n_KT_pts, int & n_Kphi_pts,
								int & nqxpts, int & nqypts, int & nqzpts );
	void Get_GF_HBTradii(string directory_in, bool use_log_fit_in=true);
	void Read_in_correlationfunction();
	void Fit_Correlationfunction3D(vector<double> & Correl_3D, int iKT, int iKphi);
	void Fit_Correlationfunction3D_withlambda(vector<double> & Correl_3D, int iKT, int iKphi);
	int print_fit_state_3D (size_t iteration, gsl_multifit_fdfsolver * solver_ptr);
	int print_fit_state_3D_withlambda (size_t iteration, gsl_multifit_fdfsolver * solver_ptr);
	void find_minimum_chisq_correlationfunction_full(vector<double> & Correl_3D, int iKT, int iKphi);
	void R2_Fourier_transform(int jKT, double plane_psi);

}
// End of file

#endif

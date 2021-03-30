#include<iostream>
#include<sstream>
#include<string>
#include<fstream>
#include<cmath>
#include<iomanip>
#include<vector>
#include<stdio.h>

#include "fit.h"

using namespace std;

namespace fitCF
{
	
	bool USE_LOG_FIT;
	
	vector<double> KT_pts, Kphi_pts, qx_pts, qy_pts, qz_pts;
	vector<vector<double> > CFvals;
	
	//HBT radii coefficients
	vector<double> lambda_Correl, lambda_Correl_err;
	vector<double> R2_side_GF, R2_out_GF, R2_long_GF, R2_outside_GF;
	vector<double> R2_side_GF_C, R2_out_GF_C, R2_long_GF_C, R2_outside_GF_C;
	vector<double> R2_side_GF_S, R2_out_GF_S, R2_long_GF_S, R2_outside_GF_S;
	vector<double> R2_side_err, R2_out_err, R2_long_err, R2_outside_err;
	
	string directory;
	
	void Get_GF_HBTradii(string directory_in, bool use_log_fit_in)
	{
		directory = directory_in;
		USE_LOG_FIT = use_log_fit_in;

		string grid_parameter_file = directory + "/pion_+_grid.params";
		Set_grid_parameters( grid_parameter_file, n_KT_pts, n_Kphi_pts,
												  nqxpts, nqypts, nqzpts );
	
		cout << "  - processing " << directory << " with log fit = " << USE_LOG_FIT << endl;
	
		KT_pts = vector<double>(n_KT_pts);
		Kphi_pts = vector<double>(n_Kphi_pts);
		qx_pts = vector<double>(nqxpts);
		qy_pts = vector<double>(nqypts);
		qz_pts = vector<double>(nqzpts);
	
		CFvals = vector<vector<double> >( n_KT_pts * n_Kphi_pts,
					vector<double> ( nqxpts * nqypts * nqzpts, 0.0 ) );
	
		lambda_Correl = vector<double>( n_KT_pts * n_Kphi_pts );
		R2_side_GF = vector<double>( n_KT_pts * n_Kphi_pts );
		R2_out_GF = vector<double>( n_KT_pts * n_Kphi_pts );
		R2_long_GF = vector<double>( n_KT_pts * n_Kphi_pts );
		R2_outside_GF = vector<double>( n_KT_pts * n_Kphi_pts );
	
		lambda_Correl_err = vector<double>( n_KT_pts * n_Kphi_pts );
		R2_side_err = vector<double>( n_KT_pts * n_Kphi_pts );
		R2_out_err = vector<double>( n_KT_pts * n_Kphi_pts );
		R2_long_err = vector<double>( n_KT_pts * n_Kphi_pts );
		R2_outside_err = vector<double>( n_KT_pts * n_Kphi_pts );
	
		R2_side_GF_C = vector<double>( nKT * n_order );
		R2_side_GF_S = vector<double>( nKT * n_order );
		R2_out_GF_C = vector<double>( nKT * n_order );
		R2_out_GF_S = vector<double>( nKT * n_order );
		R2_long_GF_C = vector<double>( nKT * n_order );
		R2_long_GF_S = vector<double>( nKT * n_order );
		R2_outside_GF_C = vector<double>( nKT * n_order );
		R2_outside_GF_S = vector<double>( nKT * n_order );
	
	
		Read_in_correlationfunction();
	
	
		for (int iKT = 0; iKT < n_KT_pts; ++iKT)
		for (int iKphi = 0; iKphi < n_Kphi_pts; ++iKphi)
		{
			vector<double> CF_for_fitting = CFvals[indexer_KT_Kphi(iKT, iKphi)];
	
			if ( USE_LOG_FIT )
				find_minimum_chisq_correlationfunction_full( CF_for_fitting, iKT, iKphi );
			else
			{
				//finally, do fits, depending on what kind you want to do
				if (USE_LAMBDA)
					Fit_Correlationfunction3D_withlambda( CF_for_fitting, iKT, iKphi );
				else
					Fit_Correlationfunction3D( CF_for_fitting, iKT, iKphi );
			}
		}
	
		// don't Fourier transform until we're sure we're doing Gaussian quadrature...
		//for (int iKT = 0; iKT < nKT; ++iKT)
		//	R2_Fourier_transform(iKT, 0.0);
	
		KT_pts.clear();
		Kphi_pts.clear();
		qx_pts.clear();
		qy_pts.clear();
		qz_pts.clear();
	
		CFvals.clear();
	
		lambda_Correl.clear();
		R2_side_GF.clear();
		R2_out_GF.clear();
		R2_long_GF.clear();
		R2_outside_GF.clear();
	
		lambda_Correl_err.clear();
		R2_side_err.clear();
		R2_out_err.clear();
		R2_long_err.clear();
		R2_outside_err.clear();
	
		R2_side_GF_C.clear();
		R2_side_GF_S.clear();
		R2_out_GF_C.clear();
		R2_out_GF_S.clear();
		R2_long_GF_C.clear();
		R2_long_GF_S.clear();
		R2_outside_GF_C.clear();
		R2_outside_GF_S.clear();
	
		return;
	}
	
	void Set_grid_parameters( string grid_parameter_file, int & n_KT_pts, int & n_Kphi_pts,
								int & nqxpts, int & nqypts, int & nqzpts )
	{
		ifstream iGridParams;
		iGridParams.open(grid_parameter_file.c_str());

		iGridParams >> n_KT_pts;
		iGridParams >> n_Kphi_pts;
		iGridParams >> nqxpts;
		iGridParams >> nqypts;
		iGridParams >> nqzpts;

		iGridParams.close();
		return;
	}

	void Read_in_correlationfunction()
	{
		ifstream iCorrFunc;
		string filename = directory + "/pion_+_HBT_correlation_function.dat";
		iCorrFunc.open(filename.c_str());
	
		double dummy;
		for (int iKT = 0; iKT < n_KT_pts; ++iKT)
		for (int iKphi = 0; iKphi < n_Kphi_pts; ++iKphi)
		for (int iqx = 0; iqx < nqxpts; ++iqx)
		for (int iqy = 0; iqy < nqypts; ++iqy)
		for (int iqz = 0; iqz < nqzpts; ++iqz)
		{
			iCorrFunc >> KT_pts[iKT]
	                  >> Kphi_pts[iKphi]
	                  >> qx_pts[iqx]
	                  >> qy_pts[iqy]
	                  >> qz_pts[iqz]
	                  >> dummy
	                  >> dummy
	                  >> dummy
	                  >> dummy
	                  >> CFvals[indexer_KT_Kphi(iKT, iKphi)][indexer_qx_qy_qz(iqx, iqy, iqz)];
		}
	
		iCorrFunc.close();
	
		return;
	}
	
	//**************************************************************
	// Gaussian fit routines below
	//**************************************************************
	
	void Fit_Correlationfunction3D(vector<double> & Correl_3D, int iKT, int iKphi)
	{
		string outfilename = directory + "/HBTradii_GF_grid0_fitLSQ.dat";
		ofstream out(outfilename.c_str(), ios::app);
	
		const size_t data_length = nqxpts*nqypts*nqzpts;  // # of points
		const size_t n_para = 4;  // # of parameters
	
		// allocate space for a covariance matrix of size p by p
		gsl_matrix *covariance_ptr = gsl_matrix_alloc (n_para, n_para);
	
		// allocate and setup for generating gaussian distibuted random numbers
		gsl_rng_env_setup ();
		const gsl_rng_type *type = gsl_rng_default;
		gsl_rng *rng_ptr = gsl_rng_alloc (type);
	
		//set up test data
		struct Correlationfunction3D_data Correlfun3D_data;
		Correlfun3D_data.data_length = data_length;
		Correlfun3D_data.q_o.resize(data_length);
		Correlfun3D_data.q_s.resize(data_length);
		Correlfun3D_data.q_l.resize(data_length);
		Correlfun3D_data.y.resize(data_length);
		Correlfun3D_data.sigma.resize(data_length);
	
		int idx = 0;
		double ckp = cos(Kphi_pts[iKphi]), skp = sin(Kphi_pts[iKphi]);
		for (int i = 0; i < nqxpts; i++)
		for (int j = 0; j < nqypts; j++)
		for (int k = 0; k < nqzpts; k++)
		{
			Correlfun3D_data.q_o[idx] = qx_pts[i] * ckp + qy_pts[j] * skp;
			Correlfun3D_data.q_s[idx] = -qx_pts[i] * skp + qy_pts[j] * ckp;
			Correlfun3D_data.q_l[idx] = qz_pts[k];
			Correlfun3D_data.y[idx] = Correl_3D[idx] - 1.0;
			Correlfun3D_data.sigma[idx] = 1e-3;
	if (i==(nqxpts-1)/2 && j==(nqypts-1)/2 && k==(nqzpts-1)/2)
		Correlfun3D_data.sigma[idx] = 1e10;	//ignore central point
			idx++;
		}
	
		double para_init[n_para] = { 1.0, 1.0, 1.0, 1.0 };  // initial guesses of parameters
	
		gsl_vector_view xvec_ptr = gsl_vector_view_array (para_init, n_para);
	
		// set up the function to be fit 
		gsl_multifit_function_fdf target_func;
		target_func.f = &Fittarget_correlfun3D_f;        // the function of residuals
		target_func.df = &Fittarget_correlfun3D_df;      // the gradient of this function
		target_func.fdf = &Fittarget_correlfun3D_fdf;    // combined function and gradient
		target_func.n = data_length;              // number of points in the data set
		target_func.p = n_para;              // number of parameters in the fit function
		target_func.params = &Correlfun3D_data;  // structure with the data and error bars
	
		const gsl_multifit_fdfsolver_type *type_ptr = gsl_multifit_fdfsolver_lmsder;
		gsl_multifit_fdfsolver *solver_ptr
			= gsl_multifit_fdfsolver_alloc (type_ptr, data_length, n_para);
		gsl_multifit_fdfsolver_set (solver_ptr, &target_func, &xvec_ptr.vector);
	
		size_t iteration = 0;         // initialize iteration counter
		if (VERBOSE > 2) print_fit_state_3D (iteration, solver_ptr);
		int status;  		// return value from gsl function calls (e.g., error)
		do
		{
			iteration++;
	      
			// perform a single iteration of the fitting routine
			status = gsl_multifit_fdfsolver_iterate (solver_ptr);
	
			// print out the status of the fit
			if (VERBOSE > 2) cout << "status = " << gsl_strerror (status) << endl;
	
			// customized routine to print out current parameters
			if (VERBOSE > 2) print_fit_state_3D (iteration, solver_ptr);
	
			if (status)    // check for a nonzero status code
			{
				break;  // this should only happen if an error code is returned 
			}
	
			// test for convergence with an absolute and relative error (see manual)
			status = gsl_multifit_test_delta (solver_ptr->dx, solver_ptr->x,
												fit_tolerance, fit_tolerance);
		}
		while (status == GSL_CONTINUE && iteration < fit_max_iterations);
	
		//cerr >> "iteration = " << iteration << endl;
	
		// calculate the covariance matrix of the best-fit parameters
		gsl_multifit_covar (solver_ptr->J, 0.0, covariance_ptr);
	
		// print out the covariance matrix using the gsl function (not elegant!)
		if (VERBOSE > 2) cout << endl << "Covariance matrix: " << endl;
		if (VERBOSE > 2) gsl_matrix_fprintf (stdout, covariance_ptr, "%g");
	
		cout.setf (ios::fixed, ios::floatfield);	// output in fixed format
		cout.precision (5);		                // # of digits in doubles
	
		int width = 7;		// setw width for output
		/*cout << endl << "Best fit results:" << endl;
		cout << "R2o = " << setw (width) << get_fit_results (0, solver_ptr)
			<< " +/- " << setw (width) << get_fit_err (0, covariance_ptr) << endl;
		cout << "R2s      = " << setw (width) << get_fit_results (1, solver_ptr)
			<< " +/- " << setw (width) << get_fit_err (1, covariance_ptr) << endl;
		cout << "R2l      = " << setw (width) << get_fit_results (2, solver_ptr)
			<< " +/- " << setw (width) << get_fit_err (2, covariance_ptr) << endl;
	  
		cout << "R2os      = " << setw (width) << get_fit_results (3, solver_ptr)
			<< " +/- " << setw (width) << get_fit_err (3, covariance_ptr) << endl;
	    
		cout << "status = " << gsl_strerror (status) << endl;
		cout << "--------------------------------------------------------------------" << endl;*/
	
		double chi = gsl_blas_dnrm2(solver_ptr->f);
		double dof = data_length - n_para;
		double c = GSL_MAX_DBL(1, chi/sqrt(dof));
	
		const int iKT_iKphi_idx 		= indexer_KT_Kphi( iKT, iKphi );
		lambda_Correl[iKT_iKphi_idx] 		= 1.0;
		lambda_Correl_err[iKT_iKphi_idx] 	= 0.0;
		R2_out_GF[iKT_iKphi_idx] 			= fabs(get_fit_results(0, solver_ptr))*hbarC*hbarC;
		R2_side_GF[iKT_iKphi_idx] 			= fabs(get_fit_results(1, solver_ptr))*hbarC*hbarC;
		R2_long_GF[iKT_iKphi_idx] 			= fabs(get_fit_results(2, solver_ptr))*hbarC*hbarC;
		R2_outside_GF[iKT_iKphi_idx] 		= get_fit_results(3, solver_ptr)*hbarC*hbarC;
		R2_out_err[iKT_iKphi_idx] 			= c*get_fit_err(0, covariance_ptr)*hbarC*hbarC;
		R2_side_err[iKT_iKphi_idx] 			= c*get_fit_err(1, covariance_ptr)*hbarC*hbarC;
		R2_long_err[iKT_iKphi_idx] 			= c*get_fit_err(2, covariance_ptr)*hbarC*hbarC;
		R2_outside_err[iKT_iKphi_idx] 		= c*get_fit_err(3, covariance_ptr)*hbarC*hbarC;
	
		out << KT_pts[iKT] << "   "
			<< Kphi_pts[iKphi] << "   "
			<< R2_side_GF[iKT_iKphi_idx] << "   "
			<< R2_out_GF[iKT_iKphi_idx] << "   "
			<< R2_outside_GF[iKT_iKphi_idx] << "   "
			<< R2_long_GF[iKT_iKphi_idx] << "   "
			<< 0 << "   " << 0 << endl;
	
		//clean up
		gsl_matrix_free (covariance_ptr);
		gsl_rng_free (rng_ptr);
	
		gsl_multifit_fdfsolver_free (solver_ptr);  // free up the solver
	
		out.close();
	
		return;
	}
	
	void Fit_Correlationfunction3D_withlambda(vector<double> & Correl_3D, int iKT, int iKphi)
	{
		string outfilename = directory + "/HBTradii_GF_grid0_fitLSQ.dat";
		ofstream out(outfilename.c_str(), ios::app);
	
		const size_t data_length = nqxpts*nqypts*nqzpts;  // # of points
		const size_t n_para = 5;  // # of parameters
	
		// allocate space for a covariance matrix of size p by p
		gsl_matrix *covariance_ptr = gsl_matrix_alloc (n_para, n_para);
	
		// allocate and setup for generating gaussian distibuted random numbers
		gsl_rng_env_setup ();
		const gsl_rng_type *type = gsl_rng_default;
		gsl_rng *rng_ptr = gsl_rng_alloc (type);
	
		//set up test data
		struct Correlationfunction3D_data Correlfun3D_data;
		Correlfun3D_data.data_length = data_length;
		Correlfun3D_data.q_o.resize(data_length);
		Correlfun3D_data.q_s.resize(data_length);
		Correlfun3D_data.q_l.resize(data_length);
		Correlfun3D_data.y.resize(data_length);
		Correlfun3D_data.sigma.resize(data_length);
	
		int idx = 0;
		double ckp = cos(Kphi_pts[iKphi]), skp = sin(Kphi_pts[iKphi]);
		for (int i = 0; i < nqxpts; i++)
		for (int j = 0; j < nqypts; j++)
		for (int k = 0; k < nqzpts; k++)
		{
			Correlfun3D_data.q_o[idx] = qx_pts[i] * ckp + qy_pts[j] * skp;
			Correlfun3D_data.q_s[idx] = -qx_pts[i] * skp + qy_pts[j] * ckp;
			Correlfun3D_data.q_l[idx] = qz_pts[k];
			Correlfun3D_data.y[idx] = Correl_3D[idx];
			//Correlfun3D_data.sigma[idx] = Correl_3D_err[i][j][k];
			Correlfun3D_data.sigma[idx] = 1e-3;
	if (i==(nqxpts-1)/2 && j==(nqypts-1)/2 && k==(nqzpts-1)/2)
		Correlfun3D_data.sigma[idx] = 1e10;	//ignore central point
			idx++;
		}
		double para_init[n_para] = { 1.0, 1.0, 1.0, 1.0, 1.0 };  // initial guesses of parameters
	
		gsl_vector_view xvec_ptr = gsl_vector_view_array (para_init, n_para);
	  
		// set up the function to be fit 
		gsl_multifit_function_fdf target_func;
		target_func.f = &Fittarget_correlfun3D_f_withlambda;        // the function of residuals
		target_func.df = &Fittarget_correlfun3D_df_withlambda;      // the gradient of this function
		target_func.fdf = &Fittarget_correlfun3D_fdf_withlambda;    // combined function and gradient
		target_func.n = data_length;              // number of points in the data set
		target_func.p = n_para;              // number of parameters in the fit function
		target_func.params = &Correlfun3D_data;  // structure with the data and error bars
	
		const gsl_multifit_fdfsolver_type *type_ptr = gsl_multifit_fdfsolver_lmsder;
		gsl_multifit_fdfsolver *solver_ptr
			= gsl_multifit_fdfsolver_alloc (type_ptr, data_length, n_para);
		gsl_multifit_fdfsolver_set (solver_ptr, &target_func, &xvec_ptr.vector);
	
		size_t iteration = 0;         // initialize iteration counter
		if (VERBOSE > 2) print_fit_state_3D (iteration, solver_ptr);
		int status;  		// return value from gsl function calls (e.g., error)
		do
		{
			iteration++;
	      
			// perform a single iteration of the fitting routine
			status = gsl_multifit_fdfsolver_iterate (solver_ptr);
	
			// print out the status of the fit
			if (VERBOSE > 2) cout << "status = " << gsl_strerror (status) << endl;
	
			// customized routine to print out current parameters
			if (VERBOSE > 2) print_fit_state_3D (iteration, solver_ptr);
	
			if (status)    // check for a nonzero status code
			{
				break;  // this should only happen if an error code is returned 
			}
	
			// test for convergence with an absolute and relative error (see manual)
			status = gsl_multifit_test_delta (solver_ptr->dx, solver_ptr->x,
												fit_tolerance, fit_tolerance);
		}
		while (status == GSL_CONTINUE && iteration < fit_max_iterations);
	
		// calculate the covariance matrix of the best-fit parameters
		gsl_multifit_covar (solver_ptr->J, 0.0, covariance_ptr);
	
		// print out the covariance matrix using the gsl function (not elegant!)
		if (VERBOSE > 2) cout << endl << "Covariance matrix: " << endl;
		if (VERBOSE > 2) gsl_matrix_fprintf (stdout, covariance_ptr, "%g");
	
		cout.setf (ios::fixed, ios::floatfield);	// output in fixed format
		cout.precision (5);		                // # of digits in doubles
	
		double chi = gsl_blas_dnrm2(solver_ptr->f);
		double dof = data_length - n_para;
		double c = GSL_MAX_DBL(1, chi/sqrt(dof));
	
		int width = 7;		// setw width for output
		/*cout << endl << "Best fit results:" << endl;
		cout << "KT = " << KT_pts[iKT] << endl;
		cout << "Kphi = " << Kphi_pts[iKphi] << endl;
		cout << "lambda      = " << setw (width)
			<< get_fit_results (0, solver_ptr)
			<< " +/- " << setw (width)
			<< c*get_fit_err (0, covariance_ptr) << endl;
		cout << "R2o = " << setw (width)
			<< get_fit_results (1, solver_ptr)*hbarC*hbarC
			<< " +/- " << setw (width)
			<< c*get_fit_err (1, covariance_ptr)*hbarC*hbarC << endl;
		cout << "R2s      = " << setw (width)
			<< get_fit_results (2, solver_ptr)*hbarC*hbarC
			<< " +/- " << setw (width)
			<< c*get_fit_err (2, covariance_ptr)*hbarC*hbarC << endl;
		cout << "R2l      = " << setw (width)
			<< get_fit_results (3, solver_ptr)*hbarC*hbarC
			<< " +/- " << setw (width)
			<< c*get_fit_err (3, covariance_ptr)*hbarC*hbarC << endl;
	  
		cout << "R2os      = " << setw (width)
			<< get_fit_results (4, solver_ptr)*hbarC*hbarC
			<< " +/- " << setw (width)
			<< c*get_fit_err (4, covariance_ptr)*hbarC*hbarC << endl;
	    
		cout << "status = " << gsl_strerror (status) << endl;
		cout << "--------------------------------------------------------------------" << endl;*/
	
		const int iKT_iKphi_idx 			= indexer_KT_Kphi( iKT, iKphi );
		lambda_Correl[iKT_iKphi_idx] 		= get_fit_results(0, solver_ptr);
		lambda_Correl_err[iKT_iKphi_idx] 	= c*get_fit_err(0, covariance_ptr);
		R2_out_GF[iKT_iKphi_idx] 			= fabs(get_fit_results(1, solver_ptr))*hbarC*hbarC;
		R2_side_GF[iKT_iKphi_idx] 			= fabs(get_fit_results(2, solver_ptr))*hbarC*hbarC;
		R2_long_GF[iKT_iKphi_idx] 			= fabs(get_fit_results(3, solver_ptr))*hbarC*hbarC;
		R2_outside_GF[iKT_iKphi_idx] 		= get_fit_results(4, solver_ptr)*hbarC*hbarC;
		R2_out_err[iKT_iKphi_idx] 			= c*get_fit_err(1, covariance_ptr)*hbarC*hbarC;
		R2_side_err[iKT_iKphi_idx] 			= c*get_fit_err(2, covariance_ptr)*hbarC*hbarC;
		R2_long_err[iKT_iKphi_idx] 			= c*get_fit_err(3, covariance_ptr)*hbarC*hbarC;
		R2_outside_err[iKT_iKphi_idx] 		= c*get_fit_err(4, covariance_ptr)*hbarC*hbarC;
	
		out << KT_pts[iKT] << "   "
			<< Kphi_pts[iKphi] << "   "
			<< R2_side_GF[iKT_iKphi_idx] << "   "
			<< R2_out_GF[iKT_iKphi_idx] << "   "
			<< R2_outside_GF[iKT_iKphi_idx] << "   "
			<< R2_long_GF[iKT_iKphi_idx] << "   "
			<< 0 << "   " << 0 << endl;
	
		//clean up
		gsl_matrix_free (covariance_ptr);
		gsl_rng_free (rng_ptr);
	
		gsl_multifit_fdfsolver_free (solver_ptr);  // free up the solver
	
		out.close();
	
		return;
	}
	
	//*********************************************************************
	// 3D case
	//*********************************************************************
	//  Simple function to print results of each iteration in nice format
	int print_fit_state_3D (size_t iteration, gsl_multifit_fdfsolver * solver_ptr)
	{
		cout.setf (ios::fixed, ios::floatfield);	// output in fixed format
		cout.precision (5);		// digits in doubles
	
		int width = 15;		// setw width for output
		cout << scientific
			<< "iteration " << iteration << ": "
			<< "  x = {" << setw (width) << gsl_vector_get (solver_ptr->x, 0)
			<< setw (width) << gsl_vector_get (solver_ptr->x, 1)
			<< setw (width) << gsl_vector_get (solver_ptr->x, 2)
			<< setw (width) << gsl_vector_get (solver_ptr->x, 3)
			<< "}, |f(x)| = " << scientific << gsl_blas_dnrm2 (solver_ptr->f) 
			<< endl << endl;
	
		return 0;
	}
	//  Simple function to print results of each iteration in nice format
	int print_fit_state_3D_withlambda (size_t iteration, gsl_multifit_fdfsolver * solver_ptr)
	{
		cout.setf (ios::fixed, ios::floatfield);	// output in fixed format
		cout.precision (5);		// digits in doubles
	
		int width = 15;		// setw width for output
		cout << scientific
			<< "iteration " << iteration << ": "
			<< "  x = {" << setw (width) << gsl_vector_get (solver_ptr->x, 0)
			<< setw (width) << gsl_vector_get (solver_ptr->x, 1)
			<< setw (width) << gsl_vector_get (solver_ptr->x, 2)
			<< setw (width) << gsl_vector_get (solver_ptr->x, 3)
			<< setw (width) << gsl_vector_get (solver_ptr->x, 4)
			<< "}, |f(x)| = " << scientific << gsl_blas_dnrm2 (solver_ptr->f) 
			<< endl << endl;
	
		return 0;
	}
	
	void find_minimum_chisq_correlationfunction_full(vector<double> & Correl_3D, int iKT, int iKphi)
	{
		string outfilename = directory + "/HBTradii_GF_grid0_fitLog.dat";
		ofstream out(outfilename.c_str(), ios::app);
	
		const size_t data_length = nqxpts*nqypts*nqzpts;  // # of points
	
	    double lambda, R_o, R_s, R_l, R_os;
	    int dim = 5;
	    int s_gsl;
	
	    double *V = new double [dim];
	    double *qweight = new double [dim];
	    double **T = new double* [dim];
	    for(int i = 0; i < dim; i++)
	    {
	        V[i] = 0.0;
	        T[i] = new double [dim];
	        for(int j = 0; j < dim; j++)
	            T[i][j] = 0.0;
	    }
	
	    gsl_matrix * T_gsl = gsl_matrix_alloc (dim, dim);
	    gsl_matrix * T_inverse_gsl = gsl_matrix_alloc (dim, dim);
	    gsl_permutation * perm = gsl_permutation_alloc (dim);
	
		double ckp = cos(Kphi_pts[iKphi]), skp = sin(Kphi_pts[iKphi]);
		double CF_err = 1e-3;
		for (int i = 0; i < nqxpts; i++)
		for (int j = 0; j < nqypts; j++)
		for (int k = 0; k < nqzpts; k++)
	    {
	        double q_out_local = qx_pts[i] * ckp + qy_pts[j] * skp;
	        double q_side_local = -qx_pts[i] * skp + qy_pts[j] * ckp;
	        double q_long_local = qz_pts[k];
	        double correl_local = Correl_3D[indexer_qx_qy_qz(i,j,k)]-1.0;
			//cout << "CHECK CF3D: " << qx_pts[i] << "   " << qy_pts[j] << "   " << qz_pts[k] << "   "
			//	<< q_out_local << "	" << q_side_local << "	 " << q_long_local << "	  "
			//	<< correl_local << endl;
	        if(correl_local < 1e-15) continue;
			//if (i==(nqxpts-1)/2 && j==(nqypts-1)/2 && k==(nqzpts-1)/2)
			//	Correlfun3D_data.sigma[idx] = 1.e10;	//ignore central point
	        double sigma_k_prime = CF_err/correl_local;
	            
	        double inv_sigma_k_prime_sq = 1./(sigma_k_prime*sigma_k_prime);
	        double log_correl_over_sigma_sq = log(correl_local)*inv_sigma_k_prime_sq;
	
	        qweight[0] = - 1.0;
	        qweight[1] = q_out_local*q_out_local;
	        qweight[2] = q_side_local*q_side_local;
	        qweight[3] = q_long_local*q_long_local;
	        qweight[4] = q_out_local*q_side_local;
	
	        for(int ij = 0; ij < dim; ij++)
	        {
	            V[ij] += qweight[ij]*log_correl_over_sigma_sq;
	            T[0][ij] += qweight[ij]*inv_sigma_k_prime_sq;
	        }
	
	        for(int ij = 1; ij < dim; ij++)
	            T[ij][0] = T[0][ij];
	            
	
	        for(int ij = 1; ij < dim; ij++)
	        {
	            for(int lm = 1; lm < dim; lm++)
	                T[ij][lm] += -qweight[ij]*qweight[lm]*inv_sigma_k_prime_sq;
	        }
	    }
	    for(int i = 0; i < dim; i++)
	        for(int j = 0; j < dim; j++)
	            gsl_matrix_set(T_gsl, i, j, T[i][j]);
	
	    // Make LU decomposition of matrix T_gsl
	    gsl_linalg_LU_decomp (T_gsl, perm, &s_gsl);
	    // Invert the matrix m
	    gsl_linalg_LU_invert (T_gsl, perm, T_inverse_gsl);
	
	    double **T_inverse = new double* [dim];
	    for(int i = 0; i < dim; i++)
	    {
	        T_inverse[i] = new double [dim];
	        for(int j = 0; j < dim; j++)
	            T_inverse[i][j] = gsl_matrix_get(T_inverse_gsl, i, j);
	    }
	    double *results = new double [dim];
	    for(int i = 0; i < dim; i++)
	    {
	        results[i] = 0.0;
	        for(int j = 0; j < dim; j++)
	            results[i] += T_inverse[i][j]*V[j];
	    }
	
	    lambda = exp(results[0]);
	    R_o = sqrt(results[1])*hbarC;
	    R_s = sqrt(results[2])*hbarC;
	    R_l = sqrt(results[3])*hbarC;
	    // the cross term is not necessary positive
	    double R2_os = results[4]*hbarC*hbarC;
	    /*cout << "lambda = " << lambda << endl;
	    cout << "R2_o = " << R_o*R_o << " fm^2, R2_s = " << R_s*R_s
			 << " fm^2, R2_l = " << R_l*R_l << " fm^2" << endl;
	    cout << "R2_os = " << R2_os << " fm^2." << endl;*/
	
		const int iKT_iKphi_idx 			= indexer_KT_Kphi( iKT, iKphi );
		lambda_Correl[iKT_iKphi_idx] 		= lambda;
		R2_out_GF[iKT_iKphi_idx] 			= R_o*R_o;
		R2_side_GF[iKT_iKphi_idx] 			= R_s*R_s;
		R2_long_GF[iKT_iKphi_idx] 			= R_l*R_l;
		R2_outside_GF[iKT_iKphi_idx] 		= R2_os;
	
		out << KT_pts[iKT] << "   "
			<< Kphi_pts[iKphi] << "   "
			<< R2_side_GF[iKT_iKphi_idx] << "   "
			<< R2_out_GF[iKT_iKphi_idx] << "   "
			<< R2_outside_GF[iKT_iKphi_idx] << "   "
			<< R2_long_GF[iKT_iKphi_idx] << "   "
			<< 0 << "   " << 0 << endl;
	
	    double chi_sq = 0.0;
		for (int i = 0; i < nqxpts; i++)
		for (int j = 0; j < nqypts; j++)
		for (int k = 0; k < nqzpts; k++)
	    {
	        double q_out_local = qx_pts[i] * ckp + qy_pts[j] * skp;
	        double q_side_local = -qx_pts[i] * skp + qy_pts[j] * ckp;
	        double q_long_local = qz_pts[k];
	        double correl_local = Correl_3D[indexer_qx_qy_qz(i,j,k)]-1.0;
	        if(correl_local < 1e-15) continue;
	        double sigma_k_prime = CF_err/correl_local;
	
	        chi_sq += pow((log(correl_local) - results[0] 
	                       + results[1]*q_out_local*q_out_local 
	                       + results[2]*q_side_local*q_side_local
	                       + results[3]*q_long_local*q_long_local
	                       + results[4]*q_out_local*q_side_local), 2)
	                  /sigma_k_prime/sigma_k_prime;
	    }
	    //cout << "chi_sq/d.o.f = " << chi_sq/(qnpts - dim) << endl;
	    //chi_sq_per_dof = chi_sq/(qnpts - dim);
	
	    // clean up
	    gsl_matrix_free (T_gsl);
	    gsl_matrix_free (T_inverse_gsl);
	    gsl_permutation_free (perm);
	
	    delete [] qweight;
	    delete [] V;
	    for(int i = 0; i < dim; i++)
	    {
	        delete [] T[i];
	        delete [] T_inverse[i];
	    }
	    delete [] T;
	    delete [] T_inverse;
	    delete [] results;
	
		out.close();
	}
	
	//*********************************************************************
	//  Function returning the residuals for each point; that is, the 
	//  difference of the fit function using the current parameters
	//  and the data to be fit.
	int Fittarget_correlfun3D_f (const gsl_vector *xvec_ptr, void *params_ptr, gsl_vector *f_ptr)
	{
		size_t n = ((struct Correlationfunction3D_data *) params_ptr)->data_length;
		vector<double> q_o = ((struct Correlationfunction3D_data *) params_ptr)->q_o;
		vector<double> q_s = ((struct Correlationfunction3D_data *) params_ptr)->q_s;
		vector<double> q_l = ((struct Correlationfunction3D_data *) params_ptr)->q_l;
		vector<double> y = ((struct Correlationfunction3D_data *) params_ptr)->y;
		vector<double> sigma = ((struct Correlationfunction3D_data *) params_ptr)->sigma;
	
		//fit parameters
		double R2_o = gsl_vector_get (xvec_ptr, 0);
		double R2_s = gsl_vector_get (xvec_ptr, 1);
		double R2_l = gsl_vector_get (xvec_ptr, 2);
		double R2_os = gsl_vector_get (xvec_ptr, 3);
	
		size_t i;
	
		for (i = 0; i < n; i++)
		{
			double Yi = 1.0 + exp(- q_l[i]*q_l[i]*R2_l - q_s[i]*q_s[i]*R2_s
								  - q_o[i]*q_o[i]*R2_o - 2.*q_o[i]*q_s[i]*R2_os);
			gsl_vector_set (f_ptr, i, (Yi - y[i]) / sigma[i]);
	//cout << "i = " << i << ": " << y[i] << "   " << Yi << "   " << (Yi - y[i]) / sigma[i]
	//		<< "   " << q_o[i] << "   " << q_s[i] << "   " << q_l[i] << "   "
	//		<< R2_o << "   " << R2_s << "   " << R2_l << "   " << R2_os << endl;
		}
	
		return GSL_SUCCESS;
	}
	
	int Fittarget_correlfun3D_f_withlambda (const gsl_vector *xvec_ptr, void *params_ptr,
											gsl_vector *f_ptr)
	{
		size_t n = ((struct Correlationfunction3D_data *) params_ptr)->data_length;
		vector<double> q_o = ((struct Correlationfunction3D_data *) params_ptr)->q_o;
		vector<double> q_s = ((struct Correlationfunction3D_data *) params_ptr)->q_s;
		vector<double> q_l = ((struct Correlationfunction3D_data *) params_ptr)->q_l;
		vector<double> y = ((struct Correlationfunction3D_data *) params_ptr)->y;
		vector<double> sigma = ((struct Correlationfunction3D_data *) params_ptr)->sigma;
	
		//fit parameters
		double lambda = gsl_vector_get (xvec_ptr, 0);
		double R2_o = gsl_vector_get (xvec_ptr, 1);
		double R2_s = gsl_vector_get (xvec_ptr, 2);
		double R2_l = gsl_vector_get (xvec_ptr, 3);
		double R2_os = gsl_vector_get (xvec_ptr, 4);
	
		size_t i;
	
		for (i = 0; i < n; i++)
		{
			//double Yi = lambda*exp(- q_l[i]*q_l[i]*R_l*R_l - q_s[i]*q_s[i]*R_s*R_s
			//             - q_o[i]*q_o[i]*R_o*R_o - q_o[i]*q_s[i]*R_os*R_os);
			double Yi = 1.0 + lambda*exp(- q_l[i]*q_l[i]*R2_l - q_s[i]*q_s[i]*R2_s
										 - q_o[i]*q_o[i]*R2_o - 2.*q_o[i]*q_s[i]*R2_os);
			gsl_vector_set (f_ptr, i, (Yi - y[i]) / sigma[i]);
	//cout << "i = " << i << ": " << y[i] << "   " << Yi << "   "
	//		<< (Yi - y[i]) / sigma[i] << "   " << lambda
	//		<< "   " << q_o[i] << "   " << q_s[i] << "   " << q_l[i] << "   "
	//		<< R2_o << "   " << R2_s << "   " << R2_l << "   " << R2_os << endl;
		}
	
		return GSL_SUCCESS;
	}
	
	//*********************************************************************
	//  Function returning the Jacobian of the residual function
	int Fittarget_correlfun3D_df (const gsl_vector *xvec_ptr, void *params_ptr,
									gsl_matrix *Jacobian_ptr)
	{
		size_t n = ((struct Correlationfunction3D_data *) params_ptr)->data_length;
		vector<double> q_o = ((struct Correlationfunction3D_data *) params_ptr)->q_o;
		vector<double> q_s = ((struct Correlationfunction3D_data *) params_ptr)->q_s;
		vector<double> q_l = ((struct Correlationfunction3D_data *) params_ptr)->q_l;
		vector<double> sigma = ((struct Correlationfunction3D_data *) params_ptr)->sigma;
	
		//fit parameters
		double R2_o = gsl_vector_get (xvec_ptr, 0);
		double R2_s = gsl_vector_get (xvec_ptr, 1);
		double R2_l = gsl_vector_get (xvec_ptr, 2);
		double R2_os = gsl_vector_get (xvec_ptr, 3);
	
		size_t i;
	
		for (i = 0; i < n; i++)
		{
			double sig = sigma[i];
	
			// derivatives
			// double common_elemt = exp(- q_l[i]*q_l[i]*R_l*R_l
			//							- q_s[i]*q_s[i]*R_s*R_s
			//							- q_o[i]*q_o[i]*R_o*R_o
			//							- q_o[i]*q_s[i]*R_os*R_os);
			double common_elemt = exp(- q_l[i]*q_l[i]*R2_l - q_s[i]*q_s[i]*R2_s 
									  - q_o[i]*q_o[i]*R2_o - 2.*q_o[i]*q_s[i]*R2_os);
	      
			gsl_matrix_set (Jacobian_ptr, i, 0, - q_o[i]*q_o[i]*common_elemt/sig);
			gsl_matrix_set (Jacobian_ptr, i, 1, - q_s[i]*q_s[i]*common_elemt/sig);
			gsl_matrix_set (Jacobian_ptr, i, 2, - q_l[i]*q_l[i]*common_elemt/sig);
			gsl_matrix_set (Jacobian_ptr, i, 3, - 2.*q_o[i]*q_s[i]*common_elemt/sig);
		}
	
		return GSL_SUCCESS;
	}
	
	int Fittarget_correlfun3D_df_withlambda( const gsl_vector *xvec_ptr, void *params_ptr,
											 gsl_matrix *Jacobian_ptr)
	{
		size_t n = ((struct Correlationfunction3D_data *) params_ptr)->data_length;
		vector<double> q_o = ((struct Correlationfunction3D_data *) params_ptr)->q_o;
		vector<double> q_s = ((struct Correlationfunction3D_data *) params_ptr)->q_s;
		vector<double> q_l = ((struct Correlationfunction3D_data *) params_ptr)->q_l;
		vector<double> sigma = ((struct Correlationfunction3D_data *) params_ptr)->sigma;
	
		//fit parameters
		double lambda = gsl_vector_get (xvec_ptr, 0);
		double R2_o = gsl_vector_get (xvec_ptr, 1);
		double R2_s = gsl_vector_get (xvec_ptr, 2);
		double R2_l = gsl_vector_get (xvec_ptr, 3);
		double R2_os = gsl_vector_get (xvec_ptr, 4);
	
		size_t i;
	
		for (i = 0; i < n; i++)
		{
			double sig = sigma[i];
	
			//derivatives
			double common_elemt = exp(- q_l[i]*q_l[i]*R2_l - q_s[i]*q_s[i]*R2_s
									  - q_o[i]*q_o[i]*R2_o - 2.*q_o[i]*q_s[i]*R2_os);
	      
			gsl_matrix_set (Jacobian_ptr, i, 0, common_elemt/sig);
			gsl_matrix_set (Jacobian_ptr, i, 1, - lambda*q_o[i]*q_o[i]*common_elemt/sig);
			gsl_matrix_set (Jacobian_ptr, i, 2, - lambda*q_s[i]*q_s[i]*common_elemt/sig);
			gsl_matrix_set (Jacobian_ptr, i, 3, - lambda*q_l[i]*q_l[i]*common_elemt/sig);
			gsl_matrix_set (Jacobian_ptr, i, 4, - 2.*lambda*q_o[i]*q_s[i]*common_elemt/sig);
		}
	
		return GSL_SUCCESS;
	}
	
	//*********************************************************************
	//  Function combining the residual function and its Jacobian
	int Fittarget_correlfun3D_fdf( const gsl_vector* xvec_ptr, void *params_ptr,
								   gsl_vector* f_ptr, gsl_matrix* Jacobian_ptr )
	{
		Fittarget_correlfun3D_f(xvec_ptr, params_ptr, f_ptr);
		Fittarget_correlfun3D_df(xvec_ptr, params_ptr, Jacobian_ptr);
	
		return GSL_SUCCESS;
	}
	
	int Fittarget_correlfun3D_fdf_withlambda( const gsl_vector* xvec_ptr,
											  void *params_ptr, gsl_vector* f_ptr,
											  gsl_matrix* Jacobian_ptr )
	{
		Fittarget_correlfun3D_f_withlambda(xvec_ptr, params_ptr, f_ptr);
		Fittarget_correlfun3D_df_withlambda(xvec_ptr, params_ptr, Jacobian_ptr);
	
		return GSL_SUCCESS;
	}
	
	//Fourier transform of HBT radii once they're calculated
	void R2_Fourier_transform(int jKT, double plane_psi)
	{
		string modeString = ( USE_LOG_FIT ) ? "Log" : "LSQ";
		string outfilename = directory + "/HBTradii_GF_cfs_fit" + modeString + ".dat";
		ofstream out(outfilename.c_str(), ios::app);
	
		double * array_KT_pts   = new double [n_KT_pts];
		double * array_Kphi_pts = new double [n_Kphi_pts];
		double * K_phi          = new double [nKphi];
		double * Kphi_wts       = new double [nKphi];
	
		gauss_quadrature(nKphi, 1, 0.0, 0.0, 0.0, 2.0*M_PI, K_phi, Kphi_wts);
	
		for (int iKT = 0; iKT < n_KT_pts; ++iKT)
			array_KT_pts[iKT] = KT_pts[iKT];
	
		for (int iKphi = 0; iKphi < n_Kphi_pts; ++iKphi)
			array_Kphi_pts[iKphi] = Kphi_pts[iKphi];
	
		double ** arr_R2_side_GF    = new double * [n_KT_pts];
		double ** arr_R2_out_GF     = new double * [n_KT_pts];
		double ** arr_R2_long_GF    = new double * [n_KT_pts];
		double ** arr_R2_outside_GF = new double * [n_KT_pts];
		for (int iKT = 0; iKT < n_KT_pts; ++iKT)
		{
			arr_R2_side_GF[iKT]     = new double [n_Kphi_pts];
			arr_R2_out_GF[iKT]      = new double [n_Kphi_pts];
			arr_R2_long_GF[iKT]     = new double [n_Kphi_pts];
			arr_R2_outside_GF[iKT]  = new double [n_Kphi_pts];
			for (int iKphi = 0; iKphi < n_Kphi_pts; ++iKphi)
			{
				arr_R2_side_GF[iKT][iKphi]    = R2_side_GF[indexer_KT_Kphi( iKT, iKphi )];
				arr_R2_out_GF[iKT][iKphi]     = R2_out_GF[indexer_KT_Kphi( iKT, iKphi )];
				arr_R2_long_GF[iKT][iKphi]    = R2_long_GF[indexer_KT_Kphi( iKT, iKphi )];
				arr_R2_outside_GF[iKT][iKphi] = R2_outside_GF[indexer_KT_Kphi( iKT, iKphi )];
			}
		}
	
		double * K_T = new double [nKT];
		double dK_T = (KT_max - KT_min)/(nKT - 1 + 1e-100);
		for (int i = 0; i < nKT; ++i)
			K_T[i] = KT_min + i*dK_T;
	
	
		const int interpMode = 1;
		for (int Morder = 0; Morder < n_order; ++Morder)
		{
			double cos_mKphi_pts[nKphi], sin_mKphi_pts[nKphi];
	
			for (int iKphi = 0; iKphi < nKphi; ++iKphi)
			{
				cos_mKphi_pts[iKphi] = cos(Morder*(K_phi[iKphi] - plane_psi));
				sin_mKphi_pts[iKphi] = sin(Morder*(K_phi[iKphi] - plane_psi));
			}
	
			double temp_sum_side_cos = 0.0,     temp_sum_side_sin = 0.0;
			double temp_sum_out_cos = 0.0,      temp_sum_out_sin = 0.0;
			double temp_sum_outside_cos = 0.0,  temp_sum_outside_sin = 0.0;
			double temp_sum_long_cos = 0.0,     temp_sum_long_sin = 0.0;
			double temp_sum_sidelong_cos = 0.0, temp_sum_sidelong_sin = 0.0;
			double temp_sum_outlong_cos = 0.0,  temp_sum_outlong_sin = 0.0;
	
			for (int iKphi = 0; iKphi < nKphi; ++iKphi)
			{
				double local_R2s = interpolate2D(array_KT_pts, array_Kphi_pts, arr_R2_side_GF,
												 K_T[jKT], K_phi[iKphi], n_KT_pts, n_Kphi_pts,
												 interpMode, false, true);
				double local_R2o = interpolate2D(array_KT_pts, array_Kphi_pts, arr_R2_out_GF,
												 K_T[jKT], K_phi[iKphi], n_KT_pts, n_Kphi_pts,
												 interpMode, false, true);
				double local_R2os = interpolate2D(array_KT_pts, array_Kphi_pts, arr_R2_outside_GF,
												 K_T[jKT], K_phi[iKphi], n_KT_pts, n_Kphi_pts,
												 interpMode, false, true);
				double local_R2l = interpolate2D(array_KT_pts, array_Kphi_pts, arr_R2_long_GF,
												 K_T[jKT], K_phi[iKphi], n_KT_pts, n_Kphi_pts,
												 interpMode, false, true);
	
				temp_sum_side_cos    += local_R2s*cos_mKphi_pts[iKphi]*Kphi_wts[iKphi];
				temp_sum_side_sin    += local_R2s*sin_mKphi_pts[iKphi]*Kphi_wts[iKphi];
				temp_sum_out_cos     += local_R2o*cos_mKphi_pts[iKphi]*Kphi_wts[iKphi];
				temp_sum_out_sin     += local_R2o*sin_mKphi_pts[iKphi]*Kphi_wts[iKphi];
				temp_sum_outside_cos += local_R2os*cos_mKphi_pts[iKphi]*Kphi_wts[iKphi];
				temp_sum_outside_sin += local_R2os*sin_mKphi_pts[iKphi]*Kphi_wts[iKphi];
				temp_sum_long_cos    += local_R2l*cos_mKphi_pts[iKphi]*Kphi_wts[iKphi];
				temp_sum_long_sin    += local_R2l*sin_mKphi_pts[iKphi]*Kphi_wts[iKphi];
			}
	
			R2_side_GF_C[jKT*n_order + Morder]    = temp_sum_side_cos/(2.*M_PI);
			R2_side_GF_S[jKT*n_order + Morder]    = temp_sum_side_sin/(2.*M_PI);
			R2_out_GF_C[jKT*n_order + Morder]     = temp_sum_out_cos/(2.*M_PI);
			R2_out_GF_S[jKT*n_order + Morder]     = temp_sum_out_sin/(2.*M_PI);
			R2_outside_GF_C[jKT*n_order + Morder] = temp_sum_outside_cos/(2.*M_PI);
			R2_outside_GF_S[jKT*n_order + Morder] = temp_sum_outside_sin/(2.*M_PI);
			R2_long_GF_C[jKT*n_order + Morder]    = temp_sum_long_cos/(2.*M_PI);
			R2_long_GF_S[jKT*n_order + Morder]    = temp_sum_long_sin/(2.*M_PI);
	
	
			out << K_T[jKT] << "   "
				<< Morder << "   "
				<< R2_side_GF_C[jKT*n_order + Morder] << "   "
				<< R2_side_GF_S[jKT*n_order + Morder] << "   "
				<< R2_out_GF_C[jKT*n_order + Morder] << "   "
				<< R2_out_GF_S[jKT*n_order + Morder] << "   "
				<< R2_outside_GF_C[jKT*n_order + Morder] << "   "
				<< R2_outside_GF_S[jKT*n_order + Morder] << "   "
				<< R2_long_GF_C[jKT*n_order + Morder] << "   "
				<< R2_long_GF_S[jKT*n_order + Morder] << "   "
				<< 0 << "   " << 0 << "   " << 0 << "   " << 0 << endl;
				//R2sl==R2ol==0 for boost-invariant
	
	
		}
	
		for (int iKT = 0; iKT < n_KT_pts; ++iKT)
		{
			delete [] arr_R2_side_GF[iKT];
			delete [] arr_R2_out_GF[iKT];
			delete [] arr_R2_long_GF[iKT];
			delete [] arr_R2_outside_GF[iKT];
		}
	
		delete [] arr_R2_side_GF;
		delete [] arr_R2_out_GF;
		delete [] arr_R2_long_GF;
		delete [] arr_R2_outside_GF;
	
		delete [] array_KT_pts;
		delete [] array_Kphi_pts;
		delete [] K_T;
		delete [] K_phi;
		delete [] Kphi_wts;
	
		out.close();
	
		return;
	}

}

//End of file

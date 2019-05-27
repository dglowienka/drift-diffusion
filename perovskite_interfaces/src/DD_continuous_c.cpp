#include "../include/drift_diffusion.h"

void Drift_diffusion::concentration_c(void)
{
	int N = grid_scaled.N_points_scaled; // Number of points with boundaries
	double dt_scaled = layers_params_scaled.device.dt_scaled;

	double beta_c;
	vector<double> cit_b, cit_alpha, cit_beta, cit_gamma; // alpha -> i-1, beta -> i, gamma -> i+1
	vector<double> tmp_c_scaled;

	vector<double> V_it1_scaled = result_it1_scaled.V_scaled;
	vector<double> c_it_scaled = result_it_scaled.c_scaled;
	vector<double> hi_scaled = grid_scaled.hi_scaled;
	vector<double> u_ci_scaled = grid_scaled.u_c_scaled;

	//Fill the matrix A and vector b with diagonal data, A matrix from right and b vector from left part of equation
	//CHANGE!!

	for (int i = 0; i < N; i++)
	{
		beta_c = -dt_scaled / ((hi_scaled[i] + hi_scaled[i - 1]) / 2);

		// Middle (no boundary around)
		if (i > 0 && i < N-1)
		{
			cit_gamma.push_back( beta_c * ((u_ci_scaled[i] + u_ci_scaled[i + 1]) / (2*hi_scaled[i])) * B(V_it1_scaled[i] - V_it1_scaled[i + 1]) ); // cit(i+1,t+1)
			cit_beta.push_back( -beta_c*(((u_ci_scaled[i] + u_ci_scaled[i + 1]) / (2*hi_scaled[i])) * B(V_it1_scaled[i + 1] - V_it1_scaled[i])
				+ ((u_ci_scaled[i - 1] + u_ci_scaled[i]) / (2*hi_scaled[i - 1])) * B(V_it1_scaled[i - 1] - V_it1_scaled[i])) + 1 ); // cit(i,t+1) 
			cit_alpha.push_back( beta_c * ((u_ci_scaled[i - 1] + u_ci_scaled[i]) / (2*hi_scaled[i - 1])) * B(V_it1_scaled[i] - V_it1_scaled[i - 1]) ); // cit(i-1,t+1)
			cit_b.push_back( c_it_scaled[i] );
		}

		// Bottom
		if (i == 0)
		{
			cit_gamma.push_back( beta_c * ((u_ci_scaled[i] + u_ci_scaled[i + 1]) / (2*hi_scaled[i])) * B(V_it1_scaled[i] - V_it1_scaled[i + 1]) ); // cit(i+1,t+1)
			cit_beta.push_back( -beta_c * ((u_ci_scaled[i] + u_ci_scaled[i + 1]) / (2*hi_scaled[i])) * B(V_it1_scaled[i + 1] - V_it1_scaled[i]) + 1 ); // cit(i,t+1) 
			cit_b.push_back( c_it_scaled[i] );
		}

		// Top
		if (i == N-1)
		{
			cit_beta.push_back( -beta_c * ((u_ci_scaled[i - 1] + u_ci_scaled[i]) / (2*hi_scaled[i - 1])) * B(V_it1_scaled[i - 1] - V_it1_scaled[i]) + 1 ); // cit(i,t+1) 
			cit_alpha.push_back( beta_c * ((u_ci_scaled[i - 1] + u_ci_scaled[i]) / (2*hi_scaled[i - 1])) * B(V_it1_scaled[i] - V_it1_scaled[i - 1]) ); // cit(i-1,t+1)
			cit_b.push_back( c_it_scaled[i] );
		}
	}

	// Vector x from Ax=b
	tmp_c_scaled = thomson_algorithm(cit_alpha, cit_beta, cit_gamma, cit_b, N);

	// Put result to result vector in t+1 position
	result_it1_scaled.c_scaled = tmp_c_scaled;
}
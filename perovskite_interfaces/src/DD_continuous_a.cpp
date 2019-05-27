#include "../include/drift_diffusion.h"

void Drift_diffusion::concentration_a(void)
{
	int N = grid_scaled.N_points_scaled; // Number of points with boundaries
	double dt_scaled = layers_params_scaled.device.dt_scaled;

	double beta_a;
	vector<double> ait_b, ait_alpha, ait_beta, ait_gamma; // alpha -> i-1, beta -> i, gamma -> i+1
	vector<double> tmp_a_scaled;

	vector<double> V_it1_scaled = result_it1_scaled.V_scaled;
	vector<double> a_it_scaled = result_it_scaled.a_scaled;
	vector<double> hi_scaled = grid_scaled.hi_scaled;
	vector<double> u_ai_scaled = grid_scaled.u_a_scaled;

	//Fill the matrix A and vector b with diagonal data, A matrix from right and b vector from left part of equation
	//CHANGE!!

	for (int i = 0; i < N; i++)
	{
		beta_a = -dt_scaled / ((hi_scaled[i] + hi_scaled[i - 1]) / 2);

		// Middle (no boundary around)
		if (i > 0 && i < N-1)
		{
			ait_gamma.push_back( beta_a * ((u_ai_scaled[i] + u_ai_scaled[i + 1]) / (2*hi_scaled[i])) * B(V_it1_scaled[i + 1] - V_it1_scaled[i]) ); // ait(i+1,t+1)
			ait_beta.push_back( -beta_a*(((u_ai_scaled[i] + u_ai_scaled[i + 1]) / (2*hi_scaled[i])) * B(V_it1_scaled[i] - V_it1_scaled[i + 1])
				+ ((u_ai_scaled[i - 1] + u_ai_scaled[i]) / (2*hi_scaled[i - 1])) * B(V_it1_scaled[i] - V_it1_scaled[i - 1])) + 1 ); // ait(i,t+1)
			ait_alpha.push_back( beta_a * ((u_ai_scaled[i - 1] + u_ai_scaled[i]) / (2*hi_scaled[i - 1])) * B(V_it1_scaled[i - 1] - V_it1_scaled[i]) ); // ait(i-1,t+1)
			ait_b.push_back( a_it_scaled[i] );
		}

		// Bottom
		if (i == 0)
		{
			ait_gamma.push_back( beta_a * ((u_ai_scaled[i] + u_ai_scaled[i + 1]) / (2*hi_scaled[i])) * B(V_it1_scaled[i + 1] - V_it1_scaled[i]) ); // ait(i+1,t+1)
			ait_beta.push_back( -beta_a * ((u_ai_scaled[i] + u_ai_scaled[i + 1]) / (2*hi_scaled[i])) * B(V_it1_scaled[i] - V_it1_scaled[i + 1]) + 1 ); // ait(i,t+1)
			ait_b.push_back( a_it_scaled[i] );
		}

		// Top
		if (i == N-1)
		{
			ait_beta.push_back( -beta_a * ((u_ai_scaled[i - 1] + u_ai_scaled[i]) / (2*hi_scaled[i - 1])) * B(V_it1_scaled[i] - V_it1_scaled[i - 1]) + 1 ); // ait(i,t+1)
			ait_alpha.push_back( beta_a * ((u_ai_scaled[i - 1] + u_ai_scaled[i]) / (2*hi_scaled[i - 1])) * B(V_it1_scaled[i - 1] - V_it1_scaled[i]) ); // ait(i-1,t+1)
			ait_b.push_back( a_it_scaled[i] );
		}
	}

	// Vector x from Ax=b
	tmp_a_scaled = thomson_algorithm(ait_alpha, ait_beta, ait_gamma, ait_b, N);

	// Put result to result vector in t+1 position
	result_it1_scaled.a_scaled = tmp_a_scaled;
}
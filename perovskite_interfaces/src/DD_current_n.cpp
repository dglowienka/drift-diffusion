#include "../include/drift_diffusion.h"

void Drift_diffusion::electron_current_density(void)
{
	int N = grid_scaled.N_points_boundaries_scaled; // Number of points without boundaries

	vector<double> Jnit_b, Jnit_alpha, Jnit_beta, Jnit_gamma; // alpha -> i-1, beta -> i, gamma -> i+1
	vector<double> tmp_Jn_scaled;
	double Jn_top, Jn_bottom;

	vector<double> n_it1_scaled = result_it1_scaled.n_scaled;
	vector<double> V_it1_scaled = result_it1_scaled.V_n_scaled;
	vector<double> hi_scaled = grid_scaled.hi_scaled;
	vector<double> u_ni_scaled = grid_scaled.u_n_scaled;

	Jn_top = boundaries_scaled.current_density_n_top_scaled;
	Jn_bottom = 0;

	// Solve the scaled function
	// WRITE!!
	
	for (int i = 1; i <= N; i++)
	{
		Jnit_beta.push_back( -1 ); // Jn(i,t+1)

		// Middle (no boundary around)
		if (i > 1 && i < N)
		{
			Jnit_gamma.push_back( 1 ); // Jn(i+1,t+1)
			Jnit_b.push_back( -(hi_scaled[i] / ((hi_scaled[i] + hi_scaled[i - 1]) / 2)) * // The minus only to have current negative
			(
				((u_ni_scaled[i] + u_ni_scaled[i + 1]) / (2*hi_scaled[i])) * B(V_it1_scaled[i + 1] - V_it1_scaled[i])*n_it1_scaled[i + 1] -
				(((u_ni_scaled[i] + u_ni_scaled[i + 1]) / (2*hi_scaled[i])) * B(V_it1_scaled[i] - V_it1_scaled[i + 1]) + ((u_ni_scaled[i - 1] + u_ni_scaled[i]) / (2*hi_scaled[i - 1])) * B(V_it1_scaled[i] - V_it1_scaled[i - 1]))*n_it1_scaled[i] +
				((u_ni_scaled[i - 1] + u_ni_scaled[i]) / (2*hi_scaled[i - 1])) * B(V_it1_scaled[i - 1] - V_it1_scaled[i])*n_it1_scaled[i - 1]
			));
		}

		// Bottom
		if (i == 1)
		{
			Jnit_gamma.push_back( 1 ); // Jn(i+1,t+1)
			Jnit_b.push_back( -(hi_scaled[i] / ((hi_scaled[i] + hi_scaled[i - 1]) / 2)) * // The minus only to have current negative
			(
				((u_ni_scaled[i] + u_ni_scaled[i + 1]) / (2*hi_scaled[i])) * B(V_it1_scaled[i + 1] - V_it1_scaled[i])*n_it1_scaled[i + 1] -
				(((u_ni_scaled[i] + u_ni_scaled[i + 1]) / (2*hi_scaled[i])) * B(V_it1_scaled[i] - V_it1_scaled[i + 1]) + ((u_ni_scaled[i - 1] + u_ni_scaled[i]) / (2*hi_scaled[i - 1])) * B(V_it1_scaled[i] - V_it1_scaled[i - 1]))*n_it1_scaled[i] +
				((u_ni_scaled[i - 1] + u_ni_scaled[i]) / (2*hi_scaled[i - 1])) * B(V_it1_scaled[i - 1] - V_it1_scaled[i])*n_it1_scaled[i - 1]
			));
		}

		// Top
		if (i == N)
		{
			Jnit_b.push_back( -(hi_scaled[i] / ((hi_scaled[i] + hi_scaled[i - 1]) / 2)) * // The minus only to have current negative
			(
				((u_ni_scaled[i] + u_ni_scaled[i + 1]) / (2*hi_scaled[i])) * B(V_it1_scaled[i + 1] - V_it1_scaled[i])*n_it1_scaled[i + 1] -
				(((u_ni_scaled[i] + u_ni_scaled[i + 1]) / (2*hi_scaled[i])) * B(V_it1_scaled[i] - V_it1_scaled[i + 1]) + ((u_ni_scaled[i - 1] + u_ni_scaled[i]) / (2*hi_scaled[i - 1])) * B(V_it1_scaled[i] - V_it1_scaled[i - 1]))*n_it1_scaled[i] +
				((u_ni_scaled[i - 1] + u_ni_scaled[i]) / (2*hi_scaled[i - 1])) * B(V_it1_scaled[i - 1] - V_it1_scaled[i])*n_it1_scaled[i - 1]
			) - Jn_top);
		}
	}

	// Vector x from Ax=b
	Jnit_alpha.resize(N-1);
	tmp_Jn_scaled = thomson_algorithm(Jnit_alpha, Jnit_beta, Jnit_gamma, Jnit_b, N);

	// Put boundaries to the result
	tmp_Jn_scaled.insert(tmp_Jn_scaled.begin(), Jn_bottom);
	tmp_Jn_scaled.insert(tmp_Jn_scaled.end(), Jn_top);

	// Put result to result vector in t+1 position
	result_it1_scaled.J_n_scaled = tmp_Jn_scaled;
}
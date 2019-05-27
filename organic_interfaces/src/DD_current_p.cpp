#include "../include/drift_diffusion.h"

void Drift_diffusion::hole_current_density(void)
{
	int N = grid_scaled.N_points_boundaries_scaled; // Number of points without boundaries

	vector<double> Jpit_b, Jpit_alpha, Jpit_beta, Jpit_gamma; // alpha -> i-1, beta -> i, gamma -> i+1
	vector<double> tmp_Jp_scaled;
	double Jp_top, Jp_bottom;

	vector<double> p_it1_scaled = result_it1_scaled.p_scaled;
	vector<double> V_it1_scaled = result_it1_scaled.V_p_scaled;
	vector<double> hi_scaled = grid_scaled.hi_scaled;
	vector<double> u_pi_scaled = grid_scaled.u_p_scaled;

	Jp_top = 0;
	Jp_bottom = boundaries_scaled.current_density_p_bottom_scaled;

	// Solve the scaled function
	for (int i = 1; i <= N; i++)
	{
		Jpit_beta.push_back( 1 ); // Jp(i,t+1)

		// Middle (no boundary around)
		if (i > 1 && i < N)
		{
			Jpit_alpha.push_back( -1 ); // Jp(i-1,t+1)
			Jpit_b.push_back( (hi_scaled[i] / ((hi_scaled[i] + hi_scaled[i - 1]) / 2)) * // The minus only to have current negative
			(
				((u_pi_scaled[i] + u_pi_scaled[i + 1]) / (2*hi_scaled[i])) * B(V_it1_scaled[i] - V_it1_scaled[i + 1])*p_it1_scaled[i + 1] -
				(((u_pi_scaled[i] + u_pi_scaled[i + 1]) / (2*hi_scaled[i])) * B(V_it1_scaled[i + 1] - V_it1_scaled[i]) + ((u_pi_scaled[i - 1] + u_pi_scaled[i]) / (2*hi_scaled[i - 1])) * B(V_it1_scaled[i - 1] - V_it1_scaled[i]))*p_it1_scaled[i] +
				((u_pi_scaled[i - 1] + u_pi_scaled[i]) / (2*hi_scaled[i - 1])) * B(V_it1_scaled[i] - V_it1_scaled[i - 1])*p_it1_scaled[i - 1]
			));
		}
		
		// Bottom
		if (i == 1)
		{
			Jpit_b.push_back( (hi_scaled[i] / ((hi_scaled[i] + hi_scaled[i - 1]) / 2)) * // The minus only to have current negative
			(
				((u_pi_scaled[i] + u_pi_scaled[i + 1]) / (2*hi_scaled[i])) * B(V_it1_scaled[i] - V_it1_scaled[i + 1])*p_it1_scaled[i + 1] -
				(((u_pi_scaled[i] + u_pi_scaled[i + 1]) / (2*hi_scaled[i])) * B(V_it1_scaled[i + 1] - V_it1_scaled[i]) + ((u_pi_scaled[i - 1] + u_pi_scaled[i]) / (2*hi_scaled[i - 1])) * B(V_it1_scaled[i - 1] - V_it1_scaled[i]))*p_it1_scaled[i] +
				((u_pi_scaled[i - 1] + u_pi_scaled[i]) / (2*hi_scaled[i - 1])) * B(V_it1_scaled[i] - V_it1_scaled[i - 1])*p_it1_scaled[i - 1]
			) + Jp_bottom);
		}

		// Top
		if (i == N)
		{
			Jpit_alpha.push_back( -1 ); // Jp(i-1,t+1)
			Jpit_b.push_back( (hi_scaled[i] / ((hi_scaled[i] + hi_scaled[i - 1]) / 2)) * // The minus only to have current negative
			(
				((u_pi_scaled[i] + u_pi_scaled[i + 1]) / (2*hi_scaled[i])) * B(V_it1_scaled[i] - V_it1_scaled[i + 1])*p_it1_scaled[i + 1] -
				(((u_pi_scaled[i] + u_pi_scaled[i + 1]) / (2*hi_scaled[i])) * B(V_it1_scaled[i + 1] - V_it1_scaled[i]) + ((u_pi_scaled[i - 1] + u_pi_scaled[i]) / (2*hi_scaled[i - 1])) * B(V_it1_scaled[i - 1] - V_it1_scaled[i]))*p_it1_scaled[i] +
				((u_pi_scaled[i - 1] + u_pi_scaled[i]) / (2*hi_scaled[i - 1])) * B(V_it1_scaled[i] - V_it1_scaled[i - 1])*p_it1_scaled[i - 1]
			));
		}
	}

	// Vector x from Ax=b
	Jpit_gamma.resize(N); // Fill with 0 
	tmp_Jp_scaled = thomson_algorithm(Jpit_alpha, Jpit_beta, Jpit_gamma, Jpit_b, N);

	// Put boundaries to the result
	tmp_Jp_scaled.insert(tmp_Jp_scaled.begin(), Jp_bottom);
	tmp_Jp_scaled.insert(tmp_Jp_scaled.end(), Jp_top);

	// Put result to result vector in t+1 position
	result_it1_scaled.J_p_scaled = tmp_Jp_scaled;
}
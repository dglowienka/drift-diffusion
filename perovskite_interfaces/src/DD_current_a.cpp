#include "../include/drift_diffusion.h"

void Drift_diffusion::anion_current_density(void)
{
	int N = grid_scaled.N_points_boundaries_scaled; // Number of points without boundaries

	vector<double> tmp_J_a_scaled;
	double J_a_top;

	vector<double> a_it1_scaled = result_it1_scaled.a_scaled;
	vector<double> V_it1_scaled = result_it1_scaled.V_n_scaled;
	vector<double> hi_scaled = grid_scaled.hi_scaled;
	vector<double> u_ai_scaled = grid_scaled.u_a_scaled;

	J_a_top = boundaries_scaled.current_density_n_top_scaled;

	// Solve the scaled function
	// WRITE!!
	
	for (int i = 0; i <= N; i++)
	{
		tmp_J_a_scaled.push_back( -((u_ai_scaled[i] + u_ai_scaled[i + 1]) / (2*hi_scaled[i])) * // The minus only to have current negative
		(
			B(V_it1_scaled[i + 1] - V_it1_scaled[i])*a_it1_scaled[i + 1] -
			B(V_it1_scaled[i] - V_it1_scaled[i + 1])*a_it1_scaled[i]
		));
	}

	// Put boundaries to the result
	tmp_J_a_scaled.insert(tmp_J_a_scaled.end(), J_a_top);

	// Put result to result vector in t+1 position
	result_it1_scaled.J_a_scaled = tmp_J_a_scaled;
}
#include "../include/drift_diffusion.h"

void Drift_diffusion::cation_current_density(void)
{
	int N = grid_scaled.N_points_boundaries_scaled; // Number of points without boundaries

	vector<double> tmp_J_c_scaled;
	double J_c_bottom;

	vector<double> c_it1_scaled = result_it1_scaled.c_scaled;
	vector<double> V_it1_scaled = result_it1_scaled.V_p_scaled;
	vector<double> hi_scaled = grid_scaled.hi_scaled;
	vector<double> u_ci_scaled = grid_scaled.u_c_scaled;

	J_c_bottom = boundaries_scaled.current_density_p_bottom_scaled;

	// Solve the scaled function
	// WRITE!!

	for (int i = 1; i <= N+1; i++)
	{
		tmp_J_c_scaled.push_back( ((u_ci_scaled[i - 1] + u_ci_scaled[i]) / (2*hi_scaled[i - 1])) * // The minus only to have current negative
		(
			B(V_it1_scaled[i - 1] - V_it1_scaled[i])*c_it1_scaled[i] -
			B(V_it1_scaled[i] - V_it1_scaled[i - 1])*c_it1_scaled[i - 1]
		));
	}

	// Put boundaries to the result
	tmp_J_c_scaled.insert(tmp_J_c_scaled.begin(), J_c_bottom);

	// Put result to result vector in t+1 position
	result_it1_scaled.J_c_scaled = tmp_J_c_scaled;
}
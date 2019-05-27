#include "../include/drift_diffusion.h"

void Drift_diffusion::dipslacement_current_density(void)
{
	int N = grid_scaled.N_points_scaled; // Number of points with boundaries
	double dt_scaled = layers_params_scaled.device.dt_scaled;

	vector<double> tmp_J_displ_scaled;
	tmp_J_displ_scaled.resize(N);

	vector<double> E_it1_scaled = result_it1_scaled.E_scaled;
	vector<double> E_it_scaled = result_it_scaled.E_scaled;
	vector<double> Er_i_scaled = grid_scaled.E_r_scaled;

	// Solve the scaled function
	// J_displ[i,t+1] = (E_scaled[i,t+1] - E_scaled[i,t]) / dt_scaled

	for (int i = 0; i < N; i++)
		tmp_J_displ_scaled[i] = Er_i_scaled[i]*(E_it1_scaled[i] - E_it_scaled[i]) / dt_scaled;

	// Put result to result vector in t+1 position
	result_it1_scaled.J_disp_scaled = tmp_J_displ_scaled;
}
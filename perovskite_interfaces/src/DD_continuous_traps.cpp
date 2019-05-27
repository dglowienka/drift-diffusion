#include "../include/drift_diffusion.h"

void Drift_diffusion::concentration_traps(void)
{
	int N = grid_scaled.N_points_scaled; // Number of points with boundaries
	double dt_scaled = layers_params_scaled.device.dt_scaled;

	vector<double> tmp_nt_scaled, tmp_pt_scaled;

	list<scaled_layer_params>::iterator it_layer=layers_params_scaled.stack_params.begin();
	current_layer_params_scaled = &(*it_layer);

	vector<double> nt_it_scaled = result_it_scaled.n_t_scaled;
	vector<double> pt_it_scaled = result_it_scaled.p_t_scaled;
	
	// WRITE FORMULA!
	for (int i = 0; i < N; i++)
	{
		current_i_position = i;
		if (grid_scaled.li_scaled[i] != it_layer->ID)
		{
			++it_layer;
			current_layer_params_scaled = &(*it_layer);
		}

		tmp_nt_scaled.push_back(
				nt_it_scaled[i]
				// + dt_scaled*R_Mn_D()
				// - dt_scaled*R_Mp_D()
			);
		tmp_pt_scaled.push_back(
				pt_it_scaled[i]
				// + dt_scaled*R_Mp_A()
				// - dt_scaled*R_Mn_A()
			);
	}

	// Put result to result vector in t+1 position
	result_it1_scaled.n_t_scaled = tmp_nt_scaled;
	result_it1_scaled.p_t_scaled = tmp_pt_scaled;
}
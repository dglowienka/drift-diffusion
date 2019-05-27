#include "../include/drift_diffusion.h"

void Drift_diffusion::energy(void)
{
	int N = grid_scaled.N_points_scaled; // Number of points with boundaries

	vector<double> tmp_Ec_scaled, tmp_Ev_scaled, tmp_Efn_scaled, tmp_Efp_scaled;

	list<scaled_layer_params>::iterator it_layer=layers_params_scaled.stack_params.begin();
	scaled_layer_params * layer_params_scaled = &(*it_layer);
	
	vector<double> V_it1_scaled = result_it1_scaled.V_scaled;
	vector<double> n_it1_scaled = result_it1_scaled.n_scaled;
	vector<double> p_it1_scaled = result_it1_scaled.p_scaled;
	
	for (int i = 0; i < N; i++)
	{
		if (grid_scaled.li_scaled[i] != layer_params_scaled->ID)
		{
			++it_layer;
			layer_params_scaled = &(*it_layer);
		}

		tmp_Ec_scaled.push_back(layer_params_scaled->E_c_scaled - V_it1_scaled[i]);	
		tmp_Ev_scaled.push_back(layer_params_scaled->E_v_scaled - V_it1_scaled[i]);

		tmp_Efn_scaled.push_back(tmp_Ec_scaled[i] + log(n_it1_scaled[i] / layer_params_scaled->N_c_scaled));
		tmp_Efp_scaled.push_back(tmp_Ev_scaled[i] - log(p_it1_scaled[i] / layer_params_scaled->N_v_scaled));		
	}

	// Put result to result vector in t+1 position
	result_it1_scaled.E_c_scaled = tmp_Ec_scaled;
	result_it1_scaled.E_v_scaled = tmp_Ev_scaled;
	result_it1_scaled.E_fn_scaled = tmp_Efn_scaled;
	result_it1_scaled.E_fp_scaled = tmp_Efp_scaled;
}
#include "../include/drift_diffusion.h"

void Drift_diffusion::concentration_p(void)
{
	int N = grid_scaled.N_points_boundaries_scaled; // Number of points without boundaries
	double dt_scaled = layers_params_scaled.device.dt_scaled;

	double beta_p, p_top, p_bottom;
	vector<double> pit_b, pit_alpha, pit_beta, pit_gamma; // alpha -> i-1, beta -> i, gamma -> i+1
	vector<double> tmp_p_scaled;

	list<scaled_layer_params>::iterator it_layer=layers_params_scaled.stack_params.begin();
	scaled_layer_params * layer_params_scaled = &(*it_layer);

	vector<double> V_it1_scaled = result_it1_scaled.V_p_scaled;
	vector<double> s_it_scaled = result_it_scaled.s_scaled;
	vector<double> n_it_scaled = result_it_scaled.n_scaled;
	vector<double> p_it_scaled = result_it_scaled.p_scaled;
	vector<double> hi_scaled = grid_scaled.hi_scaled;
	vector<double> u_pi_scaled = grid_scaled.u_p_scaled;

	p_top = boundaries_scaled.concentration_p_top_scaled;
	p_bottom = boundaries_scaled.concentration_p_bottom_scaled;

	//Fill the matrix A and vector b with diagonal data, A matrix from right and b vector from left part of equation
	for (int i = 1; i <= N; i++)
	{
		if (grid_scaled.li_scaled[i] != layer_params_scaled->ID)
		{
			++it_layer;
			layer_params_scaled = &(*it_layer);
		}

		beta_p = -dt_scaled / ((hi_scaled[i] + hi_scaled[i - 1]) / 2);
		pit_beta.push_back( (-beta_p*(((u_pi_scaled[i] + u_pi_scaled[i + 1]) / (2*hi_scaled[i])) * B(V_it1_scaled[i + 1] - V_it1_scaled[i])
			+ ((u_pi_scaled[i - 1] + u_pi_scaled[i]) / (2*hi_scaled[i - 1])) * B(V_it1_scaled[i - 1] - V_it1_scaled[i])) + 1) ); // pit(i,t+1) 
		
		// Middle (no boundary around)
		if (i > 1 && i < N)
		{
			pit_alpha.push_back( beta_p * ((u_pi_scaled[i - 1] + u_pi_scaled[i]) / (2*hi_scaled[i - 1])) * B(V_it1_scaled[i] - V_it1_scaled[i - 1]) ); // pit(i-1,t+1)
			pit_gamma.push_back( beta_p * ((u_pi_scaled[i] + u_pi_scaled[i + 1]) / (2*hi_scaled[i])) * B(V_it1_scaled[i] - V_it1_scaled[i + 1]) ); // pit(i+1,t+1)
			pit_b.push_back( p_it_scaled[i] 
				+ dt_scaled*k_d(i, layer_params_scaled) * s_it_scaled[i]
				- dt_scaled*R_B(i, layer_params_scaled) * (n_it_scaled[i]*p_it_scaled[i] - pow(layer_params_scaled->n_int_scaled,2)) 
				);
		}
		
		// Bottom
		if (i == 1)
		{
			pit_gamma.push_back( beta_p * ((u_pi_scaled[i] + u_pi_scaled[i + 1]) / (2*hi_scaled[i])) * B(V_it1_scaled[i] - V_it1_scaled[i + 1]) ); // pit(i+1,t+1)
			pit_b.push_back( p_it_scaled[i] 
				+ dt_scaled*k_d(i, layer_params_scaled) * s_it_scaled[i]
				- dt_scaled*R_B(i, layer_params_scaled) * (n_it_scaled[i]*p_it_scaled[i] - pow(layer_params_scaled->n_int_scaled,2)) 
				- beta_p * ((u_pi_scaled[i - 1] + u_pi_scaled[i]) / (2*hi_scaled[i - 1])) * B(V_it1_scaled[i] - V_it1_scaled[i - 1])*p_bottom
				);
		}

		// Top
		if (i == N)
		{
			pit_alpha.push_back( beta_p * ((u_pi_scaled[i - 1] + u_pi_scaled[i]) / (2*hi_scaled[i - 1])) * B(V_it1_scaled[i] - V_it1_scaled[i - 1]) ); // pit(i-1,t+1)
			pit_b.push_back( p_it_scaled[i] 
				+ dt_scaled*k_d(i, layer_params_scaled) * s_it_scaled[i]
				- dt_scaled*R_B(i, layer_params_scaled) * (n_it_scaled[i]*p_it_scaled[i] - pow(layer_params_scaled->n_int_scaled,2)) 
				- beta_p * ((u_pi_scaled[i] + u_pi_scaled[i + 1]) / (2*hi_scaled[i])) * B(V_it1_scaled[i] - V_it1_scaled[i + 1])*p_top
				);
		}
	}

	// Vector x from Ax=b
	tmp_p_scaled = thomson_algorithm(pit_alpha, pit_beta, pit_gamma, pit_b, N);

	// Put boundaries to the result
	tmp_p_scaled.insert(tmp_p_scaled.begin(), p_bottom);
	tmp_p_scaled.insert(tmp_p_scaled.end(), p_top);

	// Put result to result vector in t+1 position
	result_it1_scaled.p_scaled = tmp_p_scaled;
}
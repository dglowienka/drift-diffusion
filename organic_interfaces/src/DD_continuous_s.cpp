#include "../include/drift_diffusion.h"

void Drift_diffusion::concentration_s(void)
{
	int N = grid_scaled.N_points_boundaries_scaled; // Number of points without boundaries
	double dt_scaled = layers_params_scaled.device.dt_scaled;

	double beta_s, s_top, s_bottom;
	vector<double> sit_b, sit_alpha, sit_beta, sit_gamma; // alpha -> i-1, beta -> i, gamma -> i+1
	vector<double> tmp_s_scaled;

	list<scaled_layer_params>::iterator it_layer=layers_params_scaled.stack_params.begin();
	scaled_layer_params * layer_params_scaled = &(*it_layer);

	vector<double> s_it_scaled = result_it_scaled.s_scaled;
	vector<double> n_it_scaled = result_it_scaled.n_scaled;
	vector<double> p_it_scaled = result_it_scaled.p_scaled;
	vector<double> hi_scaled = grid_scaled.hi_scaled;
	vector<double> u_si_scaled = grid_scaled.u_s_scaled;

	s_top = boundaries_scaled.concentration_s_top_scaled;
	s_bottom = boundaries_scaled.concentration_s_bottom_scaled;

	//Fill the matrix A and vector b with diagonal data, A matrix from right and b vector from left part of equation
	for (int i = 1; i <= N; i++)
	{
		if (grid_scaled.li_scaled[i] != layer_params_scaled->ID)
		{
			++it_layer;
			layer_params_scaled = &(*it_layer);
		}

		beta_s = -dt_scaled / ((hi_scaled[i] + hi_scaled[i - 1]) / 2);
		sit_beta.push_back( (-beta_s*(((u_si_scaled[i] + u_si_scaled[i + 1]) / (2*hi_scaled[i]))
			+ ((u_si_scaled[i - 1] + u_si_scaled[i]) / (2*hi_scaled[i - 1]))) + 1) ); // sit(i,t+1) 

		// Middle (no boundary around)
		if (i > 1 && i < N)
		{
			sit_alpha.push_back( beta_s * ((u_si_scaled[i - 1] + u_si_scaled[i]) / (2*hi_scaled[i - 1])) ); // sit(i-1,t+1)
			sit_gamma.push_back( beta_s * ((u_si_scaled[i] + u_si_scaled[i + 1]) / (2*hi_scaled[i])) ); // sit(i+1,t+1)
			sit_b.push_back( s_it_scaled[i]
				+ dt_scaled*G(i, layer_params_scaled) 
				+ 0.25*dt_scaled*R_B(i, layer_params_scaled) * (n_it_scaled[i]*p_it_scaled[i] - pow(layer_params_scaled->n_int_scaled,2))
				- dt_scaled*k_d(i, layer_params_scaled) * s_it_scaled[i]
				- dt_scaled*R_Ms(i, layer_params_scaled) * s_it_scaled[i]
				- dt_scaled*layer_params_scaled->y_ns_scaled * (n_it_scaled[i]*s_it_scaled[i])
				- dt_scaled*layer_params_scaled->y_ps_scaled * (p_it_scaled[i]*s_it_scaled[i])
				);
		}

		// Bottom
		if (i == 1)
		{
			sit_gamma.push_back( beta_s * ((u_si_scaled[i] + u_si_scaled[i + 1]) / (2*hi_scaled[i])) ); // sit(i+1,t+1)
			sit_b.push_back( s_it_scaled[i] 
				+ dt_scaled*G(i, layer_params_scaled) 
				+ 0.25*dt_scaled*R_B(i, layer_params_scaled) * (n_it_scaled[i]*p_it_scaled[i] - pow(layer_params_scaled->n_int_scaled,2))
				- dt_scaled*k_d(i, layer_params_scaled) * s_it_scaled[i]
				- dt_scaled*R_Ms(i, layer_params_scaled) * s_it_scaled[i]
				- dt_scaled*layer_params_scaled->y_ns_scaled * (n_it_scaled[i]*s_it_scaled[i])
				- dt_scaled*layer_params_scaled->y_ps_scaled * (p_it_scaled[i]*s_it_scaled[i])
				- beta_s * ((u_si_scaled[i - 1] + u_si_scaled[i]) / (2*hi_scaled[i - 1]))*s_bottom
				);
		}

		// Top
		if (i == N)
		{
			sit_alpha.push_back( beta_s * ((u_si_scaled[i - 1] + u_si_scaled[i]) / (2*hi_scaled[i - 1])) ); // sit(i-1,t+1)
			sit_b.push_back( s_it_scaled[i] 
				+ dt_scaled*G(i, layer_params_scaled) 
				+ 0.25*dt_scaled*R_B(i, layer_params_scaled) * (n_it_scaled[i]*p_it_scaled[i] - pow(layer_params_scaled->n_int_scaled,2))
				- dt_scaled*k_d(i, layer_params_scaled) * s_it_scaled[i]
				- dt_scaled*R_Ms(i, layer_params_scaled) * s_it_scaled[i]
				- dt_scaled*layer_params_scaled->y_ns_scaled * (n_it_scaled[i]*s_it_scaled[i])
				- dt_scaled*layer_params_scaled->y_ps_scaled * (p_it_scaled[i]*s_it_scaled[i])
				- beta_s * ((u_si_scaled[i] + u_si_scaled[i + 1]) / (2*hi_scaled[i]))*s_top
				);
		}
	}

	// Vector x from Ax=b
	//tmp_s_scaled = gaussian_elimination(nit_A, nit_b);
	tmp_s_scaled = thomson_algorithm(sit_alpha, sit_beta, sit_gamma, sit_b, N);

	// Put boundaries to the result
	tmp_s_scaled.insert(tmp_s_scaled.begin(), s_bottom);
	tmp_s_scaled.insert(tmp_s_scaled.end(), s_top);

	// Put result to result vector in t+1 position
	result_it1_scaled.s_scaled = tmp_s_scaled;
}

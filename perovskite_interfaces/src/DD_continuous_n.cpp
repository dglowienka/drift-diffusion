#include "../include/drift_diffusion.h"

void Drift_diffusion::concentration_n(void)
{
	int N = grid_scaled.N_points_boundaries_scaled; // Number of points without boundaries
	double dt_scaled = layers_params_scaled.device.dt_scaled;

	double beta_n, n_top, n_bottom;
	vector<double> nit_b, nit_alpha, nit_beta, nit_gamma; // alpha -> i-1, beta -> i, gamma -> i+1
	vector<double> tmp_n_scaled;

	list<scaled_layer_params>::iterator it_layer=layers_params_scaled.stack_params.begin();
	current_layer_params_scaled = &(*it_layer);

	vector<double> V_it1_scaled = result_it1_scaled.V_n_scaled;
	vector<double> n_it_scaled = result_it_scaled.n_scaled;
	vector<double> p_it_scaled = result_it_scaled.p_scaled;
	vector<double> hi_scaled = grid_scaled.hi_scaled;
	vector<double> u_ni_scaled = grid_scaled.u_n_scaled;

	n_top = boundaries_scaled.concentration_n_top_scaled;
	n_bottom = boundaries_scaled.concentration_n_bottom_scaled;

	//Fill the matrix A and vector b with diagonal data, A matrix from right and b vector from left part of equation
	//CHANGE!! = 
	//b_n *  [B((Vit(i+1,t+1)-Vit(i,t+1)))*nit(i+1,t+1)-
	//      -(B((Vit(i,t+1)-Vit(i+1,t+1))) + B((Vit(i,t+1)-Vit(i-1,t+1))) - (1/b_n))*nit(i,t+1) 
	//      + B((Vit(i-1,t+1)-Vit(i,t+1)))*nit(i-1,t+1)]

	for (int i = 1; i <= N; i++)
	{
		current_i_position = i;
		if (grid_scaled.li_scaled[i] != current_layer_params_scaled->ID)
		{
			++it_layer;
			current_layer_params_scaled = &(*it_layer);
		}

		beta_n = -dt_scaled / ((hi_scaled[i] + hi_scaled[i - 1]) / 2);
		nit_beta.push_back( (-beta_n*(((u_ni_scaled[i] + u_ni_scaled[i + 1]) / (2*hi_scaled[i])) * B(V_it1_scaled[i] - V_it1_scaled[i + 1])
			+ ((u_ni_scaled[i - 1] + u_ni_scaled[i]) / (2*hi_scaled[i - 1])) * B(V_it1_scaled[i] - V_it1_scaled[i - 1])) + 1) ); // nit(i,t+1) 

		// Middle (no boundary around)
		if (i > 1 && i < N)
		{
			nit_alpha.push_back( beta_n * ((u_ni_scaled[i - 1] + u_ni_scaled[i]) / (2*hi_scaled[i - 1])) * B(V_it1_scaled[i - 1] - V_it1_scaled[i]) ); // nit(i-1,t+1)
			nit_gamma.push_back( beta_n * ((u_ni_scaled[i] + u_ni_scaled[i + 1]) / (2*hi_scaled[i])) * B(V_it1_scaled[i + 1] - V_it1_scaled[i]) ); // nit(i+1,t+1)
			nit_b.push_back( n_it_scaled[i]
				+ dt_scaled*G() 
				- dt_scaled*R_Mn()
				- dt_scaled*R_B() * (n_it_scaled[i]*p_it_scaled[i] - pow(current_layer_params_scaled->n_int_scaled,2)) 
				- dt_scaled*R_T() * (n_it_scaled[i]*p_it_scaled[i] - pow(current_layer_params_scaled->n_int_scaled,2))
				);
		}

		// Bottom
		if (i == 1)
		{
			nit_gamma.push_back( beta_n * ((u_ni_scaled[i] + u_ni_scaled[i + 1]) / (2*hi_scaled[i])) * B(V_it1_scaled[i + 1] - V_it1_scaled[i]) ); // nit(i+1,t+1)
			nit_b.push_back( n_it_scaled[i] 
				+ dt_scaled*G() 
				- dt_scaled*R_Mn()
				- dt_scaled*R_B() * (n_it_scaled[i]*p_it_scaled[i] - pow(current_layer_params_scaled->n_int_scaled,2)) 
				- dt_scaled*R_T() * (n_it_scaled[i]*p_it_scaled[i] - pow(current_layer_params_scaled->n_int_scaled,2))
				- beta_n * ((u_ni_scaled[i - 1] + u_ni_scaled[i]) / (2*hi_scaled[i - 1])) * B(V_it1_scaled[i - 1] - V_it1_scaled[i])*n_bottom
				);
		}

		// Top
		if (i == N)
		{
			nit_alpha.push_back( beta_n * ((u_ni_scaled[i - 1] + u_ni_scaled[i]) / (2*hi_scaled[i - 1])) * B(V_it1_scaled[i - 1] - V_it1_scaled[i]) ); // nit(i-1,t+1)
			nit_b.push_back( n_it_scaled[i] 
				+ dt_scaled*G() 
				- dt_scaled*R_Mn()
				- dt_scaled*R_B() * (n_it_scaled[i]*p_it_scaled[i] - pow(current_layer_params_scaled->n_int_scaled,2)) 
				- dt_scaled*R_T() * (n_it_scaled[i]*p_it_scaled[i] - pow(current_layer_params_scaled->n_int_scaled,2))
				- beta_n * ((u_ni_scaled[i] + u_ni_scaled[i + 1]) / (2*hi_scaled[i])) * B(V_it1_scaled[i + 1] - V_it1_scaled[i])*n_top
				);
		}
	}

	// Vector x from Ax=b
	//tmp_n_scaled = gaussian_elimination(nit_A, nit_b);
	tmp_n_scaled = thomson_algorithm(nit_alpha, nit_beta, nit_gamma, nit_b, N);

	// Put boundaries to the result
	tmp_n_scaled.insert(tmp_n_scaled.begin(), n_bottom);
	tmp_n_scaled.insert(tmp_n_scaled.end(), n_top);

	// Put result to result vector in t+1 position
	result_it1_scaled.n_scaled = tmp_n_scaled;
}
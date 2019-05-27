#include "../include/drift_diffusion.h"

void Drift_diffusion::potential(void)
{
	int N = grid_scaled.N_points_boundaries_scaled; // Number of points without boundaries

	double V_top, V_bottom;
	vector<double> Vit_b, Vit_alpha, Vit_beta, Vit_gamma; // alpha -> i-1, beta -> i, gamma -> i+1
	vector<double> tmp_V_scaled;

	list<scaled_layer_params>::iterator it_layer=layers_params_scaled.stack_params.begin();
	current_layer_params_scaled = &(*it_layer);

	vector<double> n_it_scaled = result_it_scaled.n_scaled;
	vector<double> p_it_scaled = result_it_scaled.p_scaled;
	vector<double> nt_it_scaled = result_it_scaled.n_t_scaled;
	vector<double> pt_it_scaled = result_it_scaled.p_t_scaled;
	vector<double> a_it_scaled = result_it_scaled.a_scaled;
	//vector<double> c_it_scaled = result_it_scaled.c_scaled;
	vector<double> hi_scaled = grid_scaled.hi_scaled;
	vector<double> Er_i_scaled = grid_scaled.E_r_scaled;

	V_top = boundaries_scaled.potential_top_scaled;
	V_bottom = boundaries_scaled.potential_bottom_scaled;

	//Fill the matrix A and vector b with diagonal data, A matrix from right and b vector from left part of equation
	// CHANGE!!

	for (int i = 1; i <= N; i++)
	{
		current_i_position = i;
		if (grid_scaled.li_scaled[i] != current_layer_params_scaled->ID)
		{
			++it_layer;
			current_layer_params_scaled = &(*it_layer);
		}
		
		Vit_beta.push_back( -( ((Er_i_scaled[i] + Er_i_scaled[i + 1]) / (2*hi_scaled[i])) + ((Er_i_scaled[i - 1] + Er_i_scaled[i]) / (2*hi_scaled[i - 1])) ) ); // V(i,t+1)

		// Middle (no boundary around)
		if (i > 1 && i < N)
		{
			Vit_alpha.push_back( (Er_i_scaled[i - 1] + Er_i_scaled[i]) / (2*hi_scaled[i - 1]) ); // V(i-1,t+1)
			Vit_gamma.push_back( (Er_i_scaled[i] + Er_i_scaled[i + 1]) / (2*hi_scaled[i]) ); // V(i+1,t+1)
			Vit_b.push_back( -0.5*(hi_scaled[i] + hi_scaled[i - 1]) * (
				p_it_scaled[i]
				- n_it_scaled[i]
				- nt_it_scaled[i] 
				+ pt_it_scaled[i]
				//+ c_it_scaled[i] 
				+ current_layer_params_scaled->N_0_scaled // For cations
				- a_it_scaled[i] 
				+ current_layer_params_scaled->N_D_scaled 
				- current_layer_params_scaled->N_A_scaled
				));
		}
		
		// Bottom
		if (i == 1)
		{
			Vit_gamma.push_back( (Er_i_scaled[i] + Er_i_scaled[i + 1]) / (2*hi_scaled[i]) ); // V(i+1,t+1)
			Vit_b.push_back( -0.5*(hi_scaled[i] + hi_scaled[i - 1]) * (
				p_it_scaled[i]
				- n_it_scaled[i]
				- nt_it_scaled[i] 
				+ pt_it_scaled[i]
				//+ c_it_scaled[i] 
				+ current_layer_params_scaled->N_0_scaled // For cations
				- a_it_scaled[i] 
				+ current_layer_params_scaled->N_D_scaled 
				- current_layer_params_scaled->N_A_scaled
				) - ((Er_i_scaled[i - 1] + Er_i_scaled[i]) / (2*hi_scaled[i - 1])) * V_bottom);
		}

		// Top
		if (i == N)
		{
			Vit_alpha.push_back( (Er_i_scaled[i - 1] + Er_i_scaled[i]) / (2*hi_scaled[i - 1]) ); // V(i-1,t+1)
			Vit_b.push_back( -0.5*(hi_scaled[i] + hi_scaled[i - 1]) * (
				p_it_scaled[i]
				- n_it_scaled[i]
				- nt_it_scaled[i] 
				+ pt_it_scaled[i]
				//+ c_it_scaled[i]
				+ current_layer_params_scaled->N_0_scaled // For cations 
				- a_it_scaled[i] 
				+ current_layer_params_scaled->N_D_scaled 
				- current_layer_params_scaled->N_A_scaled
				) - ((Er_i_scaled[i] + Er_i_scaled[i + 1]) / (2*hi_scaled[i])) * V_top );
		}
	}

	// Vector x from Ax=b
	tmp_V_scaled = thomson_algorithm(Vit_alpha, Vit_beta, Vit_gamma, Vit_b, N);

	// Put boundaries to the result
	tmp_V_scaled.insert(tmp_V_scaled.begin(), V_bottom);
	tmp_V_scaled.insert(tmp_V_scaled.end(), V_top);

	// Put result to result vector in t+1 position
	result_it1_scaled.V_scaled = tmp_V_scaled;
}

void Drift_diffusion::generalized_potential(void)
{
	int N = grid_scaled.N_points_scaled; // Number of points with boundaries

	list<scaled_layer_params>::iterator it_layer=layers_params_scaled.stack_params.begin();
	current_layer_params_scaled = &(*it_layer);
	scaled_layer_params * layer_params_ref_scaled = &(*it_layer); // Reference layer - first on the list

	vector<double> tmp_phi_n_scaled;
	vector<double> tmp_phi_p_scaled;
	vector<double> tmp_Vn_scaled;
	vector<double> tmp_Vp_scaled;

	vector<double> V_it1_scaled = result_it1_scaled.V_scaled;

	for (int i = 0; i < N; i++)
	{
		current_i_position = i;
		if (grid_scaled.li_scaled[i] != current_layer_params_scaled->ID)
		{
			++it_layer;
			current_layer_params_scaled = &(*it_layer);
		}

		// Generalized potentials with perovskite layer as reference
		tmp_phi_n_scaled.push_back(-(layer_params_ref_scaled->E_c_scaled - current_layer_params_scaled->E_c_scaled 
								- log(layer_params_ref_scaled->N_c_scaled / current_layer_params_scaled->N_c_scaled)));
		
		tmp_phi_p_scaled.push_back(-(layer_params_ref_scaled->E_v_scaled - current_layer_params_scaled->E_v_scaled 
								+ log(layer_params_ref_scaled->N_v_scaled / current_layer_params_scaled->N_v_scaled)));
		
		tmp_Vn_scaled.push_back(V_it1_scaled[i] - tmp_phi_n_scaled[i]);
		tmp_Vp_scaled.push_back(V_it1_scaled[i] - tmp_phi_p_scaled[i]);
	}
	

	// Put result to result vector in t+1 position
	result_it1_scaled.V_n_scaled = tmp_Vn_scaled;
	result_it1_scaled.V_p_scaled = tmp_Vp_scaled;
}

void Drift_diffusion::electric_field(void)
{
	int N = grid_scaled.N_points_boundaries_scaled; // Number of points without boundaries

	vector<double> tmp_E_scaled;

	vector<double> V_it1_scaled = result_it1_scaled.V_scaled;
	vector<double> hi_scaled = grid_scaled.hi_scaled;

	double E_top = boundaries_scaled.electric_field_top_scaled;
	double E_bottom = boundaries_scaled.electric_field_bottom_scaled;


	//Eit(i+1,t+1) = -(V_scaled[i+1,t+1] - V_scaled[i,t+1]) / hi_scaled
	for (int i = 1; i <= N; i++)
	{
		tmp_E_scaled.push_back(-(V_it1_scaled[i + 1] - V_it1_scaled[i - 1]) / (hi_scaled[i] + hi_scaled[i - 1]));
	}

	// Put boundaries to the result
	tmp_E_scaled.insert(tmp_E_scaled.begin(), E_bottom);
	tmp_E_scaled.insert(tmp_E_scaled.end(), E_top);

	// Put result to result vector in t+1 position
	result_it1_scaled.E_scaled = tmp_E_scaled;
}
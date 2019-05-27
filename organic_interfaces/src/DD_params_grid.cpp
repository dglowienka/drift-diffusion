#include "../include/drift_diffusion.h"

void Drift_diffusion::set_mesh(void) 
{
	// Definitions
	double tmp_xi_cathode, tmp_xi_anode;
	int active_layer_no = layers_params_scaled.device.activeLayer;
	list<scaled_layer_params>::iterator tmp_layer;

	// Cathode values
	tmp_layer = layers_params_scaled.stack_params.begin(); // First layer
	tmp_xi_cathode = 0;
	grid_scaled.xi_scaled.push_back(tmp_xi_cathode);
	grid_scaled.li_scaled.push_back(tmp_layer->ID); 
	grid_scaled.E_r_scaled.push_back(tmp_layer->E_r_scaled);
	grid_scaled.u_n_scaled.push_back(tmp_layer->u_n_scaled);
	grid_scaled.u_p_scaled.push_back(tmp_layer->u_p_scaled);
	grid_scaled.u_s_scaled.push_back(tmp_layer->u_s_scaled);

	// Bulk values
	for (list<scaled_layer_params>::iterator it=layers_params_scaled.stack_params.begin(); it != layers_params_scaled.stack_params.end(); ++it)
	{
		tmp_xi_anode += it->L_scaled;
		set_mesh_variable(&grid_scaled.li_scaled, it->ID, it->N_points); 			// ID
		chebyshev_grid(&grid_scaled.xi_scaled, it->L_scaled, it->N_points); 		// Grid space
		set_mesh_variable(&grid_scaled.E_r_scaled, it->E_r_scaled, it->N_points); 	// Permittivity
		set_mesh_variable(&grid_scaled.u_n_scaled, it->u_n_scaled, it->N_points);	// Electron mobility
		set_mesh_variable(&grid_scaled.u_p_scaled, it->u_p_scaled, it->N_points);	// Hole mobility
		set_mesh_variable(&grid_scaled.u_s_scaled, it->u_s_scaled, it->N_points);	// Exciton mobility
	}

	// Anode values
	tmp_layer = prev(layers_params_scaled.stack_params.end()); // Last element
	grid_scaled.xi_scaled.push_back(tmp_xi_anode);
	grid_scaled.li_scaled.push_back(tmp_layer->ID);
	grid_scaled.E_r_scaled.push_back(tmp_layer->E_r_scaled);
	grid_scaled.u_n_scaled.push_back(tmp_layer->u_n_scaled);
	grid_scaled.u_p_scaled.push_back(tmp_layer->u_p_scaled);
	grid_scaled.u_s_scaled.push_back(tmp_layer->u_s_scaled);

	grid_scaled.N_points_scaled = grid_scaled.xi_scaled.size();
	grid_scaled.N_points_boundaries_scaled = grid_scaled.N_points_scaled - 2; // Minus boundaries
	set_hi();
	

	// Generation model
	for (list<scaled_layer_params>::iterator it=layers_params_scaled.stack_params.begin(); it != layers_params_scaled.stack_params.end(); ++it)
	{
		if (it->ID == active_layer_no)
			set_generation_grid(&layers_params_scaled.G_data_scaled, it->ID, false);
		else
			set_generation_grid(&layers_params_scaled.G_data_scaled, it->ID, true);
	}
	grid_scaled.Gi_scaled.push_back(0);
}

void Drift_diffusion::uniform_grid(vector<double> * xi_scaled, double L, unsigned int N)
{
	vector<double> tmp_xi;
	double dx = L / N;
	for (size_t i = 0; i <= N; i++)
		xi_scaled->push_back(i*dx);
}

void Drift_diffusion::chebyshev_grid(vector<double> * xi_scaled, double L, unsigned int N)
{
	vector<double> tmp_xj, tmp_xi;
	double x_bottom;
	x_bottom = round(xi_scaled->back());
	//x_top = x_bottom + L;
	unsigned int k = N;

	// x_j = cos(PI*(2 * j - 1) / (2 * k))
	for (size_t j = 1; j <= k; j++)
		tmp_xj.push_back(cos(C_PI*(2 * j - 1) / (2 * k)));

	// x_i = L_i + L_f*(j+1)/2 to have x=[x_bottom, x_top]
	for (size_t i = 0; i < k; i++)
		xi_scaled->push_back(x_bottom - L*(tmp_xj[i] - 1) / 2);
}

void Drift_diffusion::set_hi(void)
{
	int N = grid_scaled.N_points_boundaries_scaled; // Number of points without boundaries

	vector<double> tmp_xi_scaled;
	double tmp_hi_scaled;
	tmp_xi_scaled = grid_scaled.xi_scaled;
	
	for (int i = 0; i <= N; i++)
	{
		tmp_hi_scaled = tmp_xi_scaled[i + 1] - tmp_xi_scaled[i];
		grid_scaled.hi_scaled.push_back(tmp_hi_scaled);
	}
} 

void Drift_diffusion::set_mesh_variable(vector<double> * vi_scaled, double variable, unsigned int N)
{
	double tmp_variable = variable;

	for (size_t i = 0; i < N; i++)
		vi_scaled->push_back(tmp_variable); // Er is uniform across single layer
}

void Drift_diffusion::set_generation_grid(vector<double> * G_input_scaled, int layer_no, bool G_zero)
{
	double tmp_G_value, tmp_x_value, tmp_x_init, N;
	N = grid_scaled.N_points_scaled;
	tmp_x_init = 0;
	int layer_no_old = -1;

	vector<double> tmp_Gi;
	vector<double> tmp_li = grid_scaled.li_scaled;

	for (size_t i = 0; i < N; ++i)
	{
		if (layer_no_old != layer_no) // Only for change layer number to the specified one (once)
			tmp_x_init = grid_scaled.xi_scaled[i];

		if (tmp_li[i] == layer_no)
		{

			if (G_zero)
				grid_scaled.Gi_scaled.push_back(0); // For inactive layers
			else
			{
				tmp_x_value = grid_scaled.xi_scaled[i] - tmp_x_init;
				tmp_G_value = interpolate(layers_params_scaled.x_data_scaled, layers_params_scaled.G_data_scaled, tmp_x_value, false);
				tmp_G_value = layers_params_scaled.device.G_suns_scaled * tmp_G_value; // Illumination
				grid_scaled.xi_scaled.push_back(tmp_x_value);
				grid_scaled.Gi_scaled.push_back(tmp_G_value);
			}
		}
		layer_no_old = grid_scaled.li_scaled[i];
	}
}
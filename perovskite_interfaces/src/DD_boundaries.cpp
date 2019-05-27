#include "../include/drift_diffusion.h"

void Drift_diffusion::set_boundaries_space(void)
{
	scaled_boundaries tmp_boundaries;
	list<scaled_layer_params>::iterator tmp_layer;

	/* Device params */
	double V_build_scaled = layers_params_scaled.device.V_build_scaled;
	double V_a_scaled = layers_params_scaled.device.V_a_scaled;
	double W_a_scaled = layers_params_scaled.device.W_a_scaled;
	double W_c_scaled = layers_params_scaled.device.W_c_scaled;

	/* ETL params */
	tmp_layer = layers_params_scaled.stack_params.begin();
	double Nc_ETL_scaled = tmp_layer->N_c_scaled;
	double Nv_ETL_scaled = tmp_layer->N_v_scaled;
	double Ec_ETL_scaled = tmp_layer->E_c_scaled;
	double Ev_ETL_scaled = tmp_layer->E_v_scaled;

	/* HTL params */
	tmp_layer = prev(layers_params_scaled.stack_params.end()); // End is theoretical object so 'prev' to get last
	double Nc_HTL_scaled = tmp_layer->N_c_scaled;
	double Nv_HTL_scaled = tmp_layer->N_v_scaled;
	double Ec_HTL_scaled = tmp_layer->E_c_scaled;
	double Ev_HTL_scaled = tmp_layer->E_v_scaled;


	// Cathode (top) -> x = 0
	// Anode  (top) -> x = L

	/* CATHODE/ETL */
	// Electric field and potential
	tmp_boundaries.potential_bottom_scaled = (V_build_scaled - V_a_scaled);
	tmp_boundaries.electric_field_bottom_scaled = 0;

	// Electronic concentration
	tmp_boundaries.concentration_n_bottom_scaled = Nc_ETL_scaled*exp(-(Ec_ETL_scaled - W_a_scaled));
	tmp_boundaries.concentration_p_bottom_scaled = Nv_ETL_scaled*exp(-(W_a_scaled - Ev_ETL_scaled));

	// Electronic current
	tmp_boundaries.current_density_p_bottom_scaled = 0;

	// Ionic current
	//tmp_boundaries.current_density_c_bottom_scaled = 0;

	/* ANODE/HTL */ 
	// Electric field and potential
	tmp_boundaries.potential_top_scaled = 0;
	tmp_boundaries.electric_field_top_scaled = 0;

	// Electronic concentration
	tmp_boundaries.concentration_n_top_scaled = Nc_HTL_scaled*exp(-(Ec_HTL_scaled - W_c_scaled));
	tmp_boundaries.concentration_p_top_scaled = Nv_HTL_scaled*exp(-(W_c_scaled - Ev_HTL_scaled));

	// Electronic current
	tmp_boundaries.current_density_n_top_scaled = 0;

	// Ionic current
	tmp_boundaries.current_density_a_top_scaled = 0;


	boundaries_scaled = tmp_boundaries;
}

void Drift_diffusion::set_boundaries_time(void)
{
	list<scaled_layer_params>::iterator tmp_layer=layers_params_scaled.stack_params.begin();

	result_it_scaled.t_scaled = 0;
	result_it1_scaled.t_scaled = 0;
	/* Boundary values for t0 */
	result_it_scaled.V_scaled.resize(grid_scaled.N_points_scaled);
	result_it_scaled.V_n_scaled.resize(grid_scaled.N_points_scaled);	
	result_it_scaled.V_p_scaled.resize(grid_scaled.N_points_scaled);
	result_it_scaled.E_scaled.resize(grid_scaled.N_points_scaled);
	result_it_scaled.n_scaled.resize(grid_scaled.N_points_scaled);
	result_it_scaled.p_scaled.resize(grid_scaled.N_points_scaled);
	result_it_scaled.n_t_scaled.resize(grid_scaled.N_points_scaled);
	result_it_scaled.p_t_scaled.resize(grid_scaled.N_points_scaled);
	result_it_scaled.E_c_scaled.resize(grid_scaled.N_points_scaled);	
	result_it_scaled.E_v_scaled.resize(grid_scaled.N_points_scaled);
	result_it_scaled.E_fn_scaled.resize(grid_scaled.N_points_scaled);
	result_it_scaled.E_fp_scaled.resize(grid_scaled.N_points_scaled);

	for (size_t i = 0; i < grid_scaled.N_points_scaled; i++)
	{
		if (grid_scaled.li_scaled[i] != tmp_layer->ID)
			++tmp_layer;

		result_it_scaled.a_scaled.push_back(tmp_layer->N_0_scaled);
		//result_it_scaled.c_scaled.push_back(tmp_layer->N_0_scaled);
	}
		
}
#include "../include/drift_diffusion.h"

void Drift_diffusion::set_voltage(double V_a)
{
	layers_params.device.V_a = V_a;
	layers_params_scaled.device.V_a_scaled = potential_scaling(V_a, &params_c);
	set_boundaries_space(); // New V_a gives new boundaries
}

void Drift_diffusion::set_illumination(double G_suns)
{
	grid_scaled.Gi_scaled.clear(); // Clear the vector first
	layers_params.device.G_suns = G_suns; // New illumination is settled
	layers_params_scaled.device.G_suns_scaled = G_suns;

	// Generation model grid is recalculated
	for (list<scaled_layer_params>::iterator it=layers_params_scaled.stack_params.begin(); it != layers_params_scaled.stack_params.end(); ++it)
	{
		if (it->ID == layers_params_scaled.device.activeLayer)
			set_generation_grid(&layers_params_scaled.G_data_scaled, it->ID, false);
		else
			set_generation_grid(&layers_params_scaled.G_data_scaled, it->ID, true);
	}
	grid_scaled.Gi_scaled.push_back(0);
}

void Drift_diffusion::set_filename(string results_filename)
{
	file_name = results_filename;
}
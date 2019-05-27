#include "../include/drift_diffusion.h"

Drift_diffusion::Drift_diffusion(unscaled_all_layers_params params_input, string const &folder_file_name)
{
	/* Input parameters */
	folder_name = folder_file_name.substr(0, folder_file_name.find('/'));
	file_name = folder_file_name.substr(folder_name.length()+1); // plus one due to '/' symbol
	
	layers_params = params_input; // The same structure


	params_c = c_parameters(layers_params.device.T);

	/* Scaling */
	device_params_scaling(&layers_params.device);
	for (list<unscaled_layer_params>::iterator it_layer=layers_params.stack_params.begin(); it_layer != layers_params.stack_params.end(); ++it_layer)
		layer_params_scaling(&(*it_layer)); // Each layer is scaled separately
	generation_input_scaling(&layers_params.x_data, &layers_params.G_data, 
					&layers_params_scaled.x_data_scaled, &layers_params_scaled.G_data_scaled);
	
	/* Grid points */
	set_mesh();

	/* Boundary conditions */
	set_boundaries_space();
	set_boundaries_time();
}

void Drift_diffusion::drift_diffusion(void)
{
	// t_step is t, but solution is always given for t+1 in the "result" vector
	/* POISSON EQUATION */
	// Potential calculation in time t+1
	potential();
	// Generalized potential in time t+1
	generalized_potential();
	// Electric field calculation in time t+1
	electric_field();

	/* CONTINUOUITY EQUATION */
	// Electrons concentration in time t+1
	concentration_n();
	// Holes concentration in time t+1
	concentration_p();
	// Traps concentration in time t+1
	concentration_traps();
	// Anion concentration in time t+1
	concentration_a();
	// Cation concentration in time t+1
	//concentration_c(); // Not movable so fix value

	/* ENERGY */
	energy();

	/* CURRENT */
	// Electron current density in time t+1
	electron_current_density();
	// Hole current density in time t+1
	hole_current_density();
	// Anion current density in time t+1
	anion_current_density();
	// Cation current density in time t+1
	//cation_current_density(); // Not movable so no current
	// Displacement current density in time t+1
	dipslacement_current_density();
	// Total current density in time t+1
	total_current_density();
}

void Drift_diffusion::drift_diffusion_steady(void)
{
	/* Define scaled values */
	double epsilon_calc, epsilon_check = 0;

	int t = 0;
	while(true)
	{
		drift_diffusion();

		/* STEADY STATE */
		epsilon_calc = steady_current();
		if (epsilon_calc <= layers_params_scaled.device.epsilon) // to make sure it is not only one value under the limit
		{
			epsilon_check++;
			if (epsilon_check == 2) break;
		}
		else epsilon_check = 0;
		result_it1_scaled.epsilon_calc = epsilon_calc;

		/* INCREMENT TIME */
		result_it1_scaled.t_scaled = t*layers_params_scaled.device.dt_scaled;
		result_it1_scaled.t_steps = t;
		t++;

		result_it_scaled = result_it1_scaled; // New results are getting old

		/* SHOW PROGRESS */
		cout << setprecision(5) << setw(10) << result_it1_scaled.epsilon_calc * 100 << " %" << endl;

		/* UNSCALING AND SAVE*/
		unscaling_results();
		save_space();
	}
}

void Drift_diffusion::drift_diffusion_time(double time)
{
	/* Scaling of input */
	double time_scaled = time_scaling(time, &params_c);
	
	unsigned int t_step = (unsigned int)(time_scaled / layers_params_scaled.device.dt_scaled);
	double t = result_it1_scaled.t_steps;
	for (unsigned int i = 0; i < t_step; i++)
	{
		drift_diffusion();

		/* INCREMENT TIME */
		result_it1_scaled.t_scaled = t*layers_params_scaled.device.dt_scaled;
		result_it1_scaled.t_steps = t;
		t++;

		result_it_scaled = result_it1_scaled; // New results are getting old

		/* SHOW PROGRESS */
		cout << setprecision(5) << setw(10) << time_unscaling(t*layers_params_scaled.device.dt_scaled, &params_c) << " s" << endl;
		
		/* UNSCALING AND SAVE*/
		unscaling_results();
		save_space();
	}
}
#define __USE_MINGW_ANSI_STDIO 0
#include <vector>
#include "../include/command_interface.h"
#include "../include/drift_diffusion.h"
#include "../include/parameters_structure.h"

void command_interface::mode_single_voltage(void)
{
	unscaled_all_layers_params tmp_layers_params = layers_params;

	string tmp_local_dir = local_dir;
	string tmp_results_filename = "/space_time_single_" + to_string(tmp_layers_params.device.V_a) + "V" + ".dat";
	string tmp_jV_name = "jV_steady.dat";

	/* Simulate drift-diffusion model */
	Drift_diffusion DD_class(tmp_layers_params, tmp_local_dir+tmp_results_filename); // Create object
	DD_class.drift_diffusion_steady();
	DD_class.save_jV_result(tmp_jV_name);
}

void command_interface::mode_series_voltage(void)
{
	unscaled_all_layers_params tmp_layers_params = layers_params;

	// Voltage series mode - parameters
	double V_min = settings.V_min;
	double V_max = settings.V_max; // V
	double V_step = settings.V_step;
	double V_a = V_min;
	
	string tmp_local_dir = local_dir;
	string tmp_results_filename = "/space_time_current_voltage_" + to_string(V_a) + "V" + ".dat";
	string tmp_jV_name = "jV_steady.dat";

	Drift_diffusion DD_class(tmp_layers_params, tmp_local_dir+tmp_results_filename); // Create object

	int steps = (int)round((V_max - V_min) / V_step);

	for (int i = 0; i <= steps; i++)
	{
		V_a = V_min + V_step*i; // Voltage applied
		tmp_results_filename = "/space_time_series_voltage_" + to_string(V_a) + "V" + ".dat";
		
		/* Simulate drift-diffusion model */
		DD_class.set_filename(tmp_results_filename); // For new voltage applied, new filename
		DD_class.set_voltage(V_a);
		DD_class.drift_diffusion_steady();
		DD_class.save_jV_result(tmp_jV_name); // Save J-V characteristic results in file
	}
}

void command_interface::mode_pulse_illumination(void)
{
	unscaled_all_layers_params tmp_layers_params = layers_params;
	
	// Pulse mode - parameters
	double tmp_G_suns = tmp_layers_params.device.G_suns;
	double tmp_first_pulse = settings.first_pulse;
	double tmp_second_pulse = settings.second_pulse;
	double tmp_third_pulse = settings.third_pulse;
	double tmp_time = tmp_first_pulse;

	string tmp_local_dir = local_dir;
	string tmp_results_filename = "/space_pulse_illumination_" + to_string(tmp_layers_params.device.V_a) + "V" + ".dat";

	Drift_diffusion DD_class(tmp_layers_params, tmp_local_dir + tmp_results_filename); // Create object

	/* STEADY STATE CONDITION FOR DARK CURRENT */
	DD_class.set_illumination(0); // 0 suns means no illumination
	//DD_class.drift_diffusion_steady();

	for (unsigned int i = 0; i < 3; i++)
	{
		if (i == 0) // Turn-off process
		{
			tmp_time = tmp_first_pulse;
			DD_class.set_illumination(0);
		}
		else if(i == 1) // Turn-on process
		{
			tmp_time = tmp_second_pulse;
			DD_class.set_illumination(tmp_G_suns); // Full illumination

		}
		else if (i == 2) // Turn-off illumination
		{
			tmp_time = tmp_third_pulse;
			DD_class.set_illumination(0);

		}
		DD_class.drift_diffusion_time(tmp_time); // DD for time specifed by pulse width
	}
}

void command_interface::mode_hysteresis(void)
{
	unscaled_all_layers_params tmp_layers_params = layers_params;

		// Hysteresis mode - parameters
	double V_min = settings.V_min;
	double V_max = settings.V_max; // V
	double V_step = settings.V_step; // dv
	double V_rate = settings.V_rate; // dv/dt
	double dt = V_step / V_rate;
	
	string tmp_local_dir = local_dir;


	int steps = (int)round((V_max - V_min) / V_step);

	/* #0 STEADY AT 0V */
	double V_a = 0; // First steady state condition without voltage applied
	string tmp_results_filename = "/space_time_hysteresis_STEADY_" + to_string(V_a) + "V" + ".dat";
	Drift_diffusion DD_class(tmp_layers_params, tmp_local_dir + tmp_results_filename); // Create object !!! WITHOUT FILENAME!! CHECK!!
	DD_class.set_voltage(V_a);
	DD_class.drift_diffusion_steady();

	/* #1 REVERSE */
	for (int i = 0; i <= steps; i++)
	{
		V_a = V_max - V_step*i;
		tmp_results_filename = "/space_time_hysteresis_RW_" + to_string(V_a) + "V" + ".dat";

		/* Simulate drift-diffusion model */
		DD_class.set_filename(tmp_results_filename); // For new voltage applied, new filename
		DD_class.set_voltage(V_a);
		DD_class.drift_diffusion_time(dt); // DD for time specifed from dv/dt
		DD_class.save_jV_result("jV_RW.dat"); // Save J-V characteristic results in file
	}

	/* #2 FORWARD */
	for (int i = 0; i <= steps; i++)
	{
		V_a = V_min + V_step*i;
		tmp_results_filename = "/space_time_hysteresis_FW_" + to_string(V_a) + "V" + ".dat";

		/* Simulate drift-diffusion model */
		DD_class.set_filename(tmp_results_filename); // For new voltage applied, new filename
		DD_class.set_voltage(V_a);
		DD_class.drift_diffusion_time(dt); // DD for time specifed from dv/dt
		DD_class.save_jV_result("jV_FW.dat"); // Save J-V characteristic results in file
	}

}
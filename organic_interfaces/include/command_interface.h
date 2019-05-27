#ifndef _COMMAND_INTERFACE
#define _COMMAND_INTERFACE

#include <string>
#include <list>
#include <iostream>
#include <cstring>
#include <sstream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "parameters_structure.h"
#include "constants.h"

class command_interface
{
private:
	/* Drift-diffusion model variables */
	unscaled_all_layers_params layers_params;

	/* Interface variables */
	struct settings {
		double first_pulse;		// Illumation width of the first pulse for drift_diffusion
		double second_pulse;	// Illumation width of the second pulse for drift_diffusion
		double third_pulse;		// Illumation width of the third pulse for drift_diffusion
		double V_min;			// Minimum voltage for jV characteristc
		double V_max;			// Maximum voltage for jV characteristc
		double V_step;			// Step voltage for jV characteristc
		double V_rate;			// Rate of voltage change dv/dt
	} settings;
	
	string local_dir;
	int systemRet;
	bool isConfigOk;
	bool isGenerationOk;
	bool isFolderCreated;
	string config_name;
	string symbol_const = ">> ";
	list<string> command_list =
	{
		"ld",
		"sd",
		"cd",
		"lg",
		"ds",
		"dv",
		"dt",
		"dh",
		"h",
		"a",
		"c",
		"q"
	};
	list<string> parameter =
	{
		"file_name",		// -ld
		"", 				// -sd
		"folder_name", 		// -cd
		"gener_dir_name", 	// -lg
		"", 				// -ds
		"",					// -dv
		"",					// -dt
		"", 				// -dh
		"", 				// -h
		"", 				// -a
		"", 				// -c
		"" 					// -q
	};
	list<string> command_description =
	{
		"Load data from config.xml file if no parameter", // -ld
		"Save data to config.xml file", // -sd
		"Create folder for the incoming results", // -cd
		"Load generation profile", //lg
		"Simulation of single point for current parameters", // -ds
		"Simulation of current-voltage characteristic - V_a is changed", // -dv
		"Simulation of transient current with modulation of light pulses",// -dt
		"Simulation of current-voltage characteristic for hysteresis - V_a is changed with dv/dt rate", // -dv
		"Help gives you all needed information for the model", // -h
		"Author of the model", // -a
		"Clear the screen", // -c
		"Exit the software" // -q
	};

	bool load_device_params(string config_dir_name);
	bool load_stack_params(string config_dir_name);
	bool load_layer_params(string config_dir_name, string layer_name);
	bool load_settings(string config_dir_name);
	bool load_generation_file(string gener_dir_name);
	
	void save_device_params(string config_dir_name);
	void save_layer_params(string config_dir_name, unscaled_layer_params * layer_params);
	void save_setting(string config_dir_name);

	void mode_single_voltage(void);	
	void mode_series_voltage(void);
	void mode_pulse_illumination(void);
	void mode_hysteresis(void);
	void show_command_list(void);
	void show_author(void);
	string doubleString(double value); // Convert double to string with scientific format
public:
	command_interface(void); // constructor
	void update_interface(void);
};

#endif // !_COMMAND_INTERFACE

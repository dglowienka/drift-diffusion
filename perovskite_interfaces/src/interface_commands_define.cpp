#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../include/command_interface.h"

command_interface::command_interface(void)
{
	show_author();
	show_command_list();
}

void command_interface::update_interface(void)
{
	/* Interface variables */
	string input, command, parameter, file_dir;
	string symbol = symbol_const;
	config_name = "config.xml"; // Default name
	string generation_name = "";

	isConfigOk = false; 
	isGenerationOk = false;
	isFolderCreated = false;

	while (1)
	{
		cout << symbol;
		getline(cin, input);
		if (input.empty()) continue; // If ENTER is pressed with no symbols
		
		istringstream iss(input);
		file_dir = "";
		command = ""; 
		parameter = "";
		iss >> command >> parameter;

		// Look until end if the input string fits command_list
		auto it = find(command_list.begin(), command_list.end(), command);

		/* Commands: load data, ld+file_name*/
		if (it == find(command_list.begin(), command_list.end(), "ld"))
		{
			// Set filename if parameter is given 
			if (!parameter.empty()) // If no filename given, then use default
				config_name = parameter;

			// If "cd" was used
			if (local_dir.empty()) 
				file_dir = config_name;
			else
				file_dir = local_dir + "/" + config_name;
			
			ifstream file(file_dir);

			// Check file
			if (file.good())
			{
				isConfigOk = true;
				file.close();
			}

			if (isConfigOk)
				isConfigOk = load_device_params(file_dir);
			if (isConfigOk)
				isConfigOk = load_stack_params(file_dir);
			if (isConfigOk)
				isConfigOk = load_settings(file_dir);
			if (isConfigOk)
				cout << "Device and settings loaded successfully" << endl;
				
			// Print current stack by iteration through layers of the stack
			cout << "The current stack is cathode/";
			for (list<unscaled_layer_params>::iterator it_layer=layers_params.stack_params.begin(); it_layer != layers_params.stack_params.end(); ++it_layer)
				cout << it_layer->name << "/";
			cout << "anode" << endl;
	
			// Report
			if (isConfigOk)
				cout << file_dir << " loaded successfully" << endl;
			else
				cout << file_dir << " has issues or doesn't exist!" << endl;
		}

		/* Commands: save data, sd */
		else if (it == find(command_list.begin(), command_list.end(), "sd"))
		{
			if (local_dir.empty()) // If "cd" was used
				file_dir = config_name;
			else
				file_dir = local_dir + "/" + config_name;
			

			if (isConfigOk)
			{
				save_device_params(file_dir);

				// Iterate through layers of the stack
				for (list<unscaled_layer_params>::iterator it_layer=layers_params.stack_params.begin(); it_layer != layers_params.stack_params.end(); ++it_layer)
					save_layer_params(file_dir, &(*it_layer));

				save_setting(file_dir);
				cout << "config.xml saved successfully" << endl;
			}
			else
				cout << "Parameters are not loaded yet, ld" << endl;		
		}		
		
		/* Commands: create directory, cd+folder_name */
		else if (it == find(command_list.begin(), command_list.end(), "cd"))
		{
			local_dir = parameter;
			symbol = local_dir + symbol_const;
			mkdir(local_dir.c_str(), 0700);
			if (parameter.empty())
			{
				symbol = symbol_const;
				isFolderCreated = false;
			}
			else
				isFolderCreated = true;
		}

		/* Commands: load generation profile, lg+gener_dir_name */
		else if (it == find(command_list.begin(), command_list.end(), "lg"))
		{
			generation_name = parameter;

			if (local_dir.empty()) // If "cd" was used
				file_dir = generation_name;
			else
				file_dir = local_dir + "/" + generation_name;

			if (parameter.empty())
				cout << "Name for file with generation profile must be given" << endl;
			else
			{
				isGenerationOk = load_generation_file(file_dir);
				if (isGenerationOk)
					cout << file_dir << " loaded successfully" << endl;
				else 
					cout << "File " << file_dir << " does not exist" << endl;
			}
		}

		/* Commands: simulate, ds */
		else if (it == find(command_list.begin(), command_list.end(), "ds"))
		{
			if (isConfigOk && isFolderCreated && isGenerationOk)
			{
				/* Simulate drift-diffusion model */
				mode_single_voltage();
			}
			else if (isConfigOk == false)
				cout << "Parameters are not loaded yet, ld" << endl;
			else if (isFolderCreated == false)
				cout << "Folder for the results is not created yet, cd" << endl;
			else if (isGenerationOk == false)
				cout << "Generation profile is not loaded, lg" << endl;
			else
				cout << "Check cd, ld or lg command" << endl;
		}

		/* Commands: simulate, dv */
		else if (it == find(command_list.begin(), command_list.end(), "dv"))
		{
			if (isConfigOk && isFolderCreated && isGenerationOk)
			{
				/* Simulate drift-diffusion model */
				mode_series_voltage();
			}
			else if (isConfigOk == false)
				cout << "Parameters are not loaded yet, ld" << endl;
			else if (isFolderCreated == false)
				cout << "Folder for the results is not created yet, cd" << endl;
			else if (isGenerationOk == false)
				cout << "Generation profile is not loaded, lg" << endl;
			else
				cout << "Check cd, ld or lg command" << endl;
		}

		/* Commands: simulate, dt */
		else if (it == find(command_list.begin(), command_list.end(), "dt"))
		{
			if (isConfigOk && isFolderCreated && isGenerationOk)
			{
				/* Simulate drift-diffusion model */
				mode_pulse_illumination();
			}
			else if (isConfigOk == false)
				cout << "Parameters are not loaded yet, ld" << endl;
			else if (isFolderCreated == false)
				cout << "Folder for the results is not created yet, cd" << endl;
			else if (isGenerationOk == false)
				cout << "Generation profile is not loaded, lg" << endl;
			else
				cout << "Check cd, ld or lg command" << endl;
		}

		/* Commands: simulate, dh */
		else if (it == find(command_list.begin(), command_list.end(), "dh"))
		{
			if (isConfigOk && isFolderCreated && isGenerationOk)
			{
				/* Simulate drift-diffusion model */
				mode_hysteresis();
			}
			else if (isConfigOk == false)
				cout << "Parameters are not loaded yet, ld" << endl;
			else if (isFolderCreated == false)
				cout << "Folder for the results is not created yet, cd" << endl;
			else if (isGenerationOk == false)
				cout << "Generation profile is not loaded, lg" << endl;
			else
				cout << "Check cd, ld or lg command" << endl;
		}		

		/* Commands: help, h */
		else if (it == find(command_list.begin(), command_list.end(), "h"))
		{
			cout << "This is a transient 1D drift-diffusion model" << endl;
			show_command_list();
		}

		/* Commands: show author, a */
		else if (it == find(command_list.begin(), command_list.end(), "a"))
		{
			show_author();
		}

		/* Commands: clean screen, c */
		else if (it == find(command_list.begin(), command_list.end(), "c"))
		{
			systemRet = system("clear");
			if (systemRet == -1) // Error handler
			{
				cout << "System error" << endl;
				break;
			}
		}

		/* Commands: exit, q */
		else if (it == find(command_list.begin(), command_list.end(), "q"))
		{
			break;
		}

		/* Command error */
		else
			cout << "There is no command like this!" << endl;
	}
}
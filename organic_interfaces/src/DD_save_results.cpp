#include <iostream>
#include <iomanip> // precision cout and fstream
#include <fstream>
#include <string>
#include "../include/drift_diffusion.h"

// Show all variables in space for the given time. If t=-1 then show steady state
void Drift_diffusion::save_space(void)
{
	string dir_file = folder_name + '/' + file_name;
	ifstream infile(dir_file.c_str());
	//bool file_exist = infile.good(); // Check if file exists

	ofstream drift_diffusion_file; // Create a reference to file
	drift_diffusion_file.open(dir_file.c_str(), ios::app);

	//if (!(file_exist)) drift_diffusion_file << "New file" << endl;

	drift_diffusion_file << "Time: " << result_it.t << " s" << endl;
	drift_diffusion_file << "The applied voltage: " << layers_params.device.V_a << " V" << endl;
	drift_diffusion_file << "The total current: " << result_it.J_total << " A m-2" << endl;
	drift_diffusion_file << "The precision: " <<  result_it.epsilon_calc * 100 << " %" << endl;

	drift_diffusion_file << setprecision(9) 
	<< setw(11) << "x" << "\t" 
	<< setw(11) << "G" << "\t" 
	<< setw(11) << "V" << "\t" 
	<< setw(11) << "E" << "\t" 
	<< setw(11) << "n" << "\t" 
	<< setw(11) << "p" << "\t" 
	<< setw(11) << "s" << "\t" 
	<< setw(11) << "E_c" << "\t"	
	<< setw(11) << "E_v" << "\t"
	<< setw(11) << "E_fn" << "\t"
	<< setw(11) << "E_fp" << "\t"	
	<< setw(11) << "J_n" << "\t" 
	<< setw(11) << "J_p" << "\t" 
	<< setw(11) << "J_disp" << "\t"
	<< setw(11) << "layer" << endl;

	for (unsigned int i = 0; i < grid.N_points; i++)
	{
		drift_diffusion_file << setprecision(9) 
		<< setw(11) << grid.xi[i] << "\t" 
		<< setw(11) << grid.Gi[i] << "\t" 
		<< setw(11) << result_it.V[i] << "\t" 
		<< setw(11) << result_it.E[i] << "\t" 
		<< setw(11) << result_it.n[i] << "\t" 
		<< setw(11) << result_it.p[i] << "\t" 
		<< setw(11) << result_it.s[i] << "\t" 
		<< setw(11) << result_it.E_c[i] << "\t" 
		<< setw(11) << result_it.E_v[i] << "\t"
		<< setw(11) << result_it.E_fn[i] << "\t"
		<< setw(11) << result_it.E_fp[i] << "\t"   		
		<< setw(11) << result_it.J_n[i] << "\t" 
		<< setw(11) << result_it.J_p[i] << "\t" 
		<< setw(11) << result_it.J_disp[i] << "\t"
		<< setw(11) << grid.li[i] << endl;
	}
	
	drift_diffusion_file << endl;

	drift_diffusion_file.close(); // Close the file
}

// Show voltage-current value - overrides the file
void Drift_diffusion::save_jV_result(string const &file_name)
{
	string dir_file = folder_name + '/' + file_name;
	ifstream infile(dir_file.c_str());
	bool file_exist = infile.good(); // Check if file exists

	ofstream drift_diffusion_file; // Create a reference to file
	drift_diffusion_file.open(dir_file.c_str(), ios::app);

	if (!(file_exist)) drift_diffusion_file << setw(11) << "Voltage" << "\t" << setw(11) << "Current" << "\n";

	drift_diffusion_file << setw(11) << layers_params.device.V_a << "\t" << setw(11) << result_it.J_total << "\n";
	cout << "Filename: " << dir_file << endl;

	drift_diffusion_file.close(); // Close the file
}
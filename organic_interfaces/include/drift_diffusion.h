#ifndef _DRIFT_DIFFUSION
#define _DRIFT_DIFFUSION

#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip> // precision cout and fstream
#include <iostream>
#include <list>
#include <iterator>
#include "parameters_structure.h"
#include "parameter_scaled.h"
#include "scaling.h"
#include "unscaling.h"
#include "constants.h"

using namespace std;

class Drift_diffusion
{
private:
	string folder_name;
	string file_name;

	// Unscaled params
	unscaled_result result_it;	// Spatial results in t+1 time
	unscaled_numerical_params grid;	// Space dependent parameters
	unscaled_all_layers_params layers_params; // ETL, HTL and perovskite layers parameters
	
	// Scaling parameters
	scaling_params params_c;

	// Scaled params
	scaled_result result_it_scaled; // Spatial results in t time
	scaled_result result_it1_scaled; // Spatial results in t+1 time
	scaled_boundaries boundaries_scaled;
	scaled_numerical_params grid_scaled; // Space dependent parameters
	scaled_all_layers_params layers_params_scaled; // ETL, HTL and perovskite layers parameters 


	// Calculate drift-diffusion function for t+1
	void drift_diffusion(void);

	/* POTENTIAL */
	// Calculate electrical potential from Poisson equation
	void potential(void);
	// Calculate generalized potentials
	void generalized_potential(void);
	// Calculate electric field from electric potential
	void electric_field(void);

	/* CONCENTRATIONS OF ELECTRONICS CARRIERS*/
	// Calculate concentration of electrons in t+1 time
	void concentration_n(void);
	// Calculate concentration of traps in t+1 time
	void concentration_traps(void);
	// Calculate concentration of holes in t+1 time
	void concentration_p(void);
	// Calculate concentration of excitons in t+1 time
	void concentration_s(void);

	/* CONCENTRATIONS OF IONICS CARRIERS */
	// Calculate concentration of anions in t+1 time
	void concentration_a(void);
	// Calculate concentration of cations in t+1 time
	void concentration_c(void);

	/* ENERGY LEVELS */
	void energy(void);

	/* CURRENTS */
	// Calculate current of electrons density in t+1 time
	void electron_current_density(void);
	// Calculate current of holes density in t+1 time
	void hole_current_density(void);
	// Calculate displacement current in t+1
	void dipslacement_current_density(void);
	// Calculate total current in t+1 time
	void total_current_density(void);
	// Calculate the rate of change of total current
	double steady_current(void);

	/* MECHANISMS */
	// DISSOCIATION OF EXCITONS (Onsager-Braun)
	double k_d(int i, scaled_layer_params * layer_params_scaled);
	// MONOMOLECULAR RECOMBINATION FOR EXCITONS
	double R_Ms(int i, scaled_layer_params * layer_params_scaled);
	// MONOMOLECULAR RECOMBINATION FOR ELECTRONS (SRH type)
	double R_Mn(int i, scaled_layer_params * layer_params_scaled);
	// Donor region
	double R_Mn_D(int i, scaled_layer_params * layer_params_scaled);
	// Acceptor region
	double R_Mn_A(int i, scaled_layer_params * layer_params_scaled);
	// MONOMOLECULAR RECOMBINATION FOR ELECTRONS (SRH type)
	double R_Mp(int i, scaled_layer_params * layer_params_scaled);
	// Donor region
	double R_Mp_D(int i, scaled_layer_params * layer_params_scaled);
	// Acceptor region
	double R_Mp_A(int i, scaled_layer_params * layer_params_scaled);
	// BIMOLECULAR RECOMBINATION (Langevin type)
	double R_B(int i, scaled_layer_params * layer_params_scaled);
	// TRIMOLECULAR RECOMBINATION (Auger type)
	double R_T(int i, scaled_layer_params * layer_params_scaled);
	// GENERATION
	double G(int i, scaled_layer_params * layer_params_scaled);

	/* MESH */
	// Create grid for numerics
	void set_mesh(void);
	void uniform_grid(vector<double> * xi_scaled, double L, unsigned int N);
	void chebyshev_grid(vector<double> * xi_scaled, double L, unsigned int N);
	void set_hi(void);
	void set_mesh_variable(vector<double> * vi_scaled, double variable, unsigned int N);
	void set_generation_grid(vector<double> * G_input_scaled, int layer_no, bool G_zero);

	/* SCALING */
	// Scaling of the parameters for the device
	void device_params_scaling(unscaled_device_params * device_params_unscaled);
	// Scaling of the parameters for the layer
	void layer_params_scaling(unscaled_layer_params * layer_params_unscaled);
	// Scaling of the generation profile for active layer
	void generation_input_scaling(vector<double> * x_data, vector<double> * G_data, vector<double> * x_data_scaled, vector<double> * G_data_scaled);
	// Unscale the results from simulation
	void unscaling_results(void);
	/* BOUNDARY CONDITIONS */
	void set_boundaries_space(void);
	void set_boundaries_time(void);

	/* SPECIAL FUNCTIONS */
	// Bernoulli function with z parameter
	double B(double z);
	// Gaussian elimination function for Ax=b, where A is matrix and b, x are vectors
	vector<double> gaussian_elimination(vector<vector<double> > A_matrix, vector<double> b_data);
	// Thomson algorithm for tridiagonal matrix Ax=b but A=LU with Lower and Upper part of triangle
	vector<double> thomson_algorithm(vector<double> &alpha, vector<double> &beta, vector<double> &gamma, vector<double> &b, int n);
	// Calculate average of 1D vector
	double average_vector(vector<double> vector_in);
	// Interpolate y from x
	double interpolate( vector<double> &xData, vector<double> &yData, double x, bool extrapolate);

	/* SAVE  */
	// Save result from the current iteration t+1
	void save_space(void);

public:
	// Constructor of drift-diffusion class
	Drift_diffusion(unscaled_all_layers_params params_input, string const &folder_file_name); // Constructor
	// Calculate drift-diffusion function until steady state is reached
	void drift_diffusion_steady(void);
	// Calculate drift-diffusion function for turn-on illumination measurements
	void drift_diffusion_time(double time);
	// Save current-voltage results for steady state only
	void save_jV_result(string const &file_name);
	// Change voltage applied for cell
	void set_voltage(double V_a);
	// Change illumination G_suns
	void set_illumination(double G_suns);
	// Change space-results filename
	void set_filename(string results_filename);
};

#endif // !_DRIFT_DIFFUSION

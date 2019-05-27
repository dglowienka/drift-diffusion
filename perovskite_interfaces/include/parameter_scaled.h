#ifndef _PARAMETERS_SCALED
#define _PARAMETERS_SCALED

#include <vector>
#include <string>
#include <list>
using namespace std;

struct scaled_result {
	vector<double> V_scaled;			// Potential
	vector<double> V_n_scaled;			// Genelarized potential for electrons
	vector<double> V_p_scaled;			// Genelarized potential for holes	
	vector<double> E_scaled;			// Electric field
	vector<double> n_scaled;			// Electron concentration
	vector<double> p_scaled;			// Hole concentration
	vector<double> n_t_scaled;			// Electron trap concentration
	vector<double> p_t_scaled;			// Hole trap concentration
	vector<double> a_scaled;			// Anion concentration
	vector<double> c_scaled;			// Cation concentration
	vector<double> E_c_scaled;			// Energy of conduction band
	vector<double> E_v_scaled;			// Energy of valence band
	vector<double> E_fn_scaled;			// Energy of quasi-fermi level for electrons
	vector<double> E_fp_scaled;			// Energy of quasi-fermi level for holes
	vector<double> J_n_scaled;			// Electron current density
	vector<double> J_p_scaled;			// Hole current density
	vector<double> J_a_scaled;			// Anion current density
	vector<double> J_c_scaled;			// Cation current density
	vector<double> J_disp_scaled;		// Displacement current density
	double J_total_scaled;				// Total current in time vector only
	double t_scaled;					// Final time
	unsigned int t_steps;				// Time steps
	double epsilon_calc;				// Precision calculated
};

struct scaling_params {
	double x_c;		// Scaling factor for dimensions, usually 1 nm [m]
	double t_c;		// Scaling of time [s]
	double N;		// Scaling of concentration [m-3]
	double Er_max;	// Scaling of permittivity [-]
	double u_max;	// Maximum mobility [m2 V^(-1) s^(-1)]
	double V_c;		// Scaling of potential [V]
	double F_c;		// Scaling of electric field [V m-1]
	double J_c;		// Scaling of current [A m-2]
	double G_c;		// Scaling of generation [m-3]
	double B_c;		// Scaling of bimolecular recombination rate [m3 s-1]
	double T_c;		// Scaling of trimolecular recombination rate [m6 s-1]
	double E_c;		// Scaling of energy [J]
};

struct scaled_numerical_params {
	vector<double> li_scaled;				// grid position for layers. 0 - ETL, 1 - per, 2 - HTL
	vector<double> xi_scaled;				// grid position
	vector<double> hi_scaled;				// grid spacing between position hi=xi+1-xi
	vector<double> Gi_scaled;				// nonuniform generation profile
	vector<double> E_r_scaled;				// nonuniform permittivity
	vector<double> u_n_scaled;				// nonuniform electron mobility
	vector<double> u_p_scaled;				// nonuniform hole mobility
	vector<double> u_a_scaled;				// nonuniform anion mobility
	vector<double> u_c_scaled;				// nonuniform hole mobility
	unsigned int N_points_scaled;			// Number of points for the x grid
	unsigned int N_points_boundaries_scaled;// Number of points for the x grid without boundaries
};

struct scaled_device_params {
	int activeLayer;			// Number of active layer starts at 0
	double dt_scaled;			// Time dimensions
	double V_a_scaled;			// Voltage applied
	double V_build_scaled;		// Voltage build
	double G_suns_scaled;		// Generation factor - how much suns for irradiation
	double W_a_scaled;			// Work function of anode
	double W_c_scaled;			// Work function of cathode
	double epsilon;				// Error of calculations
};

struct scaled_layer_params {
	string name;				// Layer name
	int ID;						// Layer ID
	double E_r_scaled;			// Relative permittivity
	double u_n_scaled;			// Mobility of electrons
	double u_p_scaled;			// Mobility of holes
	double u_a_scaled;			// Mobility of anions
	double u_c_scaled;			// Mobility of cations
	double L_scaled;			// Length of the cell
	double N_points;			// Number of points for the x grid
	double C_nc_scaled;			// Capture of electrons in conduction band for SRH
	double C_nv_scaled;			// Capture of electrons in valence band for SRH
	double C_pc_scaled;			// Capture of holes in conduction band for SRH
	double C_pv_scaled;			// Capture of holes in valence band for SRH
	double n_int_scaled;		// Density of intrinsic carriers
	double T_n_scaled;			// Trimolecular recombination rate of electrons
	double T_p_scaled;			// Trimolecular recombination rate of holes
	double E_g_scaled;			// Energy band-gap in the material
	double E_c_scaled;			// Energy level of conduction band
	double E_v_scaled;			// Energy level of valence band
	double E_tn_scaled;			// Energy level of trapped electron states
	double E_tp_scaled;			// Energy level of trapped hole states
	double W_tn_scaled;			// Energy level width of trapped electron states
	double W_tp_scaled;			// Energy level width of trapped hole states
	double N_D_scaled;			// Concentration of donor doping with (-) electrons
	double N_A_scaled;			// Concentration of acceptor doping with (+) holes
	double N_0_scaled;			// Density of ions at equilibrium state
	double N_tn_scaled;			// Density of trap site for electrons
	double N_tp_scaled;			// Density of trap site for holes
	double N_c_scaled;			// Effective density of states in conduction band
	double N_v_scaled;			// Effective density of states in valence band
	double ksi_scaled;			// Langevin recombination factor
};

struct scaled_all_layers_params {
	scaled_device_params device;
	list<scaled_layer_params> stack_params;
	vector<double> x_data_scaled;
	vector<double> G_data_scaled;
};

struct scaled_boundaries {
	// For space x=0 and x=L
	double potential_top_scaled;			// Potential - top
	double potential_bottom_scaled;			// Potential - bottom
	double electric_field_top_scaled;		// Electric field - top
	double electric_field_bottom_scaled;	// Electric field - bottom
	double concentration_n_top_scaled;		// Electron concentration - top
	double concentration_n_bottom_scaled;	// Electron concentration - bottom
	double concentration_p_top_scaled;		// Hole concentration - top
	double concentration_p_bottom_scaled;	// Hole concentration - bottom
	double current_density_n_top_scaled;	// Electron current density - top
	double current_density_p_bottom_scaled;	// Hole current density - bottom
	double current_density_a_top_scaled;	// Anion current density - top
	//double current_density_c_bottom_scaled;// Cation current density - bottom
};

#endif // !_PARAMETERS_SCALED


#ifndef _PARAMETERS_STRUCTURE
#define _PARAMETERS_STRUCTURE

#include <vector>
#include <string>
#include <list>
using namespace std;

struct unscaled_result {
	vector<double> V;			// Potential [V]
	vector<double> E;			// Electric field [V m-1]
	vector<double> n;			// Electron concentration [m-3]
	vector<double> p;			// Hole concentration [m-3]
	vector<double> n_t;			// Electron trap concentration [m-3]	
	vector<double> p_t;			// Hole trap concentration [m-3]	
	vector<double> a;			// Anion concentration [m-3]
	vector<double> c;			// Cation concentration [m-3]
	vector<double> E_c;			// Energy of conduction band [J]
	vector<double> E_v;			// Energy of valence band [J]
	vector<double> E_fn;		// Energy of quasi-fermi level for electrons [J]
	vector<double> E_fp;		// Energy of quasi-fermi level for holes [J]
	vector<double> J_n;			// Electron current density [A m-2]
	vector<double> J_p;			// Hole current density [A m-2]
	vector<double> J_a;			// Anion current density [A m-2]
	vector<double> J_c;			// Cation current density [A m-2]
	vector<double> J_disp;		// Displacement current density [A m-2]
	double J_total;				// Total current in time vector only [A m-2]
	double t;					// Final time [s]
	unsigned int t_steps;		// Time steps [-]
	double epsilon_calc;		// Precision calculated [-]
};

struct unscaled_numerical_params {
	vector<double> li;					// grid position for layers. 0 - ETL, 1 - per, 2 - HTL
	vector<double> xi;					// grid position [m]
	vector<double> hi;					// grid spacing between position hi=xi+1-xi [m]
	vector<double> Gi;					// nonuniform generation profile [m-3]
	vector<double> E_r;					// nonuniform permittivity [-]
	vector<double> u_n;					// nonuniform electron mobility [m2 V-1 s-1]
	vector<double> u_p;					// nonuniform hole mobility [m2 V-1 s-1]
	vector<double> u_a;					// nonuniform anion mobility [m2 V-1 s-1]
	vector<double> u_c;					// nonuniform cation mobility [m2 V-1 s-1]
	unsigned int N_points;				// Number of points for the x grid
	unsigned int N_points_boundaries; 	// Number of points for the x grid without boundaries
};

struct unscaled_device_params {
	int activeLayer;// Number of active layer starts at 0
	double T;		// Temperature [K]
	double dt;		// Time dimensions [s]
	double V_a;		// Voltage applied [V]
	double V_build;	// Voltage build [V]
	double G_suns;	// Generation factor - how much suns for irradiation [-]
	double W_a;		// Work function of anode [J]
	double W_c;		// Work function of cathode [J]
	double epsilon; // Error of calculations [-]
};

struct unscaled_layer_params { 
	string name;	// Layer name
	int ID;			// Layer ID
	double E_r;		// Relative permittivity [-]
	double u_n;		// Mobility of electrons [m2 V ^ (-1) s ^ (-1)]
	double u_p;		// Mobility of holes [m2 V^(-1) s^(-1)]
	double u_a;		// Mobility of anions [m2 V^(-1) s^(-1)]
	double u_c;		// Mobility of cations [m2 V^(-1) s^(-1)]
	double L;		// Length of the cell [m]
	double N_points;// Number of points for the x grid [-]
	double C_nc;	// Capture of electrons in conduction band for SRH[m3 s-1]
	double C_nv;	// Capture of electrons in valence band for SRH[m3 s-1]
	double C_pc;	// Capture of holes in conduction band for SRH[m3 s-1]
	double C_pv;	// Capture of holes in valence band for SRH[m3 s-1]
	double n_int;	// Density of intrinsic carriers [m-3]
	double T_n;		// Trimolecular recombination rate of electrons [m6 s-1]
	double T_p;		// Trimolecular recombination rate of holes [m6 s-1]
	double E_g;		// Energy band-gap in the material [J]
	double E_c;		// Energy level of conduction band [J]
	double E_v;		// Energy level of valence band [J]
	double E_tn;	// Energy level of trapped electron states [J]
	double E_tp;	// Energy level of trapped hole states [J]
	double W_tn;	// Energy level width of trapped electron states [J]
	double W_tp;	// Energy level width of trapped hole states [J]
	double N_D;		// Concentration of donor doping with (-) electrons [m-3]	
	double N_A;		// Concentration of acceptor doping with (+) holes [m-3]
	double N_0;		// Density of ions at equilibrium state [m-3]
	double N_tn;	// Density of trap site for electrons [m-3]
	double N_tp;	// Density of trap site for holes [m-3]
	double N_c;		// Effective density of states in conduction band [m-3]
	double N_v;		// Effective density of states in valence band [m-3]
	double ksi;		// Langevin recombination factor
};

struct unscaled_all_layers_params {
	unscaled_device_params device;
	list<unscaled_layer_params> stack_params;
	vector<double> x_data;
	vector<double> G_data;
};

#endif // !_PARAMETERS_STRUCTURE

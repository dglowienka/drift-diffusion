#ifndef _UNSCALING
#define _UNSCALING

#include "constants.h"
#include "parameters_structure.h"
#include "parameter_scaled.h"

/* PARAMETERS */
// Unscaling function for space
double space_unscaling(double x_scaled, scaling_params * params);

// Unscaling function for time
double time_unscaling(double t, scaling_params * params);

// Unscaling function for permittivity
double permittivity_unscaling(double Er_scaled, scaling_params * params);

// Unscaling function for concentration of electrons, holes or excitons
double concentrations_unscaling(double concentration_scaled, scaling_params * params);

// Scaling function for mobility of electrons, holes or excitons
double mobility_unscaling(double u_scaled, scaling_params * params);

// Unscaling function for electric potential
double potential_unscaling(double V_scaled, scaling_params * params);

// Unscaling function for electric field
double electric_field_unscaling(double E_scaled, scaling_params * params);

// Unscaling of current
double current_density_unscaling(double J_scaled, scaling_params * params);

// Unscaling of energy
double energy_unscaling(double E_scaled, scaling_params * params);

// Unscaling of generation rate constant
double generation_unscaling(double G_scaled, scaling_params * params);

// Unscaling of trimolecular recombination rate
double trimolecular_unscaling(double T_scaled, scaling_params * params);

#endif // !_UNSCALING
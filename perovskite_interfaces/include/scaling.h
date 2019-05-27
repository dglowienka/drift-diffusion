#ifndef _SCALING
#define _SCALING

#include "constants.h"
#include "parameters_structure.h"
#include "parameter_scaled.h"

/* PARAMETERS */
// Scaling parameters function
scaling_params c_parameters(double T);

// Scaling function for space
double space_scaling(double x, scaling_params * params);

// Scaling function for time
double time_scaling(double t, scaling_params * params);

// Scaling function for permittivity
double permittivity_scaling(double Er, scaling_params * params);

// Scaling function for concentration of electrons, holes or excitons
double concentrations_scaling(double concentration, scaling_params * params);

// Scaling function for mobility of electrons, holes or excitons
double mobility_scaling(double u, scaling_params * params);

// Scaling function for potential
double potential_scaling(double V, scaling_params * params);

// Scaling function for electric field
double electric_field_scaling(double E, scaling_params * params);

// Scaling of current
double current_density_scaling(double J, scaling_params * params);

// Scaling of energy
double energy_scaling(double E_B, scaling_params * params);

// Scaling of generation rate constant
double generation_scaling(double G, scaling_params * params);

// Scaling of bimolecular recombination rate
double bimolecular_scaling(double B, scaling_params * params);

// Scaling of trimolecular recombination rate
double trimolecular_scaling(double T, scaling_params * params);

#endif // !_SCALING
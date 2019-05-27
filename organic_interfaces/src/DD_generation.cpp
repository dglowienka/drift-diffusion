#define _USE_MATH_DEFINES // Use constants in cmath
#include "../include/drift_diffusion.h"

double Drift_diffusion::G(int i, scaled_layer_params * layer_params_scaled)
{
	double tmp_G_scaled;

	tmp_G_scaled = grid_scaled.Gi_scaled[i];

	return tmp_G_scaled;
}
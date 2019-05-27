#define _USE_MATH_DEFINES // Use constants in cmath
#include "../include/drift_diffusion.h"

double Drift_diffusion::G(void)
{
	double tmp_G_scaled;

	tmp_G_scaled = grid_scaled.Gi_scaled[current_i_position];

	return tmp_G_scaled;
}
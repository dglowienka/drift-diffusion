#include "../include/drift_diffusion.h"

void Drift_diffusion::total_current_density(void)
{
	double tmp_total_current, tmp_average_electron, tmp_average_holes, tmp_average_displacement;
	vector<double> electron_current_density = result_it1_scaled.J_n_scaled;
	vector<double> hole_current_density = result_it1_scaled.J_p_scaled;
	vector<double> dipslacement_current_density = result_it1_scaled.J_disp_scaled;

	// Calculate averages of the vector in t time
	tmp_average_electron = average_vector(electron_current_density);
	tmp_average_holes = average_vector(hole_current_density);
	tmp_average_displacement = average_vector(dipslacement_current_density);

	// Calculate total current density as a sum of J_n + J_p + J_displ
	tmp_total_current = tmp_average_electron + tmp_average_holes + tmp_average_displacement;

	result_it1_scaled.J_total_scaled = tmp_total_current;
}
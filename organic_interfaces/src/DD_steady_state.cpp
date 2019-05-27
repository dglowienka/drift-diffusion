#include "../include/drift_diffusion.h"

double Drift_diffusion::steady_current(void)
{
	double tmp_change_current;

	vector<double> Jn_t1 = result_it1_scaled.J_n_scaled;
	vector<double> Jn_t = result_it_scaled.J_n_scaled;;
	vector<double> Jp_t1 = result_it1_scaled.J_p_scaled;
	vector<double> Jp_t = result_it_scaled.J_p_scaled;

	// In steady current we do not include displacement current. As it is 0 when steady state takes place
	double tmp_average_J_tot_t1_scaled = average_vector(Jn_t1) + average_vector(Jp_t1);
	double tmp_average_J_tot_t_scaled = average_vector(Jn_t) + average_vector(Jp_t);

	// Calculate rate of change (J_total(t+1) - J_total(t)) / J_total(t)
	tmp_change_current = abs((tmp_average_J_tot_t1_scaled - tmp_average_J_tot_t_scaled) / tmp_average_J_tot_t_scaled);

	return tmp_change_current;
}
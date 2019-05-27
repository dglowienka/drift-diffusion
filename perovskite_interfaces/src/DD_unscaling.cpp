#include "../include/drift_diffusion.h"
#include "../include/unscaling.h"

void Drift_diffusion::unscaling_results(void)
{
	int N = grid_scaled.N_points_scaled; // Plus two sided boundaries
	scaled_result result_scaled = result_it1_scaled; // Only the last is used

	result_it.V.resize(N);
	result_it.E.resize(N);
	result_it.n.resize(N);
	result_it.p.resize(N);
	result_it.n_t.resize(N);
	result_it.p_t.resize(N);
	result_it.a.resize(N);
	//result_it.c.resize(N);
	result_it.E_c.resize(N);
	result_it.E_v.resize(N);
	result_it.E_fn.resize(N);
	result_it.E_fp.resize(N);
	result_it.J_n.resize(N);
	result_it.J_p.resize(N);
	result_it.J_a.resize(N);
	//result_it.J_c.resize(N);
	result_it.J_disp.resize(N);
	result_it.t = time_unscaling(result_scaled.t_scaled, &params_c);
	result_it.t_steps = result_scaled.t_steps;
	result_it.epsilon_calc = result_scaled.epsilon_calc;

	for (int i = 0; i < N; i++)
	{
			result_it.V[i] = potential_unscaling(result_scaled.V_scaled[i], &params_c);
			result_it.E[i] = electric_field_unscaling(result_scaled.E_scaled[i], &params_c);
			result_it.n[i] = concentrations_unscaling(result_scaled.n_scaled[i], &params_c);
			result_it.p[i] = concentrations_unscaling(result_scaled.p_scaled[i], &params_c);
			result_it.n_t[i] = concentrations_unscaling(result_scaled.n_t_scaled[i], &params_c);
			result_it.p_t[i] = concentrations_unscaling(result_scaled.p_t_scaled[i], &params_c);
			result_it.a[i] = concentrations_unscaling(result_scaled.a_scaled[i], &params_c);
			//result_it.c[i] = concentrations_unscaling(result_scaled.c_scaled[i], &params_c);
			result_it.E_c[i] = energy_unscaling(result_scaled.E_c_scaled[i], &params_c);
			result_it.E_v[i] = energy_unscaling(result_scaled.E_v_scaled[i], &params_c);
			result_it.E_fn[i] = energy_unscaling(result_scaled.E_fn_scaled[i], &params_c);
			result_it.E_fp[i] = energy_unscaling(result_scaled.E_fp_scaled[i], &params_c);
			result_it.J_n[i] = current_density_unscaling(result_scaled.J_n_scaled[i], &params_c);
			result_it.J_p[i] = current_density_unscaling(result_scaled.J_p_scaled[i], &params_c);
			result_it.J_a[i] = current_density_unscaling(result_scaled.J_a_scaled[i], &params_c);
			//result_it.J_c[i] = current_density_unscaling(result_scaled.J_c_scaled[i], &params_c);
			result_it.J_disp[i] = current_density_unscaling(result_scaled.J_disp_scaled[i], &params_c);
	}
	result_it.J_total = current_density_unscaling(result_scaled.J_total_scaled, &params_c);

	// Grid unscaling
	vector<double> tmp_li, tmp_xi, tmp_hi, tmp_Gi;
	grid.N_points = grid_scaled.N_points_scaled;
	grid.N_points_boundaries = grid_scaled.N_points_boundaries_scaled;

	for (size_t i = 0; i < grid_scaled.N_points_scaled; i++)
	{
		tmp_li.push_back(grid_scaled.li_scaled[i]);
		tmp_xi.push_back(space_unscaling(grid_scaled.xi_scaled[i], &params_c));
		tmp_Gi.push_back(generation_unscaling(grid_scaled.Gi_scaled[i], &params_c));
	}
	for (size_t i = 0; i <= grid_scaled.N_points_boundaries_scaled; i++)
		tmp_hi.push_back(space_unscaling(grid_scaled.hi_scaled[i], &params_c));

	grid.li = tmp_li;
	grid.xi = tmp_xi;
	grid.hi = tmp_hi;
	grid.Gi = tmp_Gi;
}


// PARAMETERS
double space_unscaling(double x_scaled, scaling_params * params)
{
	double tmp_space_unscaled;
	double x_c = params->x_c;
	
	// Unscaling space
	tmp_space_unscaled = x_scaled * x_c;

	return tmp_space_unscaled;
}

double time_unscaling(double t_scaled, scaling_params * params)
{
	double tmp_time_unscaled;
	double t_c = params->t_c;

	// Unscaling time
	tmp_time_unscaled = t_scaled * t_c;

	return tmp_time_unscaled;
}

double permittivity_unscaling(double Er_scaled, scaling_params * params)
{
	double tmp_Er_unscaled;
	double Er_max = params->Er_max;

	// Uncaling permittivity
	tmp_Er_unscaled = Er_scaled * Er_max;

	return tmp_Er_unscaled;
}

double concentrations_unscaling(double concentration_scaled, scaling_params * params)
{
	double tmp_concentration_unscaled;
	double N = params->N;

	// Uncaling concentration
	tmp_concentration_unscaled = concentration_scaled * N;

	return tmp_concentration_unscaled;
}

double mobility_unscaling(double u_scaled, scaling_params * params)
{
	double tmp_mobility_unscaled;
	double u_max = params->u_max;

	// Unscaling mobility
	tmp_mobility_unscaled = u_scaled * u_max;

	return tmp_mobility_unscaled;
}

double potential_unscaling(double V_scaled, scaling_params * params)
{
	double tmp_V_unscaled;
	double V_c = params->V_c;

	// Unscaling potential
	tmp_V_unscaled = V_scaled * V_c;

	return tmp_V_unscaled;
}

double electric_field_unscaling(double E_scaled, scaling_params * params)
{
	double tmp_E_unscaled;
	double F_c = params->F_c;

	// Unscaling electric field
	tmp_E_unscaled = E_scaled * F_c;

	return tmp_E_unscaled;
}

double current_density_unscaling(double J_scaled, scaling_params * params)
{
	double tmp_J_unscaled;
	double J_c = params->J_c;

	// Unscaling current
	tmp_J_unscaled = J_scaled * J_c;

	return tmp_J_unscaled;
}

double generation_unscaling(double G_scaled, scaling_params * params)
{
	double tmp_G_unscaled;
	double G_c = params->G_c;

	// Unscaling generation rate
	tmp_G_unscaled = G_scaled * G_c;

	return tmp_G_unscaled;
}

double bimolecular_unscaling(double B_scaled, scaling_params * params)
{
	double tmp_B_unscaled;
	double B_c = params->T_c;

	// Unscaling trimolecular recombination rate
	tmp_B_unscaled = B_scaled * B_c;

	return tmp_B_unscaled;
}

double trimolecular_unscaling(double T_scaled, scaling_params * params)
{
	double tmp_T_unscaled;
	double T_c = params->T_c;

	// Unscaling trimolecular recombination rate
	tmp_T_unscaled = T_scaled * T_c;

	return tmp_T_unscaled;
}

double energy_unscaling(double E_scaled, scaling_params * params)
{
	double tmp_E_unscaled;
	double E_c = params->E_c;

	// Unscaling energy
	tmp_E_unscaled = E_scaled * E_c;

	return tmp_E_unscaled;
}
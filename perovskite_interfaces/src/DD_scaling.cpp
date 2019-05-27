#include "../include/drift_diffusion.h"
#include "../include/scaling.h"

// High functions
void Drift_diffusion::device_params_scaling(unscaled_device_params * device_params_unscaled)
{
	scaled_device_params tmp_device_params_scaled;
	/* Device parameters scaling */
	tmp_device_params_scaled.activeLayer = device_params_unscaled->activeLayer;
	tmp_device_params_scaled.dt_scaled = time_scaling(device_params_unscaled->dt, &params_c);
	tmp_device_params_scaled.V_a_scaled = potential_scaling(device_params_unscaled->V_a, &params_c);
	tmp_device_params_scaled.V_build_scaled = potential_scaling(device_params_unscaled->V_build, &params_c);
	tmp_device_params_scaled.G_suns_scaled = device_params_unscaled->G_suns;
	tmp_device_params_scaled.W_a_scaled = energy_scaling(device_params_unscaled->W_a, &params_c);
	tmp_device_params_scaled.W_c_scaled = energy_scaling(device_params_unscaled->W_c, &params_c);
	tmp_device_params_scaled.epsilon = device_params_unscaled->epsilon;

	layers_params_scaled.device = tmp_device_params_scaled;
}


void Drift_diffusion::layer_params_scaling(unscaled_layer_params * layer_params_unscaled)
{
	scaled_layer_params tmp_layer_params_scaled;
	/* Layer parameters scaling */
	tmp_layer_params_scaled.name = layer_params_unscaled->name;
	tmp_layer_params_scaled.ID = layer_params_unscaled->ID;
	tmp_layer_params_scaled.E_r_scaled = permittivity_scaling(layer_params_unscaled->E_r, &params_c);
	tmp_layer_params_scaled.u_n_scaled = mobility_scaling(layer_params_unscaled->u_n, &params_c);
	tmp_layer_params_scaled.u_p_scaled = mobility_scaling(layer_params_unscaled->u_p, &params_c);
	tmp_layer_params_scaled.u_a_scaled = mobility_scaling(layer_params_unscaled->u_a, &params_c);
	tmp_layer_params_scaled.u_c_scaled = mobility_scaling(layer_params_unscaled->u_c, &params_c);
	tmp_layer_params_scaled.L_scaled = space_scaling(layer_params_unscaled->L, &params_c);
	tmp_layer_params_scaled.N_points = layer_params_unscaled->N_points;
	tmp_layer_params_scaled.C_nc_scaled = bimolecular_scaling(layer_params_unscaled->C_nc, &params_c);
	tmp_layer_params_scaled.C_nv_scaled = bimolecular_scaling(layer_params_unscaled->C_nv, &params_c);
	tmp_layer_params_scaled.C_pc_scaled = bimolecular_scaling(layer_params_unscaled->C_pc, &params_c);
	tmp_layer_params_scaled.C_pv_scaled = bimolecular_scaling(layer_params_unscaled->C_pv, &params_c);
	tmp_layer_params_scaled.n_int_scaled = concentrations_scaling(layer_params_unscaled->n_int, &params_c);
	tmp_layer_params_scaled.T_n_scaled = trimolecular_scaling(layer_params_unscaled->T_n, &params_c);
	tmp_layer_params_scaled.T_p_scaled = trimolecular_scaling(layer_params_unscaled->T_p, &params_c);
	tmp_layer_params_scaled.E_g_scaled = energy_scaling(layer_params_unscaled->E_g, &params_c);
	tmp_layer_params_scaled.E_c_scaled = energy_scaling(layer_params_unscaled->E_c, &params_c);
	tmp_layer_params_scaled.E_v_scaled = energy_scaling(layer_params_unscaled->E_v, &params_c);
	tmp_layer_params_scaled.E_tn_scaled = energy_scaling(layer_params_unscaled->E_tn, &params_c);
	tmp_layer_params_scaled.E_tp_scaled = energy_scaling(layer_params_unscaled->E_tp, &params_c);
	tmp_layer_params_scaled.W_tn_scaled = energy_scaling(layer_params_unscaled->W_tn, &params_c);
	tmp_layer_params_scaled.W_tp_scaled = energy_scaling(layer_params_unscaled->W_tp, &params_c);
	tmp_layer_params_scaled.N_D_scaled = concentrations_scaling(layer_params_unscaled->N_D, &params_c);	
	tmp_layer_params_scaled.N_A_scaled = concentrations_scaling(layer_params_unscaled->N_A, &params_c);
	tmp_layer_params_scaled.N_0_scaled = concentrations_scaling(layer_params_unscaled->N_0, &params_c);
	tmp_layer_params_scaled.N_tn_scaled = concentrations_scaling(layer_params_unscaled->N_tn, &params_c);
	tmp_layer_params_scaled.N_tp_scaled = concentrations_scaling(layer_params_unscaled->N_tn, &params_c);
	tmp_layer_params_scaled.N_c_scaled = concentrations_scaling(layer_params_unscaled->N_c, &params_c);
	tmp_layer_params_scaled.N_v_scaled = concentrations_scaling(layer_params_unscaled->N_v, &params_c);
	tmp_layer_params_scaled.ksi_scaled = layer_params_unscaled->ksi;

	layers_params_scaled.stack_params.push_back(tmp_layer_params_scaled);
}

void Drift_diffusion::generation_input_scaling(vector<double> * x_data, vector<double> * G_data, vector<double> * x_data_scaled, vector<double> * G_data_scaled)
{
	double N = x_data->size();

	for (int i = 0; i < N; ++i)
	{
		x_data_scaled->push_back(space_scaling(x_data->at(i), &params_c));
		G_data_scaled->push_back(generation_scaling(G_data->at(i), &params_c));
	}
}

// PARAMETERS
scaling_params c_parameters(double T)
{
	scaling_params parameters;
	
	parameters.x_c = 1E-9;
	parameters.Er_max = 100;
	parameters.u_max = 1E-2;
	parameters.t_c = (pow(parameters.x_c, 2) * C_q) / (parameters.u_max*C_k_B*T);
	parameters.N = (C_E_0*parameters.Er_max*C_k_B*T) / (pow(parameters.x_c, 2)*pow(C_q, 2));
	parameters.V_c = C_k_B*T / C_q;
	parameters.F_c = parameters.V_c / parameters.x_c;
	parameters.J_c = (parameters.u_max*parameters.N*C_k_B*T) / parameters.x_c;
	parameters.G_c = parameters.N / parameters.t_c;
	parameters.B_c = 1 / (parameters.N * parameters.t_c);
	parameters.T_c = 1 / (pow(parameters.N, 2) * parameters.t_c);
	parameters.E_c = C_k_B * T;

	return parameters;
}

double space_scaling(double x, scaling_params * params)
{
	double x_c, tmp_space_scaled;

	// Scaling space
	x_c = params->x_c;
	tmp_space_scaled = x / x_c;

	return tmp_space_scaled;
}

double time_scaling(double t, scaling_params * params)
{
	double tmp_time_scaled;
	double t_c = params->t_c;

	// Scaling time
	tmp_time_scaled = t / t_c;

	return tmp_time_scaled;
}

double permittivity_scaling(double Er, scaling_params * params)
{
	double tmp_Er_scaled;
	double Er_max = params->Er_max;

	// Scaling permittivity
	tmp_Er_scaled = Er / Er_max;

	return tmp_Er_scaled;
}

double concentrations_scaling(double concentration, scaling_params * params)
{
	double tmp_concentration_scaled;
	double N = params->N;

	// Scaling concentration
	tmp_concentration_scaled = concentration / N;

	return tmp_concentration_scaled;
}

double mobility_scaling(double u, scaling_params * params)
{
	double tmp_mobility_scaled;
	double u_max = params->u_max;

	// Scaling mobility
	tmp_mobility_scaled = u / u_max;

	return tmp_mobility_scaled;
}

double potential_scaling(double V, scaling_params * params)
{
	double tmp_V_scaled;
	double V_c = params->V_c;

	// Scaling potential
	tmp_V_scaled = V / V_c;

	return tmp_V_scaled;
}

double electric_field_scaling(double E, scaling_params * params)
{
	double tmp_E_scaled;
	double F_c = params->F_c;

	// Scaling electric field
	tmp_E_scaled = E / F_c;

	return tmp_E_scaled;
}

double current_density_scaling(double J, scaling_params * params)
{
	double tmp_J_scaled;
	double J_c = params->J_c;

	// Scaling current
	tmp_J_scaled = J / J_c;

	return tmp_J_scaled;
}

double energy_scaling(double E, scaling_params * params)
{
	double tmp_E;
	double E_c = params->E_c;

	// Scaling exciton binding energy
	tmp_E = E / E_c;

	return tmp_E;
}

double generation_scaling(double G, scaling_params * params)
{
	double tmp_G;
	double G_c = params->G_c;

	// Scaling exciton formation rate
	tmp_G = G / G_c;

	return tmp_G;
}

double bimolecular_scaling(double B, scaling_params * params)
{
	double tmp_B;
	double B_c = params->B_c;

	// Scaling bimolecular recombination rate
	tmp_B = B / B_c;

	return tmp_B;
}

double trimolecular_scaling(double T, scaling_params * params)
{
	double tmp_T;
	double T_c = params->T_c;

	// Scaling trimolecular recombination rate
	tmp_T = T / T_c;

	return tmp_T;
}
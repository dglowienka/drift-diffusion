#include "../include/drift_diffusion.h"

double Drift_diffusion::R_Ms(int i, scaled_layer_params * layer_params_scaled)
{
	double tmp_t_s_scaled = layer_params_scaled->t_s_scaled;
	double tmp_R_Ms = 1 / tmp_t_s_scaled;

	return tmp_R_Ms;
}

double Drift_diffusion::R_B(int i, scaled_layer_params * layer_params_scaled)
{
	double tmp_R_np;
	double E_r_scaled = grid_scaled.E_r_scaled[i];
	double u_n_scaled = grid_scaled.u_n_scaled[i];
	double u_p_scaled = grid_scaled.u_p_scaled[i];
	double ksi_scaled = layer_params_scaled->ksi_scaled;

	tmp_R_np = ksi_scaled * (u_n_scaled + u_p_scaled) / E_r_scaled;
	return tmp_R_np;
}
#include "../include/drift_diffusion.h"

double Drift_diffusion::k_d(int i, scaled_layer_params * layer_params_scaled)
{
	double tmp_D;
	double A_c = params_c.A_c;
	double b_c = params_c.b_c;
	double E_b_scaled = layer_params_scaled->E_b_scaled;
	double u_n_scaled = layer_params_scaled->u_n_scaled;
	double u_p_scaled = layer_params_scaled->u_p_scaled;
	double E_it_scaled = result_it_scaled.E_scaled[i];
	double E_r_scaled = grid_scaled.E_r_scaled[i];

	if (E_it_scaled != 0)
	{
		tmp_D = A_c*pow(E_r_scaled,2)*pow(E_b_scaled,3)*exp(-E_b_scaled)*(u_n_scaled + u_p_scaled)*
				(1 + b_c*E_it_scaled/E_r_scaled + pow((b_c*E_it_scaled/E_r_scaled), 2) / 3 + pow((b_c*E_it_scaled/E_r_scaled), 3) / 18 + pow((b_c*E_it_scaled/E_r_scaled), 4) / 180);
	}
	else tmp_D = 0;

	return tmp_D;
}
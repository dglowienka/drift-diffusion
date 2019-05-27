#include "../include/drift_diffusion.h"


double Drift_diffusion::R_Mn(void)
{
	// Single trap level SRH
	// double n_it_scaled = result_it_scaled.n_scaled[current_i_position];
	// double p_it_scaled = result_it_scaled.p_scaled[current_i_position];

	// double tmp_E_c = current_layer_params_scaled->E_c_scaled;
	// double tmp_E_v = current_layer_params_scaled->E_v_scaled;
	// double tmp_E_tn = current_layer_params_scaled->E_tn_scaled;
	// double tmp_N_c = current_layer_params_scaled->N_c_scaled;
	// double tmp_N_v = current_layer_params_scaled->N_v_scaled;
	// double tmp_n1_scaled = tmp_N_c*exp(tmp_E_tn-tmp_E_c);
	// double tmp_p1_scaled = tmp_N_v*exp(tmp_E_v-tmp_E_tn);
	// double tmp_N_tn_scaled = current_layer_params_scaled->N_tn_scaled;
	// double tmp_Cnc = current_layer_params_scaled->C_nc_scaled;
	// double tmp_Cpc = current_layer_params_scaled->C_pc_scaled;
	// double tmp_R_SRH = ( (tmp_Cnc*tmp_Cpc*tmp_N_tn_scaled) / (tmp_Cnc*(n_it_scaled+tmp_n1_scaled) + tmp_Cpc*(p_it_scaled+tmp_p1_scaled)) ) * ((n_it_scaled * p_it_scaled)-(tmp_n1_scaled*tmp_p1_scaled));
	// return tmp_R_SRH;
	// For trap distributions
	double tmp_E_c = current_layer_params_scaled->E_c_scaled;
	double tmp_E_v = current_layer_params_scaled->E_v_scaled;
	return integrate(&Drift_diffusion::R_M_conduction, tmp_E_v, tmp_E_c, 200);

	// For transient traps
	// return (R_Mn_D(i, layer_params_scaled) + R_Mn_A(i, layer_params_scaled))/2; 
}

double Drift_diffusion::R_M_conduction(double E)
{
	double n_it_scaled = result_it_scaled.n_scaled[current_i_position];
	double p_it_scaled = result_it_scaled.p_scaled[current_i_position];

	double tmp_n_int = current_layer_params_scaled->n_int_scaled;
	double tmp_N_tn = current_layer_params_scaled->N_tn_scaled;
	double tmp_N_c = current_layer_params_scaled->N_c_scaled;
	double tmp_N_v = current_layer_params_scaled->N_v_scaled;
	double tmp_Cnc = current_layer_params_scaled->C_nc_scaled;
	double tmp_Cpc = current_layer_params_scaled->C_pc_scaled;
	double tmp_E_c = current_layer_params_scaled->E_c_scaled;
	double tmp_E_v = current_layer_params_scaled->E_v_scaled;	
	double tmp_E_tn = current_layer_params_scaled->E_tn_scaled;
	double tmp_W_tn = current_layer_params_scaled->W_tn_scaled;
	double tmp_n1_scaled = tmp_N_c*exp(E-tmp_E_c);
	double tmp_p1_scaled = tmp_N_v*exp(tmp_E_v-E);

	double tmp_R_SRH = ( (tmp_N_tn / (tmp_W_tn*pow(2*C_PI,0.5)))*exp( -pow((E-tmp_E_tn),2) / (2*pow(tmp_W_tn,2)) ) ) 
	* ( (tmp_Cnc*tmp_Cpc) / (tmp_Cnc*(n_it_scaled+tmp_n1_scaled) + tmp_Cpc*(p_it_scaled+tmp_p1_scaled)) ) 
	* (n_it_scaled * p_it_scaled-pow(tmp_n_int,2));
	
    return tmp_R_SRH;
}
/*
double Drift_diffusion::R_Mn_D(int i, scaled_layer_params * layer_params_scaled)
{
	double n_it_scaled = result_it_scaled.n_scaled[i];
	double nt_it_scaled = result_it_scaled.n_t_scaled[i];

	double tmp_R_SRH;
	double tmp_t_n_scaled = layer_params_scaled->t_n_scaled;
	double tmp_n1_scaled = layer_params_scaled->n_1_scaled;
	double tmp_N_t_scaled = layer_params_scaled->N_t_scaled;
 
	tmp_R_SRH = (1 / (tmp_t_n_scaled*tmp_N_t_scaled))*(n_it_scaled*(tmp_N_t_scaled - nt_it_scaled) - tmp_n1_scaled*nt_it_scaled);
	return tmp_R_SRH;
}

double Drift_diffusion::R_Mn_A(int i, scaled_layer_params * layer_params_scaled)
{
	double n_it_scaled = result_it_scaled.n_scaled[i];
	double pt_it_scaled = result_it_scaled.p_t_scaled[i];

	double tmp_R_SRH;
	double tmp_t_n_scaled = layer_params_scaled->t_n_scaled;
	double tmp_n1_scaled = layer_params_scaled->n_1_scaled;
	double tmp_N_t_scaled = layer_params_scaled->N_t_scaled;
	
	tmp_R_SRH = (1 / (tmp_t_n_scaled*tmp_N_t_scaled))*(n_it_scaled*pt_it_scaled - tmp_n1_scaled*(tmp_N_t_scaled - pt_it_scaled));
	return tmp_R_SRH;
}
*/
double Drift_diffusion::R_Mp(void)
{
	// Single trap level SRH
	// double n_it_scaled = result_it_scaled.n_scaled[current_i_position];
	// double p_it_scaled = result_it_scaled.p_scaled[current_i_position];

	// double tmp_E_c = current_layer_params_scaled->E_c_scaled;
	// double tmp_E_v = current_layer_params_scaled->E_v_scaled;
	// double tmp_E_tp = current_layer_params_scaled->E_tp_scaled;
	// double tmp_N_c = current_layer_params_scaled->N_c_scaled;
	// double tmp_N_v = current_layer_params_scaled->N_v_scaled;
	// double tmp_n1_scaled = tmp_N_c*exp(tmp_E_tp-tmp_E_c);
	// double tmp_p1_scaled = tmp_N_v*exp(tmp_E_v-tmp_E_tp);
	// double tmp_N_tp_scaled = current_layer_params_scaled->N_tp_scaled;
	// double tmp_Cnv = current_layer_params_scaled->C_nv_scaled;
	// double tmp_Cpv = current_layer_params_scaled->C_pv_scaled;
	// double tmp_R_SRH = ( (tmp_Cnv*tmp_Cpv*tmp_N_tp_scaled) / (tmp_Cnv*(n_it_scaled+tmp_n1_scaled) + tmp_Cpv*(p_it_scaled+tmp_p1_scaled)) ) * ((n_it_scaled * p_it_scaled)-(tmp_n1_scaled*tmp_p1_scaled));
	// return tmp_R_SRH;

	// For trap distributions
	double tmp_E_c = current_layer_params_scaled->E_c_scaled;
	double tmp_E_v = current_layer_params_scaled->E_v_scaled;
	return integrate(&Drift_diffusion::R_M_valence, tmp_E_v, tmp_E_c, 200);
	
	// For transient traps
	// return (R_Mp_D(i, layer_params_scaled) + R_Mp_A(i, layer_params_scaled))/2;
}

double Drift_diffusion::R_M_valence(double E)
{
	double n_it_scaled = result_it_scaled.n_scaled[current_i_position];
	double p_it_scaled = result_it_scaled.p_scaled[current_i_position];

	double tmp_n_int = current_layer_params_scaled->n_int_scaled;
	double tmp_N_tp = current_layer_params_scaled->N_tp_scaled;
	double tmp_N_c = current_layer_params_scaled->N_c_scaled;
	double tmp_N_v = current_layer_params_scaled->N_v_scaled;
	double tmp_Cnv = current_layer_params_scaled->C_nv_scaled;
	double tmp_Cpv = current_layer_params_scaled->C_pv_scaled;
	double tmp_E_c = current_layer_params_scaled->E_c_scaled;
	double tmp_E_v = current_layer_params_scaled->E_v_scaled;	
	double tmp_E_tp = current_layer_params_scaled->E_tp_scaled;
	double tmp_W_tp = current_layer_params_scaled->W_tp_scaled;
	double tmp_n1_scaled = tmp_N_c*exp(E-tmp_E_c);
	double tmp_p1_scaled = tmp_N_v*exp(tmp_E_v-E);

	double tmp_R_SRH = ( (tmp_N_tp / (tmp_W_tp*pow(2*C_PI,0.5)))*exp( -pow((tmp_E_tp-E),2) / (2*pow(tmp_W_tp,2)) ) ) 
	* ( (tmp_Cnv*tmp_Cpv) / (tmp_Cnv*(n_it_scaled+tmp_n1_scaled) + tmp_Cpv*(p_it_scaled+tmp_p1_scaled)) ) 
	* (n_it_scaled * p_it_scaled-pow(tmp_n_int,2));
	
    return tmp_R_SRH;
}

/*
double Drift_diffusion::R_Mp_D(int i, scaled_layer_params * layer_params_scaled)
{
	double p_it_scaled = result_it_scaled.p_scaled[i];
	double nt_it_scaled = result_it_scaled.n_t_scaled[i];

	double tmp_R_SRH;
	double tmp_t_p_scaled = layer_params_scaled->t_p_scaled;
	double tmp_p1_scaled = layer_params_scaled->p_1_scaled;
	double tmp_N_t_scaled = layer_params_scaled->N_t_scaled;

	tmp_R_SRH = (1 / (tmp_t_p_scaled*tmp_N_t_scaled))*(p_it_scaled*nt_it_scaled - tmp_p1_scaled*(tmp_N_t_scaled - nt_it_scaled));
	return tmp_R_SRH;
}

double Drift_diffusion::R_Mp_A(int i, scaled_layer_params * layer_params_scaled)
{
	double p_it_scaled = result_it_scaled.p_scaled[i];
	double pt_it_scaled = result_it_scaled.p_t_scaled[i];

	double tmp_R_SRH;
	double tmp_t_p_scaled = layer_params_scaled->t_p_scaled;
	double tmp_p1_scaled = layer_params_scaled->p_1_scaled;
	double tmp_N_t_scaled = layer_params_scaled->N_t_scaled;

	tmp_R_SRH = (1 / (tmp_t_p_scaled*tmp_N_t_scaled))*(p_it_scaled*(tmp_N_t_scaled - pt_it_scaled) - tmp_p1_scaled*pt_it_scaled);
	return tmp_R_SRH;
}
*/
double Drift_diffusion::R_B(void)
{
	double tmp_R_np;
	double E_r_scaled = grid_scaled.E_r_scaled[current_i_position];
	double u_n_scaled = grid_scaled.u_n_scaled[current_i_position];
	double u_p_scaled = grid_scaled.u_p_scaled[current_i_position];
	double ksi_scaled = current_layer_params_scaled->ksi_scaled;

	tmp_R_np = ksi_scaled * (u_n_scaled + u_p_scaled) / E_r_scaled;
	return tmp_R_np;
}

double Drift_diffusion::R_T(void)
{
	double n_it_scaled = result_it_scaled.n_scaled[current_i_position];
	double p_it_scaled = result_it_scaled.p_scaled[current_i_position];

	double tmp_R_tn;
	double T_n_scaled = current_layer_params_scaled->T_n_scaled;
	double T_p_scaled = current_layer_params_scaled->T_p_scaled;

	tmp_R_tn = T_n_scaled*n_it_scaled + T_p_scaled*p_it_scaled;
	return tmp_R_tn;
}

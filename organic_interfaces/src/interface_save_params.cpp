#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>      // std::setprecision
#include "../include/command_interface.h"
#include "../include/constants.h"
#include "../lib/rapidxml/rapidxml.hpp"
#include "../lib/rapidxml/rapidxml_print.hpp"

using namespace rapidxml;
using namespace std;

string command_interface::doubleString(double value) {
	stringstream ss;
	ss.setf(ios::scientific);
	ss << setprecision(3);
	ss << value;
	return ss.str();
}

void command_interface::save_device_params(string config_dir_name)
{
	ofstream file_stored(config_dir_name);
	
	xml_document<> doc;
	xml_node <> * pRoot = doc.allocate_node(node_element, "document");
	
	/* Save device data */
	xml_node <> * pNode_device = doc.allocate_node(node_element, "Device");
	// T
	string T = doubleString(layers_params.device.T);
	const char* p_T = T.c_str();
	pNode_device->append_attribute(doc.allocate_attribute("T", p_T));
	// dt
	string dt = doubleString(layers_params.device.dt);
	const char* p_dt = dt.c_str();
	pNode_device->append_attribute(doc.allocate_attribute("dt", p_dt));
	// V_a
	string V_a = doubleString(layers_params.device.V_a);
	const char* p_V_a = V_a.c_str();
	pNode_device->append_attribute(doc.allocate_attribute("V_a", p_V_a));
	// V_build
	string V_build = doubleString(layers_params.device.V_build);
	const char* p_V_build = V_build.c_str();
	pNode_device->append_attribute(doc.allocate_attribute("V_build", p_V_build));
	// G_suns
	string G_suns = doubleString(layers_params.device.G_suns);
	const char* p_G_suns = G_suns.c_str();
	pNode_device->append_attribute(doc.allocate_attribute("G_suns", p_G_suns));
	// W_a
	string W_a = doubleString(layers_params.device.W_a / C_q);
	const char* p_W_a = W_a.c_str();
	pNode_device->append_attribute(doc.allocate_attribute("W_a", p_W_a));
	// W_c
	string W_c = doubleString(layers_params.device.W_c / C_q);
	const char* p_W_c = W_c.c_str();
	pNode_device->append_attribute(doc.allocate_attribute("W_c", p_W_c));
	// epsilon
	string epsilon = doubleString(layers_params.device.epsilon);
	const char* p_epsilon = epsilon.c_str();
	pNode_device->append_attribute(doc.allocate_attribute("epsilon", p_epsilon));
	pRoot->append_node(pNode_device);

	
	doc.append_node(pRoot);

	file_stored << doc;
	file_stored.close();
	doc.clear();
}

void command_interface::save_setting(string config_dir_name)
{
	xml_document<> doc;

	// Load file
	ifstream file(config_dir_name);
	stringstream buffer;
	buffer << file.rdbuf();
	file.close();

	string content(buffer.str());
	doc.parse<0>(&content[0]);

	xml_node<> *pRoot = doc.first_node();
	xml_node<> *pNode_settings = doc.clone_node( pRoot );

	pNode_settings = doc.allocate_node(node_element, "Settings");
	// first_pulse_time
	string first_pulse = doubleString(settings.first_pulse);
	const char* p_first_pulse = first_pulse.c_str();
	pNode_settings->append_attribute(doc.allocate_attribute("first_pulse_time", p_first_pulse));
	// second_pulse_time
	string second_pulse = doubleString(settings.second_pulse);
	const char* p_sec_pulse = second_pulse.c_str();
	pNode_settings->append_attribute(doc.allocate_attribute("second_pulse_time", p_sec_pulse));
	// third_pulse_time
	string third_pulse = doubleString(settings.third_pulse);
	const char* p_third_pulse = third_pulse.c_str();
	pNode_settings->append_attribute(doc.allocate_attribute("third_pulse_time", p_third_pulse));
	// V_min
	string V_min = doubleString(settings.V_min);
	const char* p_V_min = V_min.c_str();
	pNode_settings->append_attribute(doc.allocate_attribute("V_min", p_V_min));
	// V_max
	string V_max = doubleString(settings.V_max);
	const char* p_V_max = V_max.c_str();
	pNode_settings->append_attribute(doc.allocate_attribute("V_max", p_V_max));
	// V_rate
	string V_rate = doubleString(settings.V_rate);
	const char* p_V_rate = V_rate.c_str();
	pNode_settings->append_attribute(doc.allocate_attribute("V_rate", p_V_rate));
	// V_step
	string V_step = doubleString(settings.V_step);
	const char* p_V_step = V_step.c_str();
	pNode_settings->append_attribute(doc.allocate_attribute("V_step", p_V_step));
	
	doc.first_node("document")->append_node( pNode_settings ); /* Appending node a to the tree in src */

	// Save file
	ofstream file_stored(config_dir_name);
	file_stored << "<? Solar Cell Simulation ?>" << endl;
	file_stored << doc;
	file_stored.close();

	doc.clear();
}

void command_interface::save_layer_params(string config_dir_name, unscaled_layer_params * layer_params)
{
	xml_document<> doc;
	const char * tmp_layer_name = layer_params->name.c_str();;

	// Load file
	ifstream file(config_dir_name);
	stringstream buffer;
	buffer << file.rdbuf();
	file.close();

	string content(buffer.str());
	doc.parse<0>(&content[0]);

	xml_node<> *pRoot = doc.first_node(); /* Node to append */
	xml_node<> *pNode_layer = doc.clone_node( pRoot );
	
	/* Save layer data */
	pNode_layer = doc.allocate_node(node_element, tmp_layer_name);
	// name
	const char* p_name = layer_params->name.c_str();
	pNode_layer->append_attribute(doc.allocate_attribute("name", p_name));
	// ID
	string ID = to_string(layer_params->ID);
	const char* p_ID = ID.c_str();
	pNode_layer->append_attribute(doc.allocate_attribute("ID", p_ID));
	// E_r
	string E_r = doubleString(layer_params->E_r);
	const char* p_E_r = E_r.c_str();
	pNode_layer->append_attribute(doc.allocate_attribute("E_r", p_E_r));
	// u_n
	string u_n = doubleString(layer_params->u_n);
	const char* p_u_n = u_n.c_str();
	pNode_layer->append_attribute(doc.allocate_attribute("u_n", p_u_n));
	// u_p
	string u_p = doubleString(layer_params->u_p);
	const char* p_u_p = u_p.c_str();
	pNode_layer->append_attribute(doc.allocate_attribute("u_p", p_u_p));
	// u_s
	string u_s = doubleString(layer_params->u_s);
	const char* p_u_s = u_s.c_str();
	pNode_layer->append_attribute(doc.allocate_attribute("u_s", p_u_s));
	// E_b
	string E_b = doubleString(layer_params->E_b / C_q);
	const char* p_E_b = E_b.c_str();
	pNode_layer->append_attribute(doc.allocate_attribute("E_b", p_E_b));
	// y_ns
	string y_ns = doubleString(layer_params->y_ns);
	const char* p_y_ns = y_ns.c_str();
	pNode_layer->append_attribute(doc.allocate_attribute("y_ns", p_y_ns));
	// y_ps
	string y_ps = doubleString(layer_params->y_ps);
	const char* p_y_ps = y_ps.c_str();
	pNode_layer->append_attribute(doc.allocate_attribute("y_ps", p_y_ps));
	// t_s
	string t_s = doubleString(layer_params->t_s);
	const char* p_t_s = t_s.c_str();
	pNode_layer->append_attribute(doc.allocate_attribute("t_s", p_t_s));
	// L
	string L = doubleString(layer_params->L);
	const char* p_L = L.c_str();
	pNode_layer->append_attribute(doc.allocate_attribute("L", p_L));
	// N_points
	string N_points = doubleString(layer_params->N_points);
	const char* p_N_points = N_points.c_str();
	pNode_layer->append_attribute(doc.allocate_attribute("N_points", p_N_points));
	// E_c
	string E_c = doubleString(layer_params->E_c / C_q);
	const char* p_E_c = E_c.c_str();
	pNode_layer->append_attribute(doc.allocate_attribute("E_c", p_E_c));
	// E_v
	string E_v = doubleString(layer_params->E_v / C_q);
	const char* p_E_v = E_v.c_str();
	pNode_layer->append_attribute(doc.allocate_attribute("E_v", p_E_v));
	// N_D
	string N_D = doubleString(layer_params->N_D);
	const char* p_N_D = N_D.c_str();
	pNode_layer->append_attribute(doc.allocate_attribute("N_D", p_N_D));
	// N_A
	string N_A = doubleString(layer_params->N_A);
	const char* p_N_A = N_A.c_str();
	pNode_layer->append_attribute(doc.allocate_attribute("N_A", p_N_A));
	// N_c
	string N_c = doubleString(layer_params->N_c);
	const char* p_N_c = N_c.c_str();
	pNode_layer->append_attribute(doc.allocate_attribute("N_c", p_N_c));
	// N_v
	string N_v = doubleString(layer_params->N_v);
	const char* p_N_v = N_v.c_str();
	pNode_layer->append_attribute(doc.allocate_attribute("N_v", p_N_v));
	// ksi
	string ksi = doubleString(layer_params->ksi);
	const char* p_ksi = ksi.c_str();
	pNode_layer->append_attribute(doc.allocate_attribute("ksi", p_ksi));

	doc.first_node("document")->append_node( pNode_layer ); /* Appending node a to the tree in src */

	// Save file
	ofstream file_stored(config_dir_name);
	file_stored << doc;
	file_stored.close();
	doc.clear();
}
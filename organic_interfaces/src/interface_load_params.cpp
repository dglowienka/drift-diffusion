#include "../include/command_interface.h"
#include "../lib/rapidxml/rapidxml.hpp"

using namespace rapidxml;
using namespace std;

bool command_interface::load_device_params(string config_dir_name)
{
	unscaled_device_params tmp_device_params;
	xml_document<> doc;
	
	ifstream file(config_dir_name);
	stringstream buffer;
	buffer << file.rdbuf();
	file.close();

	string content(buffer.str());
	doc.parse<0>(&content[0]);

	xml_node<> *pRoot = doc.first_node(); // This is the <document/> node
	for(xml_node<> *pNode=pRoot->first_node(); pNode; pNode=pNode->next_sibling())
	{
		/* Load Device data */
		if (!strcmp(pNode->name(), "Device"))
		{
			tmp_device_params.activeLayer = atof(pNode->first_attribute("activeLayer")->value());
			tmp_device_params.T = atof(pNode->first_attribute("T")->value());
			tmp_device_params.dt = atof(pNode->first_attribute("dt")->value());
			tmp_device_params.V_a = atof(pNode->first_attribute("V_a")->value());
			tmp_device_params.V_build = atof(pNode->first_attribute("V_build")->value());
			tmp_device_params.G_suns = atof(pNode->first_attribute("G_suns")->value());
			tmp_device_params.W_a = atof(pNode->first_attribute("W_a")->value())*C_q;
			tmp_device_params.W_c = atof(pNode->first_attribute("W_c")->value())*C_q;
			tmp_device_params.epsilon = atof(pNode->first_attribute("epsilon")->value());

			/* Copy parameters */
			layers_params.device = tmp_device_params;
			return true;
		}
	}
	return false;
}

bool command_interface::load_stack_params(string config_dir_name)
{
	bool isLayerOK = false;
	xml_document<> doc;
	ifstream file(config_dir_name);
	stringstream buffer;
	buffer << file.rdbuf();
	file.close();

	string content(buffer.str());
	doc.parse<0>(&content[0]);

	xml_node<> *pRoot = doc.first_node(); // This is the <document/> node
	for(xml_node<> *pNode=pRoot->first_node(); pNode; pNode=pNode->next_sibling())
	{
		if (strcmp(pNode->name(), "Device") && strcmp(pNode->name(), "Settings")) // Only layers
			isLayerOK = load_layer_params(config_dir_name, pNode->name());
	}
	return isLayerOK;
}

bool command_interface::load_layer_params(string config_dir_name, string layer_name)
{
	unscaled_layer_params tmp_layer_params;
	const char * tmp_layer_name = layer_name.c_str();;
	xml_document<> doc;
	
	ifstream file(config_dir_name);
	stringstream buffer;
	buffer << file.rdbuf();
	file.close();

	string content(buffer.str());
	doc.parse<0>(&content[0]);

	xml_node<> *pRoot = doc.first_node(); // This is the <document/> node
	for(xml_node<> *pNode=pRoot->first_node(); pNode; pNode=pNode->next_sibling())
	{
		/* Load solar cell data */
		if (!strcmp(pNode->name(), tmp_layer_name))
		{
			tmp_layer_params.name = tmp_layer_name;
			tmp_layer_params.ID = layers_params.stack_params.size();
			tmp_layer_params.E_r = atof(pNode->first_attribute("E_r")->value());
			tmp_layer_params.u_n = atof(pNode->first_attribute("u_n")->value());
			tmp_layer_params.u_p = atof(pNode->first_attribute("u_p")->value());
			tmp_layer_params.u_s = atof(pNode->first_attribute("u_s")->value());
			tmp_layer_params.E_b = atof(pNode->first_attribute("E_b")->value())*C_q;
			tmp_layer_params.y_ns = atof(pNode->first_attribute("y_ns")->value());
			tmp_layer_params.y_ps = atof(pNode->first_attribute("y_ps")->value());
			tmp_layer_params.t_s = atof(pNode->first_attribute("t_s")->value());
			tmp_layer_params.L = atof(pNode->first_attribute("L")->value());
			tmp_layer_params.N_points = atof(pNode->first_attribute("N_points")->value());
			tmp_layer_params.E_c = atof(pNode->first_attribute("E_c")->value())*C_q;
			tmp_layer_params.E_v = atof(pNode->first_attribute("E_v")->value())*C_q;
			tmp_layer_params.N_D = atof(pNode->first_attribute("N_D")->value());
			tmp_layer_params.N_A = atof(pNode->first_attribute("N_A")->value());			
			tmp_layer_params.N_c = atof(pNode->first_attribute("N_c")->value());
			tmp_layer_params.N_v = atof(pNode->first_attribute("N_v")->value());
			tmp_layer_params.ksi = atof(pNode->first_attribute("ksi")->value());

			// Calculate variables
			tmp_layer_params.E_g = abs(tmp_layer_params.E_c - tmp_layer_params.E_v);
			tmp_layer_params.n_int = tmp_layer_params.N_c*exp((-tmp_layer_params.E_g) / (2*C_k_B*layers_params.device.T));
			
			/* Copy parameters */
			layers_params.stack_params.push_back(tmp_layer_params);
			return true;
		}
	}
	return false;
}

bool command_interface::load_settings(string config_dir_name)
{
	struct settings tmp_settings;
	xml_document<> doc;
	
	ifstream file(config_dir_name);
	stringstream buffer;
	buffer << file.rdbuf();
	file.close();

	string content(buffer.str());
	doc.parse<0>(&content[0]);

	xml_node<> *pRoot = doc.first_node(); // This is the <document/> node
	for(xml_node<> *pNode=pRoot->first_node(); pNode; pNode=pNode->next_sibling())
	{
		/* Load settings */
		if (!strcmp(pNode->name(), "Settings"))
		{		
			tmp_settings.first_pulse = atof(pNode->first_attribute("first_pulse_time")->value());
			tmp_settings.second_pulse = atof(pNode->first_attribute("second_pulse_time")->value());
			tmp_settings.third_pulse = atof(pNode->first_attribute("third_pulse_time")->value());
			tmp_settings.V_max = atof(pNode->first_attribute("V_max")->value());
			tmp_settings.V_min = atof(pNode->first_attribute("V_min")->value());
			tmp_settings.V_step = atof(pNode->first_attribute("V_step")->value());
			tmp_settings.V_rate = atof(pNode->first_attribute("V_rate")->value());
			
			/* Copy parameters */
			settings = tmp_settings;
			return true;
		}
	}
	return false;
}

bool command_interface::load_generation_file(string gener_dir_name)
{
	double tmp_x, tmp_G;
	vector<double> tmp_x_vector, tmp_G_vector;
	
	ifstream file(gener_dir_name);

	if(file.fail())
		return false; // No file

	while(!file.eof()) // Read file untill the last line
	{ 
		file >> tmp_x >> tmp_G; // read two columns of data
		tmp_x_vector.push_back(tmp_x);
		tmp_G_vector.push_back(tmp_G);
	} 
	file.close(); 

	/* Copy parameters */
	layers_params.x_data = tmp_x_vector;
	layers_params.G_data = tmp_G_vector;

	return true;
}
#include <iostream>
#include "../include/command_interface.h"

void command_interface::show_command_list(void)
{
	cout << "Command list:" << endl;
	// assuming there is the same number of descriptors as commands
	list<string>::iterator it_description = command_description.begin();
	list<string>::iterator it_sec_param = parameter.begin();
	for (list<string>::iterator it = command_list.begin(); it != command_list.end(); ++it)
	{
		cout << (*it) << " " << (*it_sec_param) << " - " << (*it_description) << endl;
		it_description++;
		it_sec_param++;
	}
}

void command_interface::show_author(void)
{
	cout << "______      _  __ _         ______ _  __  __           _             " << endl;
	cout << "|  _  \\    (_)/ _| |        |  _  (_)/ _|/ _|         (_)            " << endl;
	cout << "| | | |_ __ _| |_| |_ ______| | | |_| |_| |_ _   _ ___ _  ___  _ __  " << endl;
	cout << "| | | | '__| |  _| __|______| | | | |  _|  _| | | / __| |/ _ \\| '_ \\ " << endl;
	cout << "| |/ /| |  | | | | |_       | |/ /| | | | | | |_| \\__ | | (_) | | | |" << endl;
	cout << "|___/ |_|  |_|_|  \\__|      |___/ |_|_| |_|  \\__,_|___|_|\\___/|_| |_|" << endl;
	cout << "                                                                     " << endl;
	cout << "                                  2019 Copyright (c) Damian Glowienka" << endl;
	cout << "                                                                     " << endl;
}
//============================================================================
// Name        : MapSortedByInsertion.cpp
// Author      : Scott Rasmussen (ZaitaNZ)
// Version     : 1.0
// Copyright   : MIT License - http://choosealicense.com/licenses/mit/
// Description : This code is used to describe two different ways of
// 				generating a map object that stores objects by insertion
//============================================================================

#include <iostream>
#include <string>

#include "std_map.h"
#include "custom_map.h"

using namespace std;

void print_std_map(std_map<string, double>& map_) {
	for (auto iter : map_)
		cout << iter.first << ": " << iter.second << " || ";
	cout << endl;
}

void print_custom_map(custom_map<string, double>& map_) {
	for (auto iter : map_)
		cout << iter->first << ": " << iter->second << " || ";
	cout << endl;
}

/**
 * main method
 */
int main() {
	cout << "-- Using std_map_" << endl;
	std_map<string, double> std_map_;
	std_map_["c"] = 1.0;
	std_map_["b"] = 2.0;
	std_map_["a"] = 3.0;
	std_map_["d"] = 4.0;
	std_map_["f"] = 5.0;
	std_map_["e"] = 6.0;
	cout << "-- Initial Print of std_map_: " << endl;
	print_std_map(std_map_);

	cout << endl;
	cout << "-- Using custom_map_" << endl;
	custom_map<string, double> custom_map_;
	custom_map_["c"] = 1.0;
	custom_map_["b"] = 2.0;
	custom_map_["a"] = 3.0;
	custom_map_["d"] = 4.0;
	custom_map_["f"] = 5.0;
	custom_map_["e"] = 6.0;
	cout << "-- Initial Print of custom_map_: " << endl;
	print_custom_map(custom_map_);


	return 0;
}


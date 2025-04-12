#pragma once
#include <string>
#include <vector>
#include <map>

namespace iojson {


	bool InputData(std::string& string, std::string fname);
	bool SaveData(int n, std::string fname);
	bool LoadJson(int& string, std::string fname);
}
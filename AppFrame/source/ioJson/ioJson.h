#pragma once
#include <string>
#include <vector>
#include <map>

namespace iojson {

	bool InputScoreJson(const std::string path, int& bpm, std::vector <int>& score);
	bool InputMotionAttri(std::map<int, uint_fast8_t>& data);
	bool InputData(std::string& string, std::string fname);
	bool RoadBoard(std::vector<std::pair<std::string, int>>& board, std::string fname);
	bool SaveBoard(std::vector<std::pair<std::string, int>>& board, std::string fname);
}
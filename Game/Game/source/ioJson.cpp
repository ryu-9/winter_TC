#include <string>
#include <fstream>
#include <unordered_map>
#include "ioJson.h"
#include "ioJsonText.h"

namespace iojson {
	
	// ���ʃ��[�h�֐�
	bool InputScoreJson(const std::string path, int& bpm, std::vector<int>& score)
	{
		std::ifstream file(path + "_score.json");
		nlohmann::json json;
		file >> json;

		json.at("bpm").get_to(bpm);
		if (json.find("score") != json.end()) {
			auto scores = json.at("score");
			for (auto& sc : scores) {
				// ���f�[�^�ɒǉ�
				score.emplace_back(sc);
			}
		}
			return true;
	}

	bool InputMotionAttri(std::map<int, uint_fast8_t>& data)
	{
		std::ifstream file("res/mot.json");
		nlohmann::json json;
		file >> json;
			

		auto mots = json.at("mot");
		for (auto i = 0; i < mots.size(); i++) {
			// ���f�[�^�ɒǉ�
			data[i] = mots[i];
		}
		
		return true;
	}

	bool InputData(std::string& str,std::string fname) {
		std::ifstream file(fname);
		nlohmann::json json;
		file >> json;

		auto text = json.at("text");
		for (auto i = 0; i < text.size(); i++) {
			// ���f�[�^�ɒǉ�
			str += ConvertString(text[i]);
		}
		

		return true;
	}

	
}
#include <string>
#include <fstream>
#include <unordered_map>
#include "ioJson.h"
#include "ioJsonText.h"

namespace iojson {
	


	bool InputMotionAttri(std::map<int, uint_fast8_t>& data)
	{
		std::ifstream file("res/mot.json");
		nlohmann::json json;
		file >> json;
			

		auto mots = json.at("mot");
		for (auto i = 0; i < mots.size(); i++) {
			// 元データに追加
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
			// 元データに追加
			str += ConvertString(text[i]);
		}
		

		return true;
	}

	bool SaveData(int n, std::string fname) {
		std::ofstream file(fname);
		nlohmann::json json;
		json["text"] = n;
		file << json.dump(4);

		file.close();
		return true;
	}

	bool LoadJson(int& n, std::string fname) {
		std::ifstream file(fname);
		nlohmann::json json;
		file >> json;
		auto text = json.at("text");

		n = text;
		return true;
	}

	
}
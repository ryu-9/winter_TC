#include "ModeScenario.h"
#include "nlohmann/json.hpp"
#include "ioJsonText.h"
#include <fstream>


bool ModeScenario::Initialize() {
	if (!base::Initialize()) { return false; }
	LoadScenario("res/New2.json");
	return true;
}

bool ModeScenario::Terminate() {
	return false;
}

bool ModeScenario::Process() {
	_Tm += GetStepTm();
	return true;
}

bool ModeScenario::Render() {
	base::Render();
	DrawFormatString(0, 0, GetColor(255, 255, 255), "Scenario");
	DrawFormatString(0, 20, GetColor(255, 255, 255), _ScenarioData[0].name.c_str());
	for (int i = 0; i < _Tm / 500; i++) {
		auto t = _ScenarioData[0].text.c_str()[i];
		DrawFormatString(20 * i , 40, GetColor(255, 255, 255), (const TCHAR*)t);
	}
	return true;
}

bool ModeScenario::LoadScenario(const char* filename) {
	std::ifstream file(filename);
	nlohmann::json json;
	file >> json;
	file.close();

	nlohmann::json scenario = json["Scenario"];
	for (auto& s : scenario) {
		SCENARIO_DATA data;
		data.name = iojson::ConvertString(s["name"]);
		data.text = iojson::ConvertString(s["text"]);
		_ScenarioData.push_back(data);
	}

	return true;
}

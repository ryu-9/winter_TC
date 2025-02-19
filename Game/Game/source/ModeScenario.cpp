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
	_CurrentTime += GetStepTm();
	if (_CurrentTime > _Index * 100) {
		_Index+= 2;
		if (_Index > _ScenarioData[0].text.size()) {
			_Index = _ScenarioData[0].text.size();
		}
	}

	// â¸çséûÇÃÇ∏ÇÍÇèCê≥
	if (_ScenarioData[0].text.substr(_Index, 1) == "\n") {
		_Index++;
	}
	if (_ScenarioData[0].text.substr(_Index) == "<") {
		
	}
	
	return true;
}

bool ModeScenario::Render() {
	base::Render();
	DrawFormatString(0, 0, GetColor(255, 255, 255), "Scenario");
	DrawFormatString(0, 20, GetColor(255, 255, 255), _ScenarioData[0].name.c_str());
	
		std::string text = _ScenarioData[0].text.substr(0, _Index);
		DrawFormatString(0, 40, GetColor(255, 255, 255),"%s", text.c_str());
	
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

#include "ModeScenario.h"
#include "nlohmann/json.hpp"
#include "ioJsonText.h"
#include "ApplicationMain.h"
#include <fstream>


bool ModeScenario::Initialize() {
	if (!base::Initialize()) { return false; }
	LoadScenario("res/New2.json");

	_CurrentTime = 0;
	_Time = 0;
	_Index = 0;
	_TextNum = 0;

	return true;
}

bool ModeScenario::Terminate() {
	return false;
}

bool ModeScenario::Process() {
	_CurrentTime += GetStepTm();
	int trg = ApplicationMain::GetInstance()->GetTrg();
	auto n = 0;
	if (_CurrentTime > _Index * 50) {
		n +=2;
	}
	if (_Index + n < _ScenarioData[0].text.size()) {
		_Index += n;
		// ‰üsŽž‚Ì‚¸‚ê‚ðC³
		if (_ScenarioData[0].text.substr(_Index, 1) == "\n") {
			_Index++;
		}
		if (_ScenarioData[0].text.substr(_Index) == "<") {

		}
	} else {
		_Index = _ScenarioData[0].text.size();
		if (trg & PAD_INPUT_1) {
			_Index = 0;
			_CurrentTime = 0;
			_TextNum++;
		}
	}
	
	
	
	return true;
}

bool ModeScenario::Render() {
	base::Render();
	DrawFormatString(0, 0, GetColor(255, 255, 255), "Scenario");
	DrawFormatString(0, 20, GetColor(255, 255, 255), _ScenarioData[_TextNum].name.c_str());
	
		std::string text = _ScenarioData[_TextNum].text.substr(0, _Index);
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

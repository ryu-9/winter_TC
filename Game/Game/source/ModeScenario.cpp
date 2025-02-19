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
	_TextData.push_back(TEXT_DATA());
	return true;
}

bool ModeScenario::Terminate() {
	return false;
}

bool ModeScenario::Process() {
	_CurrentTime += GetStepTm();
	int trg = ApplicationMain::GetInstance()->GetTrg();
	if (_CurrentTime > _Index * 100) {
		AddText();
	}
	
		if (_ScenarioData[0].text.size() <= _Index) {
		_Index = _ScenarioData[0].text.size();
		if (trg & PAD_INPUT_1) {
			_Index = 0;
			_CurrentTime = 0;
			_TextNum++;
			_TextData.clear();
		}
	}
	
	
	
	return true;
}

bool ModeScenario::Render() {
	base::Render();
	DrawFormatString(0, 0, GetColor(255, 255, 255), "Scenario");
	DrawFormatString(0, 20, GetColor(255, 255, 255), _ScenarioData[_TextNum].name.c_str());
	auto t = iojson::ConvertString(_TextData.back().text) + "\n";
	DrawFormatString(0, 40, GetColor(255, 255, 255), t.c_str());
	
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
		data.text = s["text"];
		_ScenarioData.push_back(data);
	}

	return true;
}

int ModeScenario::Check(const unsigned char uc) {
	if ((uc & 0x80) == 0x00) { return 1; }	// 0*** ****
	if ((uc & 0xe0) == 0xc0) { return 2; }	// 110* ****
	if ((uc & 0xf0) == 0xe0) { return 3; }	// 1110 ****
	if ((uc & 0xf8) == 0xf0) { return 4; }	// 1111 0***
	if ((uc & 0xfc) == 0xf8) { return 5; }	// 1111 10**
	if ((uc & 0xfe) == 0xfc) { return 6; }	// 1111 110*
	return 0;	// 1•¶Žš–Ú‚¶‚á‚È‚¢	
}

bool ModeScenario::AddText() {
	if (_Index < _ScenarioData[_TextNum].text.size()) {


		auto s = _ScenarioData[_TextNum].text.substr(_Index);
		auto n = Check(static_cast<unsigned char>(s[0]));
		if (n > 0) {
			//_Index += n;
		} else {
			n++;
		}
		// TEXT_DATA‚Éˆê•¶Žš’Ç‰Á
		_TextData.back().text += s.substr(0, n);
		_Index += n;

		if (_ScenarioData[0].text.substr(_Index) == "<") {
			

		}
	}
	return true;
}

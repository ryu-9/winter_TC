#include "ModeScenario.h"
#include "nlohmann/json.hpp"
#include "ioJsonText.h"
#include "ApplicationMain.h"
#include <fstream>

namespace {
	int X = 650;			// X座標基準値
	int Y = 900;			// Y座標基準値
	int FONT_SIZE = 20;		// フォントサイズ
	int TEXT_SPEED = 70;	// テキスト表示速度
}


bool ModeScenario::Initialize() {
	if (!base::Initialize()) { return false; }
	LoadScenario("res/loadtext/LoadText.json","Scenario");
	SetFontSize(FONT_SIZE);
	_CurrentTime = 0;
	_Time = 0;
	_StCount = 0;
	_TextIndex = 0;
	_TextCount = 0;
	_TextData.push_back(TEXT_DATA());

	new UIChipClass(this,VGet(960,965,0), "res/UI/UI_MESSAGE.png");
	return true;
}

bool ModeScenario::Terminate() {
	base::Terminate();
	return false;
}

bool ModeScenario::Process() {
	_CurrentTime += GetStepTm();
	// このモードより下のレイヤーはProcess()を呼ばない
	ModeServer::GetInstance()->SkipProcessUnderLayer();
	int trg = ApplicationMain::GetInstance()->GetTrg();
	if (_CurrentTime > _TextCount * TEXT_SPEED) {
		AddText();
	}

	if (trg & PAD_INPUT_1) {
		if (_ScenarioData[_TextIndex].text.size() <= _StCount) {
			_StCount = 0;
			_TextCount = 0;
			_CurrentTime = 0;
			_TextIndex++;
			if (_TextIndex >= _ScenarioData.size()) {
				ModeServer::GetInstance()->Del(this);
				return true;
			}
			
			_TextData.clear();
			_TextData.push_back(TEXT_DATA());
		} else {
			for (auto i = 0; i < _ScenarioData[_TextIndex].text.size(); i++) {
				AddText();
			}
		}

	}

	return true;
}

bool ModeScenario::Render() {
	base::Render();
	int x = X;
	int y = Y;
	DrawFormatString(x, y, GetColor(255, 255, 255), _ScenarioData[_TextIndex].name.c_str());
	y += FONT_SIZE;
	for (int i = 0; i < _TextData.size(); i++) {
		auto text = iojson::ConvertString(_TextData[i].text);
		if (_TextData[i].br) {
			y += FONT_SIZE;
			x = X;
		}
		DrawFormatString(x, y, _TextData[i].col, text.c_str());
		x += GetDrawFormatStringWidth(text.c_str());
	}
	
	return true;
}

bool ModeScenario::LoadScenario(const char* filename,std::string index) {
	std::ifstream file(filename);
	nlohmann::json json;
	file >> json;
	file.close();

	nlohmann::json scenario = json[index];
	for (auto& s : scenario) {
		SCENARIO_DATA data;
		data.name = iojson::ConvertString(s["name"]);
		data.text = s["text"];
		_ScenarioData.push_back(data);
	}
	// TODO: 状況に合わせたデータの読み込み
	return true;
}

// 文字数チェック
int ModeScenario::Check(const unsigned char uc) {
	if ((uc & 0x80) == 0x00) { return 1; }	// 0*** ****
	if ((uc & 0xe0) == 0xc0) { return 2; }	// 110* ****
	if ((uc & 0xf0) == 0xe0) { return 3; }	// 1110 ****
	if ((uc & 0xf8) == 0xf0) { return 4; }	// 1111 0***
	if ((uc & 0xfc) == 0xf8) { return 5; }	// 1111 10**
	if ((uc & 0xfe) == 0xfc) { return 6; }	// 1111 110*
	return 0;	// 1文字目じゃない	
}

// 描画用テキスト追加
bool ModeScenario::AddText() {
	if (_StCount < _ScenarioData[_TextIndex].text.size()) {
		std::string s;
		int n = 0;
		
		while (1) {
			bool br = false;
			s = _ScenarioData[_TextIndex].text.substr(_StCount);
			n = Check(static_cast<unsigned char>(s[0]));
			if (n > 0) {
				//_Index += n;
			} else {
				n++;
			}
			if (s[0] == '<') {
				_StCount++;
				_TextData.push_back(TEXT_DATA());
				if (s[1] == 'C') {
					_StCount++;
					_TextData.back().col = GetColor(225, 25, 25);
				}
				br = true;
			}
			if (s[0] == '>') {
				_TextData.push_back(TEXT_DATA());
				_StCount++;
				br = true;
			}
			if (s[0] == '\n') {
				_TextData.push_back(TEXT_DATA());
				_TextData.back().br = true;
				_StCount++;
				br = true;
			}
			if (br == false) {
				break;
			}
		}

		// TEXT_DATAに一文字追加
		_TextData.back().text += s.substr(0, n);
		_StCount += n;
		_TextCount++;
		
	}
	return true;
}

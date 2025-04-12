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
	return true;
}

bool ModeScenario::Terminate() {
	base::Terminate();
	return true;
}

bool ModeScenario::Process() {
	return true;
}

bool ModeScenario::Render() {
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
			s = _ScenarioData[_TextIndex].text.substr(_StCount);		// 次の文字を取得
			n = Check(static_cast<unsigned char>(s[0]));					// 文字数チェック
			if (n > 0) {
				//_Index += n;
			} else {
				n++;
			}

			// 改行、<C>タグの処理
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

#include "ModeResult.h"
#include "ApplicationMain.h"
#include "ModeTitle.h"
#include <fstream>
#include "ApplicationGlobal.h"

bool ModeResult::Initialize()
{
	if (!base::Initialize()) { return false; }

	
	LPCSTR font_path = "res/x14y20pxScoreDozer.ttf"; // 読み込むフォントファイルのパス
	if (AddFontResourceEx(font_path, FR_PRIVATE, NULL) > 0) {
	}
	else {
	// フォント読込エラー処理
	MessageBox(NULL, "フォント読込失敗", "", MB_OK);
	}
	

	ChangeFont("x14y20pxScoreDozer");
	SetFontSize(48);

	_Name = "YOU";
	_type = 2;
	_Cur = 0;
	_PlayerRank = 5;
	_cl = GetColor(255, 255, 255);
	_Ms = gGlobal._ms;

	for (auto i = 0; i < _Board.size(); i++) {
		if (_Ms < _Board[i].second) {
			_type = 0;
			_Name = "";
			_PlayerRank = i;
			break;
		}
	}
	return true;
}

bool ModeResult::Terminate() {
	base::Terminate();
	return true;
}

bool ModeResult::Process() {
	base::Process();
	int key = ApplicationMain::GetInstance()->GetKey();
	int trg = ApplicationMain::GetInstance()->GetTrg();
	if (_type == 0) {

		if (trg & PAD_INPUT_RIGHT) { _Cur++; }
		if (trg & PAD_INPUT_LEFT) { _Cur--; }
		if (trg & PAD_INPUT_UP) { _Cur -= KEY_WIDTH; }
		if (trg & PAD_INPUT_DOWN) { _Cur += KEY_WIDTH; }
		if (_Cur < 0) { _Cur = 0; }
		if (_Cur > _Keys.size()) { _Cur = _Keys.size() - 1; }


		if (trg & PAD_INPUT_1) {
			if (_Cur < _Keys.size()) {
				AddText(_Keys[_Cur]);
			}
			else {
				_type = 1;
			}
		}
		if (trg & PAD_INPUT_2) {
			if (_Name.size() > 0) {
				_Name.pop_back();
			}
		}

		
	}
	else if (_type == 1) {
		if (trg& PAD_INPUT_1) {
			for (auto i = 0; i < _Board.size(); i++) {
				if (_Ms < _Board[i].second) {
					_Board.insert(_Board.begin() + i, std::make_pair(_Name, _Ms));
					_Board.pop_back();
					_type = 2;
					break;
				}
				
			}
		}
		if (trg & PAD_INPUT_2) {
			_Name = "";
			_type = 0;
		}
	}
	else if (_type == 2) {
		if (trg & PAD_INPUT_1) {
			ModeBase* mode = new ModeTitle();
			ModeServer::GetInstance()->Add(mode, 1, "TITLE");
			ModeServer::GetInstance()->Del(this);
		}
	}
	
	if (GetModeTm() % 1000 < 500) {
		_cl = GetColor(255, 255, 255);
	}
	else {
		_cl = GetColor(220, 50, 50);
	}

	return false;
}

bool ModeResult::Render()
{
	base::Render();
	if (_type == 0) {
		for (auto i = 0; i < _Board.size(); i++) {
			DrawFormatString(0, (48 * i), GetColor(255, 255, 255), "%s  %d", _Board[i].first.c_str(), _Board[i].second);
		}

		DrawFormatString(600, 48, _cl, "%s,%d", _Name.c_str(), _Ms);
		for (auto i = 0; i <= _Keys.size(); i++) {
			if (i == _Cur) DrawFormatString(0 + 60 * (i % KEY_WIDTH), 240 + (48 * (i / KEY_WIDTH)), GetColor(220, 0, 0), "%c", _Keys[i]);
			else if (i == _Keys.size()) DrawFormatString(0 + 60 * (i % KEY_WIDTH), 240 + (48 * (i / KEY_WIDTH)), GetColor(255, 255, 255), "END");
			else DrawFormatString(0 + 60 * (i % KEY_WIDTH), 240 + (48 * (i / KEY_WIDTH)), GetColor(255, 255, 255), "%c", _Keys[i]);
		}
	}
	else if (_type == 1) {
		DrawFormatString(0, 0, GetColor(255, 255, 255), "Name : %s", _Name.c_str());
		DrawFormatString(0, 48, GetColor(255, 255, 255), "Time : %d", _Ms);
		DrawFormatString(0, 96, GetColor(255, 255, 255), "Push 1 : OK");
		DrawFormatString(0, 144, GetColor(255, 255, 255), "Push 2 : NG");
	}
	else if (_type == 2) {
		for (auto i = 0; i < _Board.size(); i++) {
			if (_Board[i].first == _Name) { DrawFormatString(0, (48 * i), GetColor(220, 50, 50), "%8s %8d", _Board[i].first.c_str(), _Board[i].second);
			}
			else {
				DrawFormatString(0, (48 * i), GetColor(255, 255, 255), "%8s %8d", _Board[i].first.c_str(), _Board[i].second);
			}
		}
		if (_PlayerRank > 4) {
			DrawFormatString(0, 48 * 6, _cl, "%8s %8d", _Name.c_str(), _Ms);
		}
		
	}
	
	return true;
}

bool ModeResult::AddText(char text)
{
	
	_Name = _Name + text;
	if (_Name.size() >= NAME_MAX) {
		_type = 1;
	}
	return true;
}

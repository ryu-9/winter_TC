#include "ModeStage.h"
#include "nlohmann/json.hpp"
#include "PlayerMoveComponent.h"
#include "PlayerActor.h"
#include "ApplicationMain.h"
#include <fstream>

bool ModeStage::Initialize() {
	if (!base::Initialize()) { return false; }
	_Camera = new CameraActor(this);
	auto ac = new PlayerActor(this);
	new PlayerMoveComponent(ac);
	
	_StageVec.push_back("chutorial2.json");
	_StageVec.push_back("heimen_kari.json");
	_StageNum = 0;
	LoadStage("res/Stage/", _StageVec[_StageNum]);
	
	return true;
}

bool ModeStage::Terminate() {
	base::Terminate();
	return true;
}

bool ModeStage::Process() {
	base::Process();
	int trg = ApplicationMain::GetInstance()->GetTrg();
	if (trg & PAD_INPUT_5) {
		_StageNum++;
		if (_StageNum >= _StageVec.size()) {
			_StageNum = 0;
		}
		StageClear();
		LoadStage("res/Stage/", _StageVec[_StageNum]);
	}
	if (trg & PAD_INPUT_6) {
		_StageNum--;
		if (_StageNum < 0) {
			_StageNum = _StageVec.size() - 1;
		}
		StageClear();
		LoadStage("res/Stage/", _StageVec[_StageNum]);
	}

	return true;
}

bool ModeStage::Update() {
	base::Update();
	
	return false;
}

bool ModeStage::Render() {
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetUseBackCulling(TRUE);

	SetUseLighting(FALSE);

	SetGlobalAmbientLight(GetColorF(0.5f, 0.f, 0.f, 0.f));
	ChangeLightTypeDir(VGet(-1, -1, 0));

	{
		float linelength = 3000.f;
		VECTOR v = { 0, 0, 0 };
		DrawLine3D(VAdd(v, VGet(-linelength, 0, 0)), VAdd(v, VGet(linelength, 0, 0)), GetColor(255, 0, 0));
		DrawLine3D(VAdd(v, VGet(0, -linelength, 0)), VAdd(v, VGet(0, linelength, 0)), GetColor(0, 255, 0));
		DrawLine3D(VAdd(v, VGet(0, 0, -linelength)), VAdd(v, VGet(0, 0, linelength)), GetColor(0, 0, 255));
	}

	DrawFormatString(0, 0, GetColor(255, 255, 255), "Stage:%s", _StageVec[_StageNum].c_str());

	base::Render();
	return false;
}

bool ModeStage::LoadStage(const std::string path, const std::string jsname) {
	{
		std::ifstream file(path + jsname);
		nlohmann::json json;
		file >> json;


		nlohmann::json stage = json.at("Stage");
		for (auto& data : stage) {
			std::string name = data.at("objectName");
			auto pos = VGet(data.at("translate").at("x"), data.at("translate").at("z"), data.at("translate").at("y"));
			pos.z *= -1.f;
			auto rot = VGet(data.at("rotate").at("x"), data.at("rotate").at("z"), data.at("rotate").at("y"));
			rot.x = DEG2RAD(rot.x);
			rot.y = DEG2RAD(rot.y);
			rot.z = DEG2RAD(rot.z);
			auto scale = VGet(data.at("scale").at("x"), data.at("scale").at("z"), data.at("scale").at("y"));

			auto ac = new ActorClass(this);
			auto mc=new ModelComponent(ac, (path + "model/" + name + ".mv1").c_str());
			ac->SetPosition(pos);
			mc->SetRotation(rot);
			ac->SetSize(scale);
		}
	}
	return false;
}

void ModeStage::StageClear() {
	while (_Actors.size()>2) {
		delete _Actors.back();
	}
}
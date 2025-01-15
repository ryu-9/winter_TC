#include <algorithm>
#include <utility>
#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeGame.h"
#include "PlayerActor.h"
#include "CameraActor.h"
#include "StageBox.h"

#include <fstream>
#include "ModeResult.h"
#include "ApplicationGlobal.h"
#include "nlohmann/json.hpp"

class MenuItemViewCameraInfo : public MenuItemBase {
public:
	MenuItemViewCameraInfo(void* param, std::string text) : MenuItemBase(param, text) {}

	// ���ڂ���肵���炱�̊֐����Ă΂��
	// return int : 0 = ���j���[�p��, 1 = ���j���[�I��
	virtual int Selected() {
		ModeGame* mdGame = static_cast<ModeGame*>(_param);
		if (mdGame->GetDebugViewCameraInfo()) {
			mdGame->GetCamera()->Send(1);
		}
		else {
		}
		mdGame->SetDebugViewCameraInfo(!mdGame->GetDebugViewCameraInfo());
		return 0;
	}
};

class MenuItemSetDashInput : public MenuItemBase {
public:
	MenuItemSetDashInput(void* param, std::string text) : MenuItemBase(param, text) {}

	// ���ڂ���肵���炱�̊֐����Ă΂��
	// return int : 0 = ���j���[�p��, 1 = ���j���[�I��
	virtual int Selected() {
		ModeGame* mdGame = static_cast<ModeGame*>(_param);
		mdGame->GetPlayer()->Send(2);
		return 1;
	}
};



bool ModeGame::Initialize() {
	if (!base::Initialize()) { return false; }
	
	ChangeFont("MS�S�V�b�N");
	SetFontSize(48);
	_bUseCollision = TRUE;
	_bViewCameraInfo = FALSE;
	SetFogEnable(TRUE);
	SetFogStartEnd(200, 10000);
	
	SetDrawCollision(TRUE);

	_Camera = new CameraActor(this);
	_Player = new PlayerActor(this);
	_Player->SetPosition(VGet(0, 0, 200));
	auto box = new StageBox(this);
	box->SetPosition(VGet(0,0,0));
	box->GetComponent<ModelComponent>()->SetFront(VGet(1,0,0));
	LoadStage("res/Stage/", "chutorial2.json");
	SoundServer::GetInstance()->Add("res/debug/sound/bomb.wav", "bgm2");
	return true;
}

bool ModeGame::Terminate() {
	base::Terminate();
	return true;
}

bool ModeGame::Process() {
	base::Process();

	int key = ApplicationMain::GetInstance()->GetKey();
	int trg = ApplicationMain::GetInstance()->GetTrg();

	// ESC�L�[�Ń��j���[��J��
	if (trg & PAD_INPUT_9) {
		ModeMenu* modeMenu = new ModeMenu();
		// ModeGame����̃��C���[�Ƀ��j���[��o�^����
		ModeServer::GetInstance()->Add(modeMenu, 99, "menu");
		// ModeMenu�Ƀ��j���[���ڂ�ǉ�����
		modeMenu->Add(new MenuItemViewCameraInfo(this, "ViewCameraInfo"));
		modeMenu->Add(new MenuItemSetDashInput(this, "SetDashInput"));
	}


	return true;
}

bool ModeGame::Update()
{
	base::Update();
	
	return false;
}

bool ModeGame::Render() {

	
	// 3D��{�ݒ�
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetUseBackCulling(TRUE);

	
	SetUseLighting(FALSE);
#if 1	// ���s���C�g
	SetGlobalAmbientLight(GetColorF(0.5f, 0.f, 0.f, 0.f));
	ChangeLightTypeDir(VGet(-1, -1, 0));
#endif
#if 0	// �|�C���g���C�g
	SetGlobalAmbientLight(GetColorF(0.f, 0.f, 0.f, 0.f));
	ChangeLightTypePoint(VAdd(_vPos, VGet(0, 50.f, 0)), 1000.f, 0.f, 0.005f, 0.f);
#endif



	// 0,0,0
	{
		float linelength = 1000.f;
		VECTOR v = { 0, 0, 0 };
		DrawLine3D(VAdd(v, VGet(-linelength, 0, 0)), VAdd(v, VGet(linelength, 0, 0)), GetColor(255, 0, 0));
		DrawLine3D(VAdd(v, VGet(0, -linelength, 0)), VAdd(v, VGet(0, linelength, 0)), GetColor(0, 255, 0));
		DrawLine3D(VAdd(v, VGet(0, 0, -linelength)), VAdd(v, VGet(0, 0, linelength)), GetColor(0, 0, 255));
	}

	// �J�����^�[�Q�b�g�𒆐S�ɒZ���������
	{
		float linelength = 10.f;
		VECTOR v = _Camera->GetDirection();
		DrawLine3D(VAdd(v, VGet(-linelength, 0, 0)), VAdd(v, VGet(linelength, 0, 0)), GetColor(255, 0, 0));
		DrawLine3D(VAdd(v, VGet(0, -linelength, 0)), VAdd(v, VGet(0, linelength, 0)), GetColor(0, 255, 0));
		DrawLine3D(VAdd(v, VGet(0, 0, -linelength)), VAdd(v, VGet(0, 0, linelength)), GetColor(0, 0, 255));
	}

	//DrawCube3D(VGet(50,0,50),VGet(-50,100,-50),GetColor(255,255,255),GetColor(255,255,255),TRUE);

	base::Render();

	return true;
}

bool ModeGame::LoadStage(const std::string path, const std::string jsname) {
	std::ifstream file(path + jsname);
	nlohmann::json json;
	file >> json;

	nlohmann::json stage = json.at("Stage");
	for (auto& data : stage) {
		std::string name = data.at("objectName");
		auto pos = VGet(data.at("translate").at("x"), data.at("translate").at("z"), data.at("translate").at("y"));
		pos.z *= -1.f;
		auto rot = VGet(data.at("rotate").at("x"), data.at("rotate").at("z"), data.at("rotate").at("y"));
		rot.x = DEG2RAD(pos.x);
		rot.y = DEG2RAD(pos.y);
		rot.z = DEG2RAD(pos.z);
		auto scale = VGet(data.at("scale").at("x"), data.at("scale").at("z"), data.at("scale").at("y"));

		// ModelServerへ
//		ModelServer::GetInstance()->Add((path + name).c_str());
		if (name == "SM_Cube"|| name == "Cube") {
			auto box = new StageBox(this);
			box->SetPosition(pos);
			box->SetDirection(rot);
			box->SetSize(scale);
		}
		// �A�N�^�Ƃ��ēǂݍ���
	}
	return true;
}


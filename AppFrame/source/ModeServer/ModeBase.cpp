#include "ModeBase.h"


ModeBase::ModeBase() {
	_szName = "";
	_uid = 1;
	_layer = 0;

	_cntMode = 0;
	_tmMode = 0;
	_tmStep = 0;
	_tmModeBase = 0;
	_tmPauseBase = 0;
	_tmPauseStep = 0;
	_tmOldFrame = 0;

	SetCallPerFrame(1);
	SetCallOfCount(1);
}

ModeBase::~ModeBase() {
}


// ModeServerに接続時、Process()の前に一度だけ呼ばれる
bool	ModeBase::Initialize() {

	return true;
}

// ModeServerから削除される際、一度だけ呼ばれる
bool	ModeBase::Terminate() {

	return true;
}


// --------------------------------------------------------------------------
/// @brief 毎フレーム呼ばれる。処理部を記述
// --------------------------------------------------------------------------
bool	ModeBase::Process()
{
	_UpdatingActors = true;
	for (auto actor : _Actors) {
		actor->ProcessInput();
	}
	_UpdatingActors = false;
	return	true;
}

bool ModeBase::Update()
{
	// アクタのアップデート
	_UpdatingActors = true;
	for (int i = 0; i < _Actors.size(); i++) {
		_Actors[i]->Update();
	}
	_UpdatingActors = false;

	// 待機中アクタの追加
	for (auto p_actor : _PendingActors) {
		_Actors.emplace_back(p_actor);
	}
	_PendingActors.clear();

	// アクタの削除
	std::vector<ActorClass*> deadActors;
	for (auto actor : _Actors) {
		if (actor->GetState() == ActorClass::State::eDead) {
			deadActors.emplace_back(actor);
		}
	}
	for (auto actor : deadActors) {
		delete actor;
	}
	return false;
}

// --------------------------------------------------------------------------
/// @brief 毎フレーム呼ばれる。描画部を記述
// --------------------------------------------------------------------------
bool	ModeBase::Render()
{
	for (auto sprite : _Sprites) {
		sprite->Draw();
	}
	return	true;
}

void ModeBase::AddActor(ActorClass* actor) {
	if (_UpdatingActors) {
		_PendingActors.emplace_back(actor);
	}
	else {
		_Actors.emplace_back(actor);
	}
}

void ModeBase::RemoveActor(ActorClass* actor) {
	auto iter = std::find(_PendingActors.begin(), _PendingActors.end(), actor);
	if (iter != _PendingActors.end()) {
		std::iter_swap(iter, _PendingActors.end() - 1);
		_PendingActors.pop_back();
	}

	iter = std::find(_Actors.begin(), _Actors.end(), actor);
	if (iter != _Actors.end()) {
		std::iter_swap(iter, _Actors.end() - 1);
		_Actors.pop_back();
	}
}

void ModeBase::AddSprite(SpriteComponent* sp) {
	int myDrawOrder = sp->GetDrawOrder();
	auto iter = _Sprites.begin();
	for (; iter != _Sprites.end(); ++iter) {
		if (myDrawOrder < (*iter)->GetDrawOrder()) { break; }
	}

	_Sprites.insert(iter, sp);
}

void ModeBase::RemoveSprite(SpriteComponent* sp) {
	auto iter = std::find(_Sprites.begin(), _Sprites.end(), sp);
	_Sprites.erase(iter);
}

// 時間経過をさせる
void ModeBase::StepTime(unsigned long tmNow) {
	// 時間経過処理
	if (_cntMode == 0) {
		_tmMode = 0;
		_tmStep = 0;
		_tmModeBase = tmNow;
		_tmPauseBase = 0;
		_tmPauseStep = 0;
	}
	else {
		_tmMode = tmNow - _tmModeBase + _tmPauseStep;
		_tmStep = tmNow - _tmOldFrame;
	}
	_tmOldFrame = tmNow;
}

// カウントを進める
void ModeBase::StepCount() {
	_cntMode++;
}

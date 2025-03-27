#include "ModeBase.h"

VECTOR VMulti(VECTOR v, VECTOR m) {
	VECTOR ret;
	ret.x = v.x * m.x;
	ret.y = v.y * m.y;
	ret.z = v.z * m.z;
	return ret;
}

bool VEqual(VECTOR v, VECTOR m) {
	if (v.x == m.x && v.y == m.y && v.z == m.z) {
		return true;
	}
	else {
		return false;
	}
}

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

	_DrawCollision = true;

	SetCallPerFrame(1);
	SetCallOfCount(1);


}

ModeBase::~ModeBase() {
	while (!_Actors.empty()) {
		delete _Actors.back();
	}
	while (!_Sprites.empty()) {
		delete _Sprites.back();
	}
	while (!_MCollision.empty()) {
		delete _MCollision.back();
	}

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

	if (_DrawCollision) {
		for (auto mc : _MCollision) {
			//mc->DebugDraw();
		}
		for (auto hc : _HCollision) {
			//hc->DebugDraw();
		}
	}
	return	true;
}

// アクタの追加
void ModeBase::AddActor(ActorClass* actor) {
	if (_UpdatingActors) {
		_PendingActors.emplace_back(actor);
	}
	else {
		_Actors.emplace_back(actor);
	}
}

// アクタの削除
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


// スプライトの追加
void ModeBase::AddSprite(SpriteComponent* sp) {
	int myDrawOrder = sp->GetDrawOrder();
	auto iter = _Sprites.begin();
	for (; iter != _Sprites.end(); ++iter) {
		if (myDrawOrder < (*iter)->GetDrawOrder()) { break; }
	}

	_Sprites.insert(iter, sp);
}

// スプライトの削除
void ModeBase::RemoveSprite(SpriteComponent* sp) {
	auto iter = std::find(_Sprites.begin(), _Sprites.end(), sp);
	_Sprites.erase(iter);
}

void ModeBase::AddMCollision(MoveCollisionComponent* mc)
{
	auto iter = _MCollision.begin();
	_MCollision.insert(iter, mc);
}

void ModeBase::RemoveMCollision(MoveCollisionComponent* mc)
{
	auto iter = std::find(_MCollision.begin(), _MCollision.end(), mc);
	_MCollision.erase(iter);
}

void ModeBase::AddHCollision(HitCollisionComponent* hc)
{
	auto iter = _HCollision.begin();
	_HCollision.insert(iter, hc);
}

void ModeBase::RemoveHCollision(HitCollisionComponent* hc)
{
	auto iter = std::find(_HCollision.begin(), _HCollision.end(), hc);
	_HCollision.erase(iter);
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
		if (tmNow - _tmOldFrame > 500) {
			_tmOldFrame = tmNow;
		}
		_tmMode = tmNow - _tmModeBase +_tmPauseStep;
		_tmStep = tmNow - _tmOldFrame;
	}
	_tmOldFrame = tmNow;
}

// カウントを進める
void ModeBase::StepCount() {
	_cntMode++;
}

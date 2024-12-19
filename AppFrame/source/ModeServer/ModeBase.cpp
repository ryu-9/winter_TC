#include "ModeBase.h"

VECTOR VMulti(VECTOR v, VECTOR m) {
	VECTOR ret;
	ret.x = v.x * m.x;
	ret.y = v.y * m.y;
	ret.z = v.z * m.z;
	return ret;
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

	SetCallPerFrame(1);
	SetCallOfCount(1);
}

ModeBase::~ModeBase() {
}


// ModeServer�ɐڑ����AProcess()�̑O�Ɉ�x�����Ă΂��
bool	ModeBase::Initialize() {

	return true;
}

// ModeServer����폜�����ہA��x�����Ă΂��
bool	ModeBase::Terminate() {

	return true;
}


// --------------------------------------------------------------------------
/// @brief ���t���[���Ă΂��B���������L�q
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
	// �A�N�^�̃A�b�v�f�[�g
	_UpdatingActors = true;
	for (int i = 0; i < _Actors.size(); i++) {
		_Actors[i]->Update();
	}
	_UpdatingActors = false;

	// �ҋ@���A�N�^�̒ǉ�
	for (auto p_actor : _PendingActors) {
		_Actors.emplace_back(p_actor);
	}
	_PendingActors.clear();

	// �A�N�^�̍폜
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
/// @brief ���t���[���Ă΂��B�`�敔���L�q
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

// ���Ԍo�߂�������
void ModeBase::StepTime(unsigned long tmNow) {
	// ���Ԍo�ߏ���
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

// �J�E���g��i�߂�
void ModeBase::StepCount() {
	_cntMode++;
}

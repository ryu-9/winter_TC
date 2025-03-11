#include "GimmickWallActor.h"

GimmickWallActor::GimmickWallActor(ModeBase* mode, VECTOR pos, VECTOR size, VECTOR rot, int type, ActorClass* actor)
	:ActorClass(mode)
	, _Actor(actor)
	, _IsActive(true)
	, _Move(0)
{
	SetSize(size);
	SetPosition(pos);
	_Model = new ModelComponent(this, "res/Stage/model/GimmickWall/ENEMYWALL.mv1");
	//_Model = new ModelComponent(this, "res/cube.mv1");
	_Model->SetScale(VGet(10, 10, 10));
	int handle = ModelServer::GetInstance()->Add("res/cube.mv1");
	_MCollision = new MoveCollisionComponent(this,_Model, VGet(0, 0, 0), VGet(0.05, 0.05, 0.05), 6, false, true, handle);
	_MCollision->RefleshCollInfo();



	//new HitCollisionComponent(this, _Model, VGet(0, 0, 0), VGet(1, 1, 1), 2, true, true);
	

	if (!_IsActive) {
		_Move = 100 * GetSize().y;
		_Model->SetPosition(VGet(0, -_Move, 0));
		_MCollision->SetIsActive(false);
	}
}

GimmickWallActor::~GimmickWallActor()
{
}

void GimmickWallActor::UpdateActor()
{
	if (_Actor->GetState() != State::eActive)
	{
		SetIsActive(false);
	}
	float test = sin((float)GetNowCount() / 1000.f) * 10;
	SetPosition(VAdd(GetPosition(), VGet(0, test, 0)));
	_MCollision->RefleshCollInfo();

	if (_IsActive && _Move) {
		float move = GetMode()->GetStepTm() * GetSize().y;
		_Move -= move;
		if (_Move <= 0) {
			_Move = 0;
		}
		_Model->SetPosition(VGet(0, -_Move, 0));
	}
	else if (!_IsActive && _Move < GetSize().y * 100) {
		float move = GetMode()->GetStepTm() * GetSize().y;
		_Move += move;
		if (_Move >= GetSize().y * 100) {
			_Move = GetSize().y * 100;
		}
		_Model->SetPosition(VGet(0, -_Move, 0));
	}

}

void GimmickWallActor::SetIsActive(bool flag)
{
	_IsActive = flag;
	_MCollision->SetIsActive(flag);
}

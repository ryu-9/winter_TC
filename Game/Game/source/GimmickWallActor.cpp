#include "GimmickWallActor.h"

GimmickWallActor::GimmickWallActor(ModeBase* mode, VECTOR pos, VECTOR size, VECTOR rot, int type, ActorClass* actor)
	:ActorClass(mode)
	, _Actor(actor)
	, _IsActive(true)
	, _Move(0)
{
	SetSize(size);
	SetPosition(pos);
	_Model = new ModelComponent(this, "res/Stage/model/EnemyWall/E_WALL.mv1");
	//_Model = new ModelComponent(this, "res/model/Mapchip/Mapchip.mv1");
	_Model->SetScale(VGet(2,2,2));
	_Model->SetPosition(VGet(0, 0, 0));
	int handle = ModelServer::GetInstance()->Add("res/cube.mv1");
	_MCollision = new MoveCollisionComponent(this,_Model, VGet(0, 0, 0), VGet(1, 1, 1), 6, false, true, handle);
	_MCollision->RefleshCollInfo();



	//new HitCollisionComponent(this, _Model, VGet(0, 0, 0), VGet(1, 1, 1), 2, true, true);
	

	if (!_IsActive) {
		_Move = 400 * GetSize().y;
		_Model->SetPosition(VGet(0, -_Move, 0));
		_MCollision->SetIsActive(false);
	}
}

GimmickWallActor::~GimmickWallActor()
{
}

void GimmickWallActor::UpdateActor()
{
	if (_Actor != nullptr) {
		if (_Actor->GetState() != State::eActive) {
			SetIsActive(false);
		};
	}
	if (_IsActive && _Move) {
		float move = GetMode()->GetStepTm() * GetSize().y;
		_Move -= move;
		if (_Move <= 0) {
			_Move = 0;
		}
		_Model->SetPosition(VGet(0, -_Move, 0));
	}
	else if (!_IsActive && _Move < GetSize().y * 400) {
		float move = GetMode()->GetStepTm() * GetSize().y;
		_Move += move;
		if (_Move >= GetSize().y * 400) {
			_Move = GetSize().y * 400;
		}
		_Model->SetPosition(VGet(0, -_Move, 0));
	}

}

void GimmickWallActor::SetIsActive(bool flag)
{
	_IsActive = flag;
	_MCollision->SetIsActive(flag);
}

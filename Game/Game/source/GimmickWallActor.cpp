#include "GimmickWallActor.h"
#include "GroupAttackActor.h"
GimmickWallActor::GimmickWallActor(ModeBase* mode, VECTOR pos, VECTOR size, VECTOR rot, int type, ActorClass* actor)
	:ActorClass(mode)
	, _Actor(actor)
	, _IsActive(true)
	, _Move(0)
	, _Height(50)
{
	SetSize(size);
	SetPosition(pos);
	_Model = new ModelComponent(this, "res/Stage/model/EnemyWall/E_WALL.mv1");
	//_Model = new ModelComponent(this, "res/model/Mapchip/Mapchip.mv1");
	_Model->SetScale(VGet(2,2,2));
	_Model->SetPosition(VGet(0, -50, 0));
	int handle = ModelServer::GetInstance()->Add("res/cube.mv1");
	_MCollision = new MoveCollisionComponent(this,_Model, VGet(0, 0, 0), VGet(0.6, 0.6, 0.6), 6, false, true, handle);
	_MCollision->RefleshCollInfo();



	//new HitCollisionComponent(this, _Model, VGet(0, 0, 0), VGet(1, 1, 1), 2, true, true);
	

	if (!_IsActive) {
		_Move = _Height * GetSize().y ;
		_Model->SetPosition(VGet(0, -_Move, 0));
		_MCollision->SetIsActive(false);
	}
}

GimmickWallActor::~GimmickWallActor()
{
}

void GimmickWallActor::UpdateActor()
{

	// èWícêÌóp
	auto g = dynamic_cast<GroupSpawnerActor*>(_Actor);
	if (g != nullptr && !_IsActive) {
		if (g->GetActive()) {
			SetIsActive(true);
		}
	}
	if (_Actor != nullptr) {
		if (_Actor->GetState() != State::eActive) {
			SetIsActive(false);
		};
	}
	if (_IsActive && _Move) {
		float move = GetMode()->GetStepTm() * GetSize().y;
		move /= 50;
		_Move -= move;
		if (_Move <= 50) {
			_Move = 50;
		}
		_Model->SetPosition(VGet(0, -_Move, 0));
	}
	else if (!_IsActive && _Move < GetSize().y * _Height) {
		float move = GetMode()->GetStepTm() * GetSize().y;
		move /= 50;
		_Move += move;
		if (_Move >= GetSize().y * _Height) {
			_Move = GetSize().y * _Height;
		}
		_Model->SetPosition(VGet(0, -_Move, 0));
	}

}

void GimmickWallActor::SetIsActive(bool flag)
{
	_IsActive = flag;
	_MCollision->SetIsActive(flag);
}

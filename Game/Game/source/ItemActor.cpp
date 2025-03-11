#include "ItemActor.h"


ItemActor::ItemActor(ModeBase* mode,VECTOR pos, int type, int life)
	:ActorClass(mode)
	, _Type(type)
	, _Life(life)
{
	SetPosition(pos);
	VECTOR position = VGet(0, 0, 0);
	switch (type)
	{
	case 0:
		_Model = new ModelComponent(this, "res/Stage/model/tree_tsumoriyuki.mv1");
		_Model->SetScale(VGet(0.35, 0.4, 0.35));
		_Model->SetPosition(VGet(0, -75, 0));
		_HCollision = new HitCollisionComponent(this, _Model, VGet(0, 300, 0), VGet(100, 100, 100), 2, true, true);
		_Move = new MoveComponent(this);
		position = VGet(0, 300, 0);
		break;

	case 1:
		_Model = new ModelComponent(this, "res/Stage/model/Denchi.mv1");
		_Model->SetScale(VGet(6, 6, 6));
		_HCollision = new HitCollisionComponent(this, _Model, VGet(0, 0, 0), VGet(100, 100, 100), 2, true, true);
		_Move = new MoveComponent(this);
		_Move->SetGravity(0.1);
		_Move->SetFallLimit(2);
		break;
	case 2:
		_Model = new ModelComponent(this, "res/Stage/model/sword.mv1");
		_Model->SetScale(VGet(3, 3, 3));
		_HCollision = new HitCollisionComponent(this, _Model, VGet(0, 0, 0), VGet(100, 100, 100), 2, true, true);
		_Move = new MoveComponent(this);
		_Move->SetGravity(0.1);
		_Move->SetFallLimit(2);
		break;
	}


	auto mcoll = new MoveCollisionComponent(this, _Model, position, VGet(100, 100, 100), 2, true, true);
}

ItemActor::~ItemActor()
{
}

void ItemActor::UpdateActor()
{
	if (_Type != 0) {
		VECTOR rot = _Model->GetRotation();
		rot.y += 0.02f;
		_Model->SetRotation(rot);
	}

	if (_Move->GetStand()) {
		_Move->SetVelocity(VGet(0, 0, 0));
		if (_Type == 0) {
			VECTOR size = _Model->GetScale();
			size.y = 0.2;
			_Model->SetScale(size);

		}
	}
	if (_Life >= 0 && _Move->GetStand()) {
		_Life -= GetMode()->GetStepTm();
		if (_Life < 2000) {
			if (_Life / 50 % 2) {
				_Model->SetVisible(false);
			}
			else {
				_Model->SetVisible(true);
			}
			if (_Life <= 0) {
				SetState(State::eDead);
			}
		}
	}

	
}

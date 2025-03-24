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
	case -1:
		_Model = new ModelComponent(this, "res/Stage/model/tree_tsumoriyuki.mv1");
		_Model->SetScale(VGet(0.15, 0.2, 0.15));
		_Model->SetPosition(VGet(0, -75, 0));
		_HCollision = new HitCollisionComponent(this, _Model, VGet(0, 100, 0), VGet(10, 10, 10), 2, true, true);
		_Move = new MoveComponent(this);
		position = VGet(0, 300, 0);
		break;

	case 0:
		_Model = new ModelComponent(this, "res/Stage/model/tree_tsumoriyuki.mv1");
		_Model->SetScale(VGet(0.35, 0.4, 0.35));
		_Model->SetPosition(VGet(0, -75, 0));
		_HCollision = new HitCollisionComponent(this, _Model, VGet(0, 100, 0), VGet(200, 200, 200), 2, true, true);
		_Move = new MoveComponent(this);
		position = VGet(0, 300, 0);
		break;

	case 1:
		_Model = new ModelComponent(this, "res/Stage/model/Denchi.mv1");
		_Model->SetScale(VGet(6, 6, 6));
		_HCollision = new HitCollisionComponent(this, _Model, VGet(0, 100, 0), VGet(200, 200, 200), 2, true, true);
		_Move = new MoveComponent(this);
		_Move->SetGravity(0.1);
		_Move->SetFallLimit(2);
		break;
	case 2:
		_Model = new ModelComponent(this, "res/Stage/model/sword.mv1");
		_Model->SetScale(VGet(3, 3, 3));
		_HCollision = new HitCollisionComponent(this, _Model, VGet(0, 100, 0), VGet(200, 200, 200), 2, true, true);
		_HCollision->SetIsActive(false);
		_Move = new MoveComponent(this);
		_Move->SetGravity(0.1);
		_Move->SetFallLimit(2);
		break;

	case 11:
		_Model = new ModelComponent(this, "res/model/DropItem/stoneL.mv1");
		_Model->SetScale(VGet(10, 10, 10));
		_Model->SetPosition(VGet(0, 100, 0));
		_HCollision = new HitCollisionComponent(this, _Model, VGet(0, 100, 0), VGet(100, 100, 100), 2, true, true);
		_Move = new MoveComponent(this);
		//position = VGet(0, -200, 0);
		break;

	case 12:
		_Model = new ModelComponent(this, "res/Stage/model/stone.mv1");
		_Model->SetScale(VGet(100, 100, 100));
		_HCollision = new HitCollisionComponent(this, _Model, VGet(0, 100, 0), VGet(100, 100, 100), 2, true, true);
		_Move = new MoveComponent(this);
		//position = VGet(0, -200, 0);
		break;
	}

	if (_Type > 0) {
		new EffectSpriteComponent(this, "res/model/Item_Circl/Item_Circl.efkefc", VGet(0, 50, -25), VGet(0, 0, 0), 10, false, 0.3);
	}
	auto mcoll = new MoveCollisionComponent(this, _Model, position, VGet(100, 100, 100), 2, true, true);
}

ItemActor::~ItemActor()
{
}

void ItemActor::UpdateActor()
{
	if (_Type > 0) {
		VECTOR rot = _Model->GetRotation();
		rot.y += 0.02f;
		_Model->SetRotation(rot);
	}

	if (_Move->GetStand()) {
		_Move->SetVelocity(VGet(0, 0, 0));
		_HCollision->SetIsActive(true);
		if (_Type == 0) {
			if(_Model->GetScale().y >= _Model->GetScale().x){
				VECTOR size = _Model->GetScale();
				size.y /= 2;
				_Model->SetScale(size);
			}


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

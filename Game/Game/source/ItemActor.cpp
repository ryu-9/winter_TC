#include "ItemActor.h"


ItemActor::ItemActor(ModeBase* mode, int type, int life)
	:ActorClass(mode)
	, _Type(type)
	, _Life(life)
{
	switch (type)
	{
	case 0:
		_Model = new ModelComponent(this, "res/model/Item/Item1.mv1");
		break;

	case 1:
		_Model = new ModelComponent(this, "res/model/Item/Item2.mv1");
		break;
	}
	_HCollision = new HitCollisionComponent(this, _Model, VGet(0, 0, 0), VGet(100, 100, 100), 2, true, true);
	auto move = new MoveComponent(this);
	auto mcoll = new MoveCollisionComponent(this, _Model, VGet(0, 0, 0), VGet(100, 100, 100), 2, true, true);
}

ItemActor::~ItemActor()
{
}

void ItemActor::UpdateActor()
{
	if (_Life >= 0) {
		_Life -= FpsController::GetInstance()->GetDeltaTime();
		if (_Life < 300) {
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

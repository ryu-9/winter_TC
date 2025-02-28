#include "ItemActor.h"

ItemActor::ItemActor(ModeBase* mode, int type)
	:ActorClass(mode)
	, _Type(type)
{
	ModelComponent* model = nullptr;
	switch (type)
	{
	case 0:
		model = new ModelComponent(this, "res/model/Item/Item1.mv1");
		break;
	}
	_HCollision = new HitCollisionComponent(this, model, VGet(0, 0, 0), VGet(100, 100, 100), 2, true, true);
	auto move = new MoveComponent(this);
	auto mcoll = new MoveCollisionComponent(this, model, VGet(0, 0, 0), VGet(100, 100, 100), 2, true, true);
}

ItemActor::~ItemActor()
{
}

void ItemActor::UpdateActor()
{
}

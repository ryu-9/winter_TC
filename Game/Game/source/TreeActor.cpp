#include "TreeActor.h"
#include "ItemActor.h"

TreeActor::TreeActor(ModeBase* mode, VECTOR pos)
	:ActorClass(mode)
{
	SetPosition(pos);
	_Model = new ModelComponent(this, "res/Stage/model/Tree.mv1");
	_Model->SetScale(VGet(0.4, 0.4, 0.4));
	_HCollision = new HitCollisionComponent(this, _Model, VGet(0, 0, 0), VGet(30, 30, 30), 2, true, true);

}

TreeActor::~TreeActor()
{
}

void TreeActor::UpdateActor()
{
	_HCollision->SetOldPosition(VAdd(GetPosition(), VGet(0, 200, 0)));
}

void TreeActor::SetItem(ItemActor* item)
{
	item->SetState(State::ePaused);
	item->GetComponent<HitCollisionComponent>()[0]->SetIsActive(false);
	_Item.push_back(item);
}

void TreeActor::DropItem()
{
	for (auto& item : _Item) {
		item->SetState(State::eActive);
		item->GetComponent<HitCollisionComponent>()[0]->SetIsActive(true);
	}
	_Item.clear();
	_HCollision->SetIsActive(false);
}

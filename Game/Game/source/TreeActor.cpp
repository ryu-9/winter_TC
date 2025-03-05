#include "TreeActor.h"
#include "ItemActor.h"

TreeActor::TreeActor(ModeBase* mode, VECTOR pos)
	:ActorClass(mode)
{
	SetPosition(pos);
	_Model = new ModelComponent(this, "res/Stage/model/Tree.mv1");

}

TreeActor::~TreeActor()
{
}

void TreeActor::UpdateActor()
{
}

void TreeActor::SetItem(ItemActor* item)
{
	item->SetState(State::ePaused);
	_Item.push_back(item);
}

void TreeActor::DropItem()
{
	for (auto& item : _Item) {
		item->SetState(State::eActive);
	}
	_Item.clear();
	_HCollision->SetIsActive(false);
}

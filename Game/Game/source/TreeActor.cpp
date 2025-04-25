#include "TreeActor.h"
#include "ItemActor.h"

TreeActor::TreeActor(ModeBase* mode, VECTOR pos)
	:ActorClass(mode)
	, _AnimTime(0)
	, _AnimRate(0)
{
	SetPosition(pos);
	_Model = new ModelComponent(this, "res/Stage/model/Tree_sway.mv1");
	_Model->SetScale(VGet(0.4, 0.4, 0.4));
	_HCollision = new HitCollisionComponent(this, _Model, VGet(0, 0, 0), VGet(30, 30, 30), 2, true, true);
	MV1AttachAnim(_Model->GetHandle(), 0, _Model->GetHandle(), TRUE);
	MV1SetAttachAnimTime(_Model->GetHandle(), 0, 0);
	MV1AttachAnim(_Model->GetHandle(), 0, _Model->GetHandle(), TRUE);
	MV1SetAttachAnimTime(_Model->GetHandle(), 1, 0);
}

TreeActor::~TreeActor()
{
}

void TreeActor::UpdateActor()
{
	_HCollision->SetOldPosition(VAdd(GetPosition(), VGet(0, 300, 0)));
	if (_AnimRate > 0) {
		_AnimTime += (float)GetMode()->GetStepTm()/10;
		if (_AnimTime > 30) {
			_AnimTime = 0;
		}
		_AnimRate -= (float)GetMode()->GetStepTm() / 1000;
		if (_AnimRate <= 0) {
			_AnimRate = 0;
			SetState(State::ePaused);
		}
		MV1SetAttachAnimBlendRate(_Model->GetHandle(), 0, _AnimRate);
		//MV1SetAttachAnimBlendRate(_Model->GetHandle(), 1, 1 - _AnimTime);
		MV1SetAttachAnimTime(_Model->GetHandle(), 0, _AnimTime);
	}
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
	_AnimRate = 1;
	_Item.clear();
	_HCollision->SetIsActive(false);
}

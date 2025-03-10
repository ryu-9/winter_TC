#include "GoalItemActor.h"
#include "PlayerActor.h"

GoalItemActor::GoalItemActor(ModeBase* mode, VECTOR pos) 
	: ActorClass(mode) {

	_Model = new ModelComponent(this, "res/Stage/model/Reizoukou.mv1");
	_HCollision = new HitCollisionComponent(this, _Model, VGet(0, 25, 0), VGet(20, 20, 20), 2, true, true);
	SetPosition(pos);
	_Model->SetVisible(true);
	_HCollision->RefleshCollInfo();
	_PosY = pos.y;
	SetSize(VGet(2, 2, 2));
}

GoalItemActor::~GoalItemActor() {
}

void GoalItemActor::UpdateActor() {
	_AnimCount += GetMode()->GetStepTm();
	VECTOR rot = _Model->GetRotation();
	rot.y += 0.02f;
	_Model->SetRotation(rot);
	VECTOR pos = GetPosition();
	pos.y = _PosY + 10 * sin(_AnimCount * 0.005f);
	SetPosition(pos);

	auto hit = _HCollision->IsHit();
	for (auto h : hit) {
		auto p = dynamic_cast<PlayerActor*>(h->GetOwner());
		if (p!=nullptr) {
			auto ui = ModeServer::GetInstance()->Get("gameui");
			
		}
	}
}

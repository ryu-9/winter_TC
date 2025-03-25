#include "GoalItemActor.h"
#include "PlayerActor.h"
#include "ModeGameGoal.h"

GoalItemActor::GoalItemActor(ModeBase* mode, VECTOR pos,bool flag) 
	: ActorClass(mode) {

	_Model = new ModelComponent(this, "res/Stage/model/Reizoukou.mv1");
	
	auto m = new MoveComponent(this);
	m->SetGravity(0.0f);
	auto poss = pos;
	poss.y += 200;
	SetPosition(poss);
	new MoveCollisionComponent(this, _Model, VGet(0, -50, 0), VGet(1, 1, 1), 2, true, true);
	_HCollision = new HitCollisionComponent(this, _Model, VGet(0, 25, 0), VGet(20, 20, 20), 2, true, true);
	
	_Model->SetVisible(true);
	_HCollision->RefleshCollInfo();
	_PosY = poss.y ;
	_Fallf = flag;
	SetSize(VGet(2, 2, 2));
}

GoalItemActor::~GoalItemActor() {
}

void GoalItemActor::UpdateActor() {
	_AnimCount += GetMode()->GetStepTm();
	VECTOR rot = _Model->GetRotation();
	rot.y += 0.02f;
	_Model->SetRotation(rot);
	if (_Fallf) {
		_PosY -= 1.f;
		if (GetComponent<MoveComponent>()[0]->GetStand()) {
			_Fallf = false;
		}
	}
	VECTOR pos = GetPosition();
	pos.y = _PosY + 10 * sin(_AnimCount * 0.005f);
	SetPosition(pos);

}

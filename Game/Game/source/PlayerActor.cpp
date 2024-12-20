#include "PlayerActor.h"
#include "appframe.h"
#include "PlayerMoveComponent.h"

PlayerActor::PlayerActor(ModeBase* mode)
	:ActorClass(mode)
	,_Status(STATUS::NONE)
	,_OldStatus(STATUS::NONE)
{
	_Model = new ModelComponent(this, "res/Debug/chinpo.mv1");
	_MCollision = new MoveCollisionComponent(this);
	int n = rand() % 2;
	
		_Input = new PlayerMoveComponent(this);
	
	
}

PlayerActor::~PlayerActor()
{

}

void PlayerActor::UpdateActor()
{
	SetOldStatus();

	// 速度を取得
	VECTOR v = _Input->GetVelocity();
	// 角度を取得
	VECTOR rot = _Model->GetFront();
	VECTOR rot2 = _Model->GetUp();

	//	速度から角度を算出
	rot = VTransform(rot, MGetRotX(v.z / 100 / GetSize().x / 2));
	rot = VTransform(rot, MGetRotZ(-v.x / 100 / GetSize().x / 2));
	rot2 = VTransform(rot2, MGetRotX(v.z / 100 / GetSize().x / 2));
	rot2 = VTransform(rot2, MGetRotZ(-v.x / 100 / GetSize().x / 2));

	//	角度をセット

	MV1SetRotationZYAxis(_Model->GetHandle(), rot, rot2, 0);

	_Model->SetFront(rot);
	_Model->SetUp(rot2);
}

#include "PlayerActor.h"
#include "appframe.h"
#include "PlayerMoveComponent.h"

PlayerActor::PlayerActor(ModeBase* mode)
	:ActorClass(mode)
	,_Status(STATUS::NONE)
	,_OldStatus(STATUS::NONE)
	,_ModeNum(1)
{
	_Model = new ModelComponent(this, "res/Debug/chinpo.mv1");
	//_MCollision = new MoveCollisionComponent(this);
	//_MCollision->SetIsMove(true);
	_MCollision = new MoveCollisionComponent(this,_Model, VGet(0,0,0), VGet(100, 100, 100), 2, true, true);
	int n = rand() % 2;
	
		_Input = new PlayerMoveComponent(this);
	
		SetPosition(VGet(0, 1000, 0));
}

PlayerActor::~PlayerActor()
{

}

void PlayerActor::UpdateActor()
{
	SetOldStatus();

	VECTOR v;

	VECTOR rot;
	VECTOR rot2;

	switch (_ModeNum) {
	case 0:
		// 速度を取得
		v = _Input->GetVelocity();
		// 角度を取得
		rot = _Model->GetFront();
		rot2 = _Model->GetUp();

		//	速度から角度を算出
		rot = VTransform(rot, MGetRotX(v.z / 100 / GetSize().x / 2));
		rot = VTransform(rot, MGetRotZ(-v.x / 100 / GetSize().x / 2));
		rot2 = VTransform(rot2, MGetRotX(v.z / 100 / GetSize().x / 2));
		rot2 = VTransform(rot2, MGetRotZ(-v.x / 100 / GetSize().x / 2));

		//	角度をセット

		MV1SetRotationZYAxis(_Model->GetHandle(), rot, rot2, 0);

		_Model->SetFront(rot);
		_Model->SetUp(rot2);

		break;

	case 1:
		rot = _Input->GetDashDir();
		rot2 = VGet(0,1,0);

		MV1SetRotationZYAxis(_Model->GetHandle(), rot, rot2, 0);

		_Model->SetFront(rot);
		_Model->SetUp(rot2);
		break;


	
	}

}

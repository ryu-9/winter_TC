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

	// ���x���擾
	VECTOR v = _Input->GetVelocity();
	// �p�x���擾
	VECTOR rot = _Model->GetFront();
	VECTOR rot2 = _Model->GetUp();

	//	���x����p�x���Z�o
	rot = VTransform(rot, MGetRotX(v.z / 100 / GetSize().x / 2));
	rot = VTransform(rot, MGetRotZ(-v.x / 100 / GetSize().x / 2));
	rot2 = VTransform(rot2, MGetRotX(v.z / 100 / GetSize().x / 2));
	rot2 = VTransform(rot2, MGetRotZ(-v.x / 100 / GetSize().x / 2));

	//	�p�x���Z�b�g

	MV1SetRotationZYAxis(_Model->GetHandle(), rot, rot2, 0);

	_Model->SetFront(rot);
	_Model->SetUp(rot2);
}

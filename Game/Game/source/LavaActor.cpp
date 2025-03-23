#include "LavaActor.h"

LavaActor::LavaActor(ModeBase* mode, VECTOR pos, VECTOR scale)
	: ActorClass(mode)
{
	SetPosition(pos);
	SetSize(scale);
	_Model = new ModelComponent(this, "res/Stage/model/maguma.mv1");
	//_Model->SetPosition(VGet(0, -50, 0));
	int handle = ModelServer::GetInstance()->Add("res/cube.mv1");
	_HCollision = new HitCollisionComponent(this, _Model, VGet(0, 0, 0), VGet(0.15, 0.02, 0.15), 6, false, true, handle);
}


LavaActor::~LavaActor()
{
}

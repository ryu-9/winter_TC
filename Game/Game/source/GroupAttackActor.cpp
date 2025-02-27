#include "GroupAttackActor.h"

GroupSpawnerActor::GroupSpawnerActor(ModeBase* mode, VECTOR pos)
	: ActorClass(mode), _TmCnt(0), _PopCnt(0), _TotalPopCnt(0)
{
	SetPosition(pos);
	Init();
}

GroupSpawnerActor::~GroupSpawnerActor() {
}

void GroupSpawnerActor::Init() {
}

void GroupSpawnerActor::UpdateActor() {
}

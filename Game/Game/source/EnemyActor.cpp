#include "EnemyActor.h"
#include "EnemyMoveComponent.h"
#include "EnemyAttackComponent.h"
#include "ECornComponent.h"
#include "EBoxComponent.h"
#include "PlayerActor.h"
#include "SnowComponent.h"
#include "EnemySpawnerActor.h"
#include "GroupAttackActor.h"
#include "ItemActor.h"

EnemyActor::EnemyActor(ModeBase* mode,VECTOR pos, EnemySpawnerActor* es)
	:ActorClass(mode)
	, _Spawner(es)
{
	SetPosition(pos);
	SetSize(VGet(2, 2, 2));
	_Input = new MoveComponent(this);
}

EnemyActor::EnemyActor(ModeBase* mode, VECTOR pos, GroupSpawnerActor* es) 
	:ActorClass(mode)
	, _GroupSpawner(es) 
{
	SetPosition(pos);
	SetSize(VGet(2, 2, 2));
	_Input = new MoveComponent(this);
}

EnemyActor::~EnemyActor() {
	if (_Spawner != nullptr) {
		_Spawner->DecrementPopCnt();
	}
	if (_GroupSpawner != nullptr) {
		_GroupSpawner->DecrementPopCnt();
	}

	//todo: Death()関数を作成して、そこで処理を行う

	auto item = new ItemActor(GetMode(), VAdd(GetPosition(), VGet(0, 100, 0)), 11 + rand() % 2);
	auto m = item->GetComponent<MoveComponent>()[0];
	VECTOR dir = VGet(0, 0, 0);
	dir = VTransform(dir, MGetRotY((float)(rand() % 360) / 180.0f * DX_PI));
	m->SetVelocity(VScale(VGet(dir.x, 0.25, dir.z), 5));
}

void EnemyActor::Init() {


}

void EnemyActor::UpdateActor() {
	/*
	for (auto mc : _MCollision->GetCollResult()) {
		auto snow = mc.mc->GetOwner()->GetComponent<SnowComponent>();
		for (auto s : snow) {
			s->AddMoveCollision(_MCollision);
		}
	}
	*/
}

int Drawing(std::vector<int> w)// 重み付き抽選
 {
	int total = 0;
	// 重みの合計を計算
	for (auto i = 0; i < w.size(); i++) {
		total = total + w[i];
	}

	int n = (rand() % total);
	int sum = 0;
	// 重みの合計を超えるまで足していく
	for (auto i = 0; i < w.size(); i++) {
		sum = sum + w[i];
		if (n - sum < 0) {
			return i;
		}
	}
	return 0;
}

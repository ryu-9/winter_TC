#include "EnemyActor.h"
#include "EnemyMoveComponent.h"
#include "EnemyAttackComponent.h"
#include "ECornComponent.h"
#include "EBoxComponent.h"
#include "PlayerActor.h"
#include "SnowComponent.h"
#include "EnemySpawnerActor.h"

EnemyActor::EnemyActor(ModeBase* mode,VECTOR pos, EnemySpawnerActor* es)
	:ActorClass(mode)
	, _Spawner(es)
{
	SetPosition(pos);
	SetSize(VGet(2, 2, 2));
	_Input = new MoveComponent(this);
}

EnemyActor::~EnemyActor() {
	if (_Spawner != nullptr) {
		_Spawner->DecrementPopCnt();
	}
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
}

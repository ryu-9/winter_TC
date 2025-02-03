#include "EnemyComponent.h"
#include "PlayerActor.h"

EnemyComponent::EnemyComponent(ActorClass* owner)
	:Component(owner)
{
	_En = dynamic_cast<EnemyActor*>(owner);

	_SearchRef.dist = 1000;
	_SearchRef.angle = 60;
}

EnemyComponent::~EnemyComponent() {
}

void EnemyComponent::ProcessInput() {

	/*
	//とりあえずランダムで回転
	auto rot = _Owner->GetComponent<ModelComponent>()->GetRotation();
	if (rotflag == 0){
		if (rand() % 300 < 1) {
			if (rand() % 2 == 0) { rotflag = 1; }
			else { rotflag = -1; }
			rottm = rand() % 100+60;
		}
	}
	else{
	if (rottm > 0) {
		rottm--;
		rot.y += rotflag * 0.01;
	}
	else {
		rottm = -1;
		rotflag = 0;
	}
	}
	_Owner->GetComponent<ModelComponent>()->SetRotation(rot);
	*/
	// 一定距離内にプレイヤーがいるか判断

	// 索敵
	Search(_Target);

	// プレイヤーがいる場合
	auto rnd = rand() % 10;
	

}

void EnemyComponent::Update() {

}

bool EnemyComponent::Search(std::vector<ActorClass*> target) {
	auto epos = _En->GetPosition();
	for (auto t : target) {
		auto ppos = t->GetPosition();
		auto dist = VSize(VSub(ppos, epos));
		if (dist < _SearchRef.dist) {
			if (VDot(VGet(0, 0, 1), VGet(ppos.x - epos.x, 0, ppos.z - epos.z)) < cosf(_SearchRef.angle)) {
				return true;
			}
		}
	}
}

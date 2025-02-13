#include "EnemyComponent.h"
#include "PlayerActor.h"

EnemyComponent::EnemyComponent(ActorClass* owner)
	:Component(owner)
{
}

EnemyComponent::~EnemyComponent() {
}

void EnemyComponent::ProcessInput() {
	// 索敵
	auto epos = _Owner->GetPosition();
	auto ppos = static_cast<ModeGame*>(ModeServer::GetInstance()->Get("game"))->GetPlayer()->GetPosition();
	auto dist = VSize(VSub(ppos, epos));
	auto rot = _Owner->GetComponent<ModelComponent>()[0]->GetRotation();

	//とりあえずランダムで回転
	
	if (rotflag == 0){
		if (rand() % 300 < 1) {
			if (rand() % 2 == 0) {
				rotflag = 1;
			}
			else {
				rotflag = -1;
			}rottm = rand() % 100+60;
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

	// 一定距離内にプレイヤーがいるか判断
	if (dist < 1000) {
		if (dist < 200) {
			rot.y = atan2(epos.x - ppos.x, epos.z - ppos.z);
			
		}
		//プレイヤーが視界に入っていたら
		if (VDot(VGet(0, 0, 1), VGet(ppos.x - epos.x, 0, ppos.z - epos.z)) < 0.5) {
			
		}
	}
	_Owner->GetComponent<ModelComponent>()[0]->SetRotation(rot);
}

void EnemyComponent::Update() {

}

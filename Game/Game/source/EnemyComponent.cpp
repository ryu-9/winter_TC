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

	// 一定距離内にプレイヤーがいるか判断
	if (dist < 1000) {
		if (VDot(_Owner->GetDirection(), ppos) > 0) {
			// プレイヤーの方向を向く
			auto dir = VGet(0, 0, 0);
			dir.x = ppos.x - epos.x;
			dir.z = ppos.z - epos.z;
			dir = VNorm(dir);
			_Owner->GetComponent<ModelComponent>()->SetFront(dir);
		}
	}
}

void EnemyComponent::Update() {

}

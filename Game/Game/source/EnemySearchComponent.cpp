#include "EnemySearchComponent.h"
#include "ModeGame.h"
#include "PlayerActor.h"

EnemySearchComponent::EnemySearchComponent(ActorClass* owner,int updateOrder)
	: Component(owner,updateOrder)
{
	_SearchRef.dist = 1000.0f;
	_SearchRef.angle = 60.0f;
}

EnemySearchComponent::~EnemySearchComponent()
{

}

void EnemySearchComponent::ProcessInput() {
	// 座標取得
	auto epos = _Owner->GetPosition();
	auto ppos = dynamic_cast<ModeGame*>(ModeServer::GetInstance()->Get("game"))->GetPlayer()->GetPosition();

	auto dist = VSize(VSub(ppos, epos));

	if (dist < _SearchRef.dist) {
		if (VDot(VGet(0, 0, 1), VGet(ppos.x - epos.x, 0, ppos.z - epos.z)) < cosf(_SearchRef.angle)) {
			//プレイヤーが視界に入っていたら

		}
	}
}


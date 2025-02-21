#include "EnemySpawnerActor.h"
#include "PlayerActor.h"
#include "EnemyCreator.h"

EnemySpawnerActor::EnemySpawnerActor(ModeBase* mode, VECTOR pos) 
	: ActorClass(mode)
{
	SetPosition(pos);
	new ModelComponent(this, "res/model/Enemy_corn/Enemy_corn.mv1");
	// プレイヤーの取得
	auto g = dynamic_cast<ModeGame*>(mode);
	_Player[0] = dynamic_cast<PlayerActor*>(g->GetPlayer(0));
	_Player[1] = dynamic_cast<PlayerActor*>(g->GetPlayer(1));
	_CoolTime = 0;
}

EnemySpawnerActor::~EnemySpawnerActor() {
}

void EnemySpawnerActor::Init() {
}

void EnemySpawnerActor::UpdateActor() {
	// 一定距離内にプレイヤーがいたら敵を生成
	_CoolTime += GetMode()->GetStepTm();
	if (_CoolTime < 1000) { return; }
	auto dist = VSize(VSub(_Player[0]->GetPosition(), GetPosition()));
	if (dist < 1000) {
		// 敵生成
		// 一定距離内に敵を生成
		auto pos = VAdd(GetPosition(), VScale(VGet(1, 0, 1), GetRand(50)));
		EnemyCreator::GetInstance()->Create(GetMode(), rand()%2, 0, pos);
		_Cnt++;
		_CoolTime = 0;
	}
	if (_Cnt == 7) {
		
		SetState(State::eDead);
	}
}

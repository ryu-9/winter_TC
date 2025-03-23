#include "EnemyActor.h"
#include "EnemyMoveComponent.h"
#include "ECornComponent.h"
#include "EBoxComponent.h"
#include "PlayerActor.h"
#include "SnowComponent.h"
#include "EnemySpawnerActor.h"
#include "GroupAttackActor.h"
#include "ItemActor.h"
#include "TimerComponent.h"

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
	if (GetPosition().y < -750) {
		_Fallcnt += _Mode->GetStepTm();
		if (_Fallcnt > 1000) {
			SetState(State::eDead);
		}
	}
}

void EnemyActor::Death(int type) {
	{
		auto item = new ItemActor(GetMode(), VAdd(GetPosition(), VGet(0, 100, 0)), 11 + rand() % 2);
		auto m = item->GetComponent<MoveComponent>()[0];
		VECTOR dir = VGet(0, 0, 0);
		dir = VTransform(dir, MGetRotY((float)(rand() % 360) / 180.0f * DX_PI));
		m->SetVelocity(VScale(VGet(dir.x, 0.25, dir.z), 5));
	}
	{
		auto ac = new ActorClass(GetMode());
		ac->SetPosition(GetPosition());
		auto sp = new EffectSpriteComponent(ac, "res/model/hit/hit.efkefc", VGet(0, 0, 0), VGet(0, 0, 0), 20);
		switch (type) {
		case 0:
			SoundServer::GetInstance()->Create(ac, "KillEnemy", "AttackSE", "KillEnemy");
			SoundServer::GetInstance()->GetSourceVoice(ac, "KillEnemy")->Play();
			break;
		case 1:
			SoundServer::GetInstance()->Create(ac, "KillEnemy2", "AttackSE", "KillEnemy2");
			SoundServer::GetInstance()->GetSourceVoice(ac, "KillEnemy2")->Play();
			break;
		default:
			break;
		}
		new TimerComponent(ac, 1000);
		
	}
	SetState(State::eDead);
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

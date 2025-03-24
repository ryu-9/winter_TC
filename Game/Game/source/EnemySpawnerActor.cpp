#include "EnemySpawnerActor.h"
#include "PlayerActor.h"
#include "EnemyCreator.h"
#include "PlayerActor.h"
#include "PunchActor.h"
#include "LaserActor.h"
#include "SlashActor.h"

EnemySpawnerActor::EnemySpawnerActor(ModeBase* mode, VECTOR pos, bool breakableflag, bool resetflag)
	: ActorClass(mode)
	, _TmCnt(0), _PopCnt(0), _Breakable(breakableflag), _ResetFlag(resetflag), _Type(0), _Col(0), _TotalPopCnt(0) {
	SetPosition(pos);
	if (_Breakable) { new ModelComponent(this, "res/Stage/model/EnemyWall/ENEMYWALL.mv1"); }
	
	// ÉvÉåÉCÉÑÅ[ÇÃéÊìæ
	auto game = static_cast<ModeGame*>(GetMode());

	_Player[0] = game->GetPlayer(0);
	_Player[1] = game->GetPlayer(1);
	Init();
	if (_Breakable) { _HitCol = _HitCol = new HitCollisionComponent(this, nullptr, VGet(0, 0, 0), VGet(10, 10, 10), 2); }
}

EnemySpawnerActor::~EnemySpawnerActor() {
}

void EnemySpawnerActor::Init() {
	_Data.max_pop = 1;
	_Data.max_popcount = 1;
	_Data.pop_range = 100;
	_Data.pop_time = 2000;
	_Data.active_range = 1000;
	_HP = 20;

}

void EnemySpawnerActor::UpdateActor() {
	_TmCnt += GetMode()->GetStepTm();
	auto skipflag = false;
	if (_TmCnt <_Data.pop_time) { skipflag = true; }
	if (_PopCnt >= _Data.max_popcount) { skipflag = true; }
	auto dist = VSize(VSub(_Player[0]->GetPosition(), GetPosition()));
	auto dist2 = VSize(VSub(_Player[1]->GetPosition(), GetPosition()));
	if (dist < _Data.active_range || dist2 < _Data.active_range) {
		// ìGê∂ê¨
		
		// àÍíËãóó£ì‡Ç…ìGÇê∂ê¨
		auto randdist = rand() % _Data.pop_range;
		auto randangle = rand() % 360;
		randangle = DEG2RAD(randangle);
		auto pos = VGet(cos(randangle) * randdist, 0, sin(randangle) * randdist);
		auto e =EnemyCreator::GetInstance()->Create(GetMode(), _Type, _Col, VAdd(GetPosition(), pos),this);
		_PopCnt++;
		_TotalPopCnt++;


		_TmCnt = 0;
	}
	if (_TotalPopCnt >= _Data.max_pop && !_Breakable){
		if (_ResetFlag == true) { _PopCnt = 0; }
		else { SetState(State::eDead); }
	}
	if (_HP <= 0) {
		SetState(State::eDead);
	}
	
	if (!_Breakable) { return; }
	auto hit = _HitCol->IsHit();
	for (auto h : hit) {
		auto p = dynamic_cast<PlayerActor*>(h->GetOwner());
		if (p != nullptr) {
			if (p->GetInvincibleTime() <= 0) {
				p->Damage(0.05);
				_HP -= 5;
			}
			if (p->GetModeNum() > 0) {
				_HP -= 20;
			}
		}
		auto punch = dynamic_cast<PunchActor*>(h->GetOwner());
		if (punch != nullptr) {
			_HP -= 20;
		}
		auto laser = dynamic_cast<LaserActor*>(h->GetOwner());
		if (laser != nullptr) {
			_HP -= 1;
		}
		auto slash = dynamic_cast<SlashActor*>(h->GetOwner());
		if (slash != nullptr) {
			_HP -= 20;
		}
	}
}

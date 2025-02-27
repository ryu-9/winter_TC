#include "EnemySpawnerActor.h"
#include "PlayerActor.h"
#include "EnemyCreator.h"

EnemySpawnerActor::EnemySpawnerActor(ModeBase* mode, VECTOR pos, bool groupflag, bool resetflag)
	: ActorClass(mode)
	, _TmCnt(0), _PopCnt(0), _GroupFlag(groupflag), _ResetFlag(resetflag)
{
	SetPosition(pos);
	new ModelComponent(this, "res/model/Enemy_corn/Enemy_corn.mv1");
	// ÉvÉåÉCÉÑÅ[ÇÃéÊìæ
	auto game = static_cast<ModeGame*>(GetMode());
	_Player[0] = game->GetPlayer(0);
	_Player[1] = game->GetPlayer(1);
	Init();
}

EnemySpawnerActor::~EnemySpawnerActor() {
}

void EnemySpawnerActor::Init() {
	_Data.max_pop = 1;
	_Data.max_popcount = 1;
	_Data.pop_range = 100;
	_Data.pop_time = 2000;
	_Data.active_range = 1000;
}

void EnemySpawnerActor::UpdateActor() {
	_TmCnt += GetMode()->GetStepTm();
	
	if (_TmCnt <_Data.pop_time) { return; }
	auto dist = VSize(VSub(_Player[0]->GetPosition(), GetPosition()));
	auto dist2 = VSize(VSub(_Player[1]->GetPosition(), GetPosition()));
	if (dist < _Data.active_range || dist2 < _Data.active_range) {
		// ìGê∂ê¨
		if (_PopCnt >= _Data.max_popcount) { return; }
		// àÍíËãóó£ì‡Ç…ìGÇê∂ê¨
		auto randdist = rand() % _Data.pop_range;
		auto randangle = rand() % 360;
		randangle = DEG2RAD(randangle);
		auto pos = VGet(cos(randangle) * randdist, 0, sin(randangle) * randdist);
		auto e =EnemyCreator::GetInstance()->Create(GetMode(), rand() % 2, 0, VAdd(GetPosition(), pos),this);
		_PopCnt++;
		_TotalPopCnt++;


		_TmCnt = 0;
	}
	if (_TotalPopCnt >= _Data.max_pop){
		if (_ResetFlag == true) { _PopCnt = 0; }
		else { SetState(State::eDead); }
	}
	
}

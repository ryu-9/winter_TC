#include "GroupAttackActor.h"
#include "EnemyCreator.h"

GroupSpawnerActor::GroupSpawnerActor(ModeBase* mode, VECTOR pos)
	: ActorClass(mode), _TmCnt(0), _PopCnt(0), _TotalPopCnt(0)
{
	SetPosition(pos);
	_Player[0] = static_cast<ModeGame*>(GetMode())->GetPlayer(0);
	_Player[1] = static_cast<ModeGame*>(GetMode())->GetPlayer(1);
	Init();
}

GroupSpawnerActor::~GroupSpawnerActor() {
}

void GroupSpawnerActor::Init() {
	_Data.max_pop = 4;
	_Data.max_popcount = 8;
	_Data.pop_range = 100;
	_Data.pop_time = 1000;
	
}

void GroupSpawnerActor::UpdateActor() {
	if (_Active == false) { 
		int n = 0;
		auto hit =_HCollision->IsHit();
		for (auto h : hit) {
			auto e = dynamic_cast<PlayerActor*>(h->GetOwner());
			if (e != nullptr) {
				n++;
				if (n >= 2) {
					_Active = true;
					break;
				}
			}
		}

	}
	else {
		_TmCnt += GetMode()->GetStepTm();

		if (_TmCnt < _Data.pop_time) { return; }
		if (_PopCnt >= _Data.max_popcount) { return; }
		auto dist = new int[_PopPos.size()];
		for (auto i = 0; i < _PopPos.size(); i++) {
			dist[i] = VSize(VSub(_Player[0]->GetPosition(), _PopPos[i]));
		}
		for (auto i = 0; i < _PopPos.size(); i++) {
			dist[i] += VSize(VSub(_Player[1]->GetPosition(), _PopPos[i]));
		}

		// 一番遠いスポナーから生成
		auto randdist = rand() % _Data.pop_range;
		auto randangle = rand() % 360;
		randangle = DEG2RAD(randangle);
		auto pos = VGet(cos(randangle) * randdist, 0, sin(randangle) * randdist);
		auto max = 0;
		for (auto i = 0; i < _PopPos.size(); i++) {
			if (dist[i] > dist[max]) {
				max = i;
			}
		}

		// エリア内に生成
		
		EnemyCreator::GetInstance()->Create(GetMode(), rand() % 2, 0, VAdd(_PopPos[max], pos));
		_PopCnt++;
		_TotalPopCnt++;
		_TmCnt = 0;

		delete[] dist;
	}
}


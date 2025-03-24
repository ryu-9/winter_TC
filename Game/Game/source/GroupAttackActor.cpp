#include "GroupAttackActor.h"
#include "EnemyCreator.h"
#include "ModeGame.h"
#include "CameraActor.h"
#include "PlayerActor.h"
#include "PunchActor.h"
#include "LaserActor.h"
#include "SlashActor.h"

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
					auto g = dynamic_cast<ModeGame*>(GetMode());
					auto c = g->GetCamera()->GetComponent<CameraComponent>()[0];
					
					VECTOR* p = new VECTOR;
					*p = GetPosition();
					p->y += 1000;
					p->z += -500;
					c->SetTarget2(&_Position);
					c->SetPosition2(p);
					c->SetEasing(1000);
					break;
				}
			}
		}

	}
	else {
		_TmCnt += GetMode()->GetStepTm();
		if (_TotalPopCnt >= _Data.max_popcount && _PopCnt <= 0) {
			_Active = false;
			auto g = dynamic_cast<ModeGame*>(GetMode());
			auto c = g->GetCamera()->GetComponent<CameraComponent>()[0];
			c->DeletePosition2();
			c->SetTarget2(nullptr);
			c->SetPosition2(nullptr);
			SetState(State::ePaused);
		}
		if (_TmCnt < _Data.pop_time) { return; }
		if (_PopCnt >= _Data.max_pop) { return; }
		if (_TotalPopCnt >= _Data.max_popcount) { return; }
		
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
		
		EnemyCreator::GetInstance()->Create(GetMode(), 0, VAdd(_PopPos[max], pos),this);
		_PopCnt++;
		_TotalPopCnt++;
		_TmCnt = 0;

		delete[] dist;

		for (auto i = 0; i < _Spawner.size(); i++) {
			if (_Spawner[i].hp <= 0) {
				//delete _Spawner[i].model;
				//delete _Spawner[i].hCollision;
				_Spawner[i].model->SetVisible(false);
				_Spawner[i].hCollision->SetIsActive(false);
				_Spawner.erase(_Spawner.begin() + i);
			}

			auto hit = _Spawner[i].hCollision->IsHit();
			for (auto h : hit) {
				auto p = dynamic_cast<PlayerActor*>(h->GetOwner());
				if (p != nullptr) {
					if (p->GetInvincibleTime() <= 0) {
						p->Damage(0.05);
						_Spawner[i].hp -= 5;
					}
					if (p->GetModeNum() > 0) {
						_Spawner[i].hp -= 20;
					}
				}
				auto punch = dynamic_cast<PunchActor*>(h->GetOwner());
				if (punch != nullptr) {
					_Spawner[i].hp -= 20;
				}
				auto laser = dynamic_cast<LaserActor*>(h->GetOwner());
				if (laser != nullptr) {
					_Spawner[i].hp -= 1;
				}
				auto slash = dynamic_cast<SlashActor*>(h->GetOwner());
				if (slash != nullptr) {
					_Spawner[i].hp -= 20;
				}
			}
		}
		
	}
}

void GroupSpawnerActor::AddPopPos(VECTOR pos) {
	_PopPos.push_back(pos);
	auto tmpos = VSub(_Position,pos);
	tmpos.y *= -1;//_Position.y -(100 * _Size.y);
	auto model = new ModelComponent(this, "res/model/Enemy_corn/Enemy_corn.mv1",100,false);
	model->SetPosition(tmpos);
	auto hcol = new HitCollisionComponent(this, nullptr, tmpos, VGet(10, 10, 10), 2,false, true,-1,false);
	Spawner s;
	s.model = model;
	s.hCollision = hcol;
	s.hp = 20;
	_Spawner.push_back(s);
}


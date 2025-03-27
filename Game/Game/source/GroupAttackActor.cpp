#include "GroupAttackActor.h"
#include "EnemyCreator.h"
#include "ModeGame.h"
#include "CameraActor.h"
#include "PlayerActor.h"
#include "PunchActor.h"
#include "LaserActor.h"
#include "SlashActor.h"
#include "UITextActor.h"
#include "ApplicationGlobal.h"
#include "ExplosionActor.h"

GroupSpawnerActor::GroupSpawnerActor(ModeBase* mode, VECTOR pos,int num)
	: ActorClass(mode), _TmCnt(0), _PopCnt(0), _TotalPopCnt(0), _Active(false), _Num(num)
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
				if (e->GetModeNum() > 0) {
					n++;
				}
				if (n >= 2) {
					_Active = true;
					gGlobal._IsGroupAttack = true;
					auto g = dynamic_cast<ModeGame*>(GetMode());
					auto c = g->GetCamera()->GetComponent<CameraComponent>()[0];
					
					VECTOR* p = new VECTOR;
					*p = GetPosition();
					p->y += GetSize().z * 80;
					p->z += -GetSize().z * 55;
					VECTOR * t = new VECTOR;
					*t = GetPosition();
					t->y += -GetSize().z * 50;
					c->SetTarget2(t);
					c->SetPosition2(p);
					c->SetEasing(1000);
					
					auto s = gGlobal._SelectStage +1;
					auto n = _Num +1;
					std::string key = "g" + std::to_string(s) + "_" + std::to_string(n);
					g->GetUIT()->AddText("Scenario", key, true);
					break;
				}
			}
		}

	}
	else {
		_TmCnt += GetMode()->GetStepTm();
		if ((_TotalPopCnt >= _Data.max_popcount && _PopCnt <= 0)) {
			_Active = false;
			gGlobal._IsGroupAttack = false;
			auto g = dynamic_cast<ModeGame*>(GetMode());
			auto c = g->GetCamera()->GetComponent<CameraComponent>()[0];
			c->DeletePosition2();
			c->DeleteTarget2();
			c->SetTarget2(nullptr);
			c->SetPosition2(nullptr);
			c->SetEasing(1000);
			SetState(State::ePaused);
			if (gGlobal._SelectStage == 0) {
				g->GetUIT()->AddText("Scenario", "win", true);
			}
			if (gGlobal._SelectStage == 1) {
				switch (_Num) {
				case 0:
					g->GetUIT()->AddText("Scenario", "g2_1win", true);
					break;
				case 1:
					g->GetUIT()->AddText("Scenario", "g2_2win", true);
					break;
				case 2:
					g->GetUIT()->AddText("Scenario", "g2_3win", true);
					break;
				case 3:
					g->GetUIT()->AddText("Scenario", "g2_4win", true);
					break;
				case 4:
					g->GetUIT()->AddText("Scenario", "g2_5win", true);
					break;
				default:
					break;
				}
			}
		}


		auto skipflag = false;
		if (_TmCnt < _Data.pop_time) { skipflag = true; }
		if (_PopCnt >= _Data.max_pop) { skipflag = true; }
		if (_TotalPopCnt >= _Data.max_popcount) { skipflag = true; }
		
		if (skipflag == false) {
			auto dist = new int[_PopPos.size()];
			for (auto i = 0; i < _PopPos.size(); i++) {
				if (_Spawner[i].hp <= 0) { dist[i] = -1; } else {
					dist[i] = VSize(VSub(_Player[0]->GetPosition(), _PopPos[i]));
				}
			}
			for (auto i = 0; i < _PopPos.size(); i++) {
				if (_Spawner[i].hp <= 0) { dist[i] = -1; } else {
					dist[i] += VSize(VSub(_Player[1]->GetPosition(), _PopPos[i]));
				}
			}

			// ��ԉ����X�|�i�[���琶��
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

			// �G���A��ɐ���

			EnemyCreator::GetInstance()->Create(GetMode(), 0, VAdd(_PopPos[max], pos), this);
			_PopCnt++;
			_TotalPopCnt++;
			_TmCnt = 0;

			delete[] dist;

		}
		auto deathcnt = 0;
		for (auto i = 0; i < _Spawner.size(); i++) {
			if (_Spawner[i].hp <= 0) {
				//delete _Spawner[i].model;
				//delete _Spawner[i].hCollision;
				_Spawner[i].model->SetVisible(false);
				_Spawner[i].hCollision->SetIsActive(false);
				deathcnt++;
				continue;
			}

			auto hit = _Spawner[i].hCollision->IsHit();
			for (auto h : hit) {
				auto p = dynamic_cast<PlayerActor*>(h->GetOwner());
				if (p != nullptr) {
					if (p->GetModeNum() > 0) {
						_Spawner[i].hp -= 20;
					} else {
						if (p->GetInvincibleTime() <= 0) {
							VECTOR knock = VSub(p->GetPosition(), _PopPos[i]);
							if (VSize(knock) == 0) { knock = VGet(0, 1, 0); }
							knock = VNorm(knock);
							p->KnockBack(VGet(knock.x, 0.2, knock.z), 10);
							p->Damage(0.05);
							_Spawner[i].hp -= 5;
						}
					}
				}
				auto punch = dynamic_cast<PunchActor*>(h->GetOwner());
				if (punch != nullptr) {
					_Spawner[i].hp -= 20;
				}
				auto exp = dynamic_cast<ExplosionActor*>(h->GetOwner());
				if (exp != nullptr) {
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
		if (deathcnt >= _Spawner.size()) {
			// ポーズできるようにしておく
			_TotalPopCnt = _Data.max_popcount;
		}
	}
}

void GroupSpawnerActor::AddPopPos(VECTOR pos) {
	_PopPos.push_back(pos);
	auto tmpos = VSub(_Position,pos);
	tmpos.y *= -1;//_Position.y -(100 * _Size.y);
	auto model = new ModelComponent(this, "res/Stage/model/EnemyWall/ENEMYWALL.mv1",100,false);
	model->SetScale(VGet(0.5, 0.5, 0.5));
	model->SetPosition(tmpos);
	tmpos.y += 100;
	auto hcol = new HitCollisionComponent(this, nullptr, tmpos, VGet(15, 15, 15), 2,false, true,-1,false);
	Spawner s;
	s.model = model;
	s.hCollision = hcol;
	s.hp = 20;
	_Spawner.push_back(s);
}


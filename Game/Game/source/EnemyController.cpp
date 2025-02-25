#include "EnemyController.h"
#include "ModeGame.h"
#include "EnemyActor.h"
#include "EnemyCreator.h"
#include "PlayerActor.h"

EnemyController::EnemyController(class ModeGame* g)
	:ActorClass(g)
{
	_Player[0] = g->GetPlayer(0);
	_Player[1] = g->GetPlayer(1);
}

EnemyController::~EnemyController() {
}


void EnemyController::UpdateActor() {
	for (auto& f : _FightData) {
		switch (f.state) {
		case 0:
			// 待機
			// 仮で起動
			if (_Player[0] != nullptr && _Player[1] != nullptr) {
				auto dist = VSize(VSub(_Player[0]->GetPosition(), f.spawner[0]));
				if (dist < 1000) {
					f.state = 1;
				}
			}
			break;
		case 1:
			// 生成 (仮)
			if (f.spawner.size() > 0) {
				if (f.pop >= f.max_pop) {
					continue;
				}
				VECTOR pos[2];
				auto dist = new int[f.spawner.size()];
				auto dist2 = new int[f.spawner.size()];
				for (auto i = 0; i < 2; i++) {
					pos[i] = _Player[i]->GetPosition();
					for (auto j = 0; j < f.spawner.size(); j++) {
						dist[j] = VSize(VSub(pos[i], f.spawner[j]));
					}
				}
				for (auto i = 0; i < f.spawner.size(); i++) {
					if (dist[i] < dist2[i]) {
						dist2[i] = dist[i];
					}
				}
				// 一番遠いスポナーから生成
				auto max = 0;
				for (auto i = 0; i < f.spawner.size(); i++) {
					if (dist2[i] > dist2[max]) {
						max = i;
					}
				}
				// 生成
				//EnemyCreator::GetInstance()->Create(GetMode(), 0, 0, f.spawner[max]);
				f.pop++;
			}
			break;
		}
	}
}

void EnemyController::AddEnemy(EnemyActor* enemy) {
}

void EnemyController::RemoveEnemy(EnemyActor* enemy) {
}

void EnemyController::AddSpawner(int fightnum, VECTOR spawnerpos) {
	if (_FightData.size() <= fightnum) {
		FIGHT_DATA data;
		data.state = 0;
		data.pop = 0;
		data.norma = 8;
		data.max_pop = 8;
		_FightData.push_back(data);
	}
	else {
		_FightData[fightnum].spawner.push_back(spawnerpos);
	}
}


#pragma once
#include <vector>
#include "EnemyActor.h"
#include "EnemySpawnerActor.h"

class EnemyActor;

struct FIGHT_DATA {
	int state;
	int pop;
	int norma;
	int max_pop;
	std::vector<VECTOR> spawner;
	std::vector<EnemyActor*> enemy;
};

class EnemyController : public ActorClass{
public:
	EnemyController(class ModeGame* g);
	~EnemyController();
	
	void UpdateActor() override;
	void AddEnemy(EnemyActor* enemy);
	void RemoveEnemy(EnemyActor* enemy);

	void AddSpawner(int fightnum,VECTOR spawnerpos);

private:
	std::vector<FIGHT_DATA> _FightData;
	class PlayerActor* _Player[2];
};


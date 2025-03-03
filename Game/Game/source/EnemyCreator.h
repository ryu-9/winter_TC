#pragma once
#include "appframe.h"
#include "EnemyActor.h"
#include "EnemySpawnerActor.h"

#include <array>

class EnemyCreator {
#define PATTERN_MAX 8

	
public:

	EnemyCreator();
	virtual ~EnemyCreator();

	static EnemyCreator* _lpInstance;
	static EnemyCreator* GetInstance() { return (EnemyCreator*)_lpInstance; }
	
	void Init();
	class EnemyActor* Create(ModeBase* mode,int shape, int col,VECTOR vec,EnemySpawnerActor* es = nullptr);
	class EnemyActor* Create(ModeBase* mode, int type, VECTOR vec, EnemySpawnerActor* es = nullptr);
	
protected:

	void Import();

	std::vector<int> _CreatePattern[PATTERN_MAX];
};


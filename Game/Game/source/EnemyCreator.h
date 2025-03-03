#pragma once
#include "appframe.h"
#include "EnemyActor.h"
#include "EnemySpawnerActor.h"

#include <array>

class EnemyCreator {
#define PATTERN_MAX 8

	struct PATTERN {
		int corn_a;
		int corn_b;
		int corn_c;
		int corn_d;
		int box_a;
	};
public:

	EnemyCreator();
	virtual ~EnemyCreator();

	static EnemyCreator* _lpInstance;
	static EnemyCreator* GetInstance() { return (EnemyCreator*)_lpInstance; }
	
	void Init();
	class EnemyActor* Create(ModeBase* mode,int shape, int col,VECTOR vec,EnemySpawnerActor* es = nullptr);
	
protected:

	void Import();

	PATTERN _CreatePattern[PATTERN_MAX];
};


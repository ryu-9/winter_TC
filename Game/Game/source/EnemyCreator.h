#pragma once
#include "appframe.h"
#include "EnemyActor.h"

#include <array>

class EnemyCreator {
#define ENEMY_SHAPE 2
#define ENEMY_COL 6

public:

	EnemyCreator();
	virtual ~EnemyCreator();

	static EnemyCreator* _lpInstance;
	static EnemyCreator* GetInstance() { return (EnemyCreator*)_lpInstance; }
	
	void Init();
	class EnemyActor* Create(ModeBase* mode,int shape, int col);
	
protected:

	void Import();


	std::array<std::array<EnemyActor::PARAM, ENEMY_COL>, ENEMY_SHAPE> _Data;
};


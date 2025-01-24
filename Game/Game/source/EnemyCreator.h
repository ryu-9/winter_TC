#pragma once
#include "appframe.h"
#include "EnemyActor.h"

class EnemyCreator {
public:
	EnemyCreator();
	virtual ~EnemyCreator();
	
	void Init();
	class EnemyActor* Create();
	
protected:

	void Import();


};


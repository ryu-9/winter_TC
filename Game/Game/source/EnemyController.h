#pragma once

class EnemyActor;



class EnemyController {
public:
	EnemyController();
	~EnemyController();
	void Update();
	void AddEnemy(EnemyActor* enemy);
	void RemoveEnemy(EnemyActor* enemy);
};


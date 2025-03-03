#pragma once
#include "appframe.h"
#include "EnemyActor.h"



class EnemyComponent : public Component {
public:
	EnemyComponent(ActorClass* owner);
	~EnemyComponent();
	void ProcessInput() {};
	void Update() {};

	enum  STATUS {
		NON,
		SEARCH,
		WAIT,
		DISCOVER,
		ATTACK,
		COOLTIME,
	};

protected:
	bool Search(std::vector<ActorClass*> target);

	// 移動
	virtual bool Move() { return false; };
	virtual bool Turn() { return false; };
	virtual bool SetVelocity() { return false; };
	
	// 攻撃
	virtual bool Attack() { return false; };



	EnemyActor* _En;
	// 全般
	STATUS _Status;
	int _CurrentTime;	// 経過時間
	int _Duration;		// 行動時間
	int _CoolTime;		// クールタイム

	// 索敵
	std::vector<ActorClass*> _Target;
	std::vector<int> _Index;
	Enemy::SEARCH _SearchRef;

	//移動
	float _MoveDist;

	// 重み
	std::vector<int> _Weight;

};


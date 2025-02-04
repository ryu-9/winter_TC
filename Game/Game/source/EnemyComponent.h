#pragma once
#include "appframe.h"
#include "EnemyActor.h"

class EnemyComponent : public Component{
public:
	EnemyComponent(ActorClass* owner);
	~EnemyComponent();
	void ProcessInput();
	void Update();

	enum  STATUS {
		NON,
		SEARCH,
		WAIT,
		DISCOVERY,
		ATTACK,
	};

protected:
	bool Search(std::vector<ActorClass*> target);
	
	// 移動
	virtual bool Move();
	virtual bool Turn();
	virtual bool SetVelocity();
	
	// 攻撃
	virtual bool Attack() { return false; };




	// 全般
	STATUS _Status;
	int _CurrentTime;	// 経過時間
	int _Duration;		// 持続時間
	
	EnemyActor* _En;

	// 索敵
	std::vector<ActorClass*> _Target;
	std::vector<int> _Index;
	Enemy::SEARCH _SearchRef;



	// 重み付き抽選
	template<class... Args>
	int Drawing(Args... args) {
		int total = 0;
		uint8_t l = sizeof...(args);
		int data[] = { args... };
		// 重みの合計を計算
		for (auto i = 0; i < l; i++) {
			total = total + data[i];
		}
			
			int n = (rand() % total);
			int sum = 0;
			// 重みの合計を超えるまで足していく
			for (auto i = 0; i < l; i++) {
				sum = sum + data[i];
				if (n - sum < 0) {
					return i;
				}
			}
	}

};


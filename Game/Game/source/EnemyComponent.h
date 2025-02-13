#pragma once
#include "appframe.h"
#include "EnemyActor.h"
struct SEARCH {
	float dist;
	float angle;
	int cooltime;
};
struct MOVE {
	int duration;
	int dur_rand;
	int cooltime;
	int cool_rand;
	float dist;
	float dist_rand;
};
struct EPARAM {
	int damage;
	int speed;
	int serchdist;
};

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

	// 重み付き抽選
	int Drawing(std::vector<int> w) {
		int total = 0;
		// 重みの合計を計算
		for (auto i = 0; i < w.size(); i++) {
			total = total + w[i];
		}

		int n = (rand() % total);
		int sum = 0;
		// 重みの合計を超えるまで足していく
		for (auto i = 0; i < w.size(); i++) {
			sum = sum + w[i];
			if (n - sum < 0) {
				return i;
			}
		}
	}

};


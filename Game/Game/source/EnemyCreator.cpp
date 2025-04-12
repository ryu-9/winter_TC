#include "EnemyCreator.h"
#include "EnemyActor.h"
#include "ECornComponent.h"
#include "EBoxComponent.h"

#include "nlohmann/json.hpp"
#include <fstream>

EnemyCreator* EnemyCreator::_lpInstance = nullptr;

EnemyCreator::EnemyCreator() {
	_lpInstance = this;
	Import();
	Init();
}

EnemyCreator::~EnemyCreator() {
}

void EnemyCreator::Init() {

	_CreatePattern[0] = { 5, 2, 1, 0, 4 };
	_CreatePattern[1] = { 3, 1, 2, 1, 1 };
	_CreatePattern[2] = { 1, 1, 1, 1, 1 };
	_CreatePattern[3] = { 1, 2, 3, 2, 0 };
	_CreatePattern[4] = { 0, 2, 3, 1, 4 };
	_CreatePattern[5] = { 1, 1, 1, 1, 1 };
	_CreatePattern[6] = { 1, 1, 1, 1, 6 };
	_CreatePattern[7] = { 0, 1, 3, 3, 3 };


}

EnemyActor* EnemyCreator::Create(ModeBase* mode, int shape, int col,VECTOR vec,EnemySpawnerActor* es) {

	EnemyActor* enemy = new EnemyActor(mode,vec,es);

	// ¶¬
	switch (shape)
	{
	case 0:
	{
		auto c = new ECornComponent(enemy);
		auto m = new ModelComponent(enemy, "res/model/Enemy_Corn/Enemy_corn.mv1");
		enemy->SetModel(m);
		enemy->SetMoveCollision(new MoveCollisionComponent(enemy, m, VGet(0, 0, 0), VGet(15, 15, 15), 2, true, true));
		enemy->SetHitCollision(new HitCollisionComponent(enemy, m, VGet(0, 0, 0), VGet(20, 20, 20), 2, true, true));
		c->SetAttackType(col); switch (col) {
		case 1:
			c->SetAttackType(1);
		default:
			break;
		}
	}
		break;
	case 1:
	{
		
		auto m = new ModelComponent(enemy, "res/model/Enemy_Box/mouse_Open/Enemy_box.mv1");
		m->SetIndipendent(true);
		enemy->SetModel(m);
		enemy->SetMoveCollision(new MoveCollisionComponent(enemy, m, VGet(0, 0, 0), VGet(15, 15, 15), 2, true, true));
		new EBoxComponent(enemy);
		enemy->SetHitCollision(new HitCollisionComponent(enemy, m, VGet(0, 0, 0), VGet(20, 20, 20), 2, true, true));
		enemy->SetSize(VGet(4, 4, 4));
	
		}	break;
	default:
		break;
	}

	
	return nullptr;
}



EnemyActor* EnemyCreator::Create(ModeBase* mode, int type, VECTOR vec, GroupSpawnerActor* es) {
	EnemyActor* enemy = new EnemyActor(mode, vec, es);

	auto t = Drawing(_CreatePattern[type]);
	switch (t) {
	case 0:
	case 1:
	case 2:
	case 3:
	{
		auto c = new ECornComponent(enemy);
		auto m = new ModelComponent(enemy, "res/model/Enemy_Corn/Enemy_corn.mv1");
		enemy->SetModel(m);
		enemy->SetMoveCollision(new MoveCollisionComponent(enemy, m, VGet(0, 0, 0), VGet(15, 15, 15), 2, true, true));
		enemy->SetHitCollision(new HitCollisionComponent(enemy, m, VGet(0, 0, 0), VGet(15, 15, 15), 2, true, true));
		c->SetAttackType(t);
	}	break;
	case 4:
	{
		auto m = new ModelComponent(enemy, "res/model/Enemy_Box/mouse_Open/Enemy_box.mv1");
		m->SetIndipendent(true);
		enemy->SetModel(m);
		enemy->SetMoveCollision(new MoveCollisionComponent(enemy, m, VGet(0, 0, 0), VGet(15, 15, 15), 2, true, true));
		new EBoxComponent(enemy);
		enemy->SetHitCollision(new HitCollisionComponent(enemy, m, VGet(0, 0, 0), VGet(20, 20, 20), 2, true, true));
		enemy->SetSize(VGet(4, 4, 4));
	}	break;
	default:
		break;
	}
	
	return nullptr;
}




void EnemyCreator::Import() {

}

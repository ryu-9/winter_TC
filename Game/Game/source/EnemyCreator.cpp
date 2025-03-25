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
/*
	std::array<std::string, ENEMY_SHAPE> shape;
	std::array<std::string, ENEMY_COL> col;
	shape[0] = "corn";
	shape[1] = "cube";

	col[0] = "orange";
	col[1] = "green";
	col[2] = "pink";
	col[3] = "blue";
	col[4] = "white";
	col[5] = "black";

	std::ifstream file("res/EnemyData.json");
	nlohmann::json json;
	file >> json;

	int i = 0, j = 0;
	for (auto&& s : shape) {
		for (auto&& c : col) {
			nlohmann::json cols = json[s];
			nlohmann::json palam = cols[c];

			EnemyActor::PARAM param{};
			param.hp = palam["hp"];
			param.damage = palam["damage"];
			param.speed = palam["speed"];
			param.serchdist = palam["serchdist"];
			param.interval = palam["interval"];

			_Data[i][j] = param;
			j++;
		}
		i++;
	}
		*/
	_CreatePattern[0] = { 5, 2, 1, 0, 0 };
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
	//enemy->SetParam(_Data[shape][col]);

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
	// TODO: エネミーの情報を良い感じにとれるように
}



EnemyActor* EnemyCreator::Create(ModeBase* mode, int type, VECTOR vec, GroupSpawnerActor* es) {
	EnemyActor* enemy = new EnemyActor(mode, vec, es);
	//enemy->SetParam(_Data[shape][col]);

	auto t = Drawing(_CreatePattern[type]);
	// TODO: 0~3の生成法を変更
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

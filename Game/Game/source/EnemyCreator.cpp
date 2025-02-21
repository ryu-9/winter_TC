#include "EnemyCreator.h"
#include "EnemyActor.h"
#include "EnemyAttackComponent.h"
#include "ECornComponent.h"
#include "EBoxComponent.h"

#include "nlohmann/json.hpp"
#include <fstream>

EnemyCreator* EnemyCreator::_lpInstance = nullptr;

EnemyCreator::EnemyCreator() {

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
}

EnemyActor* EnemyCreator::Create(ModeBase* mode, int shape, int col,VECTOR vec) {

	EnemyActor* enemy = new EnemyActor(mode,vec);
	//enemy->SetParam(_Data[shape][col]);

	switch (shape)
	{
	case 0:
	{
		new ECornComponent(enemy);
		auto m = new ModelComponent(enemy, "res/model/Enemy_Corn/Enemy_corn.mv1");
		enemy->SetModel(m);
		enemy->SetMoveCollision(new MoveCollisionComponent(enemy, m, VGet(0, 0, 0), VGet(15, 15, 15), 2, true, true));
		enemy->SetHitCollision(new HitCollisionComponent(enemy, m, VGet(0, 0, 0), VGet(15, 15, 15), 2, true, true));
	}
		break;
	case 1:
	{
		
		auto m = new ModelComponent(enemy, "res/model/Enemy_Box/mouse_Open/Enemy_box.mv1");
		m->SetIndipendent(true);
		enemy->SetModel(m);
		enemy->SetMoveCollision(new MoveCollisionComponent(enemy, m, VGet(0, 0, 0), VGet(15, 15, 15), 2, true, true));
		new EBoxComponent(enemy);
		enemy->SetHitCollision(new HitCollisionComponent(enemy, m, VGet(0, 0, 0), VGet(15, 15, 15), 2, true, true));
	}	break;
	default:
		break;
	}
	return nullptr;
	// TODO: エネミーの情報を良い感じにとれるように
}




void EnemyCreator::Import() {

}

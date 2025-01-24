#include "EnemyCreator.h"
#include "EnemyActor.h"
#include "EnemyAttackComponent.h"

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
			
}

EnemyActor* EnemyCreator::Create(ModeBase* mode, int shape, int col) {

	EnemyActor* enemy = new EnemyActor(mode);
	enemy->SetParam(_Data[shape][col]);

	switch (col)
	{
	case 0:
		new EnemyAttackComponent(enemy);
		break;
	default:
		new EnemyAttackComponent(enemy);
		break;
	}
	return nullptr;
}




void EnemyCreator::Import() {

}

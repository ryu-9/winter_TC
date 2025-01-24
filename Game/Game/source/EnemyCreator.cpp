#include "EnemyFactory.h"
#include "EnemyCreator.h"

EnemyCreator::EnemyCreator() {
	Import();
	Init();
}

EnemyCreator::~EnemyCreator() {
}

void EnemyCreator::Init() {
}

EnemyActor* EnemyCreator::Create() {
	return nullptr;
}

void EnemyCreator::Import() {

}

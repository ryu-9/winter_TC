#include "EnemyComponent.h"
#include "PlayerActor.h"

EnemyComponent::EnemyComponent(ActorClass* owner)
	:Component(owner)
{
	_En = dynamic_cast<EnemyActor*>(owner);

	_SearchRef.dist = 1000;
	_SearchRef.angle = 60;
}

EnemyComponent::~EnemyComponent() {}

bool EnemyComponent::Search(std::vector<ActorClass*> target) {
	_Index.clear();
	bool f = false;
	auto epos = _En->GetPosition();
	for (auto i = 0; i < target.size();i++) {
		auto ppos = target[i]->GetPosition();
		auto dist = VSize(VSub(ppos, epos));
		if (dist < _SearchRef.dist) {
			auto front = _Owner->GetComponent<ModelComponent>()->GetFront();
			front = VScale(front, -1);
			// ‚à‚µƒvƒŒƒCƒ„[‚ªŽ‹ŠE‚É“ü‚Á‚Ä‚¢‚é‚©
			if (VDot(VNorm(VSub(ppos, epos)), front) > cosf(_SearchRef.angle * DX_PI / 180)) {
				_Index.push_back(i);
				f = true;
			}
		}
	}
	return f;
}


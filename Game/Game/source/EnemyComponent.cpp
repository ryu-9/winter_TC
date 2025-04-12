#include "EnemyComponent.h"
#include "PlayerActor.h"

EnemyComponent::EnemyComponent(ActorClass* owner)
	:Component(owner)
{
	_En = dynamic_cast<EnemyActor*>(owner);

	_SearchRef.dist = 2000;
	_SearchRef.angle = 60;
}

EnemyComponent::~EnemyComponent() {}

bool EnemyComponent::Search(std::vector<ActorClass*> target) {
	_Index.clear();
	bool f = false;
	auto epos = _En->GetPosition();
	std::vector<int> d;
	for (auto i = 0; i < target.size();i++) {
		auto ppos = target[i]->GetPosition();
		auto tdist = VSize(VSub(ppos, epos));
		
		if (tdist < _SearchRef.dist) {
			auto front = _Owner->GetComponent<ModelComponent>()[0]->GetFront();
			front = VScale(front, -1);
			
			if (VDot(VNorm(VSub(ppos, epos)), front) > cosf(_SearchRef.angle * DX_PI / 180)) {
				auto p = dynamic_cast<PlayerActor*>(target[i]);
				if (p != nullptr) {
					if (p->GetModeNum() != -1) {		// Ž€‚ñ‚Å‚È‚©‚Á‚½‚ç
						f = true;
						if (!d.empty()) {
							if (tdist < d[0]) {		// ‹ß‚¢‡‚É•À‚×‚é
								d.push_back(tdist);
							} else {
								d.insert(d.begin(), tdist);
								_Index.insert(_Index.begin(), i);
							}
						} else {
							d.push_back(tdist);
							_Index.push_back(i);
						}
					}
				} else {
					f = true;
					if (!d.empty()) {
						if (tdist < d[0]) {		// ‹ß‚¢‡‚É•À‚×‚é
							d.push_back(tdist);
						} else {
							d.insert(d.begin(), tdist);
							_Index.insert(_Index.begin(), i);
						}
					} else {
						d.push_back(tdist);
						_Index.push_back(i);
					}
				}
			}
		}
	}
	return f;
}


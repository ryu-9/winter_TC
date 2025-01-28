#include "SEComponent.h"

SEComponent::SEComponent(ActorClass* owner,ActorClass* listner,int seflag, int updateOrder)
	:Component(owner, updateOrder)
	, _Listener(listner)
{

}

SEComponent::~SEComponent() {

}

void SEComponent::Update() {
	auto pos = _Owner->GetPosition();
	auto lpos = _Listener->GetPosition();
	// 点音源のパターン

	auto dist = VSize(VSub(pos, lpos));
	auto db = 10 * log10(100/dist);
	_SV->SetVolume(db);
}

void SEComponent::Receive(int message) {

}

#include "SEComponent.h"


SEComponent::SEComponent(ActorClass* owner,int seflag, int updateOrder)
	:Component(owner, updateOrder)
{

}

SEComponent::~SEComponent() {
//	_SourceVoice->Stop();
//	_SourceVoice->DestroyVoice();
//	_SourceVoice = nullptr;
}

void SEComponent::Update() {

}

void SEComponent::Receive(int message) {

}

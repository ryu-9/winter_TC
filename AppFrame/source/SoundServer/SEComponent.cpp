#include "SEComponent.h"


SEComponent::SEComponent(ActorClass* owner,int seflag, int updateOrder)
	:Component(owner, updateOrder)
{

}

SEComponent::~SEComponent() {

}

void SEComponent::Update() {

}

void SEComponent::Receive(int message) {

}

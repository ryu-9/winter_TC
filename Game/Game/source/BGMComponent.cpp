#include "BGMComponent.h"
#include "ModeGame.h"
#include "PlayerActor.h"


BGMComponent::BGMComponent(ActorClass* owner)
	:SoundComponent(owner)
{
}

BGMComponent::~BGMComponent()
{
}

void BGMComponent::Update() {
	auto n = dynamic_cast<ModeGame*>(_Owner->GetMode())->GetPlayer()->GetModeNum();

}

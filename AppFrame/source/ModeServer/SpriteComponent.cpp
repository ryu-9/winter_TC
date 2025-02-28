#include "SpriteComponent.h"
#include "ActorClass.h"


SpriteComponent::SpriteComponent(ActorClass* owner, int drawOrder)
	:Component(owner)
	, _DrawOrder(drawOrder)
{
	_Owner->GetMode()->AddSprite(this);
}

SpriteComponent::~SpriteComponent() {
	_Owner->GetMode()->RemoveSprite(this);
}

void SpriteComponent::Draw() {

}


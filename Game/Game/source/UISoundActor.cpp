#include "UISoundActor.h"

UISoundActor::UISoundActor(ModeBase* mode) 
	: ActorClass(mode)
{
}

UISoundActor::~UISoundActor() {
}

void UISoundActor::UpdateActor() {

}

void UISoundActor::PlayActSound(std::string actname) {
	auto s = new SoundComponent(this);
	s->SetSourceVoice(new SourceVoiceItem(_SoundMap[actname]));
	s->Play(0);

}

#include "UISoundActor.h"

UISoundActor::UISoundActor(ModeBase* mode) 
	: ActorClass(mode)
{
}

UISoundActor::~UISoundActor() {
	SoundServer::GetInstance()->Release(this);
}

void UISoundActor::UpdateActor() {
	SoundServer::GetInstance()->Update(this);

}

void UISoundActor::AddSound(std::string actname, std::string svname) {
	SoundServer::GetInstance()->Create(this, svname, "UI", actname);
}

void UISoundActor::PlayActSound(std::string actname) {
	SoundServer::GetInstance()->GetSourceVoice(this, actname)->Play();
}

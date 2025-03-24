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
	_ActSV[actname] = svname;
	
}

void UISoundActor::PlayActSound(std::string actname) {
	auto p = SoundServer::GetInstance()->Create(this, _ActSV[actname], "UI", actname);
	SoundServer::GetInstance()->GetSourceVoice(this, actname)->Play();
}

#pragma once


#include "SoundComponent.h"
#include "SourceVoiceItem.h"
#include "SoundServer.h"
#include <vector>

class SourceVoiceItem;

class SEComponent : public SoundComponent {
public:
	SEComponent(class ActorClass* owner, ActorClass* listner);
	virtual ~SEComponent();
	void Update() override;

	void Receive(int message) override;

private:
	
	ActorClass* _Listener;
};


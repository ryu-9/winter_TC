#pragma once

#include "SourceVoiceItem.h"
#include "SoundServer.h"
#include <vector>

class SourceVoiceItem;
class SEComponent : public Component {
public:
	SEComponent(class ActorClass* owner, ActorClass* listner, int seflag,int updateOrder = 100);
	virtual ~SEComponent();

	void Update() override;

	void Receive(int message) override;
	void SetSourceVoice(SourceVoiceItem* sv) { _SV = sv; }
private:
	
	SourceVoiceItem* _SV;
	ActorClass* _Listener;
};


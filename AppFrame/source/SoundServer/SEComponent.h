#pragma once

#include "SourceVoiceItem.h"
#include "SoundServer.h"
#include <vector>

class SEComponent : public Component {
public:
	SEComponent(class ActorClass* owner, int seflag,int updateOrder = 100);
	virtual ~SEComponent();

	void Update() override;

	void Receive(int message) override;
private:
	
	int _SEFlag;
	std::vector<SourceVoiceItem*> _SEvec;
};


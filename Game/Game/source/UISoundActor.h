#pragma once
#include "appframe.h"
class UISoundActor : public ActorClass{
public:
	UISoundActor(ModeBase* mode);
	virtual ~UISoundActor();
	void UpdateActor() override;

	void AddSound(std::string actname, std::string svname);
	

	void PlayActSound(std::string actname);
private:

};


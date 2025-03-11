#pragma once
#include "appframe.h"
class UISoundActor : public ActorClass{
public:
	UISoundActor(ModeBase* mode);
	virtual ~UISoundActor();
	void UpdateActor() override;

	void AddSound(std::string actname, std::string svname) {
		_SoundMap[actname] = svname;
	}

	void PlayActSound(std::string actname);
private:
	std::map<std::string, std::string> _SoundMap;

};


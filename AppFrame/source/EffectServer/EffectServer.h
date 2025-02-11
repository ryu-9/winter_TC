#pragma once

#include <list>
#include <map>
#include "../ModeServer/ActorClass.h"


class EffectServer
{


public:
	EffectServer();
	virtual	~EffectServer();
	static EffectServer* _lpInstance;
	static EffectServer* GetInstance() { return (EffectServer*)_lpInstance; }




private:

};


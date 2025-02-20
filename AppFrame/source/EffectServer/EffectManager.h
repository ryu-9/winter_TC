#pragma once
#include <map>
#include "../AppFrame/source/ModeServer/SpriteComponent.h"

class EffectManager
{
	public:
	EffectManager(SpriteComponent* sp);
	~EffectManager();

	void Draw();

	void AddEffectFlag(std::string name, bool flag) {_Effectflag[name] = flag;}
	std::map<std::string, bool>* GetEffectFlag() { return &_Effectflag; }

	SpriteComponent* GetSprite() { return _Sprite; }

	private:
		SpriteComponent* _Sprite;
		std::map <std::string, bool> _Effectflag;

};


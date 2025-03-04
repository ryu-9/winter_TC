#pragma once
#include <map>
#include "../AppFrame/source/ModeServer/SpriteComponent.h"

class EffectManager : public SpriteComponent
{
	public:
		EffectManager(ActorClass* owner, int draworder);
		~EffectManager();

		virtual void SetIsUse(bool flag);
		bool GetIsUse() const { return _IsUse; }

		std::string GetEffectName() const { return _EffectName; }

	protected:

		void SetEffectName(std::string name) { _EffectName = name; }


	private:
		bool _IsUse;
		std::string _EffectName;
};


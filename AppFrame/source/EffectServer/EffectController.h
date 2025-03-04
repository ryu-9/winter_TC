#pragma once

#include <deque>
#include "../ModeServer/ActorClass.h"
#include "EffectManager.h"
#include "../ModeServer/ModeBase.h"
#include "PlayerEmphasisEffect.h"


class EffectController : ActorClass
{
public:
	EffectController(ModeBase* mode);
	virtual	~EffectController();
	
	void Draw();

	static EffectController* _lpInstance;
	static EffectController* GetInstance() { return (EffectController*)_lpInstance; }

	void AddEffect(EffectManager* effect);
	void DelEffect(EffectManager* effect);

	template <typename T>
	std::deque<T*> GetEffect();

	void AddEffectFlag(SpriteComponent* sprite, std::string flagname, bool flag);

	void AddEmphasisEffect(SpriteComponent* sprite, int alpha, int wide, int height, int draworder = 200);


	bool SearchFlag(std::map<std::string, bool>* flagList, std::string flagname);

private:
	std::deque<EffectManager*> _EffectList;

	std::deque<PlayerEmphasisEffect*> _EmphasisEffectList;
	std::map<SpriteComponent*, std::map<std::string, bool>> _FlagList;

};

template <typename T>
std::deque<T*> EffectController::GetEffect()
{
	std::deque<T*> ret;
	for (auto& em : _EffectList) {
		T* castedComp = dynamic_cast<T*>(em);
		if (castedComp != nullptr) {
			ret.emplace_front(castedComp);
		}
	}
	return ret;
}


class FogSpriteComponent : public EffectManager
{
public:
	FogSpriteComponent(ActorClass* owner, int draworder = -1000000, unsigned int color = GetColor(255, 255, 255), float mindist = 0, float maxdist = 1000);
	virtual ~FogSpriteComponent();

	void Draw() override;

	void SetIsUse(bool flag) override;

private:
	unsigned int _Color;

	float _MinDist;
	float _MaxDist;
};


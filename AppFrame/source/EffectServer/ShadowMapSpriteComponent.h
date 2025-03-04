#pragma once
#include "../AppFrame/source/ModeServer/SpriteComponent.h"
#include "EffectManager.h"
#include "Dxlib.h"



class ShadowMapSpriteComponent : public EffectManager
{
public:
	ShadowMapSpriteComponent(class ActorClass* owner, int size, VECTOR dir = VGet(0,-1, 0), VECTOR target = VGet(0, 0, 0), int index = 0, float length = 100, int drawOrder = -1000000);
	virtual ~ShadowMapSpriteComponent();
	void Draw() override;

	void SetDirection(VECTOR dir) { _Direction = dir; }
	VECTOR GetDirection() const { return _Direction; }

	void SetTarget(VECTOR target) { _Target = target; }
	VECTOR GetTarget() const { return _Target; }

	void AddSprite(class SpriteComponent* sprite) { _Sprites.push_front(sprite); }
	void RemoveSprite(class SpriteComponent* sprite);

	void AddRemoveSprite(class SpriteComponent* sprite) { _RemoveSprites.push_front(sprite); }
	void RemoveRemoveSprite(class SpriteComponent* sprite);

	void SwitchSprites(std::deque<class SpriteComponent*>& sprites);

	int GetHandle() const { return _Handle; }

	void SetIndex(int index) { _Index = index; }
	int GetIndex() const { return _Index; }

	void SetMinLength(VECTOR min) { _MinLength = min; }
	VECTOR GetMinLength() const { return _MinLength; }

	void SetMaxLength(VECTOR max) { _MaxLength = max; }
	VECTOR GetMaxLength() const { return _MaxLength; }

	void SetIsDraw(bool isDraw) { _IsDraw = isDraw; }
	bool GetIsDraw() const { return _IsDraw; }

	void SetIsUse(bool isUse) override;

private:
	int _Index;
	bool _IsDraw;
	VECTOR _MinLength;
	VECTOR _MaxLength;
	int _Handle;
	VECTOR _Direction;
	VECTOR _Target;
	std::deque<class SpriteComponent*> _Sprites;
	std::deque<class SpriteComponent*> _RemoveSprites;
};


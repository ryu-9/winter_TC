#include "ShadowMapSpriteComponent.h"

ShadowMapSpriteComponent::ShadowMapSpriteComponent(ActorClass* owner, int size, VECTOR dir, VECTOR target, int index, float length, int drawOrder)
	:EffectManager(owner, drawOrder)
	, _Direction(dir)
	, _Target(target)
	, _Index(index)
	, _IsDraw(TRUE)
{
	_Handle = MakeShadowMap(size, size);
	_MinLength = VGet(-length, -length, -length);
	_MaxLength = VGet(length, length, length);

}

ShadowMapSpriteComponent::~ShadowMapSpriteComponent()
{
}

void ShadowMapSpriteComponent::Draw()
{
	//SetUseShadowMap(0, -1);
	//SetUseShadowMap(1, -1);
	//SetUseShadowMap(2, -1);


	SetShadowMapLightDirection(_Handle, _Direction);

	SetShadowMapDrawArea(_Handle, VAdd(_Target, _MinLength), VAdd(_Target, _MaxLength));

	ShadowMap_DrawSetup(_Handle);

	std::vector<SpriteComponent*> debug;
	if (_Sprites.size() > 0) {
		for (auto sp : _Sprites) {
			auto null = dynamic_cast<EffectManager*>(sp);
			if (null == nullptr) {
				bool flag = false;
				for (auto r : _RemoveSprites) {
					if (sp == r) {
						flag = true;
						break;
					}
				}
				if (flag) { continue; }
				if (sp->GetOwner()->GetState() == ActorClass::State::eDead) {
					RemoveSprite(sp);
					continue;
				}

				sp->Draw();
			}
		}
	}
	else {
		for (auto sp : _Owner->GetMode()->GetSprites()) {
			auto null = dynamic_cast<EffectManager*>(sp);
			if (null == nullptr) {
				bool flag = false;
				for (auto r : _RemoveSprites) {
					if (sp == r) { 
						flag = true;
						break;
					}
				}
				if (flag) {continue;}
				if (sp->GetOwner() == nullptr) { continue; }
				sp->Draw();
				debug.emplace_back(sp);
			}
		}
	}

	ShadowMap_DrawEnd();
	GraphFilter(_Handle, DX_GRAPH_FILTER_GAUSS, 8, 1000);
	
	SetUseShadowMap(_Index, _Handle);


	TestDrawShadowMap(_Handle, _Index * 128, 0, (_Index + 1) * 128, 128);
}

void ShadowMapSpriteComponent::RemoveRemoveSprite(SpriteComponent* sprite)
{
	for (int i = 0; i < _RemoveSprites.size(); i++)
	{
		if (_RemoveSprites[i] == sprite) {
			_RemoveSprites.erase(_RemoveSprites.begin() + i);
			break;
		}
	}
}

void ShadowMapSpriteComponent::SwitchSprites(std::deque<class SpriteComponent*>& sprites)
{
	_Sprites = sprites;

}

void ShadowMapSpriteComponent::SetIsUse(bool isUse)
{
	if (isUse) {
		SetUseShadowMap(_Index, _Handle);
	}
	else {
		SetUseShadowMap(_Index, -1);
	}
	
	EffectManager::SetIsUse(isUse);
}

void ShadowMapSpriteComponent::RemoveSprite(SpriteComponent* sprite)
{
	for (int i = 0; i < _Sprites.size(); i++)
	{
		if (_Sprites[i] == sprite) {
			_Sprites.erase(_Sprites.begin() + i);
			break;
		}
	}
}

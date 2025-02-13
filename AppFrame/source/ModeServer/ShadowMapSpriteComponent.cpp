#include "ShadowMapSpriteComponent.h"

ShadowMapSpriteComponent::ShadowMapSpriteComponent(ActorClass* owner, int size, VECTOR dir, VECTOR target, int index, float length, int drawOrder)
	:SpriteComponent(owner, drawOrder)
	, _Direction(dir)
	, _Target(target)
	, _Index(index)
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
	if (_Sprites.size() > 0) {
		for (auto sp : _Sprites) {
			if (sp->GetOwner() != _Owner) {
				bool flag = false;
				for (auto r : _RemoveSprites) {
					if (sp == r) { 
						flag = true;
						break;
					}
				}
				if (flag) { continue; }
				sp->Draw();
			}
		}
	}
	else {
		for (auto sp : _Owner->GetMode()->GetSprites()) {
			if (sp->GetOwner() != _Owner) {
				bool flag = false;
				for (auto r : _RemoveSprites) {
					if (sp == r) { 
						flag = true;
						break;
					}
				}
				if (flag) {continue;}
				sp->Draw();
			}
		}
	}

	ShadowMap_DrawEnd();
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
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

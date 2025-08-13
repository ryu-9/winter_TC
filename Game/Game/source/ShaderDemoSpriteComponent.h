#pragma once
#include "appframe.h"

class ShaderDemoSpriteComponent : public SpriteComponent
{
	public:
	ShaderDemoSpriteComponent(ActorClass* owner, int draworder = -1);
	virtual ~ShaderDemoSpriteComponent();
	void Draw() override;

	void CreateZMask();
	void CreateReflect();

	int GetZMaskHandle(int index = 0) const { return _ZMaskHandle[index]; }
	int GetReflectHandle() const { return _ReflectHandle; }

	static ShaderDemoSpriteComponent* _Instance;
	static ShaderDemoSpriteComponent* GetInstance() { return _Instance; }

private:
	int _PShader[4];
	int _VShader[4];
	int _Handle[4];
	bool _DrawFlag;
	int _ZMaskHandle[2];
	int _ReflectHandle;
};


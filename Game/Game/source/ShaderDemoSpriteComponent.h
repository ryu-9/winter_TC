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
	int _PShader[6];
	int _VShader[2];
	int _MatCapShader[4];
	int _Handle[4];
	bool _DrawFlag;
	int _ZMaskHandle[5];
	int _ReflectHandle;
};



class ShaderDemoSubActor : public ActorClass
{
public:
	ShaderDemoSubActor(ModeBase* mode, const VECTOR pos, float scale);
	~ShaderDemoSubActor() override;

	void UpdateActor() override;

private:
	int _Count = 0;
};


class ShaderDemoSubSpriteComponent : public ModelSpriteComponent
{
public:
	ShaderDemoSubSpriteComponent(ActorClass* owner, ModelComponent* model, int drawOrder = 100);
	~ShaderDemoSubSpriteComponent() override;

	void Draw() override;
	void DrawMask();

	int GetZMaskHandle(int index = 0) const { return _ZBufferMask[index]; }

private:
	int _PShader[4];
	int _VShader[3];
	int _ZBufferMask[5];
	int _BackCullMask;

};
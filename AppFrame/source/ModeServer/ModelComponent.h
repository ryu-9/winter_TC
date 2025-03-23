#pragma once
#include "Component.h"
#include "Dxlib.h"
#include "ModeBase.h"

#include <map>


class ModelComponent : public Component
{
public:
	ModelComponent(class ActorClass* owner, const TCHAR* file, int draworeder = 100,bool useownersize = true);
	virtual ~ModelComponent();

	void ProcessInput() override;
	void Update() override;
	void SetModelInfo();

	int GetHandle() const { return _Handle; }
	void SetHandle(int handle) { _Handle = handle; }

	VECTOR GetFront();
	void SetFront(VECTOR front);

	VECTOR GetUp();
	void SetUp(VECTOR up) { _Up = up; }
	
	void SetVisible(bool visible) { MV1SetVisible(_Handle, visible); }
	bool GetVisible() const { return MV1GetVisible(_Handle); }

	void SetPosition(VECTOR pos) { _Position = pos; }
	VECTOR GetPosition() const { return _Position; }

	void SetScale(VECTOR scale) { _Scale = scale; }
	VECTOR GetScale() const { return _Scale; }

	void SetRotation(VECTOR rot);
	VECTOR GetRotation();

	void SetIndipendent(bool indipendent) { _Indipendent = indipendent; }

	void SetRotationZY(VECTOR front, VECTOR up);

	VECTOR GetCenter();
	void SetCenter(VECTOR center) { _Center = center; }

	class ModelSpriteComponent* GetSprite() { return _Sprite; }

private:

	// 3Dƒ‚ƒfƒ‹•`‰æ—p
	int _Handle;
	float _TotalTime;
	float _PlayTime;
	int _AttachIndex;

	VECTOR _Rot;
	VECTOR _Front;
	VECTOR _Up;

	VECTOR _Position;
	VECTOR _Scale;
	VECTOR _Center;

	bool _Indipendent;
	bool _UseOwnerSize;

	class ModelSpriteComponent* _Sprite;
};

class ModelSpriteComponent : public SpriteComponent
{
public:
	ModelSpriteComponent(class ActorClass* owner,class ModelComponent* model, int drawOrder = 100);
	virtual ~ModelSpriteComponent();

	void Draw() override;
	void SetImage() override;

	void AddMaterial(std::string name, COLOR_F value) { _Material[name] = value; }
	void DelMaterial(std::string name) { _Material.erase(name); }
	
private:

	class ModelComponent* _Model;
	//std::deque<std::string> _Material;
	std::map<std::string, COLOR_F> _Material;
	
};


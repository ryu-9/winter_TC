#pragma once
#include <vector>
#include "DxLib.h"

#include "Component.h"
#include "SpriteComponent.h"

class ActorClass
{
public:
	enum class State
	{
		ePreparation,
		eActive,
		ePaused,
		eEnd,
		eDead
	};

	ActorClass(class ModeBase* mode);
	virtual ~ActorClass();

	void ProcessInput();
//	virtual void ActorInput() {};

	void Update();
	void UpdateComponents();
	virtual void UpdateActor();

	virtual class ModeBase* GetMode() { return _Mode; }

	VECTOR GetPosition() const { return _Position; }
	void SetPosition(const VECTOR pos) { _Position = pos; }
	VECTOR GetDirection() const { return _Direction; }
	void SetDirection(const VECTOR dir) { _Direction = dir; }
	VECTOR GetRotation() const { return _Rotation; }
	void SetRotation(const VECTOR rot) { _Rotation = rot; }
	VECTOR GetRotation2() const { return _Rotation2; }
	void SetRotation2(const VECTOR rot) { _Rotation2 = rot; }
	VECTOR GetMove() const { return _Move; }
	void SetMove(const VECTOR move) { _Move = move; }
	VECTOR GetSize() const { return _Size; }
	void SetSize(const VECTOR size) { _Size = size; }
	
	State GetState() const { return _State; }
	void SetState(const State state) { _State = state; }

	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

	void Send(int message);
protected:
	VECTOR _Position;	// �ʒu
	VECTOR _Direction;	// ����
	VECTOR _Rotation;	// �O�̌���
	VECTOR _Rotation2;	// ��̌���
	VECTOR _Move;		// �ړ��x�N�g��
	VECTOR _Size;		// �傫��

	std::vector<class Component*> _Components;
	class ModeBase* _Mode;

	State _State;
};


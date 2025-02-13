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
	VECTOR GetMove() const { return _Move; }
	void SetMove(const VECTOR move) { _Move = move; }
	VECTOR GetSize() const { return _Size; }
	void SetSize(const VECTOR size) { _Size = size; }
	
	State GetState() const { return _State; }
	void SetState(const State state) { _State = state; }

	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

	template <typename T>
	std::deque<T*> GetComponent();

	void Send(int message);
protected:
	VECTOR _Position;	// 位置
	VECTOR _Direction;	// 向き
	VECTOR _Move;		// 移動ベクトル
	VECTOR _Size;		// 大きさ

	std::vector<class Component*> _Components;
	class ModeBase* _Mode;

	State _State;
};
 
template <typename T>
std::deque<T*> ActorClass::GetComponent()
{
	std::deque<T*> ret;
	for (auto& comp : _Components) {
		T* castedComp = dynamic_cast<T*>(comp);
		if (castedComp != nullptr) {
			ret.emplace_back(castedComp);
		}
	}
	return ret;
}
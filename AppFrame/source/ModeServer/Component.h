	#pragma once


class Component
{
public:
	Component(class ActorClass* owner, int updateOrder = 100);
	virtual ~Component();

	virtual void Update();
	virtual void ProcessInput(){}

	int GetUpdateOrder() const { return _UpdateOrder; }
	virtual void Receive(int message) {};

	ActorClass* GetOwner() { return _Owner; }

protected:
	class ActorClass* _Owner;

	int _UpdateOrder;

};


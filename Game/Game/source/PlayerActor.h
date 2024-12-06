#pragma once
#include "appframe.h"
#include "ModeGame.h" 

class PlayerActor : public ActorClass {
public:
	PlayerActor(class ModeBase* mode);
	virtual ~PlayerActor();

	void UpdateActor() override;

	enum class STATUS {
		NONE,
		WAIT,
		WALK,
		_EOT_
	};

	STATUS GetStatus() const { return _Status; }
	void SetStatus(STATUS status) { _Status = status; }
	bool IsChangeStatus() const { return _Status != _OldStatus; }
	void SetOldStatus() { _OldStatus = _Status; }
	class ModeGame* GetMode() override { return static_cast<ModeGame*>(_Mode);}
	
	void SetInput(class Component* input) { delete _Input; _Input = input; }

private:
	STATUS _Status;
	STATUS _OldStatus;

	class Component* _Input;
};


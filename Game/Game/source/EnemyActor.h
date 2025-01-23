#pragma once
#include "appframe.h"
#include "ModeGame.h"


class EnemyActor : public ActorClass {
public:
	EnemyActor(class ModeBase* mode);
	virtual ~EnemyActor();

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
	class ModeGame* GetMode() override { return static_cast<ModeGame*>(_Mode); }

	void SetInput(class EnemyMoveComponent* input) { delete _Input; _Input = input; }

private:
	STATUS _Status;
	STATUS _OldStatus;

	class EnemyMoveComponent* _Input;

	class ModelComponent* _Model;
	class MoveCollisionComponent* _MCollision;
};


#pragma once
#include "appframe.h"
#include "ModeGame.h"


class EnemyActor : public ActorClass {
public:

	struct PARAM
	{
		int hp;
		float damage;
		float speed;
		float serchdist;
		int interval;
	};

	EnemyActor(class ModeBase* mode);
	virtual ~EnemyActor();

	void UpdateActor() override;

	enum class STATUS {
		NONE,
		SEARCH,
		MOVE,
		_EOT_
	};

	STATUS GetStatus() const { return _Status; }
	void SetStatus(STATUS status) { _Status = status; }
	bool IsChangeStatus() const { return _Status != _OldStatus; }
	void SetOldStatus() { _OldStatus = _Status; }
	class ModeGame* GetMode() override { return static_cast<ModeGame*>(_Mode); }

	void SetInput(class MoveComponent* input) { delete _Input; _Input = input; }

	void SetParam(PARAM param) { _Param = param; }
	
private:
	STATUS _Status;
	STATUS _OldStatus;

	class MoveComponent* _Input;

	class ModelComponent* _Model;
	class MoveCollisionComponent* _MCollision;

	PARAM _Param;
	uint_fast8_t _SearchFlag;
};


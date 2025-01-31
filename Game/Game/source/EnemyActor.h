#pragma once
#include "appframe.h"
#include "ModeGame.h"
#include "EnemyStruct.h"


class EnemyActor : public ActorClass {
public:

	EnemyActor(class ModeBase* mode);
	virtual ~EnemyActor();


	void Init();
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

	bool Search();
private:
	STATUS _Status;
	STATUS _OldStatus;

	
	class ModelComponent* _Model;
	class MoveCollisionComponent* _MCollision;


	Enemy::SEARCH _SearchRef;

};


#pragma once
#include "ModeBase.h"
#include "ActorClass.h"
#include "Component.h"

#include <deque>


class HitCollisionComponent : public Component
{
public:
	HitCollisionComponent(class ActorClass* owner, class ModelComponent* model, VECTOR pos = VGet(0, 0, 0), VECTOR size = VGet(1, 1, 1)
		, int type = 0, bool move = FALSE, bool active = TRUE, int handle = -1, bool useownersize = TRUE);

	~HitCollisionComponent() override;

	std::deque<HitCollisionComponent*>& IsHit();


	void Update() override;

	void RefleshCollInfo();

	VECTOR GetPosition();
	VECTOR GetSize();
	VECTOR GetOldPosition() const { return OldPos; }
	void SetOldPosition(VECTOR pos) { OldPos = pos; }

	VECTOR GetUp();
	VECTOR GetFront();
	VECTOR GetRight();

	VECTOR GetRPosition() const { return Pos; }
	VECTOR GetRSize() const { return Size; }

	void SetRPosition(VECTOR pos) { Pos = pos; }
	void SetRSize(VECTOR size) { Size = size; }

	int GetType() const { return Type; }
	void SetType(int type) { Type = type; }
	// collision�̌`�@0: �� 1:���� 2:�J�v�Z�� 3:���b�V��

	bool GetIsMove() const { return isMove; }
	void SetIsMove(bool move) { isMove = move; }

	bool GetIsActive() const { return isActive; }
	void SetIsActive(bool active) { isActive = active; }

	int GetHandle() const { return Handle; }
	void SetHandle(int handle) { Handle = handle; }

	void DebugDraw();

	bool GetFlag() const { return flag; }

	void SetRotation(VECTOR rot);

private:
	// �ʒu
	VECTOR Pos;

	// �T�C�Y
	VECTOR Size;

	VECTOR Rot;
	VECTOR Front;
	VECTOR Up;

	// collision�̌`
	int Type;

	// �ړ����邩
	bool isMove;

	// �A�N�e�B�u��
	bool isActive;

	// ���f���n���h��
	int Handle;

	bool flag;
	bool shomen;

	bool _UseOwnerSize;
	VECTOR OldMove;
	VECTOR OldPos;

	VECTOR devpos;

	class ModelComponent* _Model;

	std::deque<HitCollisionComponent*> _IsHitList;
};


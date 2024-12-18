#pragma once
#include "ModeBase.h"
#include "Component.h"


class MoveCollisionComponent : public	Component
{
public:
	MoveCollisionComponent(class ActorClass* owner, VECTOR pos = VGet(0,0,0), VECTOR size = VGet(1,1,1)
	, int type = 0, bool move = FALSE, bool isMove = FALSE);
	MoveCollisionComponent(class ActorClass* owner, VECTOR pos = VGet(0, 0, 0), VECTOR size = VGet(1, 1, 1)
		, int type = 0, int handle, bool move = FALSE, bool isMove = FALSE);
	~MoveCollisionComponent() override;

	void Update() override;

	VECTOR GetPosition();
	VECTOR GetSize();

	VECTOR GetRPosition() const { return Pos; }
	VECTOR GetRSize() const { return Size; }

	void SetRPosition(VECTOR pos) { Pos = pos; }
	void SetRSize(VECTOR size) { Size = size; }

	int GetType() const { return Type; }
	void SetType(int type) { Type = type; }
	// collision�̌`�@0: ���b�V�� 1:�����́@2:�� 3:�~�� 4:�J�v�Z�� 5:�l�p��

	bool GetIsMove() const { return isMove; }
	void SetIsMove(bool move) { isMove = move; }

	bool GetIsActive() const { return isActive; }
	void SetIsActive(bool active) { isActive = active; }

private:
	// �ʒu
	VECTOR Pos;
	
	// �T�C�Y
	VECTOR Size;

	// collision�̌`
	int Type;

	//	�ړ����邩
	bool isMove;

	//	�A�N�e�B�u��
	bool isActive;

	//	���f���n���h��
	int Handle;
};


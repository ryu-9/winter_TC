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
	// collisionの形　0: メッシュ 1:直方体　2:球 3:円柱 4:カプセル 5:四角錐

	bool GetIsMove() const { return isMove; }
	void SetIsMove(bool move) { isMove = move; }

	bool GetIsActive() const { return isActive; }
	void SetIsActive(bool active) { isActive = active; }

private:
	// 位置
	VECTOR Pos;
	
	// サイズ
	VECTOR Size;

	// collisionの形
	int Type;

	//	移動するか
	bool isMove;

	//	アクティブか
	bool isActive;

	//	モデルハンドル
	int Handle;
};


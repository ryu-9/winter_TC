#pragma once
#include "ModeBase.h"
#include "Component.h"

struct CollResult {
	class MoveCollisionComponent* mc;
	std::deque<MV1_COLL_RESULT_POLY> mesh;
};


class MoveCollisionComponent : public	Component
{
public:
	MoveCollisionComponent(class ActorClass* owner, class ModelComponent* model,  VECTOR pos = VGet(0,0,0), VECTOR size = VGet(1,1,1)
	, int type = 0, bool move = FALSE, bool active = TRUE , int handle = -1);

	~MoveCollisionComponent() override;

	void Update() override;

	void RefleshCollInfo();

	VECTOR GetPosition();
	void SetOldPosition(VECTOR pos) { OldPos = pos; }

	VECTOR GetSize();

	VECTOR GetUp();
	VECTOR GetFront();
	VECTOR GetRight();

	VECTOR GetRPosition() const { return Pos; }
	VECTOR GetRSize() const { return Size; }

	VECTOR GetRotation();

	void SetRPosition(VECTOR pos) { Pos = pos; }
	void SetRSize(VECTOR size) { Size = size; }

	int GetType() const { return Type; }
	void SetType(int type) { Type = type; }
	// collisionの形　0: 球 1:線分 2:カプセル 3:メッシュ

	bool GetIsMove() const { return isMove; }
	void SetIsMove(bool move) { isMove = move; }

	bool GetIsActive() const { return isActive; }
	void SetIsActive(bool active) { isActive = active; }

	int GetHandle() const { return Handle; }
	void SetHandle(int handle) { Handle = handle; }

	void DebugDraw();

	bool GetFlag() const { return flag; }

	void Push();

	void SetRotation(VECTOR rot);

	class MoveComponent* GetMove() const { return _Move; }

	VECTOR GetDrawPos(int num = 0) const { return drawpos[num]; }

	std::deque<CollResult> GetCollResult(int num = 0) const { return _CollResult; }

protected:
	// 位置
	VECTOR Pos;
	
	// サイズ
	VECTOR Size;

	VECTOR Rot;
	VECTOR Front;
	VECTOR Up;

	// collisionの形
	int Type;

	//	移動するか
	bool isMove;

	//	アクティブか
	bool isActive;

	//	能動的に処理するか
	bool isCalc;

	//	モデルハンドル
	int Handle;

	bool flag;
	bool shomen;
	VECTOR OldMove;
	VECTOR OldPos;

	VECTOR devpos;
	VECTOR drawpos[2];

	class ModelComponent* _Model;
	class MoveComponent* _Move;

	std::deque<CollResult> _CollResult;

	std::deque<VECTOR> debugpos;
};


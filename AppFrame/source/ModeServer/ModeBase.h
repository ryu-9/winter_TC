#pragma once

#include <string>
#include <vector>
#include <deque>
#include <algorithm>

#include "SpriteComponent.h"
#include "MoveCollisionComponent.h"
#include "HitCollisionComponent.h"
#include "ActorClass.h"

class ModeServer;

VECTOR VMulti(VECTOR v, VECTOR m);

bool VEqual(VECTOR v, VECTOR m);

class ModeBase {
public:
	ModeBase();
	virtual ~ModeBase();

	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Update();
	virtual bool Render();

public:
	int	GetModeCount() { return _cntMode; }			// このモードが始まってからのカウンタ
	unsigned long GetModeTm() { return _tmMode; }	// このモードが始まってからの時間ms
	unsigned long GetStepTm() { return _tmStep; }	// 前フレームからの経過時間ms

	void SetCallPerFrame(int frame) { _callPerFrame = _callPerFrame_cnt = frame; }	// 何フレームに1回Process()を呼ぶか(def:1)
	void SetCallOfCount(int count) { _callOfCount = count; }		// 1回の呼び出しに何回Process()を呼ぶか(def:1)
	int GetCallPerFrame() { return _callPerFrame; }
	int GetCallOfCount() { return _callOfCount; }



	void AddActor(class ActorClass* actor);
	void RemoveActor(class ActorClass* actor);

	virtual void AddSprite(class SpriteComponent* sp);
	virtual void RemoveSprite(class SpriteComponent* sp);

	void AddMCollision(class MoveCollisionComponent* mc);
	void RemoveMCollision(class MoveCollisionComponent* mc);
	std::deque<MoveCollisionComponent*>& GetMCollision() { return _MCollision; }

	std::vector<class SpriteComponent*>& GetSprites() { return _Sprites; }

	void AddHCollision(class HitCollisionComponent* hc);
	void RemoveHCollision(class HitCollisionComponent* hc);
	std::deque<HitCollisionComponent*>& GetHCollision() { return _HCollision; }

	bool GetIsUpdate() const { return _IsUpdate; };
	void SetIsUpdate(const bool flag) { _IsUpdate = flag; };

	void SetDrawCollision(const bool flag) { _DrawCollision = flag; }

protected:

	bool _IsUpdate;
	bool _UpdatingActors;
	//	アクタ
	std::vector<class ActorClass*> _Actors;

	//	待機中アクタ
	std::vector<class ActorClass*> _PendingActors;

	// 描画コンポーネント
	std::vector<class SpriteComponent*> _Sprites;

	// 移動判定用コンポーネント
	std::deque<class MoveCollisionComponent*> _MCollision;

	// 当たり判定用コンポーネント
	std::deque<class HitCollisionComponent*> _HCollision;

private:
	friend	ModeServer;
	// ModeServer用
	std::string		_szName;
	int				_uid;
	int				_layer;

	void StepTime(unsigned long tmNow);
	void StepCount();

	// モード内処理用
	int		_cntMode;	// 本モードに入ってからのカウンタ, 0スタート
	unsigned long	_tmMode;	// 本モードに入ってからの時間。ms
	unsigned long	_tmStep;	// 前フレームからの経過時間。ms
	unsigned long	_tmModeBase;	// このモードが始まった時間。ms
	unsigned long	_tmPauseBase;	// ポーズ中のベース時間。ms
	unsigned long	_tmPauseStep;	// ポーズ中の積算時間。ms 動作中、この値を _tmMode に用いる
	unsigned long	_tmOldFrame;	// 前フレームの時間。ms

	// CallPerFrame / CallOfCount用
	int		_callPerFrame, _callPerFrame_cnt;
	int		_callOfCount;

	bool _DrawCollision;		// 衝突判定を描画するかどうか


};


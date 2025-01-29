#include "PlayerActor.h"
#include "appframe.h"
#include "PlayerMoveComponent.h"

PlayerActor::PlayerActor(ModeBase* mode, int playerNo)
	:ActorClass(mode)
	,_Status(STATUS::NONE)
	,_OldStatus(STATUS::NONE)
	,_ModeNum(0)
	, _PlayerNo(playerNo)
	, _ChangeTime(0)
{
	_BallModel = new ModelComponent(this, "res/model/Yukidama_bro/Yukidama_Bro.mv1");
	_BallModel->SetScale(VGet(2, 2, 2));
	_TopModel = new ModelComponent(this, "res/model/Sundercross/Sundercross_Upbody.mv1");
	_TopModel->SetScale(VGet(2, 2, 2));
	_TopModel->SetPosition(VGet(0, -400, 0));
	_BottomModel = new ModelComponent(this, "res/model/Sundercross/Sundercross_Downbody.mv1");
	_BottomModel->SetScale(VGet(2, 2, 2));
	_TopModel->SetVisible(false);
	_BottomModel->SetVisible(false);
	//_MCollision = new MoveCollisionComponent(this);
	//_MCollision->SetIsMove(true);
	_Input = new PlayerMoveComponent(this);
	_MCollision = new MoveCollisionComponent(this,_BallModel, VGet(0,0,0), VGet(100, 100, 100), 0, true, true);
	int n = rand() % 2;
	

	
		SetPosition(VGet(0, 1000, 0));

		SetSize(VGet(2,2,2));
}

PlayerActor::~PlayerActor()
{

}

void PlayerActor::UpdateActor()
{
	SetOldStatus();

	VECTOR v;

	VECTOR rot;
	VECTOR rot2;

	float friSize;
	float dist;

	if (_ChangeTime > 0) {
		_ChangeTime--;
		if (_ChangeTime == 0) {
			ChangeMode(0);
		}
	}





	switch (_ModeNum) {
	case 0:
		// 速度を取得
		v = _Input->GetVelocity();
		// 角度を取得
		rot = _BallModel->GetFront();
		rot2 = _BallModel->GetUp();

		//	速度から角度を算出
		rot = VTransform(rot, MGetRotX(v.z / 100 / GetSize().x / 2));
		rot = VTransform(rot, MGetRotZ(-v.x / 100 / GetSize().x / 2));
		rot2 = VTransform(rot2, MGetRotX(v.z / 100 / GetSize().x / 2));
		rot2 = VTransform(rot2, MGetRotZ(-v.x / 100 / GetSize().x / 2));

		//	角度をセット

		MV1SetRotationZYAxis(_BallModel->GetHandle(), rot, rot2, 0);

		_BallModel->SetFront(rot);
		_BallModel->SetUp(rot2);


		friSize = _Friend->GetSize().y;
		if (friSize >= 1 && GetSize().y >= 1) {
			dist = VSize(VSub(_Friend->GetPosition(), GetPosition()));
			if (dist < (friSize + GetSize().y) * 100) {
				if (_Friend->GetInput()->GetStand() == TRUE && _Input->GetStand() == FALSE) {
					ChangeMode(2);
					_Friend->ChangeMode(1);
				}
				else if (_Friend->GetInput()->GetStand() == FALSE && _Input->GetStand() == TRUE) {
					ChangeMode(1);
					_Friend->ChangeMode(2);
				}
			}
		}

		break;

	case 1:
		rot = _Input->GetDashDir();
		rot2 = VGet(0,1,0);

		MV1SetRotationZYAxis(_BottomModel->GetHandle(), rot, rot2, 0);

		_BottomModel->SetFront(rot);
		_BottomModel->SetUp(rot2);
		break;

	case 2:
		break;
	
	}

}

void PlayerActor::ChangeMode(int mode)
{
	if (_ModeNum == mode) { return; }
	float dist = 0;
	switch (mode) {
	case 0:
		if (_ModeNum == 1) {
			dist = GetSize().x * 100 * 2;
			SetPosition(VAdd(GetPosition(), VGet(dist, 0, 0)));
			_Friend->SetPosition(VAdd(GetPosition(), VGet( - 2 * dist, 0, 0)));
		}
		_ModeNum = 0;
		_Friend->ChangeMode(0);
		SetSize(VGet(0.5, 0.5, 0.5));
		_BallModel->SetVisible(true);
		_TopModel->SetVisible(false);
		_BottomModel->SetVisible(false);
		break;
	case 1:
		_ModeNum = 1;
		_BottomModel->SetVisible(true);
		_BallModel->SetVisible(false);
		SetPosition(VAdd(GetPosition(), VGet(0, GetSize().y * 1/2, 0)));
		_ChangeTime = (GetSize().y + _Friend->GetSize().y) * 100;
		break;

	case 2:
		_ModeNum = 2;
		_TopModel->SetVisible(true);
		_BallModel->SetVisible(false);
		_ChangeTime = (GetSize().y + _Friend->GetSize().y) * 100;
		break;
	
	}
}

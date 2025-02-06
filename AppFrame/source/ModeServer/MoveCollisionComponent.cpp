// MoveCollisionComponent.cpp
#include "MoveCollisionComponent.h"
#include "ActorClass.h"
#include "ModelComponent.h"
#include "MoveComponent.h"
#include "../AppFrame/source/ModelServer/ModelServer.h"

#include "../Application/ApplicationBase.h"

// �R���X�g���N�^
MoveCollisionComponent::MoveCollisionComponent(class ActorClass* owner, ModelComponent* model, VECTOR pos, VECTOR size, int type, bool move, bool active, int handle)
	: Component(owner)
	, _Model(model), Pos(pos), Size(size), Type(type), isMove(move), isActive(active), Handle(handle)
	, Rot(VGet(0, 0, 0)), Front(VGet(0, 0, 1)), Up(VGet(0, 1, 0)), OldMove(VGet(0, 0, 0)), devpos(VGet(0, 0, 0))
	, flag(false), shomen(false)
{
	// ���[�h�ɂ��̃R���|�[�l���g��ǉ�
	_Owner->GetMode()->AddMCollision(this);

	// �ړ��\�ȏꍇ�AMoveComponent��擾
	if (isMove == TRUE) {
		_Move = _Owner->GetComponent<MoveComponent>();
	}
	OldPos = GetPosition();
	// �^�C�v��2�ȉ��̏ꍇ�͏�������I��
	if (type == 2) { return; }

	// ���f���R���|�[�l���g��擾���A�n���h����ݒ�
	ModelComponent* modelComp = _Owner->GetComponent<ModelComponent>();
	if (Handle == -1 && modelComp != nullptr) {
		Handle = modelComp->GetHandle();
	}


	// ���f���̈ʒu�A�X�P�[���A��]��ݒ肵�A�Փˏ���Z�b�g�A�b�v
	MV1SetPosition(Handle, GetPosition());
	MV1SetScale(Handle, GetSize());
	MV1SetRotationZYAxis(Handle, GetFront(), GetUp(), 0);
	MV1SetupCollInfo(Handle);
}

// �f�X�g���N�^
MoveCollisionComponent::~MoveCollisionComponent()
{
	// ���[�h���炱�̃R���|�[�l���g��폜
	_Owner->GetMode()->RemoveMCollision(this);
}

void MoveCollisionComponent::Update() {


	devpos = VGet(0, 0, 0);

	if (isActive == TRUE && isMove == TRUE) {

		int pushnum = 0;
		_CollResult.clear();

		if (_Move == nullptr) {
			_Move = _Owner->GetComponent<MoveComponent>();
		}

		MV1SetPosition(Handle, GetPosition());
		MV1SetScale(Handle, GetSize());
		MV1SetRotationZYAxis(Handle, GetFront(), GetUp(), 0);
		MV1RefreshCollInfo(Handle);


		flag = FALSE;
		shomen = FALSE;
		OldMove = VGet(0, 0, 0);
		bool sqrtFlag = FALSE;
		for (auto mcoll : _Owner->GetMode()->GetMCollision()) {
			VECTOR oldmove = VGet(0, 0, 0);
			if (mcoll->GetIsActive() == TRUE) {
				if (mcoll->GetOwner() != _Owner) {


					VECTOR tmp = VSub(GetPosition(), OldPos);

					MV1_COLL_RESULT_POLY_DIM result;
					{
						VECTOR size = GetSize();
						float radian = size.x + size.y + size.z;
						radian /= 3;
						result = MV1CollCheck_Capsule(mcoll->GetHandle(), -1, mcoll->GetPosition(), OldPos, radian);
					}
					if (result.HitNum > 0) {

						CollResult tmp_CollResult;
						tmp_CollResult.mc = mcoll;
						for (int i = 0; i < result.HitNum; i++) {
							if (VDot(tmp, result.Dim[i].Normal) < 0) {
								tmp_CollResult.mesh.push_front(result.Dim[i]);

								flag = TRUE;
							}
						}
						if (tmp_CollResult.mesh.size() > 0) {
							_CollResult.push_front(tmp_CollResult);
						}

						flag = TRUE;
						std::deque <MV1_COLL_RESULT_POLY> polyList;

					}
					MV1CollResultPolyDimTerminate(result);
				}
			}
		}

		std::deque<VECTOR> HitPos;
		std::deque<VECTOR> HitNormal;
		std::deque<float> dist;
		std::deque<MoveCollisionComponent*> coll;
		std::deque<VECTOR> movedList;
		for (auto r : _CollResult) {
			MV1_COLL_RESULT_POLY_DIM Presult;
			{
				VECTOR tmp = VSub(GetPosition(), OldPos);
				VECTOR size = GetSize();
				float radian = size.x + size.y + size.z;
				radian /= 3;
				Presult = MV1CollCheck_Sphere(r.mc->GetHandle(), -1, OldPos, radian + VSize(tmp));
			}
			if (Presult.HitNum <= 0) {
				continue;
			}
			VECTOR tmpHit = Presult.Dim[0].HitPosition;
			VECTOR tmpNormal = Presult.Dim[0].Normal;
			float tmpDist = VSize(VSub(OldPos, tmpHit));

			for (int i = 0; i < Presult.HitNum; i++) {
				for (int j = 0; j < r.mesh.size(); j++) {
					if (VEqual(Presult.Dim[i].Position[0], r.mesh[j].Position[0]) == TRUE &&
						VEqual(Presult.Dim[i].Position[1], r.mesh[j].Position[1]) == TRUE &&
						VEqual(Presult.Dim[i].Position[2], r.mesh[j].Position[2]) == TRUE) {
						if (dist.size() == 0) {
							HitPos.push_front(tmpHit);
							HitNormal.push_front(tmpNormal);
							dist.push_front(tmpDist);
							coll.push_front(r.mc);
							break;
						}
						for (int j = 0; j < dist.size(); j++) {
							if (tmpDist < dist[j]) {
								HitPos.insert(HitPos.begin() + j, tmpHit);
								HitNormal.insert(HitNormal.begin() + j, tmpNormal);
								dist.insert(dist.begin() + j, tmpDist);
								coll.insert(coll.begin() + j, r.mc);
								break;
							}
						}
						break;
					}
				}
			}
			MV1CollResultPolyDimTerminate(Presult);
		}

		VECTOR tmp = VSub(GetPosition(), OldPos);
		float radius = GetSize().x + GetSize().y + GetSize().z;
		radius /= 3;
		int sqrtnum = -1;
		for (int i = 0; i < HitPos.size(); i++) {

			VECTOR move = VSub(OldPos, HitPos[i]);
			if (VSize(move) == 0) {
				move = HitNormal[i];
			}
			move = VNorm(move);
			if (VSize(move) == 0) {
				move = HitNormal[i];
			}
			if (VDot(move, HitNormal[i]) < 0) {
				continue;
			}
			if (VDot(move, tmp) > 0) {
				continue;
			}
			if (VSize(move) != 0) {
				float length = 0;


				float v = VDot(move, OldMove);
				move = VSub(move, VScale(OldMove, v));
				if (VSize(move) == 0) {
					move = HitNormal[i];
				}
				else { move = VNorm(move); }
				float judge = VDot(move, HitNormal[i]);

				if (HitNormal[i].y > 0.7 && move.y < 0.01) {
					int test = 0;
				}

				if (judge >= 0.98) {

					float a = VDot(tmp, move);
					if (a < 0) { a *= -1; }
					float b =VDot(VSub(HitPos[i], GetPosition()), move);
					if (b < 0) { b *= -1; }
					float c = radius;
					length = -b + c;
					if (length >= 2000) {
						int test = 0;
					}
					if (-b > c) {
						//continue;
					}
					if (move.y < 0.001) {
						int test = 0;
					}
				}
				else {
					float dot = VDot(move, VSub(HitPos[i], GetPosition()));
					float a = VSize(VSub(HitPos[i], GetPosition()));
					float r = radius;
					float debugSQRT = 4 * (dot * dot) - 4 * (a * a - r * r);
					if (debugSQRT < 0) {
						//									OldMove = move;
						if (sqrtFlag == FALSE) {
							sqrtFlag = TRUE;
							sqrtnum = i;
						}
						continue;
					}
					else {
						length = (2.0f * dot + sqrt(debugSQRT)) / 2.0f;
					}
					if (move.y > 0.1) {
						int test = 0;
					}
				}

				_Owner->SetPosition(VAdd(_Owner->GetPosition(), VScale(move, length)));
				pushnum++;

				movedList.push_front(move);
				tmp = VSub(GetPosition(), OldPos);
				OldMove = move;
				devpos = VAdd(devpos, VScale(move, length));

				if (move.y < 0.3) {
					int test = 0;
				}

				MoveComponent* EnMoveCon = coll[i]->GetOwner()->GetComponent<class MoveComponent>();
				VECTOR EnMove;
				if (EnMoveCon != nullptr) {
					EnMove = EnMoveCon->GetVelocity();
				}
				else { EnMove = VGet(0, 0, 0); }
				VECTOR velocity = _Move->GetVelocity();
				if (VDot(move, velocity) < VDot(move, EnMove)) {
					VECTOR velocity = VAdd(VSub(_Move->GetVelocity(), VScale(move, VDot(move, _Move->GetVelocity()))), VScale(move, VDot(move, EnMove)));
					_Move->SetVelocity(velocity);
				}


				float deg = 0;
				if (move.y >= cos(deg / 180 * atan(1) * 4)) {
					MoveComponent* SelfMC = _Owner->GetComponent<MoveComponent>();
					if (SelfMC != nullptr) {
						SelfMC->SetStand(TRUE);
					}
				}

				MV1SetPosition(Handle, GetPosition());
				MV1SetScale(Handle, GetSize());
				MV1SetRotationZYAxis(Handle, GetFront(), GetUp(), 0);
				MV1RefreshCollInfo(Handle);

				if (sqrtFlag == TRUE) {
					sqrtFlag = FALSE;

					move = VNorm(VSub(OldPos, HitPos[sqrtnum]));
					if (VSize(move) == 0) {
						move = HitNormal[sqrtnum];
					}
					if (VDot(move, HitNormal[sqrtnum]) < 0) {
						continue;
					}
					if (VDot(move, tmp) > 0) {
						continue;
					}
					if (VSize(move) != 0) {
						length = 0;


						v = VDot(move, OldMove);

						if (v < 0) {
							move = VSub(move, VScale(OldMove, v));
							move = VNorm(move);

						}
						judge = VDot(move, HitNormal[sqrtnum]);

						 {
							float dot = VDot(move, VSub(HitPos[sqrtnum], GetPosition()));
							float a = VSize(VSub(HitPos[sqrtnum], GetPosition()));
							float r = GetSize().x;
							float debugSQRT = 4 * (dot * dot) - 4 * (a * a - r * r);
							if (debugSQRT < 0) {
								continue;
							}
							else {
								length = (2.0f * dot + sqrt(debugSQRT)) / 2.0f;
							}
							if (move.y > 0.1) {
								int test = 0;
							}
						}

						_Owner->SetPosition(VAdd(_Owner->GetPosition(), VScale(move, length)));
						pushnum++;

						movedList.push_front(move);
						tmp = VSub(GetPosition(), OldPos);
						OldMove = move;
						devpos = VAdd(devpos, VScale(move, length));

						if (move.y < 0.3) {
							int test = 0;
						}

						MoveComponent* EnMoveCon = coll[sqrtnum]->GetOwner()->GetComponent<class MoveComponent>();
						VECTOR EnMove;
						if (EnMoveCon != nullptr) {
							EnMove = EnMoveCon->GetVelocity();
						}
						else { EnMove = VGet(0, 0, 0); }
						VECTOR velocity = _Move->GetVelocity();
						if (VDot(move, velocity) < VDot(move, EnMove)) {
							VECTOR velocity = VAdd(VSub(_Move->GetVelocity(), VScale(move, VDot(move, _Move->GetVelocity()))), VScale(move, VDot(move, EnMove)));
							_Move->SetVelocity(velocity);
						}


						if (move.y >= cos(deg / 180 * atan(1) * 4)) {
							MoveComponent* SelfMC = _Owner->GetComponent<MoveComponent>();
							if (SelfMC != nullptr) {
								SelfMC->SetStand(TRUE);
							}
						}

						MV1SetPosition(Handle, GetPosition());
						MV1SetScale(Handle, GetSize());
						MV1SetRotationZYAxis(Handle, GetFront(), GetUp(), 0);
						MV1RefreshCollInfo(Handle);
					}
				}

			}
		
		
		}
		if (devpos.y > radius && _CollResult.size() > 0) {
			int test = 0;
		}
		if (devpos.y < 0.01 && radius > 10 && dist.size()>1) {
			int test = 0;
		}
		if ( ApplicationBase::GetInstance()->GetKey(1)& PAD_INPUT_4) {
			if (devpos.y > radius  && _CollResult.size() > 0) {
				int test = 0;
			}
			int num = movedList.size() - dist.size();
			if (num < -1) {
				int test = 0;
			}
		}

	}



	OldPos = GetPosition();

}

void MoveCollisionComponent::RefleshCollInfo()
{
	MV1SetPosition(Handle, GetPosition());
	VECTOR size = GetSize();
	MV1SetScale(Handle, GetSize());
	MV1SetRotationZYAxis(Handle, GetFront(), GetUp(), 0);
	MV1SetupCollInfo(Handle);
}


VECTOR MoveCollisionComponent::GetPosition() {
	return VAdd(_Owner->GetPosition(), VMulti(Pos, _Owner->GetSize())); 
}

VECTOR MoveCollisionComponent::GetSize() {
	return VMulti(Size, _Owner->GetSize()); 
}

VECTOR MoveCollisionComponent::GetUp()
{
	return _Owner->GetComponent<ModelComponent>()->GetUp();
}

VECTOR MoveCollisionComponent::GetFront()
{
	return _Owner->GetComponent<ModelComponent>()->GetFront();
}

VECTOR MoveCollisionComponent::GetRight()
{
	return VCross(GetUp(),GetFront());
}

void MoveCollisionComponent::DebugDraw()
{

	if (isActive == FALSE) {
		return;
	}
	if (isMove == TRUE) {
		DrawFormatString(0, 160, GetColor(255, 255, 255), "%f , %f, %f", _Owner->GetPosition().x, _Owner->GetPosition().y, _Owner->GetPosition().z);
	}

	if (flag == TRUE) {
		DrawFormatString(0, 0, GetColor(255, 255, 255), "Hit");
		DrawFormatString(0, 16, GetColor(255, 255, 255), "%f , %f, %f", devpos.x, devpos.y, devpos.z);
	}
	if (shomen == TRUE) {
		DrawFormatString(0, 16, GetColor(255, 255, 255), "Shomen");
	}
	switch (Type) {

		// collision�̌`�@0: ���b�V�� 1:���� 2:�� 3:�J�v�Z�� 4:�~��  5:�l�p�� 6:������

	case 1:
		DrawLine3D(VAdd(GetPosition(), VGet(0, 0, 0)), VAdd(GetPosition(), VGet(0, 0, GetSize().z)), GetColor(255, 255, 255));
		break;

	case 2:
		DrawSphere3D(GetPosition(), GetSize().x, 16, GetColor(255, 255, 255), GetColor(255, 255, 255), TRUE);
		break;


	default:
		MV1DrawModel(Handle);
		break;

	}
}

void MoveCollisionComponent::Push()
{
}

void MoveCollisionComponent::SetRotation(VECTOR rot)
{
	Rot = rot;
	VECTOR rotate = VAdd(_Model->GetRotation(), rot);
	MV1SetRotationXYZ(Handle, rotate);
	Front = VTransform(VGet(0, 0, 1), MGetRotX(rotate.x));
	Front = VTransform(Front, MGetRotY(rotate.y));
	Front = VTransform(Front, MGetRotZ(rotate.z));
	Up = VTransform(VGet(0, 1, 0), MGetRotX(rotate.x));
	Up = VTransform(Up, MGetRotY(rotate.y));
	Up = VTransform(Up, MGetRotZ(rotate.z));
}

// MoveCollisionComponent.cpp
#include "MoveCollisionComponent.h"
#include "ActorClass.h"
#include "ModelComponent.h"
#include "MoveComponent.h"
#include "../AppFrame/source/ModelServer/ModelServer.h"

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
		MoveComponent* moveComp = _Owner->GetComponent<MoveComponent>();
	}

	// �^�C�v��2�ȉ��̏ꍇ�͏�������I��
	if (type <= 2) { return; }

	// ���f���R���|�[�l���g��擾���A�n���h����ݒ�
	ModelComponent* modelComp = _Owner->GetComponent<ModelComponent>();
	if (Handle == -1 && modelComp != nullptr) {
		Handle = modelComp->GetHandle();
	}
	MoveComponent* moveComp = _Owner->GetComponent<MoveComponent>();
	if (moveComp != nullptr) {
		_Move = moveComp;
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

	CollisionUpdate();

	int num = 0;
	int PushNum = 0;
	devpos = VGet(0, 0, 0);
	length = 1;

	if (isActive == FALSE || isMove == FALSE) {
		return;
	}
	movedLength = 0;
	if (_Move == nullptr) {
		_Move = _Owner->GetComponent<MoveComponent>();
	}
	else {
		movedLength = VSize(VSub(_Owner->GetPosition(), _Move->GetOldPosition()));
		OldPos = _Move->GetOldPosition();
	}
	if (movedLength != 0) {
		int num = 0;
		while (VSize(VScale(_Move->GetVelocity(), length)) > 0.1f) {
			Process();
			num++;
			if (num > 10) {
				break;
			}
		}
	}
	return;

	flag = FALSE;
	shomen = FALSE;
	OldMove = VGet(0, 0, 0);
	bool sqrtFlag = FALSE;
	for (auto mcoll : _Owner->GetMode()->GetMCollision()) {
		VECTOR oldmove = VGet(0, 0, 0);
		if (mcoll->GetIsActive() == TRUE) {
 			if (mcoll != this) {

				int typeSum = 0;
				VECTOR move = VGet(0, 0, 0);
				MoveCollisionComponent* coll[2];

				typeSum += Type * Type;
				if (typeSum < mcoll->GetType() * mcoll->GetType()) {
					coll[0] = this; coll[1] = mcoll;
				}
				else {
					coll[0] = mcoll; coll[1] = this;
				}

				MoveComponent* MoveCom = coll[0]->GetOwner()->GetComponent<class MoveComponent>();

				VECTOR oldPos = coll[0]->GetPosition();
				
				bool moveflag = FALSE;
					if (MoveCom != nullptr) {
						oldPos = MoveCom->GetOldPosition();
						moveflag = TRUE;
					}
				VECTOR tmp = VSub(coll[0]->GetOwner()->GetPosition(), oldPos);

				typeSum += mcoll->GetType() * mcoll->GetType();
				MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Capsule(coll[1]->GetHandle(), -1, coll[0]->GetPosition(), oldPos, coll[0]->GetSize().x);
				if (result.HitNum > 0) {

	 				num++;

					flag = TRUE;
					std::deque <MV1_COLL_RESULT_POLY> polyList;


					for (int i = 0; i < result.HitNum; i++) {
						polyList.push_back(result.Dim[i]);
						if (moveflag == FALSE || VDot(tmp, result.Dim[i].Normal) < 0) {
							
						}
					}
					if (polyList.size() <= 0) {
						int test = 0;
					}
					MV1_COLL_RESULT_POLY_DIM Presult = MV1CollCheck_Sphere(coll[1]->GetHandle(), -1, oldPos, coll[0]->GetSize().x + VSize(tmp));
					if (Presult.HitNum <= 0) {
						continue;
					}
					std::list<MV1_COLL_RESULT_POLY> mesh;
					MV1_COLL_RESULT_POLY tmpMesh;
					mesh.push_back(Presult.Dim[Presult.HitNum - 1]);
					for (int i = 0; i < Presult.HitNum - 1; i++) {
						bool tmpFlag = FALSE;
						for (int j = 0; j < polyList.size(); j++) {
							if (VEqual(polyList[j].Position[0], Presult.Dim[i].Position[0]) == TRUE &&
								VEqual(polyList[j].Position[1], Presult.Dim[i].Position[1]) == TRUE &&
								VEqual(polyList[j].Position[2], Presult.Dim[i].Position[2]) == TRUE) {
								tmpFlag = TRUE;
								break;
							}
						}
						if (tmpFlag == FALSE) { continue; }
						float Dist = VSize(VSub(oldPos, Presult.Dim[0].HitPosition));
						int j = 0;
						for (auto &m : mesh) {
							if (j > 1000) {
								break;
							}
							float tmpDist = VSize(VSub(oldPos, m.HitPosition));
							if (tmpDist > Dist) {
								mesh.insert(std::next(mesh.begin(),j), Presult.Dim[i]);
								j++;
							}
							else if (&m == &mesh.back()) {
								mesh.push_back(Presult.Dim[i]);
								break;
							}
						
						}
					}

					for (auto& m : mesh) {
						move = VNorm(VSub(oldPos, m.HitPosition));
						if (VSize(move) == 0) {
							move = m.Normal;
						}
						if (VDot(move, m.Normal) < 0) {
							continue;
						}
						if (VDot(move, tmp) > 0) {
							continue;
						}
						if (VSize(move) != 0) {
							float length = 0;
							

							float v = VDot(move, OldMove);

							if (v < 0) {
								move = VSub(move, VScale(OldMove, v));
								move = VNorm(move);

							}
							float judge = VDot(move, m.Normal);

							if (judge >= 0.98) {
								
								float a = VDot(tmp, move);
								if (a < 0) { a *= -1; }
								float b = VSize(VSub(m.HitPosition, oldPos));
								if (b < 0) { b *= -1; }
								float c = coll[0]->GetSize().x;;
								length = a - b + c;

							}
							else {
								float dot = VDot(move, VSub(m.HitPosition, coll[0]->GetPosition()));
								float a = VSize(VSub(m.HitPosition, coll[0]->GetPosition()));
								float r = coll[0]->GetSize().x;
								float debugSQRT = 4 * (dot * dot) - 4 * (a * a - r * r);
								if (debugSQRT < 0) {
//									OldMove = move;
									if (sqrtFlag == FALSE) {
										sqrtFlag = TRUE;
										tmpMesh = m;
									}
									continue;
								}
								else {
									length = (2.0f * dot + sqrt(debugSQRT)) / 2.0f;
								}
							}
							_Owner->SetPosition(VAdd(_Owner->GetPosition(), VScale(move, length)));
							PushNum++;
							tmp = VSub(coll[0]->GetOwner()->GetPosition(), oldPos);
							OldMove = move;
							devpos = VAdd(devpos, VScale(move, length));

							MoveComponent* EnMoveCon = mcoll->GetOwner()->GetComponent<class MoveComponent>();
							VECTOR EnMove;
							if (EnMoveCon != nullptr) {
								EnMove = EnMoveCon->GetVelocity();
							}
							else { EnMove = VGet(0, 0, 0); }
							VECTOR velocity = MoveCom->GetVelocity();
							if (VDot(move, velocity) < VDot(move, EnMove)) {
								VECTOR velocity = VAdd(VSub(MoveCom->GetVelocity(), VScale(move, VDot(move, MoveCom->GetVelocity()))), VScale(move, VDot(move, EnMove)));
								MoveCom->SetVelocity(velocity);
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
								tmpMesh;
								sqrtFlag = FALSE;

								move = VNorm(VSub(oldPos, tmpMesh.HitPosition));
								if (VSize(move) == 0) {
									move = tmpMesh.Normal;
								}
								if (VDot(move, tmpMesh.Normal) < 0) {
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
									judge = VDot(move, tmpMesh.Normal);

									if( VEqual(tmpMesh.HitPosition, tmpMesh.Position[0])==FALSE&&
										VEqual(tmpMesh.HitPosition, tmpMesh.Position[1]) == FALSE&&
										VEqual(tmpMesh.HitPosition, tmpMesh.Position[2]) == FALSE) {

										float a = VDot(tmp, move);
										if (a < 0) { a *= -1; }
										float b = VSize(VSub(tmpMesh.HitPosition, oldPos));
										if (b < 0) { b *= -1; }
										float c = coll[0]->GetSize().x;;
										length = a - b + c;

									}
									else {
										float dot = VDot(move, VSub(tmpMesh.HitPosition, coll[0]->GetPosition()));
										float a = VSize(VSub(tmpMesh.HitPosition, coll[0]->GetPosition()));
										float r = coll[0]->GetSize().x;
										float debugSQRT = 4 * (dot * dot) - 4 * (a * a - r * r);
										if (debugSQRT < 0) {
											continue;
										}
										else {
											length = (2.0f * dot + sqrt(debugSQRT)) / 2.0f;
										}
									}
									_Owner->SetPosition(VAdd(_Owner->GetPosition(), VScale(move, length)));
									PushNum++;
									tmp = VSub(coll[0]->GetOwner()->GetPosition(), oldPos);
									OldMove = move;
									devpos = VAdd(devpos, VScale(move, length));

									MoveComponent* EnMoveCon = mcoll->GetOwner()->GetComponent<class MoveComponent>();
									VECTOR EnMove;
									if (EnMoveCon != nullptr) {
										EnMove = EnMoveCon->GetVelocity();
									}
									else { EnMove = VGet(0, 0, 0); }
									VECTOR velocity = MoveCom->GetVelocity();
									if (VDot(move, velocity) < VDot(move, EnMove)) {
										VECTOR velocity = VAdd(VSub(MoveCom->GetVelocity(), VScale(move, VDot(move, MoveCom->GetVelocity()))), VScale(move, VDot(move, EnMove)));
										MoveCom->SetVelocity(velocity);
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


					MV1CollResultPolyDimTerminate(Presult);

				}

				MV1CollResultPolyDimTerminate(result);

			}
		}

	}
	if (num > 2) {
		int test = 0;
	}
}

void MoveCollisionComponent::Process()
{
	MV1SetPosition(Handle, GetPosition());
	MV1SetScale(Handle, GetSize());
	MV1SetRotationZYAxis(Handle, GetFront(), GetUp(), 0);
	MV1RefreshCollInfo(Handle);

	int num = 0;
	int PushNum = 0;
	devpos = VGet(0, 0, 0);

	if (isActive == FALSE || isMove == FALSE) {
		return;
	}

	std::deque<poly> polyList;
	flag = FALSE;
	shomen = FALSE;
	OldMove = VGet(0, 0, 0);

	VECTOR moved = VSub(_Owner->GetPosition(), _Move->GetOldPosition());
	VECTOR Center;
	float dist = 0;

	bool HitFlag = FALSE;
	for (auto mcoll : _Owner->GetMode()->GetMCollision()) {
		if (mcoll->GetIsActive() == TRUE) {
			if (mcoll != this) {


				bool moveflag = FALSE;

				VECTOR tmp = VSub(_Owner->GetPosition(), OldPos);
				
				if (mcoll->GetType() == 0) {
					VECTOR tmpV = VSub(mcoll->GetPosition(), OldPos);
					float dot = - VDot(tmpV, VNorm(tmp));	
					float a = VSize(VSub(tmpV, VScale(VNorm(tmp), - dot)));
					float r = GetSize().x + mcoll->GetSize().x;
					if (a < r) {
						float judge = sqrt(r * r - a * a) - dot;
						judge /= VSize(tmp);
						judge += 1;
						if (judge > dist) {
							dist = judge;
							Center = mcoll->GetPosition();
							HitFlag = TRUE;
						}
					}
				}
				else {
					MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Capsule(mcoll->GetHandle(), -1, GetPosition(), OldPos, GetSize().x);

					for (int i = 0; i < result.HitNum; i++) {
						if (VDot(tmp, result.Dim[i].Normal) < 0) {
							polyList.insert(polyList.begin(), { result.Dim[i],mcoll });
							HitFlag = TRUE;
						}
					}
					MV1CollResultPolyDimTerminate(result);
				}

			}
		}
	}
	if (HitFlag!=TRUE) { 
		length = 0;
		return;
	}
	poly tmpoly;
	{
		bool flag = false;
		float tmp = 0;
		float r = GetSize().x;
		for (auto p : polyList) {
			float judge = VDot(VSub(p.mesh.Position[0], GetPosition()), p.mesh.Normal);
			judge += r;
			judge /= VDot(VScale(moved, -1), p.mesh.Normal);
			if (judge > tmp) {
				tmp = judge;
				tmpoly = p;
				flag = true;
			}
		}
		if (flag == false) { return; }
		if (dist > tmp) {
			_Owner->SetPosition(VAdd(_Owner->GetPosition(), VScale(VNorm(moved), dist)));
		
		
		
		}
		else {
			_Owner->SetPosition(VAdd(_Owner->GetPosition(), VScale(moved, -tmp)));
			length = tmp * length;
			MV1_COLL_RESULT_POLY_DIM result;
			{
				float tmpR = GetSize().x * VSize(moved) / -VDot(moved, tmpoly.mesh.Normal);
				result = MV1CollCheck_Sphere(tmpoly.mc->GetHandle(), -1, GetPosition(), tmpR);
			}
			VECTOR pos;
			for (int i = 0; i < result.HitNum; i++) {
				if (VEqual(tmpoly.mesh.Position[0], result.Dim[i].Position[0]) == TRUE &&
					VEqual(tmpoly.mesh.Position[1], result.Dim[i].Position[1]) == TRUE &&
					VEqual(tmpoly.mesh.Position[2], result.Dim[i].Position[2]) == TRUE) {

					VECTOR move = VNorm(moved);

					float movelength = 0;
					float dot = VDot(move, VSub(result.Dim[i].HitPosition, GetPosition()));
					float a = VSize(VSub(result.Dim[i].HitPosition, GetPosition()));
					//float r = GetSize().x;
					float debugSQRT = 4 * (dot * dot) - 4 * (a * a - r * r);
					if (debugSQRT > 0) {
						movelength = (2.0f * dot - sqrt(debugSQRT)) / 2.0f;
					}

					_Owner->SetPosition(VAdd(_Owner->GetPosition(), VScale(move, movelength)));
					length -= movelength / movedLength;

					move = VNorm(VSub(GetPosition(), result.Dim[i].HitPosition));
					MoveComponent* EnMoveCon = tmpoly.mc->GetOwner()->GetComponent<class MoveComponent>();
					VECTOR EnMove;
					if (EnMoveCon != nullptr) {
						EnMove = EnMoveCon->GetVelocity();
					}
					else { EnMove = VGet(0, 0, 0); }
					VECTOR velocity = _Move->GetVelocity();
					movedLength /= VSize(velocity);
					if (velocity.x > 1) {
						int test = 0;
					}
					if (VDot(move, velocity) < VDot(move, EnMove)) {
						velocity = VSub(velocity, VScale(move, VDot(velocity, move)));
						velocity = VAdd(velocity, VScale(move, VDot(move, EnMove)));
						_Move->SetVelocity(velocity);
					}
					movedLength *= VSize(velocity);
					_Owner->SetPosition(VAdd(_Owner->GetPosition(), VScale(VNorm(velocity), movedLength * length)));
				}
			}
			MV1CollResultPolyDimTerminate(result);
		
		}

	}

	CollisionUpdate();

	if (num > 2) {
		int test = 0;
	}
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
	case 0:
		DrawSphere3D(GetPosition(), GetSize().x, 16, GetColor(255, 255, 255), GetColor(255, 255, 255), TRUE);
		break;
		// collision�̌`�@0: ���b�V�� 1:���� 2:�� 3:�J�v�Z�� 4:�~��  5:�l�p�� 6:������

	case 1:
		DrawLine3D(VAdd(GetPosition(), VGet(0, 0, 0)), VAdd(GetPosition(), VGet(0, 0, GetSize().z)), GetColor(255, 255, 255));
		break;




	default:
		MV1DrawModel(Handle);
		break;

	}
}

void MoveCollisionComponent::CollisionUpdate()
{
	MV1SetPosition(Handle, GetPosition());
	MV1SetScale(Handle, GetSize());
	MV1SetRotationZYAxis(Handle, GetFront(), GetUp(), 0);
	MV1RefreshCollInfo(Handle);
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

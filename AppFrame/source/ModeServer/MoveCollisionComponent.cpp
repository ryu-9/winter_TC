// MoveCollisionComponent.cpp
#include "MoveCollisionComponent.h"
#include "ActorClass.h"
#include "ModelComponent.h"
#include "MoveComponent.h"
#include "../AppFrame/source/ModelServer/ModelServer.h"

#include "../Application/ApplicationBase.h"


MoveCollisionComponent::MoveCollisionComponent(class ActorClass* owner, ModelComponent* model, VECTOR pos, VECTOR size, int type, bool move, bool active, int handle)
	: Component(owner)
	, _Model(model), Pos(pos), Size(size), Type(type), isMove(move), isCalc(move), isActive(active), Handle(handle)
	, Rot(VGet(0, 0, 0)), Front(VGet(0, 0, 1)), Up(VGet(0, 1, 0)), OldMove(VGet(0, 0, 0)), devpos(VGet(0, 0, 0))
	, flag(false), shomen(false)
{
	// モードに追加
	_Owner->GetMode()->AddMCollision(this);

	// 移動するならMoveComponentを取得
	if (isMove == TRUE) {
		auto m = _Owner->GetComponent<MoveComponent>();
		if (m.size() > 0) {
			_Move = m[0];
		}
	}
	OldPos = GetPosition();
	
	if (type == 2) { return; }

	// modelがnullptrならModelComponentを取得
	ModelComponent* modelComp = nullptr;
	if (model == nullptr)
	{
		auto modelC = _Owner->GetComponent<ModelComponent>();
		if (modelC.size() > 0) {
			modelComp = modelC[0];
		}

	}
	else
	{
		modelComp = model;
	}
	if (Handle == -1 && modelComp != nullptr) {
		Handle = modelComp->GetHandle();
	}


	// コリジョンリフレッシュ
	RefleshCollInfo();
}


MoveCollisionComponent::~MoveCollisionComponent()
{
	_Owner->GetMode()->RemoveMCollision(this);
}

void MoveCollisionComponent::Update() {


	devpos = VGet(0, 0, 0);
	_CollResult.clear();

	if (isActive == TRUE && isCalc == TRUE) {

		int pushnum = 0;
		debugpos.clear();

		drawpos[0] = GetPosition();

		if (_Move == nullptr && isMove) {
			auto mc = _Owner->GetComponent<MoveComponent>();
			if (mc.size() > 0) {
				_Move = mc[0];
			}
		}




		flag = FALSE;
		shomen = FALSE;
		OldMove = VGet(0, 0, 0);
		std::deque<MV1_COLL_RESULT_POLY_DIM> debugresult;
		bool sqrtFlag = FALSE;
		bool shomen = false;
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
						result = MV1CollCheck_Capsule(mcoll->GetHandle(), -1, GetPosition(), OldPos,  radian );
					}
					debugresult.push_front(result);
					if (result.HitNum > 0) {


						CollResult tmp_CollResult;
						tmp_CollResult.mc = mcoll;
						VECTOR Ntmp = VNorm(tmp);
						for (int i = 0; i < result.HitNum; i++) {
							if (VDot(Ntmp, result.Dim[i].Normal) <= 0) {
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

		if (_CollResult.size() <= 0) {
			RefleshCollInfo();
			drawpos[1] = OldPos;
			OldPos = GetPosition();
			return;
		}
		std::deque<VECTOR> HitPos;
		std::deque<VECTOR> HitNormal;
		std::deque<float> dist;
		std::deque<MoveCollisionComponent*> coll;
		std::deque<VECTOR> movedList;
		VECTOR tmp = VSub(GetPosition(), OldPos);
		for (auto & r : _CollResult) {
			MV1_COLL_RESULT_POLY_DIM Presult;
			{
			
				VECTOR size = GetSize();
				float radian = size.x + size.y + size.z;
				radian /= 3;
				Presult = MV1CollCheck_Sphere(r.mc->GetHandle(), -1, OldPos, radian + VSize(tmp));
			}
			if (Presult.HitNum <= 0) {
				MV1CollResultPolyDimTerminate(Presult);
				continue;
			}
			VECTOR tmpHit;
			VECTOR tmpNormal;
			float tmpDist;

			for (int i = 0; i < Presult.HitNum; i++) {
				VECTOR tmpHit = Presult.Dim[i].HitPosition;
				VECTOR tmpNormal = Presult.Dim[i].Normal;
				float tmpDist = VSize(VSub(OldPos, tmpHit));

				for (int j = 0; j < r.mesh.size(); j++) {
					if (VEqual(Presult.Dim[i].Position[0], r.mesh[j].Position[0]) == TRUE &&
						VEqual(Presult.Dim[i].Position[1], r.mesh[j].Position[1]) == TRUE &&
						VEqual(Presult.Dim[i].Position[2], r.mesh[j].Position[2]) == TRUE) {
						r.mesh[j].HitPosition = tmpHit;
						if (dist.size() == 0) {
							HitPos.push_front(tmpHit);
							HitNormal.push_front(r.mesh[j].Normal);
							dist.push_front(tmpDist);
							coll.push_front(r.mc);
							break;
						}
						for (int k = 0; k < dist.size(); k++) {
							if (tmpDist < dist[k]) {
								HitPos.insert(HitPos.begin() + k, tmpHit);
								HitNormal.insert(HitNormal.begin() + k, r.mesh[j].Normal);
								dist.insert(dist.begin() + k, tmpDist);
								coll.insert(coll.begin() + k, r.mc);
								break;
							}
							if (tmpDist == dist[k]) {
								VECTOR tmpmove = VNorm(VSub(GetPosition(), OldPos));
								if (VDot(tmpmove, r.mesh[j].Normal)>VDot(tmpmove, HitNormal[k])) {
									HitPos.insert(HitPos.begin() + k, tmpHit);
									HitNormal.insert(HitNormal.begin() + k, r.mesh[j].Normal);
									dist.insert(dist.begin() + k, tmpDist);
									coll.insert(coll.begin() + k, r.mc);
									break;
								}
							}
							if (k == dist.size() - 1) {
								HitPos.push_back(tmpHit);
								HitNormal.push_back(r.mesh[j].Normal);
								dist.push_back(tmpDist);
								coll.push_back(r.mc);
								break;
							}
						}
						break;
					}
				}
			}
			MV1CollResultPolyDimTerminate(Presult);
		}

		float radius = GetSize().x + GetSize().y + GetSize().z;
		radius /= 3;
		int sqrtnum = -1;
		for (int i = 0; i < HitPos.size(); i++) {

			VECTOR move = VSub(OldPos, HitPos[i]);
			if (VSize(move) == 0) {
				move = HitNormal[i];
			}
			move = VNorm(move);
			if (VSize(move) <= 0) {
				continue;
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
				float judge = VDot(move, HitNormal[i]);

	
				move = VSub(move, VScale(OldMove, v));
				if (VSize(move) <= 0.01) {

					continue;
					move = HitNormal[i];
				}
				else { move = VNorm(move); }
				
				if (move.z < -0.5) {
					int test = 0;
				}


				if (HitNormal[i].y > 0.7 && move.y < 0.01) {
					int test = 0;
				}
				
				if (judge >= 1){

					float a = VDot(tmp, HitNormal[i]);
					if (a < 0) { a *= -1; }
					float b =VDot(VSub( GetPosition(), HitPos[i]), HitNormal[i]);
					//if (b < 0) { b *= -1; }
					float c = radius;
					length = -b + c;
					if (length >= 2000) {
						int test = 0;
					}
					if (length < 0) {
						continue;
					}
					if (move.x < -0.5) {
						int test = 0;
					}
					if (move.x > 0.5) {
						int test = 0;
					}
					shomen = true;
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
					if (move.x > 0.7) {
						int test = 0;
					}
					if (i == 0) {
						int test = 0;
					}
					if (length < 0) {
						continue;
					}
				}

				if (VDot(move, tmp) > 0) {
					continue;
				}

				if (isMove) {
					_Owner->SetPosition(VAdd(_Owner->GetPosition(), VScale(move, length)));
					pushnum++;


					debugpos.push_front(GetPosition());

					movedList.push_front(move);
					tmp = VSub(GetPosition(), OldPos);
					OldMove = move;
					devpos = VAdd(devpos, move);

					if (move.y < 0.3) {
						int test = 0;
					}

					MoveComponent* EnMoveCon = coll[i]->GetMove();
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
				}




				float deg = 0;
				if (move.y >= cos(deg / 180 * atan(1) * 4)) {
					if (_Move != nullptr) {
						_Move->SetStand(TRUE);
					}
				}

				RefleshCollInfo();

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
							if (length < 0) {
								continue;
							}
						}


						 if (isMove) {
							 _Owner->SetPosition(VAdd(_Owner->GetPosition(), VScale(move, length)));

							 debugpos.push_front(GetPosition());

							 pushnum++;

							 movedList.push_front(move);
							 tmp = VSub(GetPosition(), OldPos);
							 OldMove = move;
							 devpos = VAdd(devpos, move);

							 if (move.y < 0.3) {
								 int test = 0;
							 }

							 MoveComponent* EnMoveCon = coll[sqrtnum]->GetMove();
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
						 }



						if (move.y >= cos(deg / 180 * atan(1) * 4)) {
							if (_Move != nullptr) {
								_Move->SetStand(TRUE);
							}
						}
						RefleshCollInfo();
					}
				}

			}
		
		
		}
		if (VSize(OldPos) != 0) {
			if (!shomen && _CollResult.size() > 0) {
				int test = 0;
			}
		}

		if (devpos.y < 0.5 && radius > 10 && dist.size()>1) {
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

		drawpos[1] = OldPos;
		OldPos = GetPosition();

		RefleshCollInfo();
	}

}

void MoveCollisionComponent::RefleshCollInfo()
{
	MV1SetPosition(Handle, GetPosition());
	VECTOR size = GetSize();
	MV1SetScale(Handle, GetSize());
	MV1SetRotationXYZ(Handle, GetRotation());
	MV1SetupCollInfo(Handle);
}


VECTOR MoveCollisionComponent::GetPosition() {
	VECTOR pos = VMulti(Pos, _Owner->GetSize());
	VECTOR dir = _Owner->GetDirection();
	pos = VTransform(pos, MGetRotX(dir.x));
	pos = VTransform(pos, MGetRotY(dir.y));
	pos = VTransform(pos, MGetRotZ(dir.z));
	return VAdd(_Owner->GetPosition(), pos); 
}

VECTOR MoveCollisionComponent::GetSize() {
	return VMulti(Size, _Owner->GetSize()); 
}

VECTOR MoveCollisionComponent::GetUp()
{
	return _Model->GetUp();
}

VECTOR MoveCollisionComponent::GetFront()
{
	return _Model->GetFront();
}

VECTOR MoveCollisionComponent::GetRight()
{
	return VCross(GetUp(),GetFront());
}

VECTOR MoveCollisionComponent::GetRotation()
{
	return VAdd(Rot, _Owner->GetDirection());
}

void MoveCollisionComponent::DebugDraw()
{

	if (isActive == FALSE) {
		return;
	}
	if (isMove == TRUE) {
		DrawFormatString(0, 160, GetColor(255, 255, 255), "%f , %f, %f", _Owner->GetPosition().x, _Owner->GetPosition().y, _Owner->GetPosition().z);
		DrawCapsule3D(drawpos[0], drawpos[1], GetSize().x, 5, GetColor(255, 0, 0), 0, false);
		for (auto r : _CollResult) {
			for (auto m : r.mesh) {
				//DrawTriangle3D(m.Position[0], m.Position[1], m.Position[2], GetColor(0, 255, 0), TRUE);
			}
		}
		for (auto d : debugpos) {
			DrawSphere3D(d, GetSize().x, 16, GetColor(0, 0, 255), GetColor(0, 0, 255), FALSE);
		}
	}

	if (flag == TRUE) {
		DrawFormatString(0, 0, GetColor(255, 255, 255), "Hit");
		DrawFormatString(0, 16, GetColor(255, 255, 255), "%f , %f, %f", devpos.x, devpos.y, devpos.z);
	}
	if (shomen == TRUE) {
		DrawFormatString(0, 16, GetColor(255, 255, 255), "Shomen");
	}
	switch (Type) {

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

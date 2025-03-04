#include "SnowComponent.h"
#include "PlayerActor.h"


SnowComponent::SnowComponent(ActorClass* owner, MV1_COLL_RESULT_POLY m, bool flag0, bool flag1, bool flag2)
	:SpriteComponent(owner)
	, _Split(25)
	, _Flag(FALSE)
{

	_Handle = ModelServer::GetInstance()->AddGraph("res/Stage/Maptexture_ground.png");
	float dist = 0;
	int longestnum = 0;
	float tmp = VSize(VSub(m.Position[0], m.Position[1]));
	if (tmp > dist) { dist = tmp; }
	tmp = VSize(VSub(m.Position[1], m.Position[2]));
	if (tmp > dist) { dist = tmp; longestnum = 1; }
	tmp = VSize(VSub(m.Position[2], m.Position[0]));
	if (tmp > dist) { dist = tmp; longestnum = 2; }
	
	std::vector<unsigned short> index;
	std::vector<VERTEX3D> snow;
	VECTOR root = m.Position[0];
	VECTOR a = VSub(m.Position[1], root);
	VECTOR b = VSub(m.Position[2], root);


	if (root.z > a.z) { 
		VECTOR tmpv = root;
		root = VAdd(root, a);
		a = VSub(tmpv, root);
		b = VSub(VAdd(b, tmpv), root);
	}
	if (root.z > b.z) {
		VECTOR tmpv = root;
		root = VAdd(root, b);
		b = VSub(tmpv, root);
		a = VSub(VAdd(a, tmpv), root);
	}
	if (VCross(a,b).y * m.Normal.y > 0){
		VECTOR tmpv = a;
		a = b;
		b = tmpv;
	}

	int minx = m.Position[0].x;
	if(minx > m.Position[1].x){ minx = m.Position[1].x; }
	if(minx > m.Position[2].x){ minx = m.Position[2].x; }
	minx /= 500;
	int minz = m.Position[0].z;
	if(minz > m.Position[1].z){ minz = m.Position[1].z; }
	if(minz > m.Position[2].z){ minz = m.Position[2].z; }
	minz /= 500;

	int num = dist / _Split + 1;

	int k = 0;
	for (int i = 0; i <= num; i++) {
		int n = k - i - flag1;
		for (int j = 0; j <= i; j++) {
			
			if (i != 0) {
				if (j == 0) {
					if (flag0) {
						index.emplace_back(n - 1);
						index.emplace_back(k);
						index.emplace_back(n);

						index.emplace_back(n);
						index.emplace_back(k);
						index.emplace_back(k + 1);
					}
					

					//*/
				}
				else if (i == j) {
					index.emplace_back(n);
					index.emplace_back(k - 1);
					index.emplace_back(k);

					if (flag1) {
						index.emplace_back(n + 1);
						index.emplace_back(n);
						index.emplace_back(k);
						index.emplace_back(k);
						index.emplace_back(k + 1);
						index.emplace_back(n + 1);
					}
					//*/
				}
				else{
					index.emplace_back(n);
					index.emplace_back(k - 1);
					index.emplace_back(k);
					index.emplace_back(n++);
					index.emplace_back(k);
					index.emplace_back(n);
				}
			}
			k++;
			VECTOR p = VAdd(root, VAdd(VScale(a, (float)j / num), VScale(b, (float)(i - j) / num)));
			p = VAdd(p, VGet(0, 15, 0));
			VERTEX3D tmp;
			tmp.pos = p;
			tmp.norm = m.Normal;
			tmp.dif = GetColorU8(255, 255, 255, 255);
			tmp.spc = GetColorU8(0, 0, 0, 0);
			tmp.u = p.x / 500 - minx + 1;
			tmp.v = p.z / 500 - minz + 1;
			if(tmp.u < 0 || tmp.v < 0){
				int test = 0;
			}
			tmp.su = 1;
			tmp.sv = 1;
			if (j == 0 && flag0) {
				tmp.pos = VAdd(tmp.pos, VGet(0, -30, 0));
				snow.emplace_back(tmp);
				k++;
				tmp.pos = p;
			}
			snow.emplace_back(tmp);
			if (j == i && flag1) {
				tmp.pos = VAdd(tmp.pos, VGet(0, -30, 0));
				snow.emplace_back(tmp);
				k++;
			}

			if (i == j &&i == num && flag2) {
				for (int l = 0; l <= i; l++) {
					tmp = snow[snow.size() - 1 - i - flag1];
					tmp.pos = VAdd(tmp.pos, VGet(0, -30, 0));
					snow.emplace_back(tmp);

					index.emplace_back(k - i + l - 1);
					index.emplace_back(k - i + l - 2);
					index.emplace_back(k + l);
					/**/
					index.emplace_back(k - i + l - 1);
					index.emplace_back(k + l);
					index.emplace_back(k + l + 1);
					//*/
					if (l != 0) {


					}
					if (l != i) {
						///*

						//*/

					}

				}
			}

		}
	}

	if (snow.size() > 10000) {
		MV1_COLL_RESULT_POLY m1 = m;
		MV1_COLL_RESULT_POLY m2 = m;
		VECTOR v;
		bool flag[2][3] = { { flag0, flag1, flag2 } , { flag0, flag1, flag2 } };
		switch (longestnum) {
		case 0:
			v = VSub(m.Position[1], m.Position[0]);
			m1.Position[1] = VAdd(m.Position[0], VScale(v, 0.5));
			m2.Position[0] = VAdd(m.Position[0], VScale(v, 0.5));
			//flag[0][2] = false;
			//flag[1][2] = false;
			break;
		
		case 1:
			v = VSub(m.Position[2], m.Position[1]);
			m1.Position[2] = VAdd(m.Position[1], VScale(v, 0.5));
			m2.Position[1] = VAdd(m.Position[1], VScale(v, 0.5));
			//flag[0][2] = false;
			//flag[1][0] = false;
			break;
		
		case 2:
			v = VSub(m.Position[0], m.Position[2]);
			m1.Position[2] = VAdd(m.Position[2], VScale(v, 0.5));
			m2.Position[0] = VAdd(m.Position[2], VScale(v, 0.5));
			//flag[0][1] = false;
			//flag[1][2] = false;
			break;
		
		}

		new SnowComponent(owner, m1, flag[0][0], flag[0][1], flag[0][2]);
		new SnowComponent(owner, m2, flag[1][0], flag[1][1], flag[1][2]);
		delete this;
		return;
	
	}

	_Longest = dist * dist;

	_SnowSize = snow.size();
	_Snow = new VERTEX3D[_SnowSize];
	_Height = new float[_SnowSize];
	for (int i = 0; i < _SnowSize; i++) {
		_Snow[i] = snow[i];
		_Height[i] = 0;
	}



	_IndexSize = index.size();
	_Index = new unsigned short[_IndexSize];
	for (int i = 0; i < _IndexSize; i++) {
		_Index[i] = index[i];
	}


	EffectController* ec = EffectController::GetInstance();
	if (ec != nullptr) {
		auto sm = ec->GetEffect<ShadowMapSpriteComponent>();
		if (sm.size() > 0) {
			sm[0]->AddRemoveSprite(this);
		}
	}

	_LowSnow[0] = _Snow[1]; _LowSnow[1] = _Snow[0]; _LowSnow[2] = _Snow[_SnowSize - 2 * num - 3];
	_LowSnow[3] = _Snow[_SnowSize - num - 1]; _LowSnow[4] = _Snow[_SnowSize - num - 3]; _LowSnow[5] = _Snow[_SnowSize - 1];

	_LowIndex[0] = 0; _LowIndex[1] = 1; _LowIndex[2] = 2; _LowIndex[3] = 1; _LowIndex[4] = 3; _LowIndex[5] = 2;
	_LowIndex[6] = 3; _LowIndex[7] = 4; _LowIndex[8] = 5; _LowIndex[9] = 3; _LowIndex[10] = 5; _LowIndex[11] = 2;
	_LowIndex[12] = 4; _LowIndex[13] = 5; _LowIndex[14] = 0; _LowIndex[15] = 5; _LowIndex[16] = 1; _LowIndex[17] = 0;
	_LowIndex[18] = 0; _LowIndex[19] = 2; _LowIndex[20] = 4;

	_Area = VSize(VCross(a, b));
	_Area /= num * num;

}

SnowComponent::~SnowComponent()
{
	delete[] _Snow;
	delete[] _Index;
	delete[] _Height;
}

void SnowComponent::Draw()
{
	bool flag = false;
	int olddist = 1000000000;
	int num = 0;
	int que = 0;
	int calc = 0;
	int split = 2 * _Split * _Split;

	SetTextureAddressModeUV(DX_TEXADDRESS_WRAP, DX_TEXADDRESS_WRAP);
	/*
	DrawPolygonIndexed3D(_Snow, _SnowSize, _Index, _IndexSize / 3, DX_NONE_GRAPH, FALSE);
	_MCList.clear();
	return;
	//*/


	if (_SnowSize < 3) { return; }
	std::deque<MoveCollisionComponent*> mclist;
	for (auto mc : _MCList) {
		mclist.push_back(mc);
	}
	for (auto mc : _OldMCList) {
		bool mcflag = false;
		for (auto mc2 : _MCList) {
			if (mc == mc2) {
				mcflag = true;
			}
		}
		if (!mcflag) {
			mclist.push_back(mc);
		}
	}

	for (auto mc : mclist) {

		float vertexdepth = 0;
		if(mc ==nullptr){
			continue;
		}
		num = 0;
		que = 0;
		olddist = 1000000000;
		int debugnum = 0;
		float size = mc->GetSize().x * mc->GetSize().x;
		for (int i = 0; i < _SnowSize; i++) {
			debugnum++;
			float dist = Segment_Point_MinLength_Square(mc->GetPosition(), mc->GetDrawPos(1), _Snow[i].pos);
			if (dist > _Longest) {
				break;
			}
			if (olddist > dist) {
				flag = false;
				olddist = dist;
			}
			if (num < i) {
				if (flag) {
					break;
				}
				que = sqrt(i * 2);
				num = (que + 2) * (que + 3) / 2;
				flag = true;
			}

			float depth = dist - size;
			if (depth < 0) {
				flag = false;
				depth = sqrt(-depth);
				if (depth > 30) { depth = 30; }
				vertexdepth += depth - _Height[i];
				_Snow[i].pos = VAdd(_Snow[i].pos, VGet(0, -_Height[i], 0));
				_Snow[i].pos = VAdd(_Snow[i].pos, VGet(0, -depth, 0));
				_Height[i] = -depth;
				_Flag = TRUE;

			}
			calc--;
			if (calc < 0) {
				unsigned int nande = que * que * split;
				if ((dist - size) > nande && flag) {
					int t = dist - size;
					t = sqrt(t);
					t /= _Split;
					t /= que + 1;
					for (int j = 0; j < t; j++) {
						i += que + j;
					}
					flag = false;
					int test = 0;
				}
			}
			else { calc = 100; }

		}
		if(debugnum > 5000){
			int test = 0;
		}
		debugnum = 0;

		auto pl = dynamic_cast<PlayerActor*>(mc->GetOwner());
		if (pl != nullptr) {
			float tmp = vertexdepth * _Area / 10000000 / pl->GetSize().x;
			pl->AddSize(tmp);
		}
	}

	if (_Flag) {
		_Flag = FALSE;
		for (int i = 0; i < _SnowSize; i++) {
			if (_Height[i] < 0) {
				_Height[i] += 0.01;
				_Snow[i].pos = VAdd(_Snow[i].pos, VGet(0, 0.01, 0));
				_Flag = TRUE;
			}

		}
	}


	MATERIALPARAM MatParam;

	MatParam.Diffuse = GetColorF(1, 1, 1, 1.0f);	// ディフューズカラーは白
	MatParam.Ambient = GetColorF(1, 1, 1, 1.0f);	// アンビエントカラーは白( ライトのアンビエントカラーをそのまま反映する )
	MatParam.Specular = GetColorF(0.0f, 0.0f, 0.0f, 0.0f);	// スペキュラカラーは無し
	MatParam.Emissive = GetColorF(0.1f, 0.1f, 0.2f, 0.5f);	// エミッシブカラー( 自己発光 )もなし
	MatParam.Power = 0.0f;						// スペキュラはないので０

	// マテリアルのパラメータをセット
	SetMaterialParam(MatParam);
	if (_Flag) { DrawPolygonIndexed3D(_Snow, _SnowSize, _Index, _IndexSize / 3, _Handle, FALSE); }
	else { DrawPolygonIndexed3D(_LowSnow, 6, _LowIndex, 7, _Handle, FALSE); }

	//_OldMCList = _MCList;
	_MCList.clear();
	return;

}

#include "SnowComponent.h"


SnowComponent::SnowComponent(ActorClass* owner, MV1_COLL_RESULT_POLY m, bool flag0, bool flag1, bool flag2)
	:SpriteComponent(owner)
	, _Split(25)
{


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
			; }
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
			tmp.u = 0;
			tmp.v = 0;
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
		ec->GetShadowMap(0)->AddRemoveSprite(this);
	}

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


	/*
	DrawPolygonIndexed3D(_Snow, _SnowSize, _Index, _IndexSize / 3, DX_NONE_GRAPH, FALSE);
	_MCList.clear();
	return;
	//*/

	if (_SnowSize < 3) { return; }
	for (auto mc : _MCList) {
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

			if (dist < size + 50) {
				flag = false;
				_Snow[i].pos = VAdd(_Snow[i].pos, VGet(0,- 50 -_Height[i], 0));
				_Height[i] = -50;
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
	}

	for (int i = 0; i < _SnowSize; i++) {
		if (_Height[i] < 0) {
			_Height[i]+= 0.01;
			_Snow[i].pos = VAdd(_Snow[i].pos, VGet(0, 0.01, 0));
		}
		
	}

	int debug = DrawPolygonIndexed3D(_Snow, _SnowSize, _Index, _IndexSize/3, DX_NONE_GRAPH, FALSE);
	_MCList.clear();
	return;

}

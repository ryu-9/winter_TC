#include "UIChipSpriteComponent.h"

UIChipSpriteComponent::UIChipSpriteComponent(UIChipClass* owner, int drawOrder)
	:SpriteComponent(owner,drawOrder)
	,_UIc(owner)
	,_BlendMode(DX_BLENDMODE_NOBLEND)
	,_BlendPalam(255)
{
}

void UIChipSpriteComponent::Draw() {
	auto uiD = _UIc->GetUIData();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, uiD->alpha);
	MATRIX mRot = MGetRotZ(uiD->angle);
	MATRIX mScale = MGetScale(uiD->scale);
	MATRIX mTrans = MGetTranslate(_UIc->GetPosition());
	MATRIX m;
	m = MMult(mRot, mScale);
	m = MMult(m, mTrans);
	auto num = _UIc->GetImageNum();
	// �`�悷��摜��4�̒��_���W
	VECTOR pos[4] = {
		// �ʏ�p�i���ォ��E���j
		VGet(_CGData[num].width  * -1 / 2.0, _CGData[num].height * -1 / 2.0, 0),	// ����
		VGet(_CGData[num].width / 2.0, _CGData[num].height * -1 / 2.0 ,0),	// �E��
		VGet(_CGData[num].width / 2.0,  _CGData[num].height / 2.0 ,0),	// �E��
		VGet(_CGData[num].width * -1 / 2.0,  _CGData[num].height / 2.0 ,0),	// ����
	};
	if (uiD->turn != 0) {
		// ���]�p
		VECTOR posTurn[4] = {
			// ���]�p�i�E�ォ�獶���j
			VGet(_CGData[num].width / 2.0, _CGData[num].height *-1 / 2.0 ,0),	// �E��
			VGet(_CGData[num].width * -1 / 2.0, _CGData[num].height *-1 / 2.0, 0),	// ����
			VGet(_CGData[num].width *-1 / 2.0,  _CGData[num].height / 2.0 ,0),	// ����
			VGet(_CGData[num].width / 2.0,  _CGData[num].height / 2.0 ,0),	// �E��
		};
		for (int i = 0; i < 4; i++) {
			pos[i] = posTurn[i];
		}
	}

	// 4�̒��_���W�S�Ăɍs����|���ĕϊ�����
	for (int i = 0; i < 4; i++) {
		pos[i] = VTransform(pos[i], m);
	}
	// �ϊ��������W�ŕ`�悷��
	DrawModiGraph(
		pos[0].x, pos[0].y,
		pos[1].x, pos[1].y,
		pos[2].x, pos[2].y,
		pos[3].x, pos[3].y,
		_CGData[num].handle, true);

	
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	
}

void UIChipSpriteComponent::SetImage(const TCHAR* filename)
{
	CGData tmp;
	tmp.handle = LoadGraph(filename);
	GetGraphSize(tmp.handle, &tmp.width, &tmp.height);
	_CGData.emplace_back(tmp);
}
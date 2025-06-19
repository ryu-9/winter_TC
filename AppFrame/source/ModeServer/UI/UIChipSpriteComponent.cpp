#include "UIChipSpriteComponent.h"

UIChipSpriteComponent::UIChipSpriteComponent(UIChipClass* owner, int drawOrder)
	:SpriteComponent(owner,drawOrder)
	,_UIc(owner)
{
}

void UIChipSpriteComponent::Draw() {
	auto uiD = _UIc->GetUIData();
	SetDrawBlendMode(uiD->blendMode, uiD->blendParam);
	MATRIX mRot = MGetRotZ(uiD->angle);
	MATRIX mScale = MGetScale(uiD->scale);
	MATRIX mTrans = MGetTranslate(_UIc->GetPosition());
	MATRIX m;
	m = MMult(mRot, mScale);
	m = MMult(m, mTrans);
	auto num = _UIc->GetImageNum();
	// 描画する画像の4つの頂点座標
	VECTOR pos[4] = {
		// 通常用（左上から右回り）
		VGet(_CGData[num].width  * -1 / 2.0, _CGData[num].height * -1 / 2.0, 0),	// 左上
		VGet(_CGData[num].width / 2.0, _CGData[num].height * -1 / 2.0 ,0),	// 右上
		VGet(_CGData[num].width / 2.0,  _CGData[num].height / 2.0 ,0),	// 右下
		VGet(_CGData[num].width * -1 / 2.0,  _CGData[num].height / 2.0 ,0),	// 左下
	};
	if (uiD->turn != 0) {
		// 反転用
		VECTOR posTurn[4] = {
			// 反転用（右上から左回り）
			VGet(_CGData[num].width / 2.0, _CGData[num].height *-1 / 2.0 ,0),	// 右上
			VGet(_CGData[num].width * -1 / 2.0, _CGData[num].height *-1 / 2.0, 0),	// 左上
			VGet(_CGData[num].width *-1 / 2.0,  _CGData[num].height / 2.0 ,0),	// 左下
			VGet(_CGData[num].width / 2.0,  _CGData[num].height / 2.0 ,0),	// 右下
		};
		for (int i = 0; i < 4; i++) {
			pos[i] = posTurn[i];
		}
	}

	// 4つの頂点座標全てに行列を掛けて変換する
	for (int i = 0; i < 4; i++) {
		pos[i] = VTransform(pos[i], m);
	}
	// 変換した座標で描画する
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

void UIChipSpriteComponent::Receive(int message) {
}

#include "SourceVoiceItemEffect.h"
#include "SourceVoiceItem.h"
#include "../ModeServer/ActorClass.h"

SourceVoiceItemEffectBase::SourceVoiceItemEffectBase(SourceVoiceItem* sv) {
	_svItem = sv;
	_svItem->AddEffect(this);
}

SourceVoiceItemEffectBase::~SourceVoiceItemEffectBase() {
	_svItem->RemoveEffect(this);
}

SVItemVolumeFade::SVItemVolumeFade(SourceVoiceItem* sv)
	:base(sv) 
	, _FadeTime(0)
	, _FadeTimeMax(0)
{
	_VolumeStart = _svItem->GetVolume();
	_VolumeEnd = 0;
	_Destroy = true;
}



void SVItemVolumeFade::Update() {
	// TODO: Modeに触って時間を取得する
	_FadeTime += 16;
	float rate = _FadeTime / _FadeTimeMax;
	auto vol = _VolumeStart + (_VolumeEnd - _VolumeStart) * rate;
	vol *= vol;
	_svItem->SetVolume(vol);

	if (_FadeTime >= _FadeTimeMax && _Destroy) {
		delete this;
	}

}

SVItemPitchRand::SVItemPitchRand(SourceVoiceItem* sv)
	:base(sv)
	, _Rand(25)
{
}

void SVItemPitchRand::SetUp() {
	float r = rand() %_Rand;
	r *= 0.01f;
	if (rand() % 2 == 0) r *= -1;
	_svItem->SetPitch(1.0f + r);
}

SVItemDistanceDecay::SVItemDistanceDecay(SourceVoiceItem* sv)
	:base(sv)
{
}

void SVItemDistanceDecay::Update() {
	// 距離による音量減衰
	auto pos =VGet(0,0,0);// _Owner->GetPosition();
	auto lpos = _Listener->GetPosition();

	auto dist = VSize(VSub(pos, lpos));

	// 点音源のパターン
	auto db = 10 * log10((10000) / (dist * dist));

	_svItem->SetVolumeDB(db);
}

SVItemPanning::SVItemPanning(SourceVoiceItem* sv) 
	:base(sv)
{

}

void SVItemPanning::Update() {
	// パンニング

	auto pos = VGet(0, 0, 0);// _Owner->GetPosition();
	auto lpos = _Listener->GetPosition();
	// 角度の計算
	pos = VNorm(pos);
	lpos = VNorm(lpos);
	auto dot = VDot(pos, lpos);
	auto angle = acos(dot);

	// 0~90度の範囲に変換
	angle = (angle + 180) / 4;
	angle = (angle / 180) * DX_PI;
	// 0~1に変換
	auto pan1 = cos(angle);
	auto pan2 = sin(angle);

}

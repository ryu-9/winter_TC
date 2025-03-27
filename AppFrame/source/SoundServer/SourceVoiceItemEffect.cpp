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



void SVItemVolumeFade::Update(ActorClass* p) {
	if (p == nullptr) { _FadeTime += 16;}
	else { _FadeTime += p->GetMode()->GetStepTm(); }
	float rate = _FadeTime / _FadeTimeMax;
	auto vol = _VolumeStart + (_VolumeEnd - _VolumeStart) * rate;
	vol *= vol;
	_svItem->SetVolume(vol);
	
	if (_FadeTime >= _FadeTimeMax) {
		_svItem->SetVolume(_VolumeEnd);
		if (_Destroy) {
			delete this;
		}
	}

}

SVItemPitchRand::SVItemPitchRand(SourceVoiceItem* sv)
	:base(sv)
	, _Rand(25)
{

}

void SVItemPitchRand::SetUp() {
	float r = rand() % _Rand;
	r *= 0.01f;
	if (rand() % 2 == 0) r *= -1;
	_svItem->SetPitch(1.0f + r);
}

SVItemDistanceDecay::SVItemDistanceDecay(SourceVoiceItem* sv)
	:base(sv)
{
	_Listener = nullptr;
}

void SVItemDistanceDecay::Update(ActorClass* p) {
	if (p == nullptr) { return; }
	auto pos = p->GetPosition	();
	if (_Listener == nullptr) { return; }
	auto lpos = _Listener->GetPosition();

	auto dist = VSize(VSub(pos, lpos));

	// 点音源のパターン
	auto db = 1 * log10((1000) / (dist * dist));

	_svItem->SetVolumeDB(db);
}

SVItemPanning::SVItemPanning(SourceVoiceItem* sv) 
	:base(sv)
	, _Listener(nullptr)
{
	
}

void SVItemPanning::Update(ActorClass* p) {
	if (p == nullptr) { return; }
	auto pos = p->GetPosition();
	auto lpos = VGet(960, 540, 0);
	if (_Listener != nullptr) { auto lpos = _Listener->GetPosition(); }
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

	_svItem->SetPan(pan1, pan2);
}

SVItemFilterFade::SVItemFilterFade(SourceVoiceItem* sv)
	:base(sv)
	, _FilterStart(0)
	, _FilterEnd(0)
	, _FadeTime(0)
	, _FadeTimeMax(0)
{
	XAUDIO2_FILTER_PARAMETERS param2;
	sv->GetSourceVoice()->GetFilterParameters(&param2);
	_FilterStart = param2.Frequency;
	_Destroy = true;

}

void SVItemFilterFade::Update(ActorClass* p) {
	if (p == nullptr) { _FadeTime += 16; } else { _FadeTime += p->GetMode()->GetStepTm(); }
	float rate = _FadeTime / _FadeTimeMax;
	auto prm = _FilterStart + (_FilterEnd - _FilterStart) * rate;
	
	_svItem->SetFilter(prm);

	if (_FadeTime >= _FadeTimeMax) {
		_svItem->SetFilter(_FilterEnd);
		if (_Destroy) {
			delete this;
		}
	}
}

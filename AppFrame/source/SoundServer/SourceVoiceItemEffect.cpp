#include "SourceVoiceItemEffect.h"
#include "SourceVoiceItem.h"

SourceVoiceItemEffectBase::SourceVoiceItemEffectBase(SourceVoiceItem* sv) {
	_SV = sv;
	_SV->AddEffect(this);
}

SourceVoiceItemEffectBase::~SourceVoiceItemEffectBase() {
	_SV->RemoveEffect(this);
}

SVItemVolumeFade::SVItemVolumeFade(SourceVoiceItem* sv)
	:base(sv) 
	, _FadeTime(0)
	, _FadeTimeMax(0)
{
	_VolumeStart = _SV->GetVolume();
	_VolumeEnd = 0;
	_Destroy = true;
}



void SVItemVolumeFade::Update() {
	// TODO: Mode‚ÉG‚Á‚ÄŽžŠÔ‚ðŽæ“¾‚·‚é
	_FadeTime += 0.016;
	float rate = _FadeTime / _FadeTimeMax;
	auto vol = _VolumeStart + (_VolumeEnd - _VolumeStart) * rate;
	vol *= vol;
	_SV->SetVolume(vol);

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
	_SV->SetPitch(1.0f + r);
}

#include "SourceVoiceItemEffect.h"
#include "SourceVoiceItem.h"

SourceVoiceItemEffectBase::SourceVoiceItemEffectBase(SourceVoiceItem* sv) {
	_SV = sv;
}

SVItemVolumeFade::SVItemVolumeFade(SourceVoiceItem* sv)
	:SourceVoiceItemEffectBase(sv) 
	, _FadeTime(0)
	, _FadeTimeMax(0)
{
	_VolumeStart = _SV->GetVolume();
	_Destroy = true;
}

SVItemVolumeFade::~SVItemVolumeFade() {
}

void SVItemVolumeFade::Update() {
	// TODO: Mode‚ÉG‚Á‚ÄŽžŠÔ‚ðŽæ“¾‚·‚é
	_FadeTime += 0.016;
	float rate = _FadeTime / _FadeTimeMax;
	auto vol = _VolumeStart + (_VolumeEnd - _VolumeStart) * rate;
	vol *= vol;
	_SV->SetVolume(vol);

	
}

#pragma once

class SourceVoiceItem;

class SourceVoiceItemEffectBase {
public:
	SourceVoiceItemEffectBase(class SourceVoiceItem* sv);
	virtual ~SourceVoiceItemEffectBase() {};

	virtual void Update() = 0;
	

protected:
	class SourceVoiceItem* _SV;
	bool _Destroy;
	bool _DestroyOwner;
};

class SVItemVolumeFade : public SourceVoiceItemEffectBase {
public:
	SVItemVolumeFade(class SourceVoiceItem* sv);
	virtual ~SVItemVolumeFade();
	void Update() override;


private:
	float _FadeTime;
	float _FadeTimeMax;
	float _Volume;
	float _VolumeStart;
	float _VolumeEnd;
};



#pragma once

class SourceVoiceItem;

class SourceVoiceItemEffectBase {
public:
	SourceVoiceItemEffectBase(class SourceVoiceItem* sv);
	virtual ~SourceVoiceItemEffectBase();

	virtual void SetUp() {};
	virtual void Update() {};
	

protected:
	class SourceVoiceItem* _SV;
	bool _Destroy;
	bool _DestroyOwner;
	bool _isUnique;				// 重複しないエフェクトか
};

class SVItemVolumeFade : public SourceVoiceItemEffectBase {
	typedef SourceVoiceItemEffectBase base;
public:
	SVItemVolumeFade(class SourceVoiceItem* sv);
	void Update() override;


private:
	float _FadeTime;
	float _FadeTimeMax;
	float _VolumeStart;
	float _VolumeEnd;
};

class SVItemPitchRand : public SourceVoiceItemEffectBase {
	typedef SourceVoiceItemEffectBase base;
public:
	SVItemPitchRand(class SourceVoiceItem* sv);
	void SetUp() override;

private:
	int _Rand;
};


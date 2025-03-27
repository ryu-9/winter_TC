#pragma once



class SourceVoiceItem;
class ActorClass;

class SourceVoiceItemEffectBase {
public:
	SourceVoiceItemEffectBase(class SourceVoiceItem* sv);
	virtual ~SourceVoiceItemEffectBase();

	virtual void SetUp() {};
	virtual void Update(class ActorClass* p) {};
	
	virtual bool IsUnique() { return false; }
protected:
	class SourceVoiceItem* _svItem;
	bool _Destroy;
	bool _DestroyOwner;
};

class SVItemVolumeFade : public SourceVoiceItemEffectBase {
	typedef SourceVoiceItemEffectBase base;
public:
	SVItemVolumeFade(class SourceVoiceItem* sv);
	void Update(class ActorClass* p) override;

	void SetFadeTime(float time) { _FadeTimeMax = time; }
	void SetVolume(float vol) { _VolumeEnd = vol; }

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

	bool IsUnique() override { return true; }
private:
	int _Rand;
};

class SVItemDistanceDecay : public SourceVoiceItemEffectBase {
	typedef SourceVoiceItemEffectBase base;
public:
	SVItemDistanceDecay(class SourceVoiceItem* sv);
	void Update(class ActorClass* p) override;

	void SetListener(class ActorClass* listener) { _Listener = listener; }
private:
	class ActorClass* _Listener;		// リスナー(とりあえずアクタのみ)
};

class SVItemPanning : public SourceVoiceItemEffectBase {
	typedef SourceVoiceItemEffectBase base;
public:
	SVItemPanning(class SourceVoiceItem* sv);
	void Update(class ActorClass* p) override;

	void SetListener(class ActorClass* listener) { _Listener = listener; }

private:
	
	class ActorClass* _Listener;		// リスナー(とりあえずアクタのみ)
};

class SVItemFilterFade : public SourceVoiceItemEffectBase {
	typedef SourceVoiceItemEffectBase base;
public:
	SVItemFilterFade(class SourceVoiceItem* sv);
	void Update(class ActorClass* p) override;

	void SetFadeTime(float time) { _FadeTimeMax = time; }
	void SetFilParam(float fil) { _FilterEnd = fil; }

private:
	float _FadeTime;
	float _FadeTimeMax;
	float _FilterStart;
	float _FilterEnd;
};
#pragma once

class SourceVoiceItem;

class SourceVoiceEffectBase {
public:
	virtual ~SourceVoiceEffectBase() = default;

	virtual void Update() = 0;
	void SetSourceVoice(SourceVoiceItem* sv) { _SV = sv; }

protected:
	SourceVoiceItem* _SV;
};

class SVFade : public SourceVoiceEffectBase {
public:
	SVFade();

	void Set(float vol, int tm);
	virtual void Update() override;
private:
	float _Start;
	float _End;
	int _Tm;
	int _Cnt;
};

class SVEcho : public SourceVoiceEffectBase {
	typedef SourceVoiceEffectBase base;
public:
	SVEcho();

	void Set(float palam);
	virtual void Update() override;

private:

	float palam;
};
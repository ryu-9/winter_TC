#pragma once

class SourceVoiceItem;

class SourceVoiceEffectBase {
public:
	virtual ~SourceVoiceEffectBase() = default;

	virtual void Update() = 0;
	void SetSourceVoice(SourceVoiceItem* sv) { _SV = sv; }
	virtual void Set() {};

protected:
	SourceVoiceItem* _SV;
};

class SVFade : public SourceVoiceEffectBase {
public:
	SVFade();

	void Set(float vol, int tm);
	void Update() override;

	bool IsEnd() { return _Cnt >= _Tm; }
private:
	float _Start;
	float _End;
	int _Tm;
	int _Cnt;
};


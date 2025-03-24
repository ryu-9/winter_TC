#include "UIChipEffectComponent.h"


float EasingInOutQuart(float cnt, float start, float end, float frames) {
	cnt /= frames / 2.0;
	if (cnt < 1) {
		return (end - start) / 2.0 * cnt * cnt * cnt * cnt + start;
	}
	cnt -= 2;
	return -(end - start) / 2.0 * (cnt * cnt * cnt * cnt - 2) + start;
}

UIChipEffectComponent::UIChipEffectComponent(ActorClass* owner, int updateOrder)
	:Component(owner, updateOrder)
{
	_UIChip = dynamic_cast<UIChipClass*>(owner);
}




UIChipFadeComponent::UIChipFadeComponent(ActorClass* owner, float alpha, int tm, int updateOrder)
	:base(owner, updateOrder)
{
	_UIChip->GetUIData()->blendMode = DX_BLENDMODE_ALPHA;
	_Data.start = _UIChip->GetUIData()->blendParam;
	_Data.tm = tm;
	_Data.end = alpha;
}

UIChipFadeComponent::~UIChipFadeComponent()
{
}

void UIChipFadeComponent::ProcessInput() {
	_Data.cnt += _Owner->GetMode()->GetStepTm();
	_UIChip->GetUIData()->blendParam = (_Data.end - _Data.start) * ((float)_Data.cnt / (float)_Data.tm) + _Data.start;
	if (_Data.cnt >= _Data.tm) {
		_UIChip->GetUIData()->blendParam = _Data.end;
		delete this;
	}
}

void UIChipFadeComponent::Update()
{
	
}

UIChipFocusComponent::UIChipFocusComponent(ActorClass* owner, VECTOR scale, int tm, int updateOrder)
	:base(owner, updateOrder)
{
	_Data.start = _UIChip->GetUIData()->scale;
	_Data.tm = tm;
	_Data.end = scale;
	_isFocus = false;
	_Data.cnt = tm;
}

UIChipFocusComponent::~UIChipFocusComponent()
{
}

void UIChipFocusComponent::ProcessInput()
{
	if (_isFocus) {
		if (_Data.cnt < _Data.tm) {
			_Data.cnt += _Owner->GetMode()->GetStepTm();
			auto tmp = (VSub(_Data.end, _Data.start));
			auto tmp2 = ((float)_Data.cnt / (float)_Data.tm);
			tmp.x = tmp.x * tmp2 + _Data.start.x;
			tmp.y = tmp.y * tmp2 + _Data.start.y;
			tmp.z = tmp.z;
			_UIChip->GetUIData()->scale = tmp;
			if (_Data.cnt >= _Data.tm) {
				_UIChip->GetUIData()->scale = _Data.end;
			}
		}
	}
	else {
		if (_Data.cnt < _Data.tm) {
			_Data.cnt += _Owner->GetMode()->GetStepTm();
			auto tmp = (VSub(_Data.start, _Data.end));
			auto tmp2 = ((float)_Data.cnt / (float)_Data.tm);
			tmp.x = tmp.x * tmp2 + _Data.end.x;
			tmp.y = tmp.y * tmp2 + _Data.end.y;
			tmp.z = tmp.z;
			_UIChip->GetUIData()->scale = tmp;
			if (_Data.cnt >= _Data.tm) {
				_UIChip->GetUIData()->scale = _Data.start;
//				_Owner->RemoveComponent(this);
			}
		}
	}
	
}

void UIChipFocusComponent::Update()
{
}

void UIChipFocusComponent::Receive(int message)
{
	if (message == 1) {
		_isFocus = true;
		_Data.cnt = 0;
	}
	else if (message == 0) {
		if (_isFocus) {
			_isFocus = false;
			_Data.cnt = 0;
		}
	}
}

UIChipMoveComponent::UIChipMoveComponent(ActorClass* owner, VECTOR pos, int tm, int updateOrder)
	:base(owner, updateOrder)
{
	_Data.start = _UIChip->GetPosition();
	_Data.tm = tm;
	_Data.end = pos;
	_Data.cnt = 0;
}

UIChipMoveComponent::~UIChipMoveComponent()
{
}

void UIChipMoveComponent::ProcessInput()
{
	/*if (_Data.cnt < _Data.tm) {
		_Data.cnt += _Owner->GetMode()->GetStepTm();
		auto tmp = (VSub(_Data.end, _Data.start));
		auto tmp2 = ((float)_Data.cnt / (float)_Data.tm);
		tmp.x = tmp.x * tmp2 + _Data.start.x;
		tmp.y = tmp.y * tmp2 + _Data.start.y;
		tmp.z = tmp.z;
		_UIChip->SetPosition(tmp);
		if (_Data.cnt >= _Data.tm) {
			_UIChip->SetPosition(_Data.end);
			_Owner->RemoveComponent(this);
		}
	}
	*/
	if (_Data.cnt < _Data.tm) {
		_Data.cnt += _Owner->GetMode()->GetStepTm();
		auto tmp = (VSub(_Data.end, _Data.start));
		tmp.x = EasingInOutQuart(_Data.cnt, _Data.start.x, _Data.end.x, _Data.tm);
		tmp.y = EasingInOutQuart(_Data.cnt, _Data.start.y, _Data.end.y, _Data.tm);
		_UIChip->SetPosition(tmp);
		if (_Data.cnt >= _Data.tm) {
			_UIChip->SetPosition(_Data.end);
			delete this;
		}
	}
	
}

void UIChipMoveComponent::Update()
{
}

UIChipScaleComponent::UIChipScaleComponent(ActorClass* owner, VECTOR scale, int tm, int updateOrder) 
	:base(owner, updateOrder) {
	_Data.start = _UIChip->GetUIData()->scale;
	_Data.tm = tm;
	_Data.end = scale;
	_Data.cnt = 0;
}

UIChipScaleComponent::~UIChipScaleComponent() {
}

void UIChipScaleComponent::ProcessInput() {
	if (_Data.cnt < _Data.tm) {
		_Data.cnt += _Owner->GetMode()->GetStepTm();
		auto tmp = (VSub(_Data.end, _Data.start));
		auto tmp2 = ((float)_Data.cnt / (float)_Data.tm);
		tmp.x = tmp.x * tmp2 + _Data.start.x;
		tmp.y = tmp.y * tmp2 + _Data.start.y;
		tmp.z = tmp.z;
		_UIChip->GetUIData()->scale = tmp;
		if (_Data.cnt >= _Data.tm) {
			_UIChip->GetUIData()->scale = _Data.end;
			delete this;
		}
	}
}

void UIChipScaleComponent::Update() {
}

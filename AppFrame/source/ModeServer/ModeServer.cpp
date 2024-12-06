
#include <cstddef>
#include "DxLib.h"
#include "ModeServer.h"


/// �C���X�^���X 
ModeServer* ModeServer::_lpInstance = NULL;

// --------------------------------------------------------------------------
/// @brief �R���X�g���N�^ 
// --------------------------------------------------------------------------
ModeServer::ModeServer()
{
	_lpInstance = this;
	_uid_count = 1;
	_nowMode = NULL;
	_skipProcessMode = NULL;
	_skipRenderMode = NULL;
	_pauseProcessMode = NULL;
}

// --------------------------------------------------------------------------
/// @brief 
// --------------------------------------------------------------------------
ModeServer::~ModeServer()
{
	Clear();
	_lpInstance = NULL;
}


// �o�^�͂��邪�A��x���C�����񂳂Ȃ��Ƃ����Ȃ�
int ModeServer::Add(ModeBase* mode, int layer, const char* name) {
	_vModeAdd.push_back(mode);		// �o�^�\��
	mode->_uid = _uid_count;
	_uid_count++;
	mode->_layer = layer;
	mode->_szName = name;
	return mode->_uid;
}

// �폜�\��
int ModeServer::Del(ModeBase* mode) {
	_vModeDel.push_back(mode);
	return 0;
}

// �폜��delete
int ModeServer::Release(ModeBase* mode) {
	lstModeBase::iterator ite = _vMode.begin();
	for (; ite != _vMode.end(); ) {
		if ((*ite) == mode) {
			(*ite)->Terminate();
			delete (*ite);
			ite = _vMode.erase(ite);
		}
		else {
			++ite;
		}
	}
	return 1;
}

// �S���폜
void ModeServer::Clear() {
	lstModeBase::reverse_iterator ite = _vMode.rbegin();
	for (; ite != _vMode.rend(); ++ite) {
		(*ite)->Terminate();
		delete (*ite);
	}
	lstModeBase::iterator iteAdd = _vModeAdd.begin();
	for (; iteAdd != _vModeAdd.end(); ++iteAdd) {
		(*iteAdd)->Terminate();
		delete (*iteAdd);
	}
	_vMode.clear();
	_vModeAdd.clear();
	_vModeDel.clear();
}



// �폜�\�񂳂�Ă��邩�H
bool ModeServer::IsDelRegist(ModeBase* mode) {
	if (_vModeDel.size() > 0) {
		lstModeBase::iterator iteDel = _vModeDel.begin();
		for (; iteDel != _vModeDel.end(); ++iteDel) {
			if ((*iteDel) == mode) { return true; }
		}
	}
	return false;
}

// ���[�h���X�g�ɂ��邩�H
bool ModeServer::IsAdd(ModeBase* mode) {
	// �o�^���̂��́A�o�^�\�񒆂̂��̂��猟������
	lstModeBase::iterator ite;
	ite = _vMode.begin();
	for (; ite != _vMode.end(); ++ite) {
		if (!IsDelRegist((*ite)) && (*ite) == mode) { return true; }
	}
	ite = _vModeAdd.begin();
	for (; ite != _vModeAdd.end(); ++ite) {
		if (!IsDelRegist((*ite)) && (*ite) == mode) { return true; }
	}
	return false;
}

// �o�^ID���猟��
ModeBase* ModeServer::Get(int uid) {
	// �o�^���̂��́A�o�^�\�񒆂̂��̂��猟������
	lstModeBase::iterator ite;
	ite = _vMode.begin();
	for (; ite != _vMode.end(); ++ite) {
		if (!IsDelRegist((*ite)) && (*ite)->_uid == uid) { return (*ite); }
	}
	ite = _vModeAdd.begin();
	for (; ite != _vModeAdd.end(); ++ite) {
		if (!IsDelRegist((*ite)) && (*ite)->_uid == uid) { return (*ite); }
	}
	return NULL;
}

// ���O���猟��
ModeBase* ModeServer::Get(const char* name) {
	// �o�^���̂��́A�o�^�\�񒆂̂��̂��猟������
	lstModeBase::iterator ite;
	ite = _vMode.begin();
	for (; ite != _vMode.end(); ++ite) {
		if (!IsDelRegist((*ite)) && (*ite)->_szName == name) { return (*ite); }
	}
	ite = _vModeAdd.begin();
	for (; ite != _vModeAdd.end(); ++ite) {
		if (!IsDelRegist((*ite)) && (*ite)->_szName == name) { return (*ite); }
	}
	return NULL;
}

// ID�擾
int ModeServer::GetId(ModeBase* mode) {
	if (IsAdd(mode)) {
		return mode->_uid;
	}
	return 0;
}
int ModeServer::GetId(const char* name) {
	return GetId(Get(name));
}

// ���O�擾
const char* ModeServer::GetName(ModeBase* mode) {
	if (IsAdd(mode)) {
		return mode->_szName.c_str();
	}
	return NULL;
}
const char* ModeServer::GetName(int uid) {
	return GetName(Get(uid));
}




// �v���Z�X���񂷂��߂̏�����
int ModeServer::ProcessInit() {
	// �폜�\�񂳂�Ă������̂��폜
	if (_vModeDel.size() > 0) {
		lstModeBase::iterator ite = _vModeDel.begin();
		for (; ite != _vModeDel.end(); ++ite) {
			Release((*ite));
		}
		_vModeDel.clear();
	}
	// Add()���ꂽ���̂�L���ɂ��A�K�v�ł���΃\�[�g���s��
	if (_vModeAdd.size() > 0) {
		lstModeBase::iterator ite = _vModeAdd.begin();
		for (; ite != _vModeAdd.end(); ++ite) {
			(*ite)->Initialize();
			_vMode.push_back((*ite));
		}
		_vModeAdd.clear();

		// ���C���[�ɂ��\�[�g���s��
		_vMode.sort(modeSort);
	}

	// �X�L�b�v�A�|�[�Y�̉���
	_skipProcessMode = NULL;
	_skipRenderMode = NULL;
	_pauseProcessMode = NULL;
	return 0;
}

int ModeServer::Process() {

	// ���݂̎��Ԃ��擾
	unsigned int t = GetNowCount();
	bool pause = false;

	// ���C���[�̏�̕����珈��
	lstModeBase::reverse_iterator ite = _vMode.rbegin();
	for (; ite != _vMode.rend(); ++ite) {
		if (!IsDelRegist(*ite)) {
			_nowMode = (*ite);

			if (!pause) {
				// ���Ԍo�ߏ���
				(*ite)->StepTime(t);
			}

			// Process�R�[��
			(*ite)->Process();
			(*ite)->Update();

			// Mode�������ꍇ (Reboot��)
			if (_vMode.empty()) {
				break;
			}

			if (!pause) {
				// �J�E���^����
				(*ite)->StepCount();
			}
		}
		if (_skipProcessMode == (*ite)) { break; }
		if (_pauseProcessMode == (*ite)) { pause = true; }
	}
	_nowMode = NULL;

	return 0;
}

// �v���Z�X���񂵂���̌�n��
int ModeServer::ProcessFinish() {
	return 0;
}

// �`����񂷂��߂̏�����
int ModeServer::RenderInit() {
	return 0;
}

int ModeServer::Render() {
	// ���C���[�̉��̕����珈��
	lstModeBase::iterator ite = _vMode.begin();
	for (; ite != _vMode.end(); ++ite) {
		if (_skipRenderMode && _skipRenderMode != (*ite)) { continue; }
		_skipRenderMode = NULL;

		if (!IsDelRegist(*ite)) {
			_nowMode = (*ite);
			(*ite)->Render();
		}
	}
	_nowMode = NULL;
	return 0;
}

// �`����񂵂���̌�n��
int ModeServer::RenderFinish() {
	return 0;
}



// ���������Ă��郌�C���[��艺�̃��C���[�́A�������Ă΂Ȃ�
int ModeServer::SkipProcessUnderLayer() {
	_skipProcessMode = _nowMode;
	return 0;
}

// ���������Ă��郌�C���[��艺�̃��C���[�́A�`����Ă΂Ȃ�
int ModeServer::SkipRenderUnderLayer() {
	_skipRenderMode = _nowMode;
	return 0;
}


// ���������Ă��郌�C���[��艺�̃��C���[�́A���Ԍo�߂��~�߂�
int ModeServer::PauseProcessUnderLayer() {
	_pauseProcessMode = _nowMode;
	return 0;
}


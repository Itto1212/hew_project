#pragma once

struct GameObject;

struct Action {

	bool isActive; // �A�N�V�������s�����ǂ����\��
	int id; // �A�N�V������ID�@������enum�萔��������
	float time; // �A�N�V�����J�n����̌o�ߎ���
	GameObject* pOwner; // ���̃A�N�V�����������Ă���GameObject�C���X�^���X

};

enum {
	ACTION_NONE,
	ACTION_ATTACK1,
	ACTION_MAGIC,
	ACTION_FX_SWORD,
	ACTION_FX_WINDSHOT,
	ACTION_KILLED,
	ACTION_BLINK,
	ACTION_FADEIN_FADEOUT,
	ACTION_GREEN_BLINK,
};

// �����o�[�֐�

// �A�N�V�����̏������B�e��GameObject�A�h���X��n���B
void Action_Initialize(Action* pThis, GameObject* pObj);

// ID�ŃA�N�V�������w�肵�ĊJ�n����
void Action_Set(Action* pThis, int actionId);

// �A�N�V�����̓��e�����s���邽�߂ɖ��t���[���Ăяo�����֐�
void Action_Update(Action* pThis);

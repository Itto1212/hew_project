#pragma once

// AI�N���X
struct CharAI {

	int id; // AI��ID�@���@�G�̎�ނ��Ƃ�ID������
	int state; // AI�̏�ԁ@���@�ҋ@�A�U���A���S�Ȃ�
	struct GameObject* pOwner; // ����AI�̏��L��

	float basePosX; // ���L�҂̊�ʒu
	float moveDirection; // ���L�҂̐i�s����
	float time; // ���ԊǗ��p
	float lastAttackTime; // �O��U������

};

// AI��ID���X�g
enum {
	AI_ID_NONE,
	AI_ID_PLAYER,
	AI_ID_GHOST,

	// ���@�G�̎�ނ𑝂₵���炱����ID��ǉ�����
	AI_ID_GOBLIN,
	AI_ID_SLIME, // �X���C��

};

// AI�̏�ԃ��X�g
enum {
	AI_STATE_INIT, // ����
	AI_STATE_IDLE, // �ҋ@
	AI_STATE_APPROACH, // �ڋ�
	AI_STATE_ATTACK, // �U��
	AI_STATE_ESCAPE, // ���S
	AI_STATE_DEAD // �퓬�s�\
};

// AI�̍X�V�iAI���s�j
void CharAI_Update(CharAI* pThis);

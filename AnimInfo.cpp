#include "AnimInfo.h"
#include <Windows.h>

const AnimInfo * AnimInfo_Get(int id)
{
	// �f�t�H���g�A�j���[�V����
	static const int t0[] = { 0, ANIMATION_LOOP };
	// 32x32�L�����̋��ʃA�j���[�V����
	static const int t1[] = { 0, 0, 1, 2, 2, 1, ANIMATION_LOOP };
	// �E�C���h�V���b�g�̃A�j���[�V����
	static const int t2[] = {
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
			14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, ANIMATION_LOOP };
	// ���G�t�F�N�g�̃A�j���[�V����
	static const int t3[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, ANIMATION_END };
	// �A�j���[�V�������̃e�[�u��
	static const AnimInfo animinfo[] = {
		{ t0, ARRAYSIZE(t0), 8.0f },
		{ t1, ARRAYSIZE(t1), 8.0f },
		{ t2, ARRAYSIZE(t2), 8.0f },
		{ t3, ARRAYSIZE(t3), 24.0f },
	};

	return &animinfo[id];
}

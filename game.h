#pragma once

//*****************************************************************************
// DX21�@�T���v���\�[�X
// �Q�[���̃��C���ƂȂ鏈���̃v���g�^�C�v�錾���L�q�����w�b�_�[�t�@�C��
// �ʏ�A�����̊֐���WinMain�̓K�؂ȏꏊ�ŌĂяo���ăQ�[���������悤�ɂ���B
//*****************************************************************************

#include <Windows.h>

BOOL Game_Initialize();
void Game_Update();
void Game_Draw();
void Game_Relese();

void Game_UseWindShot(struct GameObject* pUser);
void Game_UseSwordAttack(struct GameObject* pUser);

void Game_UpdateVertexBuffer();


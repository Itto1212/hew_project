#pragma once

//*****************************************************************************
// DX21　サンプルソース
// ゲームのメインとなる処理のプロトタイプ宣言を記述したヘッダーファイル
// 通常、これらの関数をWinMainの適切な場所で呼び出してゲームが動くようにする。
//*****************************************************************************

#include <Windows.h>

BOOL Game_Initialize();
void Game_Update();
void Game_Draw();
void Game_Relese();

void Game_UseWindShot(struct GameObject* pUser);
void Game_UseSwordAttack(struct GameObject* pUser);

void Game_UpdateVertexBuffer();


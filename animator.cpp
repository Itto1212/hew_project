#include "animator.h"
#include "GameTimer.h"

void Animator_Initialize(Animator * pThis)
{
	pThis->frame = 0;
	pThis->time = 0.0f;
	pThis->isActive = true;  // デフォルトはアニメーションON

	// テーブルアニメーションのデータを取得して設定
	Animator_SetAnimInfo(pThis, ANIM_ID_DEFAULT);
}

void Animator_Update(Animator * pThis)
{
	if (pThis->isActive) {  // アニメーションがONの時だけ処理を実行
		const int* table = pThis->pInfo->pTable;

		// デルタタイムを取得して加算
		pThis->time += GameTimer_GetDeltaTime() * pThis->speed;

		// テーブルを参照するのに使う添え字変数
		// ゲームループ停止時に配列領域オーバーを防ぐため％を使う
		int animationCounter = (int)pThis->time % pThis->pInfo->tableSize;

		// ループの最後に達したら
		if (table[animationCounter] == ANIMATION_LOOP) {
			animationCounter = 0;
			pThis->time = 0;
		}
		else if (table[animationCounter] == ANIMATION_END) {
			animationCounter = 0;
			pThis->time = 0;
			pThis->isActive = false;
		}

		pThis->frame = table[animationCounter];  // テーブルから現在のコマ番号取る
	}
}

void Animator_SetAnimInfo(Animator * pThis, int id)
{
	pThis->pInfo = AnimInfo_Get(id);
	pThis->speed = pThis->pInfo->speed;
}

#pragma once

#include "AnimInfo.h"



// Animatorクラス

// 必要な変数を変数セットとして構造体にする
struct Animator {

	int frame;  // 現在のアニメのフレーム番号を持つ
	float time;  // デルタタイムを加算した時間を持つ変数
	float speed;  // アニメーションの再生速度

	const AnimInfo* pInfo; // テーブルアニメーションの情報

	//アニメーションのON/OFFを切り替えるための変数
	bool isActive;

	int flip; // 反転処理のIDを指定する

};


enum { // 反転処理のID
	FLIP_NONE, // 反転しない
	FLIP_VERTICAL, // 垂直反転
	FLIP_HORIZONTAL // 水平反転
};


// 構造体の変数を初期化する関数
void Animator_Initialize(Animator* pThis);

// 毎フレーム呼び出され、アニメーションのコマを進める関数
void Animator_Update(Animator* pThis);

// アニメーションテーブルをIDを指定してセットする
void Animator_SetAnimInfo(Animator* pThis, int id);

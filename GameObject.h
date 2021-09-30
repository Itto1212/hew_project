#pragma once
// ↑　この命令を書いておくと、ヘッダーが何度もインクルードされて定義重複エラーが
// 　　出るのを防ぐことができる。

#include "animator.h"
#include "CharController.h"
#include "UvInfo.h"
#include "collider.h"
#include "action.h"
#include "CharAI.h"

// GameObjectクラス

// GameObjectクラスで必要になる変数セットを構造体として定義
struct GameObject {

	// 変数書いていく
	float posX, posY;  // 中心点の座標
	float sizeX, sizeY; // 大きさ

	// 階層構造にするための変数
	// このオブジェクトを所有しているオブジェクトの中心からどの位置にあるか
	// 特定の位置を原点とした座標値をローカル座標という
	// 所有されているオブジェクトの位置は、所有しているオブジェクトの座標＋ローカル座標で決まる
	float localPosX, localPosY;
	GameObject* pOwner; // 所有しているオブジェクトのアドレス

	CharController charController;  // キャラ移動のクラス変数

	Animator animator;  // アニメーションのクラス変数

	struct {
		float r, g, b, a;
	} color;

	UvInfo uvinfo; // テクスチャの情報

	Collider collider;  // 当たり判定

	Action action; // アクション

	CharAI ai;  // AI

	float hp, maxHp;  // HP / MAX HP

};


// 左右上下の４つの座標値を持つ構造体
struct FRECT {

	float left;
	float right;
	float top;
	float bottom;

};


// ↓↓　メンバー関数のプロトタイプ宣言

// スプライトの４隅のXY座標を計算して返す関数
FRECT GameObject_GetXY(GameObject* pThis);

// スプライトの４隅のUV座標を計算して返す関数
FRECT GameObject_GetUV(GameObject* pThis);

// 毎フレームの更新処理
void GameObject_Update(GameObject* pThis);



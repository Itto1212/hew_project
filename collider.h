#pragma once

struct GameObject;

struct Collider {

	bool isActive;
	GameObject* pOwner;

	// 矩形
	float centerX, centerY; // 中心点
	float sizeX, sizeY;  // 縦横の長さ

};


// 当たり判定の結果を格納するための構造体
struct COLLIDER_RESULT {

	bool isHit;
	float dx, dy;

};

// 初期化関数
void Collider_Initialize(Collider* pThis, GameObject* pOwner, float x, float y, float sizeX, float sizeY);
// 毎フレームの更新処理
void Collider_Update(Collider* pThis, float x, float y);
// ２つのコライダーが衝突しているか判定結果を返す関数
COLLIDER_RESULT Collider_Test(Collider* pThis, Collider* pOther);


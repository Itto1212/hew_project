#include "ObjectGenerator.h"
#include <Windows.h>

void ObjectGenerator_SetDragon(GameObject * pObj)
{
	// ポインタ変数の左に＊をつけると、アドレスが差している変数そのものとして振る舞う
	*pObj = {
		0, 0,  // 中心座標
		0.25f, 0.25f,  // サイズ
	};

	Animator_Initialize(&pObj->animator);  // アニメーション初期化

	pObj->uvinfo = { 0, 0, 80.0f/TEXTURE_SIZE_X, 64.0f/TEXTURE_SIZE_Y };  // テクスチャ情報を渡す
}

void ObjectGenerator_SetBG(GameObject * pObj)
{
	*pObj = {
		0, 0,  // 中心座標
		2, 2,  // サイズ
	};

	Animator_Initialize(&pObj->animator);  // アニメーション初期化
	pObj->animator.isActive = false;  // アニメーションOFF

	pObj->uvinfo = { 0, 0.5f, 640.0f / TEXTURE_SIZE_X, 480.0f / TEXTURE_SIZE_Y };  // テクスチャ情報を渡す
}

void ObjectGenerator_SetTitleLogo(GameObject * pObj)
{
	*pObj = {
		0, 0.2f,  // 中心座標
		1.0f*1.5f, 0.37f*1.33f*1.5f,  // サイズ
	};

	Animator_Initialize(&pObj->animator);  // アニメーション初期化
	pObj->animator.isActive = false;  // アニメーションOFF

	pObj->uvinfo = {
		640.0f / TEXTURE_SIZE_X, 0.5f,
		640.0f / TEXTURE_SIZE_X, 237.0f / TEXTURE_SIZE_Y };  // テクスチャ情報を渡す

	Action_Initialize(&pObj->action, pObj);
	Action_Set(&pObj->action, ACTION_GREEN_BLINK);
}

void ObjectGenerator_SetPressEnter(GameObject * pObj)
{
	*pObj = {
		0, -0.5f,  // 中心座標
		0.50f, 0.12f,  // サイズ
	};

	Animator_Initialize(&pObj->animator);  // アニメーション初期化
	pObj->animator.isActive = false;  // アニメーションOFF

	pObj->uvinfo = {
		640.0f / TEXTURE_SIZE_X, 1261.0f / TEXTURE_SIZE_Y,
		256.0f / TEXTURE_SIZE_X, 36.0f / TEXTURE_SIZE_Y };  // テクスチャ情報を渡す

	pObj->color = { 1,1,1,1 };

	Action_Initialize(&pObj->action, pObj);
	Action_Set(&pObj->action, ACTION_BLINK);
}

void ObjectGenerator_SetGameOver(GameObject * pObj)
{
	*pObj = {
		0, 0,  // 中心座標
		1.0f, 0.45f,  // サイズ
	};

	Animator_Initialize(&pObj->animator);  // アニメーション初期化
	pObj->animator.isActive = false;  // アニメーションOFF

	pObj->uvinfo = {
		895.0f / TEXTURE_SIZE_X, 1261.0f / TEXTURE_SIZE_Y,
		256.0f / TEXTURE_SIZE_X, 80.0f / TEXTURE_SIZE_Y };  // テクスチャ情報を渡す

	pObj->color = { 1,1,1,1 };
}

void ObjectGenerator_SetStageClear(GameObject * pObj)
{
	*pObj = {
		0, 0,  // 中心座標
		1.2f, 0.45f,  // サイズ
	};

	pObj->uvinfo = {
		895.0f / TEXTURE_SIZE_X, 1350.0f / TEXTURE_SIZE_Y,
		256.0f / TEXTURE_SIZE_X, 60.0f / TEXTURE_SIZE_Y };  // テクスチャ情報を渡す

	pObj->color = { 1,1,1,1 };
}

void ObjectGenerator_SetStageStart(GameObject * pObj)
{
	*pObj = {
		0, 0,  // 中心座標
		1.0f, 0.50f,  // サイズ
	};

	pObj->uvinfo = {
		895.0f / TEXTURE_SIZE_X, 1400.0f / TEXTURE_SIZE_Y,
		256.0f / TEXTURE_SIZE_X, 80.0f / TEXTURE_SIZE_Y };  // テクスチャ情報を渡す

	pObj->color = { 1,1,1,0 };

	Action_Initialize(&pObj->action, pObj);
	Action_Set(&pObj->action, ACTION_FADEIN_FADEOUT);
}

void ObjectGenerator_SetFadePanel(GameObject * pObj)
{
	*pObj = {
		0, 0,  // 中心座標
		2, 2,  // サイズ
	};

	pObj->uvinfo = {
		640.0f / TEXTURE_SIZE_X, 1300.0f / TEXTURE_SIZE_Y,
		32.0f / TEXTURE_SIZE_X, 32.0f / TEXTURE_SIZE_Y };  // テクスチャ情報を渡す
}

void ObjectGenerator_SetHpGauge(GameObject * pGauge, GameObject * pBase)
{
	*pGauge = {
		0, 0.75f,  // 中心座標
		1.0f, 0.1f,  // サイズ
	};

	pGauge->uvinfo = {
		640.0f / TEXTURE_SIZE_X, 1300.0f / TEXTURE_SIZE_Y,
		32.0f / TEXTURE_SIZE_X, 32.0f / TEXTURE_SIZE_Y };  // テクスチャ情報を渡す

	pGauge->color = { 0,1,0,1 };

	// 下地
	pBase->posX = pGauge->posX;
	pBase->posY = pGauge->posY;
	pBase->sizeX = pGauge->sizeX + 0.02f;
	pBase->sizeY = pGauge->sizeY + 0.02f;

	pBase->uvinfo = pGauge->uvinfo;

	pBase->color = { 0.5f,0.5f,0.5f,1 };
}

// この関数を成立させるためには、テクスチャを以下の条件で作成する
// ・32x32キャラは横並びで配置する
// ・32x32キャラはUV(0, 0.25f)の位置を左上として並べる
void ObjectGenerator_Character32x32(GameObject * pObj, int id)
{
	// ポインタ変数の左に＊をつけると、アドレスが差している変数そのものとして振る舞う
	*pObj = {
		0, 0,  // 中心座標
		0.15f, 0.20f,  // サイズ
	};

	Animator_Initialize(&pObj->animator);  // アニメーション初期化
	Animator_SetAnimInfo(&pObj->animator, ANIM_ID_CHAR32x32);

	// テクスチャ情報を渡す
	pObj->uvinfo = {
		(float)id*96.0f/ TEXTURE_SIZE_X, 0.25f,  // オフセットUV
		32.0f / TEXTURE_SIZE_X, 32.0f / TEXTURE_SIZE_Y  // １コマサイズUV
	};

	// キャラクターコントローラ初期化
	CharController_Initialize(&pObj->charController);

	//当たり判定の初期化
	// centerX, centerY, sizeX, sizeYは、画像の余白などによって調整する必要あり
	Collider_Initialize(&pObj->collider, pObj, 0, 0, 0.11f, 0.20f);

	Action_Initialize(&pObj->action, pObj);

	pObj->ai.pOwner = pObj;
}

void ObjectGenerator_SetFx(GameObject * pObj, int id)
{
	*pObj = {
		0, 0,  // 中心座標
		0.35f, 0.35f,  // サイズ
	};

	Animator_Initialize(&pObj->animator);  // アニメーション初期化

	// テクスチャ情報を渡す
	pObj->uvinfo = {
		0.0f, 0.3125f + (float)id*64.0f / TEXTURE_SIZE_Y,  // オフセットUV
		64.0f / TEXTURE_SIZE_X, 64.0f / TEXTURE_SIZE_Y  // １コマサイズUV
	};

	// キャラクターコントローラ初期化
	CharController* pCtrl = &pObj->charController;
	CharController_Initialize(pCtrl);
	pCtrl->direction = 0;
	pCtrl->maxMoveSpeedX *= 3.0f;  // デフォルトの３倍の速度
	pCtrl->accelForceX = pCtrl->maxMoveSpeedX*0.1f;

	//当たり判定の初期化
	Collider_Initialize(&pObj->collider, pObj, 0, 0, 0.20f, 0.20f);

	if (id == 0) { // ウインドショット
		Animator_SetAnimInfo(&pObj->animator, ANIM_ID_WINDSHOT);

		pObj->sizeX = 0.20f;
		pObj->sizeY = 0.20f;
	}
	else if (id == 1) { // 剣エフェクト
		Animator_SetAnimInfo(&pObj->animator, ANIM_ID_SWORD);

		pObj->sizeX = 0.30f;
		pObj->sizeY = 0.30f;
		pCtrl->isActive = false;
		pObj->collider.isActive = false;
	}

	

	
	// アクション初期化
	Action_Initialize(&pObj->action, pObj);
}

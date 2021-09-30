//*****************************************************************************
// DX21　サンプルソース
// ゲームのメインとなる処理を書いていくソースファイル
//*****************************************************************************

#include "game.h"
#include "direct3d.h"
#include "GameTimer.h"
#include "input.h"
#include "GameObject.h"
#include "ObjectGenerator.h"
#include "QuestMaster.h"
#include <math.h>

//*****************************************************************************
// 構造体定義
//*****************************************************************************

// 頂点１つあたりを表す構造体
struct VERTEX_POSTEX {
	float x, y, z;  // 頂点の位置
	float r, g, b, a;  // 頂点の色（赤, 緑, 青, 透明度）
	float u, v;  // テクスチャのUV座標
};


//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define MAX_SPRITE  64
// ポリゴン２つで１つの四角形（スプライト）。ポリゴンは３頂点なので、１スプライトは６頂点。
#define VERTEX_PER_SPRITE  (3*2)
#define VERTEX_BUFFER_SIZE  (MAX_SPRITE*sizeof(VERTEX_POSTEX)*VERTEX_PER_SPRITE)


// ゲームオブジェクト最大数
#define MAX_OBJECT   20

// 画面（シーン）の定数
enum SCENE_ID
{
	SCENE_ID_TITLE_START, // タイトル画面開始
	SCENE_ID_TITLE, // タイトル画面のID
	SCENE_ID_TITLE_END, // タイトル画面終了中

	SCENE_ID_GAME_START, // ゲーム画面開始
	SCENE_ID_GAME,  // ゲーム画面のID
	SCENE_ID_GAME_END,

	GAME_OVER_DEMO,  // プレイヤー死亡時のデモ

	RESULT_START, RESULT, RESULT_END,  // リザルト画面のID
};

// フェードの状態ID定数
enum FADE_ID
{
	FADE_ID_NO,  // フェードなし
	FADE_ID_IN,  // フェードイン
	FADE_ID_OUT, // フェードアウト
};

//*****************************************************************************
// グローバル変数
//*****************************************************************************

ID3D11Buffer* gpVertexBuffer;  // 頂点バッファ用の変数

ID3D11ShaderResourceView* gpTexture; // テクスチャ用変数

GameObject gObjects[MAX_OBJECT];  // オブジェクト配列

GameObject* gpPlayer = gObjects + 1;
GameObject* gpEnemy[MAX_STAGE_ENEMY] = { gObjects+2, gObjects+3, gObjects+4, gObjects+5, gObjects+6 };
GameObject* gpMagic = gObjects + 10;
GameObject* gpSword = gObjects + 11;

GameObject* gpFadePanel = gObjects + MAX_OBJECT - 1;
GameObject* gpHpGauge = gpFadePanel - 1;  // HPゲージ
GameObject* gpHpGaugeBase = gpHpGauge - 1; // HPゲージ下地
GameObject* gpStageStart = gpHpGaugeBase - 1; // Stage Startの文字

SCENE_ID gCurrentSceneID;  // 現在の画面（シーン）IDが入ってる変数

FADE_ID gFadeStatusID; // 現在のフェードの種類を持つ変数

bool gStageClear;
int gNumKilledEnemy;
int gNumStageEnemy;

int gCurrentQuestID = 2;
int gCurrentStageID;
const Quest* gpCurrentQuest;

//*****************************************************************************
// 関数のプロトタイプ宣言　（このgame.cpp内でのみ使う関数）
//*****************************************************************************

void TitleScene_Initialize();  // タイトル画面の初期化関数
void TitleScene_Update();
void GameScene_Initialize();  // ゲーム画面の初期化関数
void GameScene_Update();

void ResultScene_Initialize(); // リザルト画面初期化
void ResultScene_Update();     // リザルト画面更新

void EndScene();
void WaitForFade(SCENE_ID nextSceneID);

//*****************************************************************************
// 関数の定義　ここから　↓
//*****************************************************************************

BOOL Game_Initialize()
{
	HRESULT hr;

	// 頂点バッファ作成
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = VERTEX_BUFFER_SIZE;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	hr = Direct3D_GetDevice()->CreateBuffer(&bufferDesc, NULL, &gpVertexBuffer);

	if (FAILED(hr))
		return FALSE;
	// ↑　頂点バッファ作成　ここまで

	// ここで使用したい画像ファイルをテクスチャとして読み込む
	// 第一引数：画像ファイル名。もしフォルダに入っているならフォルダ名も一緒に書く。
	// 第二引数：読み込んだテクスチャが入る変数を指定
	hr = Direct3D_LoadTexture("assets/texture.png", &gpTexture);

	if (FAILED(hr)) {
		return FALSE;
	}

	// ゲーム時間の初期化をし、FPSを60に設定した。
	GameTimer_Initialize(60);

	srand(GetTickCount());

	ObjectGenerator_SetFadePanel(gpFadePanel); // フェード用スプライト
	gpFadePanel->color = { 0,0,0,0 };

//	GameScene_Initialize(); // ゲーム画面の初期化
	TitleScene_Initialize(); // タイトル画面の初期化
	gCurrentSceneID = SCENE_ID_TITLE;

	return TRUE;
}


// ゲームループ中に実行したい、ゲームの処理を書く関数
void Game_Update()
{
	Input_Update();  // このゲームで使うキーの押下状態を調べて保存

	// 全オブジェクトの更新処理を行う　→　アニメーション進める・移動する、など
	for (int i = 0; i < MAX_OBJECT; i++) {	
		GameObject_Update(&gObjects[i]);
	}

//	GameScene_Update(); // ゲーム画面の更新処理
//	TitleScene_Update(); // タイトル画面の更新処理
	switch (gCurrentSceneID) // 画面IDで更新処理を分岐させる
	{
	case SCENE_ID_TITLE_START: // タイトル画面開始
		TitleScene_Initialize();
		gCurrentSceneID = SCENE_ID_TITLE;
		break;

	case SCENE_ID_TITLE: // タイトル画面
	case SCENE_ID_TITLE_END:
		TitleScene_Update(); // タイトル画面の更新処理
		break;

	case SCENE_ID_GAME_START: // ゲーム画面開始
		GameScene_Initialize();
		gCurrentSceneID = SCENE_ID_GAME;
		break;

	case SCENE_ID_GAME: // ゲーム画面
	case SCENE_ID_GAME_END:
	case SCENE_ID::GAME_OVER_DEMO: // 勇者死亡デモ
		GameScene_Update(); // ゲーム画面の更新処理
		break;

	case SCENE_ID::RESULT_START: // リザルト画面開始
		ResultScene_Initialize();
		gCurrentSceneID = SCENE_ID::RESULT;
		break;

	case SCENE_ID::RESULT: // リザルト画面
	case SCENE_ID::RESULT_END:
		ResultScene_Update(); // リザルト画面の更新処理
		break;
	}

	switch (gFadeStatusID)  // フェードの状態IDで処理を分岐
	{
	case FADE_ID_IN:
		gpFadePanel->color.a -= GameTimer_GetDeltaTime(); // 徐々に透明にする
		if (gpFadePanel->color.a < 0.0f) {
			gFadeStatusID = FADE_ID_NO; // フェードなし
			gpFadePanel->color.a = 0.0f;
		}
		break;

	case FADE_ID_OUT:
		gpFadePanel->color.a += GameTimer_GetDeltaTime(); // 徐々に不透明にする
		if (gpFadePanel->color.a > 1.0f) {
			gFadeStatusID = FADE_ID_NO; // フェードなし
			gpFadePanel->color.a = 1.0f;
		}
		break;
	}

	Game_UpdateVertexBuffer();
}


void Game_Draw()
{
	// 画面クリア（指定色で塗りつぶし）
	// 塗りつぶし色の指定（小数：0.0f～1.0f）
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; //red,green,blue,alpha

	Direct3D_GetContext()->ClearRenderTargetView(Direct3D_GetRenderTargetView(), clearColor);

	// ↓　自前の描画処理をここに書く *******

	// 描画で使用するテクスチャを指定する関数呼び出し
	// ここで指定されたテクスチャが、ピクセルシェーダーのグローバル変数にセットされる
	Direct3D_GetContext()->PSSetShaderResources(0, 1, &gpTexture);

	// 描画に使う頂点バッファを指定する
	UINT strides = sizeof(VERTEX_POSTEX);
	UINT offsets = 0;
	Direct3D_GetContext()->IASetVertexBuffers(0, 1, &gpVertexBuffer, &strides, &offsets);   // gpVertexBufferが頂点バッファ

	// これまでの設定で実際に描画する  第一引数：実際に描画する頂点数
	Direct3D_GetContext()->Draw(MAX_SPRITE*VERTEX_PER_SPRITE, 0);

	// ↑　自前の描画処理をここに書く *******

	// ダブル・バッファのディスプレイ領域へのコピー命令
	Direct3D_GetSwapChain()->Present(0, 0);
}

void Game_Relese()
{
	COM_SAFE_RELEASE(gpTexture);  // テクスチャを読み込んだら、忘れずリリースすること
	COM_SAFE_RELEASE(gpVertexBuffer); // 頂点バッファを作成したら、忘れずにリリースすること
}

void Game_UseWindShot(GameObject * pUser)
{
	gpMagic->posY = pUser->posY; // 魔法のY座標に使用者のY座標をコピー

	if (pUser->charController.direction == DIR_LEFT) { // 使用者が左向きなら
		gpMagic->posX = pUser->posX - 0.05f; // 使用者の少し左に魔法を配置
		gpMagic->charController.moveSpeedX = -gpMagic->charController.stopForceX*1.5f;
	}
	else { // 使用者が右向きなら
		gpMagic->posX = pUser->posX + 0.05f; // 使用者の少し右に魔法を配置
		gpMagic->charController.moveSpeedX = gpMagic->charController.stopForceX*1.5f;
	}

	Action_Set(&pUser->action, ACTION_MAGIC); // 使用者：魔法使用アクション開始
	Action_Set(&gpMagic->action, ACTION_FX_WINDSHOT); // ウインドショットのアクション開始
}

void Game_UseSwordAttack(GameObject * pUser)
{
	// 剣のエフェクトを使ったキャラが所有している状態にする
	gpSword->pOwner = pUser;

	if (pUser->charController.direction == DIR_RIGHT) { // プレイヤーが右向き
		gpSword->animator.flip = FLIP_NONE; // 画像反転しない
		gpSword->localPosX = 0.05f; // 剣エフェクトのローカル座標を使用者の少し右に設定
	}
	else { // プレイヤーが左向き
		gpSword->animator.flip = FLIP_HORIZONTAL; // 画像を水平反転
		gpSword->localPosX = -0.05f; // 剣エフェクトのローカル座標を使用者の少し左に設定
	}
	// 剣エフェクトのローカル座標を設定

	gpSword->localPosY = 0.05f; // 剣エフェクトのローカル座標を使用者の少し上に設定

	Action_Set(&pUser->action, ACTION_ATTACK1); // 使用者：攻撃アクション開始
	Action_Set(&gpSword->action, ACTION_FX_SWORD); // 剣エフェクトのアクション開始
}

void Game_UpdateVertexBuffer()
{
	// ポリゴンの頂点を定義
	// 頂点を結んでポリゴンを形成するときの法則
	// ・頂点は、配列の番号の若い順番に使用される
	// ・頂点を結んでポリゴンが形成されたとき、その結び順が時計回りになる場合だけポリゴンが表示される
	VERTEX_POSTEX vx[MAX_SPRITE*VERTEX_PER_SPRITE] = { 0 };

	// オブジェクト配列のXY計算、UV計算、頂点配列への適用を一括処理
	for (int i = 0; i < MAX_OBJECT; i++) {
		// XY計算
		FRECT xy = GameObject_GetXY(&gObjects[i]);
		// UV計算
		FRECT uv = GameObject_GetUV(gObjects + i);
		// 頂点配列への適用
		vx[0 + i * 6 + 0] = {
			xy.left, xy.top, 0,
			gObjects[i].color.r, gObjects[i].color.g, gObjects[i].color.b, gObjects[i].color.a,
			uv.left, uv.top };  // １つめの頂点   ①左上
		vx[0 + i * 6 + 1] = {
			xy.right, xy.top, 0,
			gObjects[i].color.r, gObjects[i].color.g, gObjects[i].color.b, gObjects[i].color.a,
			uv.right, uv.top
		};  // ２つめの頂点　②右上
		vx[0 + i * 6 + 2] = {
			xy.right, xy.bottom, 0,
			gObjects[i].color.r, gObjects[i].color.g, gObjects[i].color.b, gObjects[i].color.a,
			uv.right, uv.bottom };  // ３つめの頂点　③右下

		vx[0 + i * 6 + 3] = {
			xy.right,  xy.bottom, 0,
			gObjects[i].color.r, gObjects[i].color.g, gObjects[i].color.b, gObjects[i].color.a,
			uv.right, uv.bottom };  // ４つめの頂点　③右下
		vx[0 + i * 6 + 4] = {
			xy.left,  xy.bottom, 0,
			gObjects[i].color.r, gObjects[i].color.g, gObjects[i].color.b, gObjects[i].color.a,
			uv.left, uv.bottom };  // ５つめの頂点　④左下
		vx[0 + i * 6 + 5] = {
			xy.left,  xy.top, 0,
			gObjects[i].color.r, gObjects[i].color.g, gObjects[i].color.b, gObjects[i].color.a,
			uv.left, uv.top };  // ６つめの頂点　　  ①左上　　  ①左上
	}

	// 頂点バッファのデータをvx配列のデータで更新する
	Direct3D_GetContext()->UpdateSubresource(
		gpVertexBuffer,		// 更新対象のバッファ
		0,				// インデックス(0)
		NULL,				// 更新範囲(nullptr)
		vx,		// 反映データ
		0,				// データの1行のサイズ(0)
		0);				// 1深度スライスのサイズ(0)
}

void TitleScene_Initialize()
{
	// HPゲージを消す
	gpHpGauge->sizeX = 0;
	gpHpGaugeBase->sizeX = 0;
	// ゲームオブジェクトをフェード以外クリアしたい場合
	for (GameObject* p = gObjects + 0; p < gpFadePanel; p++) {
		ZeroMemory(p, sizeof(GameObject));
	}

	ObjectGenerator_SetBG(&gObjects[0]);
	gObjects[0].color = { 0.5f,0.5f,0.5f,1 };

	ObjectGenerator_SetTitleLogo(gObjects + 1);
	gObjects[1].color = {1,1,1,1};
	ObjectGenerator_SetPressEnter(gObjects + 2);
	gObjects[2].color = { 1,1,1,1 };

}

void TitleScene_Update()
{
	// エンターキーが押されたらゲーム画面に移る
	if (Input_GetKeyTrigger(VK_RETURN) && gFadeStatusID == FADE_ID_NO)
	{
		EndScene();

		gCurrentStageID = 0; // ステージクリア状況をリセット
	}

	if (gCurrentSceneID == SCENE_ID_TITLE_END) { // タイトル画面終了中
		WaitForFade(SCENE_ID_GAME_START);
	}
}

void GameScene_Initialize()
{
	// 配列０番に背景スプライトをセットする
	ObjectGenerator_SetBG(&gObjects[0]);
	gObjects[0].color = { 1,1,1,1 };  // RGBAを表す４つのfloat値

	// プレイヤー（配列１番）に勇者スプライトをセットする
	ObjectGenerator_Character32x32(gpPlayer, 0);
	gpPlayer->posX = -0.3f;
	gpPlayer->posY = -0.5f;
	gpPlayer->ai.id = AI_ID_PLAYER; // AIのIDをプレイヤーにする
	gpPlayer->color = { 1,1,1,1 };
	gpPlayer->hp = gpPlayer->maxHp = 100.0f;

	// ウインドショット初期化
	ObjectGenerator_SetFx(gpMagic, 0); // 第二引数が０ならウインドショット
	gpMagic->posX = 1000;
	gpMagic->color = { 1,1,1,1 };
	// 剣エフェクト初期化
	ObjectGenerator_SetFx(gpSword, 1); // 第二引数が１なら剣エフェクト
	gpSword->posX = 1000;
	gpSword->color = { 1,1,1,1 };

	// HPゲージ初期化
	ObjectGenerator_SetHpGauge(gpHpGauge, gpHpGaugeBase);

	ObjectGenerator_SetStageStart(gpStageStart);

	gStageClear = false;
	gNumKilledEnemy = 0;

	// クエストデータ取得と設定
	gpCurrentQuest = QuestMaster_GetQuest(gCurrentQuestID);
	const Stage* pStage = QuestMaster_GetStage(gCurrentQuestID, gCurrentStageID);

	gNumStageEnemy = pStage->numEnemy;

	for (int i = 0; i < gNumStageEnemy; i++) {
		auto e = gpEnemy[i];
		auto pEnemyMaster = pStage->enemies+i;

		ObjectGenerator_Character32x32(e, pEnemyMaster->type.spriteID);
		e->posX = pEnemyMaster->posX;
		e->posY = -0.5f;
		e->ai.id = pEnemyMaster->type.aiID;
		e->color = { 1,1,1,1 };
		e->hp = e->maxHp = pEnemyMaster->type.hp;
		e->charController.direction = DIR_LEFT;
	}
}

void GameScene_Update()
{
	if (gCurrentSceneID == SCENE_ID_GAME) {
		COLLIDER_RESULT res;

		// CharControllerへのアクセスを短く書けるようにポインタ変数を宣言する
		CharController* pPlayerCtrl = &gpPlayer->charController;

		for (int i = 0; i < gNumStageEnemy; i++) {
			auto e = gpEnemy[i];
			CharController* pEnemyCtrl = &e->charController;

			// プレイヤーと敵との当たり判定
			res = Collider_Test(&gpPlayer->collider, &e->collider);

			// 衝突状態なら
			if (res.isHit) {
				// 押し戻し処理
				float absPlayerSpeedX = fabsf(pPlayerCtrl->moveSpeedX);
				float absEnemySpeedX = fabsf(pEnemyCtrl->moveSpeedX);
				float totalSpeedX = absPlayerSpeedX + absEnemySpeedX;

				if (totalSpeedX != 0.0f) {  // ゼロ除算防止
					// 現在の移動速度に応じてプレイヤーと敵をそれぞれX方向に押し戻し
					gpPlayer->posX += res.dx * absPlayerSpeedX / totalSpeedX;
					e->posX += -res.dx * absEnemySpeedX / totalSpeedX;
				}
				else {
					gpPlayer->posX += res.dx;
				}

				gpPlayer->posY += res.dy; // プレイヤーをY方向に押し戻し
			}

			// 魔法と敵との当たり判定
			res = Collider_Test(&gpMagic->collider, &e->collider);

			if (res.isHit) {
				CharController_Accelerate(&e->charController, ACCEL_RIGHT);
				e->hp -= 0.5f;
			}

			// 剣と敵との当たり判定
			res = Collider_Test(&gpSword->collider, &e->collider);

			if (res.isHit && gpSword->pOwner == gpPlayer) {
				CharController_Accelerate(&e->charController, ACCEL_RIGHT);
				e->hp -= 1.0f;
			}

			// 敵が死んだ
			if (e->hp <= 0 && e->collider.isActive == true) {
				//e->color.a = 0.0f;
				Action_Set(&e->action, ACTION_KILLED);
				e->collider.isActive = false;
				e->ai.id = AI_ID_NONE;
				gNumKilledEnemy++;
			}
		}

		if (gNumKilledEnemy >= gNumStageEnemy) {  // ステージの敵を全部倒した
			//gCurrentSceneID = SCENE_ID::RESULT_START;
			EndScene();
			gStageClear = true;
		}

		// 剣とプレイヤーとの当たり判定
		res = Collider_Test(&gpSword->collider, &gpPlayer->collider);

		if (res.isHit && gpSword->pOwner != gpPlayer) {
			// 当たってるときの処理
			CharController_Accelerate(&gpPlayer->charController, ACCEL_LEFT);
			gpPlayer->hp -= 1.0f;
			// ゲージの色を赤くする
			gpHpGauge->color = { 1,0,0,1 };
		}

		// プレイヤーのＨＰが０以下なら、デモ開始
		if (gpPlayer->hp <= 0.0f)
		{
			gpFadePanel->color = { 1,0,0,1 };
			gFadeStatusID = FADE_ID_IN;
			gCurrentSceneID = SCENE_ID::GAME_OVER_DEMO;
			gpPlayer->color.a = 0.0f; // 勇者を消す
		}
	}
	else if (gCurrentSceneID == SCENE_ID::GAME_OVER_DEMO) {
		// デモが終わったらリザルト画面へ
		if (gFadeStatusID == FADE_ID_NO)
		{
			gCurrentSceneID = SCENE_ID_GAME;
			EndScene();
		}
	}
	else if (gCurrentSceneID == SCENE_ID_GAME_END) {
		WaitForFade(SCENE_ID::RESULT_START);
	}

	// エンターキーが押されたらリザルト画面に戻る
	if (Input_GetKeyTrigger(VK_RETURN))
	{
		gCurrentSceneID = SCENE_ID::RESULT_START; //リザルト画面へ
		gStageClear = true;
	}

	// HPゲージの長さ変更
	gpHpGauge->sizeX = 1.0f * gpPlayer->hp / gpPlayer->maxHp;
	gpHpGauge->posX = (1.0f - gpHpGauge->sizeX) / 2.0f * -1.0f;

	// ゲージの色が通常色でなければ、通常色に近づける
	if (gpHpGauge->color.g < 1.0f) { // ゲージの緑成分が1.0より小さい
		gpHpGauge->color.r -= 0.1f;
		gpHpGauge->color.g += 0.1f;
	}
}

void ResultScene_Initialize()
{
	// ゲームオブジェクトをフェード以外クリアしたい場合
	for (GameObject* p = gObjects + 0; p < gpFadePanel; p++) {
		ZeroMemory(p, sizeof(GameObject));
	}

	if (gStageClear) {
		ObjectGenerator_SetStageClear(gObjects + 0);
	}
	else {
		ObjectGenerator_SetGameOver(gObjects + 0);
	}

	auto pPressEnter = gObjects + 1;
	ObjectGenerator_SetPressEnter(pPressEnter);
//	Action_Set(&pPressEnter->action, ACTION_BLINK);

}

void ResultScene_Update()
{
	if (Input_GetKeyTrigger(VK_RETURN))
	{
		EndScene();
		gCurrentStageID++;
	}

	if (gCurrentSceneID == SCENE_ID::RESULT_END) {
		SCENE_ID next;

		if (gStageClear) {
			if (gCurrentStageID >= gpCurrentQuest->numStage) {
				next = SCENE_ID_TITLE_START;
			}
			else {
				next = SCENE_ID_GAME_START;
			}
		}
		else {
			next = SCENE_ID_TITLE_START;
		}

		WaitForFade(next);
	}
}

void EndScene()
{
	gCurrentSceneID = (SCENE_ID)(gCurrentSceneID+1);
	gFadeStatusID = FADE_ID_OUT;
	gpFadePanel->color.r = 0;
	gpFadePanel->color.g = 0;
	gpFadePanel->color.b = 0;
}

void WaitForFade(SCENE_ID nextSceneID)
{
	if (gFadeStatusID == FADE_ID_NO) { // フェードなし
		gCurrentSceneID = nextSceneID; // 
		gFadeStatusID = FADE_ID_IN; // フェードイン開始
	}
}

//*****************************************************************************
// DX21�@�T���v���\�[�X
// �Q�[���̃��C���ƂȂ鏈���������Ă����\�[�X�t�@�C��
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
// �\���̒�`
//*****************************************************************************

// ���_�P�������\���\����
struct VERTEX_POSTEX {
	float x, y, z;  // ���_�̈ʒu
	float r, g, b, a;  // ���_�̐F�i��, ��, ��, �����x�j
	float u, v;  // �e�N�X�`����UV���W
};


//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define MAX_SPRITE  64
// �|���S���Q�łP�̎l�p�`�i�X�v���C�g�j�B�|���S���͂R���_�Ȃ̂ŁA�P�X�v���C�g�͂U���_�B
#define VERTEX_PER_SPRITE  (3*2)
#define VERTEX_BUFFER_SIZE  (MAX_SPRITE*sizeof(VERTEX_POSTEX)*VERTEX_PER_SPRITE)


// �Q�[���I�u�W�F�N�g�ő吔
#define MAX_OBJECT   20

// ��ʁi�V�[���j�̒萔
enum SCENE_ID
{
	SCENE_ID_TITLE_START, // �^�C�g����ʊJ�n
	SCENE_ID_TITLE, // �^�C�g����ʂ�ID
	SCENE_ID_TITLE_END, // �^�C�g����ʏI����

	SCENE_ID_GAME_START, // �Q�[����ʊJ�n
	SCENE_ID_GAME,  // �Q�[����ʂ�ID
	SCENE_ID_GAME_END,

	GAME_OVER_DEMO,  // �v���C���[���S���̃f��

	RESULT_START, RESULT, RESULT_END,  // ���U���g��ʂ�ID
};

// �t�F�[�h�̏��ID�萔
enum FADE_ID
{
	FADE_ID_NO,  // �t�F�[�h�Ȃ�
	FADE_ID_IN,  // �t�F�[�h�C��
	FADE_ID_OUT, // �t�F�[�h�A�E�g
};

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

ID3D11Buffer* gpVertexBuffer;  // ���_�o�b�t�@�p�̕ϐ�

ID3D11ShaderResourceView* gpTexture; // �e�N�X�`���p�ϐ�

GameObject gObjects[MAX_OBJECT];  // �I�u�W�F�N�g�z��

GameObject* gpPlayer = gObjects + 1;
GameObject* gpEnemy[MAX_STAGE_ENEMY] = { gObjects+2, gObjects+3, gObjects+4, gObjects+5, gObjects+6 };
GameObject* gpMagic = gObjects + 10;
GameObject* gpSword = gObjects + 11;

GameObject* gpFadePanel = gObjects + MAX_OBJECT - 1;
GameObject* gpHpGauge = gpFadePanel - 1;  // HP�Q�[�W
GameObject* gpHpGaugeBase = gpHpGauge - 1; // HP�Q�[�W���n
GameObject* gpStageStart = gpHpGaugeBase - 1; // Stage Start�̕���

SCENE_ID gCurrentSceneID;  // ���݂̉�ʁi�V�[���jID�������Ă�ϐ�

FADE_ID gFadeStatusID; // ���݂̃t�F�[�h�̎�ނ����ϐ�

bool gStageClear;
int gNumKilledEnemy;
int gNumStageEnemy;

int gCurrentQuestID = 2;
int gCurrentStageID;
const Quest* gpCurrentQuest;

//*****************************************************************************
// �֐��̃v���g�^�C�v�錾�@�i����game.cpp���ł̂ݎg���֐��j
//*****************************************************************************

void TitleScene_Initialize();  // �^�C�g����ʂ̏������֐�
void TitleScene_Update();
void GameScene_Initialize();  // �Q�[����ʂ̏������֐�
void GameScene_Update();

void ResultScene_Initialize(); // ���U���g��ʏ�����
void ResultScene_Update();     // ���U���g��ʍX�V

void EndScene();
void WaitForFade(SCENE_ID nextSceneID);

//*****************************************************************************
// �֐��̒�`�@��������@��
//*****************************************************************************

BOOL Game_Initialize()
{
	HRESULT hr;

	// ���_�o�b�t�@�쐬
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
	// ���@���_�o�b�t�@�쐬�@�����܂�

	// �����Ŏg�p�������摜�t�@�C�����e�N�X�`���Ƃ��ēǂݍ���
	// �������F�摜�t�@�C�����B�����t�H���_�ɓ����Ă���Ȃ�t�H���_�����ꏏ�ɏ����B
	// �������F�ǂݍ��񂾃e�N�X�`��������ϐ����w��
	hr = Direct3D_LoadTexture("assets/texture.png", &gpTexture);

	if (FAILED(hr)) {
		return FALSE;
	}

	// �Q�[�����Ԃ̏����������AFPS��60�ɐݒ肵���B
	GameTimer_Initialize(60);

	srand(GetTickCount());

	ObjectGenerator_SetFadePanel(gpFadePanel); // �t�F�[�h�p�X�v���C�g
	gpFadePanel->color = { 0,0,0,0 };

//	GameScene_Initialize(); // �Q�[����ʂ̏�����
	TitleScene_Initialize(); // �^�C�g����ʂ̏�����
	gCurrentSceneID = SCENE_ID_TITLE;

	return TRUE;
}


// �Q�[�����[�v���Ɏ��s�������A�Q�[���̏����������֐�
void Game_Update()
{
	Input_Update();  // ���̃Q�[���Ŏg���L�[�̉�����Ԃ𒲂ׂĕۑ�

	// �S�I�u�W�F�N�g�̍X�V�������s���@���@�A�j���[�V�����i�߂�E�ړ�����A�Ȃ�
	for (int i = 0; i < MAX_OBJECT; i++) {	
		GameObject_Update(&gObjects[i]);
	}

//	GameScene_Update(); // �Q�[����ʂ̍X�V����
//	TitleScene_Update(); // �^�C�g����ʂ̍X�V����
	switch (gCurrentSceneID) // ���ID�ōX�V�����𕪊򂳂���
	{
	case SCENE_ID_TITLE_START: // �^�C�g����ʊJ�n
		TitleScene_Initialize();
		gCurrentSceneID = SCENE_ID_TITLE;
		break;

	case SCENE_ID_TITLE: // �^�C�g�����
	case SCENE_ID_TITLE_END:
		TitleScene_Update(); // �^�C�g����ʂ̍X�V����
		break;

	case SCENE_ID_GAME_START: // �Q�[����ʊJ�n
		GameScene_Initialize();
		gCurrentSceneID = SCENE_ID_GAME;
		break;

	case SCENE_ID_GAME: // �Q�[�����
	case SCENE_ID_GAME_END:
	case SCENE_ID::GAME_OVER_DEMO: // �E�Ҏ��S�f��
		GameScene_Update(); // �Q�[����ʂ̍X�V����
		break;

	case SCENE_ID::RESULT_START: // ���U���g��ʊJ�n
		ResultScene_Initialize();
		gCurrentSceneID = SCENE_ID::RESULT;
		break;

	case SCENE_ID::RESULT: // ���U���g���
	case SCENE_ID::RESULT_END:
		ResultScene_Update(); // ���U���g��ʂ̍X�V����
		break;
	}

	switch (gFadeStatusID)  // �t�F�[�h�̏��ID�ŏ����𕪊�
	{
	case FADE_ID_IN:
		gpFadePanel->color.a -= GameTimer_GetDeltaTime(); // ���X�ɓ����ɂ���
		if (gpFadePanel->color.a < 0.0f) {
			gFadeStatusID = FADE_ID_NO; // �t�F�[�h�Ȃ�
			gpFadePanel->color.a = 0.0f;
		}
		break;

	case FADE_ID_OUT:
		gpFadePanel->color.a += GameTimer_GetDeltaTime(); // ���X�ɕs�����ɂ���
		if (gpFadePanel->color.a > 1.0f) {
			gFadeStatusID = FADE_ID_NO; // �t�F�[�h�Ȃ�
			gpFadePanel->color.a = 1.0f;
		}
		break;
	}

	Game_UpdateVertexBuffer();
}


void Game_Draw()
{
	// ��ʃN���A�i�w��F�œh��Ԃ��j
	// �h��Ԃ��F�̎w��i�����F0.0f�`1.0f�j
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; //red,green,blue,alpha

	Direct3D_GetContext()->ClearRenderTargetView(Direct3D_GetRenderTargetView(), clearColor);

	// ���@���O�̕`�揈���������ɏ��� *******

	// �`��Ŏg�p����e�N�X�`�����w�肷��֐��Ăяo��
	// �����Ŏw�肳�ꂽ�e�N�X�`�����A�s�N�Z���V�F�[�_�[�̃O���[�o���ϐ��ɃZ�b�g�����
	Direct3D_GetContext()->PSSetShaderResources(0, 1, &gpTexture);

	// �`��Ɏg�����_�o�b�t�@���w�肷��
	UINT strides = sizeof(VERTEX_POSTEX);
	UINT offsets = 0;
	Direct3D_GetContext()->IASetVertexBuffers(0, 1, &gpVertexBuffer, &strides, &offsets);   // gpVertexBuffer�����_�o�b�t�@

	// ����܂ł̐ݒ�Ŏ��ۂɕ`�悷��  �������F���ۂɕ`�悷�钸�_��
	Direct3D_GetContext()->Draw(MAX_SPRITE*VERTEX_PER_SPRITE, 0);

	// ���@���O�̕`�揈���������ɏ��� *******

	// �_�u���E�o�b�t�@�̃f�B�X�v���C�̈�ւ̃R�s�[����
	Direct3D_GetSwapChain()->Present(0, 0);
}

void Game_Relese()
{
	COM_SAFE_RELEASE(gpTexture);  // �e�N�X�`����ǂݍ��񂾂�A�Y�ꂸ�����[�X���邱��
	COM_SAFE_RELEASE(gpVertexBuffer); // ���_�o�b�t�@���쐬������A�Y�ꂸ�Ƀ����[�X���邱��
}

void Game_UseWindShot(GameObject * pUser)
{
	gpMagic->posY = pUser->posY; // ���@��Y���W�Ɏg�p�҂�Y���W���R�s�[

	if (pUser->charController.direction == DIR_LEFT) { // �g�p�҂��������Ȃ�
		gpMagic->posX = pUser->posX - 0.05f; // �g�p�҂̏������ɖ��@��z�u
		gpMagic->charController.moveSpeedX = -gpMagic->charController.stopForceX*1.5f;
	}
	else { // �g�p�҂��E�����Ȃ�
		gpMagic->posX = pUser->posX + 0.05f; // �g�p�҂̏����E�ɖ��@��z�u
		gpMagic->charController.moveSpeedX = gpMagic->charController.stopForceX*1.5f;
	}

	Action_Set(&pUser->action, ACTION_MAGIC); // �g�p�ҁF���@�g�p�A�N�V�����J�n
	Action_Set(&gpMagic->action, ACTION_FX_WINDSHOT); // �E�C���h�V���b�g�̃A�N�V�����J�n
}

void Game_UseSwordAttack(GameObject * pUser)
{
	// ���̃G�t�F�N�g���g�����L���������L���Ă����Ԃɂ���
	gpSword->pOwner = pUser;

	if (pUser->charController.direction == DIR_RIGHT) { // �v���C���[���E����
		gpSword->animator.flip = FLIP_NONE; // �摜���]���Ȃ�
		gpSword->localPosX = 0.05f; // ���G�t�F�N�g�̃��[�J�����W���g�p�҂̏����E�ɐݒ�
	}
	else { // �v���C���[��������
		gpSword->animator.flip = FLIP_HORIZONTAL; // �摜�𐅕����]
		gpSword->localPosX = -0.05f; // ���G�t�F�N�g�̃��[�J�����W���g�p�҂̏������ɐݒ�
	}
	// ���G�t�F�N�g�̃��[�J�����W��ݒ�

	gpSword->localPosY = 0.05f; // ���G�t�F�N�g�̃��[�J�����W���g�p�҂̏�����ɐݒ�

	Action_Set(&pUser->action, ACTION_ATTACK1); // �g�p�ҁF�U���A�N�V�����J�n
	Action_Set(&gpSword->action, ACTION_FX_SWORD); // ���G�t�F�N�g�̃A�N�V�����J�n
}

void Game_UpdateVertexBuffer()
{
	// �|���S���̒��_���`
	// ���_������Ń|���S�����`������Ƃ��̖@��
	// �E���_�́A�z��̔ԍ��̎Ⴂ���ԂɎg�p�����
	// �E���_������Ń|���S�����`�����ꂽ�Ƃ��A���̌��я������v���ɂȂ�ꍇ�����|���S�����\�������
	VERTEX_POSTEX vx[MAX_SPRITE*VERTEX_PER_SPRITE] = { 0 };

	// �I�u�W�F�N�g�z���XY�v�Z�AUV�v�Z�A���_�z��ւ̓K�p���ꊇ����
	for (int i = 0; i < MAX_OBJECT; i++) {
		// XY�v�Z
		FRECT xy = GameObject_GetXY(&gObjects[i]);
		// UV�v�Z
		FRECT uv = GameObject_GetUV(gObjects + i);
		// ���_�z��ւ̓K�p
		vx[0 + i * 6 + 0] = {
			xy.left, xy.top, 0,
			gObjects[i].color.r, gObjects[i].color.g, gObjects[i].color.b, gObjects[i].color.a,
			uv.left, uv.top };  // �P�߂̒��_   �@����
		vx[0 + i * 6 + 1] = {
			xy.right, xy.top, 0,
			gObjects[i].color.r, gObjects[i].color.g, gObjects[i].color.b, gObjects[i].color.a,
			uv.right, uv.top
		};  // �Q�߂̒��_�@�A�E��
		vx[0 + i * 6 + 2] = {
			xy.right, xy.bottom, 0,
			gObjects[i].color.r, gObjects[i].color.g, gObjects[i].color.b, gObjects[i].color.a,
			uv.right, uv.bottom };  // �R�߂̒��_�@�B�E��

		vx[0 + i * 6 + 3] = {
			xy.right,  xy.bottom, 0,
			gObjects[i].color.r, gObjects[i].color.g, gObjects[i].color.b, gObjects[i].color.a,
			uv.right, uv.bottom };  // �S�߂̒��_�@�B�E��
		vx[0 + i * 6 + 4] = {
			xy.left,  xy.bottom, 0,
			gObjects[i].color.r, gObjects[i].color.g, gObjects[i].color.b, gObjects[i].color.a,
			uv.left, uv.bottom };  // �T�߂̒��_�@�C����
		vx[0 + i * 6 + 5] = {
			xy.left,  xy.top, 0,
			gObjects[i].color.r, gObjects[i].color.g, gObjects[i].color.b, gObjects[i].color.a,
			uv.left, uv.top };  // �U�߂̒��_�@�@  �@����@�@  �@����
	}

	// ���_�o�b�t�@�̃f�[�^��vx�z��̃f�[�^�ōX�V����
	Direct3D_GetContext()->UpdateSubresource(
		gpVertexBuffer,		// �X�V�Ώۂ̃o�b�t�@
		0,				// �C���f�b�N�X(0)
		NULL,				// �X�V�͈�(nullptr)
		vx,		// ���f�f�[�^
		0,				// �f�[�^��1�s�̃T�C�Y(0)
		0);				// 1�[�x�X���C�X�̃T�C�Y(0)
}

void TitleScene_Initialize()
{
	// HP�Q�[�W������
	gpHpGauge->sizeX = 0;
	gpHpGaugeBase->sizeX = 0;
	// �Q�[���I�u�W�F�N�g���t�F�[�h�ȊO�N���A�������ꍇ
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
	// �G���^�[�L�[�������ꂽ��Q�[����ʂɈڂ�
	if (Input_GetKeyTrigger(VK_RETURN) && gFadeStatusID == FADE_ID_NO)
	{
		EndScene();

		gCurrentStageID = 0; // �X�e�[�W�N���A�󋵂����Z�b�g
	}

	if (gCurrentSceneID == SCENE_ID_TITLE_END) { // �^�C�g����ʏI����
		WaitForFade(SCENE_ID_GAME_START);
	}
}

void GameScene_Initialize()
{
	// �z��O�Ԃɔw�i�X�v���C�g���Z�b�g����
	ObjectGenerator_SetBG(&gObjects[0]);
	gObjects[0].color = { 1,1,1,1 };  // RGBA��\���S��float�l

	// �v���C���[�i�z��P�ԁj�ɗE�҃X�v���C�g���Z�b�g����
	ObjectGenerator_Character32x32(gpPlayer, 0);
	gpPlayer->posX = -0.3f;
	gpPlayer->posY = -0.5f;
	gpPlayer->ai.id = AI_ID_PLAYER; // AI��ID���v���C���[�ɂ���
	gpPlayer->color = { 1,1,1,1 };
	gpPlayer->hp = gpPlayer->maxHp = 100.0f;

	// �E�C���h�V���b�g������
	ObjectGenerator_SetFx(gpMagic, 0); // ���������O�Ȃ�E�C���h�V���b�g
	gpMagic->posX = 1000;
	gpMagic->color = { 1,1,1,1 };
	// ���G�t�F�N�g������
	ObjectGenerator_SetFx(gpSword, 1); // ���������P�Ȃ猕�G�t�F�N�g
	gpSword->posX = 1000;
	gpSword->color = { 1,1,1,1 };

	// HP�Q�[�W������
	ObjectGenerator_SetHpGauge(gpHpGauge, gpHpGaugeBase);

	ObjectGenerator_SetStageStart(gpStageStart);

	gStageClear = false;
	gNumKilledEnemy = 0;

	// �N�G�X�g�f�[�^�擾�Ɛݒ�
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

		// CharController�ւ̃A�N�Z�X��Z��������悤�Ƀ|�C���^�ϐ���錾����
		CharController* pPlayerCtrl = &gpPlayer->charController;

		for (int i = 0; i < gNumStageEnemy; i++) {
			auto e = gpEnemy[i];
			CharController* pEnemyCtrl = &e->charController;

			// �v���C���[�ƓG�Ƃ̓����蔻��
			res = Collider_Test(&gpPlayer->collider, &e->collider);

			// �Փˏ�ԂȂ�
			if (res.isHit) {
				// �����߂�����
				float absPlayerSpeedX = fabsf(pPlayerCtrl->moveSpeedX);
				float absEnemySpeedX = fabsf(pEnemyCtrl->moveSpeedX);
				float totalSpeedX = absPlayerSpeedX + absEnemySpeedX;

				if (totalSpeedX != 0.0f) {  // �[�����Z�h�~
					// ���݂̈ړ����x�ɉ����ăv���C���[�ƓG�����ꂼ��X�����ɉ����߂�
					gpPlayer->posX += res.dx * absPlayerSpeedX / totalSpeedX;
					e->posX += -res.dx * absEnemySpeedX / totalSpeedX;
				}
				else {
					gpPlayer->posX += res.dx;
				}

				gpPlayer->posY += res.dy; // �v���C���[��Y�����ɉ����߂�
			}

			// ���@�ƓG�Ƃ̓����蔻��
			res = Collider_Test(&gpMagic->collider, &e->collider);

			if (res.isHit) {
				CharController_Accelerate(&e->charController, ACCEL_RIGHT);
				e->hp -= 0.5f;
			}

			// ���ƓG�Ƃ̓����蔻��
			res = Collider_Test(&gpSword->collider, &e->collider);

			if (res.isHit && gpSword->pOwner == gpPlayer) {
				CharController_Accelerate(&e->charController, ACCEL_RIGHT);
				e->hp -= 1.0f;
			}

			// �G������
			if (e->hp <= 0 && e->collider.isActive == true) {
				//e->color.a = 0.0f;
				Action_Set(&e->action, ACTION_KILLED);
				e->collider.isActive = false;
				e->ai.id = AI_ID_NONE;
				gNumKilledEnemy++;
			}
		}

		if (gNumKilledEnemy >= gNumStageEnemy) {  // �X�e�[�W�̓G��S���|����
			//gCurrentSceneID = SCENE_ID::RESULT_START;
			EndScene();
			gStageClear = true;
		}

		// ���ƃv���C���[�Ƃ̓����蔻��
		res = Collider_Test(&gpSword->collider, &gpPlayer->collider);

		if (res.isHit && gpSword->pOwner != gpPlayer) {
			// �������Ă�Ƃ��̏���
			CharController_Accelerate(&gpPlayer->charController, ACCEL_LEFT);
			gpPlayer->hp -= 1.0f;
			// �Q�[�W�̐F��Ԃ�����
			gpHpGauge->color = { 1,0,0,1 };
		}

		// �v���C���[�̂g�o���O�ȉ��Ȃ�A�f���J�n
		if (gpPlayer->hp <= 0.0f)
		{
			gpFadePanel->color = { 1,0,0,1 };
			gFadeStatusID = FADE_ID_IN;
			gCurrentSceneID = SCENE_ID::GAME_OVER_DEMO;
			gpPlayer->color.a = 0.0f; // �E�҂�����
		}
	}
	else if (gCurrentSceneID == SCENE_ID::GAME_OVER_DEMO) {
		// �f�����I������烊�U���g��ʂ�
		if (gFadeStatusID == FADE_ID_NO)
		{
			gCurrentSceneID = SCENE_ID_GAME;
			EndScene();
		}
	}
	else if (gCurrentSceneID == SCENE_ID_GAME_END) {
		WaitForFade(SCENE_ID::RESULT_START);
	}

	// �G���^�[�L�[�������ꂽ�烊�U���g��ʂɖ߂�
	if (Input_GetKeyTrigger(VK_RETURN))
	{
		gCurrentSceneID = SCENE_ID::RESULT_START; //���U���g��ʂ�
		gStageClear = true;
	}

	// HP�Q�[�W�̒����ύX
	gpHpGauge->sizeX = 1.0f * gpPlayer->hp / gpPlayer->maxHp;
	gpHpGauge->posX = (1.0f - gpHpGauge->sizeX) / 2.0f * -1.0f;

	// �Q�[�W�̐F���ʏ�F�łȂ���΁A�ʏ�F�ɋ߂Â���
	if (gpHpGauge->color.g < 1.0f) { // �Q�[�W�̗ΐ�����1.0��菬����
		gpHpGauge->color.r -= 0.1f;
		gpHpGauge->color.g += 0.1f;
	}
}

void ResultScene_Initialize()
{
	// �Q�[���I�u�W�F�N�g���t�F�[�h�ȊO�N���A�������ꍇ
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
	if (gFadeStatusID == FADE_ID_NO) { // �t�F�[�h�Ȃ�
		gCurrentSceneID = nextSceneID; // 
		gFadeStatusID = FADE_ID_IN; // �t�F�[�h�C���J�n
	}
}

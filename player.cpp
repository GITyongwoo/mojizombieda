//=============================================================================
//
// ���f������ [player.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "model.h"
#include "player.h"
#include "shadow.h"
#include "light.h"
#include "bullet.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_PLAYER		"data/MODEL/jibunn.obj"			// �ǂݍ��ރ��f����

#define	VALUE_MOVE			(2.0f)							// �ړ���
#define	VALUE_ROTATE		(XM_PI * 0.02f)					// ��]��

#define PLAYER_SHADOW_SIZE	(1.0f)							// �e�̑傫��
#define PLAYER_OFFSET_Y		(1.0f)							// �v���C���[�̑��������킹��

//#define PLAYER_PARTS_MAX	(2)								// �v���C���[�̃p�[�c�̐�


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static PLAYER		g_Player;						// �v���C���[

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	LoadModel(MODEL_PLAYER, &g_Player.model);
	g_Player.load = true;

	g_Player.pos = { 0.0f, PLAYER_OFFSET_Y, -20.0f };
	g_Player.rot = { 0.0f, XM_PI, 0.0f };
	g_Player.scl = { 1.0f, 1.0f, 1.0f };

	g_Player.spd = 0.0f;			// �ړ��X�s�[�h�N���A
	g_Player.size = PLAYER_SIZE;	// �����蔻��̑傫��

	g_Player.use = true;

	// �����Ńv���C���[�p�̉e���쐬���Ă���
	XMFLOAT3 pos = g_Player.pos;
	pos.y -= (PLAYER_OFFSET_Y - 0.1f);
	g_Player.shadowIdx = CreateShadow(pos, PLAYER_SHADOW_SIZE, PLAYER_SHADOW_SIZE);
	//          ��
	//        ���̃����o�[�ϐ������������e��Index�ԍ�



	// �K�w�A�j���[�V�����p�̏���������
	g_Player.parent = NULL;			// �{�́i�e�j�Ȃ̂�NULL������

	// �p�[�c�̏�����
	//for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	//{
	//	g_Parts[i].use = true;

	//	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	//	g_Parts[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//	g_Parts[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//	g_Parts[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

	//	// �e�q�֌W
	//	g_Parts[i].parent = &g_Player;		// �� �����ɐe�̃A�h���X������
	////	g_Parts[�r].parent= &g_Player;		// �r��������e�͖{�́i�v���C���[�j
	////	g_Parts[��].parent= &g_Parts[�r];	// �w���r�̎q���������ꍇ�̗�

	//	// �K�w�A�j���[�V�����p�̃����o�[�ϐ��̏�����
	//	g_Parts[i].time = 0.0f;			// ���`��ԗp�̃^�C�}�[���N���A
	//	g_Parts[i].tblNo = 0;			// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
	//	g_Parts[i].tblMax = 0;			// �Đ�����s���f�[�^�e�[�u���̃��R�[�h�����Z�b�g

	//	// �p�[�c�̓ǂݍ��݂͂܂����Ă��Ȃ�
	//	g_Parts[i].load = 0;
	//}

	//g_Parts[0].tblNo = 0;			// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
	//g_Parts[0].tblMax = sizeof(move_tbl_right) / sizeof(INTERPOLATION_DATA);			// �Đ�����s���f�[�^�e�[�u���̃��R�[�h�����Z�b�g
	//LoadModel(MODEL_PLAYER_PARTS, &g_Parts[0].model);
	//g_Parts[0].load = 1;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	// ���f���̉������
	if (g_Player.load)
	{
		UnloadModel(&g_Player.model);
		g_Player.load = false;
	}


}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{

	CAMERA *cam = GetCamera();

	// �ړ��������Ⴄ
	if (GetKeyboardPress(DIK_LEFT))
	{	// ���ֈړ�
		g_Player.spd = VALUE_MOVE;	
		--g_Player.pos.x;
	}
	if (GetKeyboardPress(DIK_RIGHT))
	{	// �E�ֈړ�
		g_Player.spd = VALUE_MOVE;
		++g_Player.pos.x;
	}


#ifdef _DEBUG

#endif


	//	// Key���͂���������ړ���������
	//if (g_Player.spd > 0.0f)
	//{
	//	// ���͂̂����������փv���C���[���������Ĉړ�������
	//	++g_Player.pos.x;
	//	++g_Player.pos.;
	//}

	// �e���v���C���[�̈ʒu�ɍ��킹��
	XMFLOAT3 pos = g_Player.pos;
	pos.y -= (PLAYER_OFFSET_Y - 0.1f);
	SetPositionShadow(g_Player.shadowIdx, pos);

	// �e���ˏ���
	if (GetKeyboardTrigger(DIK_SPACE))
	{
		SetBullet(g_Player.pos, g_Player.rot);
		PlaySound(SOUND_LABEL_SE_shot000);
	}

	g_Player.spd *= 0.5f;


	// �K�w�A�j���[�V����
	//for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	//{
	//	// �g���Ă���Ȃ珈������
	//	if ((g_Parts[i].use == true)&&(g_Parts[i].tblMax > 0))
	//	{	// ���`��Ԃ̏���
	//		int nowNo = (int)g_Parts[i].time;			// �������ł���e�[�u���ԍ������o���Ă���
	//		int maxNo = g_Parts[i].tblMax;				// �o�^�e�[�u�����𐔂��Ă���
	//		int nextNo = (nowNo + 1) % maxNo;			// �ړ���e�[�u���̔ԍ������߂Ă���
	//		INTERPOLATION_DATA* tbl = g_MoveTblAdr[g_Parts[i].tblNo];	// �s���e�[�u���̃A�h���X���擾

	//		XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTOR�֕ϊ�
	//		XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTOR�֕ϊ�
	//		XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTOR�֕ϊ�

	//		XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ�ړ��ʂ��v�Z���Ă���
	//		XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ��]�ʂ��v�Z���Ă���
	//		XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ�g�嗦���v�Z���Ă���

	//		float nowTime = g_Parts[i].time - nowNo;	// ���ԕ����ł��鏭�������o���Ă���

	//		Pos *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���
	//		Rot *= nowTime;								// ���݂̉�]�ʂ��v�Z���Ă���
	//		Scl *= nowTime;								// ���݂̊g�嗦���v�Z���Ă���

	//		// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
	//		XMStoreFloat3(&g_Parts[i].pos, nowPos + Pos);

	//		// �v�Z���ċ��߂���]�ʂ����݂̈ړ��e�[�u���ɑ����Ă���
	//		XMStoreFloat3(&g_Parts[i].rot, nowRot + Rot);

	//		// �v�Z���ċ��߂��g�嗦�����݂̈ړ��e�[�u���ɑ����Ă���
	//		XMStoreFloat3(&g_Parts[i].scl, nowScl + Scl);

	//		// frame���g�Ď��Ԍo�ߏ���������
	//		g_Parts[i].time += 1.0f / tbl[nowNo].frame;	// ���Ԃ�i�߂Ă���
	//		if ((int)g_Parts[i].time >= maxNo)			// �o�^�e�[�u���Ō�܂ňړ��������H
	//		{
	//			g_Parts[i].time -= maxNo;				// �O�ԖڂɃ��Z�b�g�������������������p���ł���
	//		}

	//	}

	//}



	{	// �|�C���g���C�g�̃e�X�g
		LIGHT *light = GetLightData(1);
		XMFLOAT3 pos = g_Player.pos;
		pos.y += 20.0f;

		light->Position = pos;
		light->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light->Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light->Type = LIGHT_TYPE_POINT;
		light->Enable = true;
		SetLightData(1, light);
	}


#ifdef _DEBUG	// �f�o�b�O����\������
	PrintDebugProc("Player:�� �� �� ���@Space\n");
	PrintDebugProc("Player:X:%f Y:%f Z:%f\n", g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	// �X�P�[���𔽉f
	mtxScl = XMMatrixScaling(g_Player.scl.x, g_Player.scl.y, g_Player.scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// ��]�𔽉f
	mtxRot = XMMatrixRotationRollPitchYaw(g_Player.rot.x, g_Player.rot.y + XM_PI, g_Player.rot.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	SetWorldMatrix(&mtxWorld);

	XMStoreFloat4x4(&g_Player.mtxWorld, mtxWorld);


	// ���f���`��
	DrawModel(&g_Player.model);

	// �匳�̐e����q���ւƏ��Ԃɕ`�揈�����s���Ă����K�v������܂�
	// �Ƃ������Ƃ́A�o�^���鎞�ɁA
	// �e����o�^���Ă����K�v������܂�

	// �p�[�c�̊K�w�A�j���[�V����
	//for (int i = 0; i < PLAYER_PARTS_MAX; i++)

	//	// 1. �܂��̓p�[�c���g��SRT���s��
	//	// 2. �e���q�����`�F�b�N����
	//	// 3. �q���������ꍇ�A�e��mtxWorld�Ǝ�����mtxWorld���|���Z����
	//	// 4. ���̌���(mtxWorld)���g���Ď���(�p�[�c)��`�悷��
	//{
	//	// ���[���h�}�g���b�N�X�̏�����
	//	mtxWorld = XMMatrixIdentity();

	//	// �X�P�[���𔽉f
	//	mtxScl = XMMatrixScaling(g_Parts[i].scl.x, g_Parts[i].scl.y, g_Parts[i].scl.z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	//	// ��]�𔽉f
	//	mtxRot = XMMatrixRotationRollPitchYaw(g_Parts[i].rot.x, g_Parts[i].rot.y, g_Parts[i].rot.z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	//	// �ړ��𔽉f
	//	mtxTranslate = XMMatrixTranslation(g_Parts[i].pos.x, g_Parts[i].pos.y, g_Parts[i].pos.z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	//	if (g_Parts[i].parent != NULL)	// �q����������e�ƌ�������
	//	{
	//		mtxWorld = XMMatrixMultiply(mtxWorld, XMLoadFloat4x4(&g_Parts[i].parent->mtxWorld));
	//		// ��
	//		// g_Player.mtxWorld���w���Ă���
	//	}

	//	XMStoreFloat4x4(&g_Parts[i].mtxWorld, mtxWorld);

	//	// �g���Ă���Ȃ珈������B�����܂ŏ������Ă��闝�R�͑��̃p�[�c�����̃p�[�c���Q�Ƃ��Ă���\�������邩��B
	//	if (g_Parts[i].use == false) continue;

	//	// ���[���h�}�g���b�N�X�̐ݒ�
	//	SetWorldMatrix(&mtxWorld);


	//	// ���f���`��
	//	DrawModel(&g_Parts[i].model);

	//}



	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}


//=============================================================================
// �v���C���[�����擾
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_Player;
}


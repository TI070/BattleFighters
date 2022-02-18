#pragma once
#include "main.h"
#include "gameObject.h"
#include "stageSystem.h"


#define CAMERA_MOVESPEED_DEFAULT (0.05f)
#define CAMERA_ROTATESPEED_DEFAULT (0.05f)

class AnimationPlayer;


class Camera : public GameObject {

private:

	//�ϐ�
	D3DXVECTOR3 m_Target;
	D3DXVECTOR3 v_Up = {0.0f,1.0f,0.0f};
	D3DXVECTOR3 v_Right = { 1.0f,0.0f,0.0f };
	D3DXVECTOR3 v_Front = { 0.0f,0.0f,1.0f };
	float cameraMoveSpeed = CAMERA_MOVESPEED_DEFAULT;
	float cameraRotateSpeed = CAMERA_ROTATESPEED_DEFAULT;
	bool cameraModeSw;

	AnimationPlayer* pAP;

	//������J�����O�ǉ�
	D3DXMATRIX m_ProjectionMatrix;
	D3DXMATRIX m_ViewMatrix;

	//�֐�
	void CameraMove();	
	bool CamerRotate();
	
	void CameraAutoMove();

	//DIRECTION autoMoveDir;

	float viewAngle;

	bool cameraDebug;

	float targetViewAngle;

	//�f�o�b�O�p
	Area debugCheckcameraArea;

public:

	void Init();
	void Uninit();
	void Update();
	void Draw();

	//void TitleUpdate();
	//void SetAutoMoveDir(DIRECTION s_dir) { autoMoveDir = s_dir; }

	D3DXVECTOR3 GetFrontV();
	D3DXVECTOR3 GetRightV();

	void SetAtPos(D3DXVECTOR3 s_atpos) { m_Target = s_atpos; }

	bool GetCameraModeSw();

	D3DXMATRIX GetViewMatrix();
	void SetPlayer(AnimationPlayer* sP) { pAP = sP; }

	bool CheckView(D3DXVECTOR3 Position, float cullingScale);

	//�J�����̑ΏۂƂȂ�I�u�W�F�N�g����S�҂̒[���擾���A�����ƈʒu�E�����_��ݒ肷��
	void CameraZoom();

 	void DebugCameraMove();

	void CameraAngleLerp();
};
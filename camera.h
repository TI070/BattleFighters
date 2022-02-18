#pragma once
#include "main.h"
#include "gameObject.h"
#include "stageSystem.h"


#define CAMERA_MOVESPEED_DEFAULT (0.05f)
#define CAMERA_ROTATESPEED_DEFAULT (0.05f)

class AnimationPlayer;


class Camera : public GameObject {

private:

	//変数
	D3DXVECTOR3 m_Target;
	D3DXVECTOR3 v_Up = {0.0f,1.0f,0.0f};
	D3DXVECTOR3 v_Right = { 1.0f,0.0f,0.0f };
	D3DXVECTOR3 v_Front = { 0.0f,0.0f,1.0f };
	float cameraMoveSpeed = CAMERA_MOVESPEED_DEFAULT;
	float cameraRotateSpeed = CAMERA_ROTATESPEED_DEFAULT;
	bool cameraModeSw;

	AnimationPlayer* pAP;

	//視錐台カリング追加
	D3DXMATRIX m_ProjectionMatrix;
	D3DXMATRIX m_ViewMatrix;

	//関数
	void CameraMove();	
	bool CamerRotate();
	
	void CameraAutoMove();

	//DIRECTION autoMoveDir;

	float viewAngle;

	bool cameraDebug;

	float targetViewAngle;

	//デバッグ用
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

	//カメラの対象となるオブジェクトから４編の端を取得し、距離と位置・注視点を設定する
	void CameraZoom();

 	void DebugCameraMove();

	void CameraAngleLerp();
};

#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "key_manager.h"
#include "fieldConfig.h"
#include "animationPlayer.h"
#include "playerManager.h"
#include "stageSystem.h"
#include "gameSystem.h"

#define CAMERALIMIT_UP (10.0f)
#define CAMERALIMIT_DOWN (1.0f)

#define CAMERALINEAR_TIME (10)

void Camera::Init() {

	//m_Position = D3DXVECTOR3(0.0f,2.0f,-5.0f);
	m_Position = D3DXVECTOR3(0.0f, 3.0f, -27.5f);
	m_Target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_Position.x = pAP->GetPosition().x;
	//m_Position.x = pP->GetPosition().x;
	m_Position.z = pAP->GetPosition().z - 12.0f;

	m_Target = pAP->GetPosition();

	//m_Position = D3DXVECTOR3(10.0f, 1.0f, 7.5f);


	//m_Target = m_Position + v_Front * 2.5f;
	m_Target.y = 0;

	cameraModeSw = false;
	cameraDebug = false;

	viewAngle = 1.0f;
}

void Camera::Uninit() {


}

void Camera::Update() {

	if (KeyManager_Press(KM_E) && KeyManager_Trigger(KM_O)) cameraDebug = !cameraDebug;

	if(!cameraDebug) CameraZoom();
	else DebugCameraMove();
}

void Camera::Draw() {

	//ビューマトリックス(カメラの設置)
	//D3DXMATRIX viewMatrix;
	D3DXMatrixLookAtLH(&m_ViewMatrix, &m_Position, &m_Target, &v_Up);

	Renderer::SetViewMatrix(&m_ViewMatrix);

	//プロジェクションマトリクス(表示範囲等)
	//D3DXMATRIX projectionMatrix;
	D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, viewAngle, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(&m_ProjectionMatrix);

	//シェーダー用パラメータセット
	Renderer::SetCameraPosition({ m_Position.x,m_Position.y,m_Position.z,1.0f });

	/*
	ImGui::Begin("camera");

	ImGui::InputFloat("leftMax", &debugCheckcameraArea.leftMax);
	ImGui::InputFloat("rightMax", &debugCheckcameraArea.rightMax);
	ImGui::InputFloat("upMax", &debugCheckcameraArea.upMax);
	ImGui::InputFloat("downMax", &debugCheckcameraArea.downMax);

	//ImGui::SliderFloat("hayasa", &boundPolygon[0].vec.x, -10.0f, 10.0f);

	//ImGui::SliderFloat("hayasa2", &boundPolygon[0].vec.y, -10.0f, 10.0f);


	ImGui::End();
	*/


}

void Camera::DebugCameraMove() {

	if (KeyManager_Press(KM_M)) {

		m_Position.z += 0.1f;
	}

}

void Camera::CameraMove() {

	//カメラ回転状態なら移動しない
	if (CamerRotate()) return;

	//プレイヤーが移動できなければ移動しない
	//if (!pP->GetSuccessMove()) return;


	if (KeyManager_Press(KM_D)) {

		//カメラ座標移動
		//m_Position.x += cameraMoveSpeed;
		m_Position += v_Right * cameraMoveSpeed;

		//注視点移動
		//m_Target.x += cameraMoveSpeed;
		m_Target += v_Right * cameraMoveSpeed;
		

	}
	if (KeyManager_Press(KM_A)) {

		//カメラ座標移動
		//m_Position.x -= cameraMoveSpeed;
		m_Position -= v_Right * cameraMoveSpeed;

		//注視点移動
		//m_Target.x -= cameraMoveSpeed;
		m_Target -= v_Right * cameraMoveSpeed;

	}
	if (KeyManager_Press(KM_S)) {

		//カメラ座標移動
		//m_Position.z -= cameraMoveSpeed;
		m_Position -= v_Front * cameraMoveSpeed;

		//注視点移動
		//m_Target.z -= cameraMoveSpeed;
		m_Target -= v_Front * cameraMoveSpeed;

	}
	if (KeyManager_Press(KM_W)) {

		//カメラ座標移動
		//m_Position.z += cameraMoveSpeed;
		m_Position += v_Front * cameraMoveSpeed;

		//注視点移動
		//m_Target.z += cameraMoveSpeed;
		m_Target += v_Front * cameraMoveSpeed;

	}

	//高さを合わせる
	m_Position.y = m_Target.y + 5.0f;

	//if (KeyManager_Press(KM_U)) {
	//	//カメラ座標移動
	//	m_Position += v_Up * cameraMoveSpeed;
	//
	//}
	//if (KeyManager_Press(KM_O)) {
	//	//カメラ座標移動
	//	m_Position -= v_Up * cameraMoveSpeed;
	//
	//}

//	m_Position = D3DXVECTOR3(0,5,-5) + pP->GetPosition();
//	m_Target = pP->GetPosition();
//	m_Position = v_Front;

}

bool Camera::CamerRotate() {

	//カメラボタンを押していなければfalseを返す
	//if (!KeyManager_Press(KM_J)) {
	//	return false;
	//}
	//else {//押していれば回転

		//D3DXVECTOR3 At = m_Target;
		//D3DXVECTOR3 m_Target = m_Position + v_Front * 2.5f;
		//m_Target = m_Position + v_Front * 2.5f;
		m_Target = pAP->GetPosition();
		//m_Target.y = 0;

		bool rot_F = false;

		if (!KeyManager_Press(KM_J) && KeyManager_Press(KM_U)) {
			D3DXMATRIX mtxR;
			//D3DXMatrixRotationAxis(&mtxR, &g_vUp, -g_RotationalSpeed);
			D3DXMatrixRotationY(&mtxR, -cameraRotateSpeed);

			D3DXVec3TransformNormal(&v_Up, &v_Up, &mtxR);
			D3DXVec3TransformNormal(&v_Front, &v_Front, &mtxR);
			D3DXVec3TransformNormal(&v_Right, &v_Right, &mtxR);
		
			rot_F = true;
		}
		else if (!KeyManager_Press(KM_J) && KeyManager_Press(KM_O)) {
			D3DXMATRIX mtxR;
			//D3DXMatrixRotationAxis(&mtxR, &g_vUp, -g_RotationalSpeed);
			D3DXMatrixRotationY(&mtxR, cameraRotateSpeed);

			D3DXVec3TransformNormal(&v_Up, &v_Up, &mtxR);
			D3DXVec3TransformNormal(&v_Front, &v_Front, &mtxR);
			D3DXVec3TransformNormal(&v_Right, &v_Right, &mtxR);
		
			rot_F = true;
		}
		else if (KeyManager_Press(KM_U)) {
//			D3DXMATRIX mtxR;
//			D3DXMatrixRotationAxis(&mtxR, &g_vRight, -g_RotationalSpeed);
//
//			D3DXVec3TransformNormal(&g_vFront, &g_vFront, &mtxR);
//			D3DXVec3TransformNormal(&g_vUp, &g_vUp, &mtxR);
			
			if (m_Position.y + v_Up.y * cameraMoveSpeed < CAMERALIMIT_UP) {
				m_Position += v_Up * cameraMoveSpeed;
			}
			rot_F = true;
		}
		else if (KeyManager_Press(KM_O)) {
//			D3DXMATRIX mtxR;
//			D3DXMatrixRotationAxis(&mtxR, &v_Right, cameraRotateSpeed);
//
//			D3DXVec3TransformNormal(&v_Front, &v_Front, &mtxR);
//			D3DXVec3TransformNormal(&v_Up, &v_Up, &mtxR);
			if (m_Position.y - v_Up.y * cameraMoveSpeed > CAMERALIMIT_DOWN) {
				m_Position -= v_Up * cameraMoveSpeed;
			}
			rot_F = true;
		}

		//m_Position = m_Target - v_Front * 2.5f;
		m_Position.x = m_Target.x - v_Front.x * 10.0f;
		m_Position.z = m_Target.z - v_Front.z * 10.0f;


		//if(rot_F) m_Target += v_Front * 2.5f;

	//}

	//cameraModeSw = true;

	return rot_F;

}


D3DXVECTOR3 Camera::GetFrontV() {
	return v_Front;
}

D3DXVECTOR3 Camera::GetRightV() {
	return v_Right;
}

bool Camera::GetCameraModeSw() {
	return cameraModeSw;
}

D3DXMATRIX Camera::GetViewMatrix() {
	D3DXMATRIX mtxView;
	D3DXMatrixLookAtLH(&mtxView, &m_Position, &m_Target, &v_Up);//(LH)レフトハンドル 左手座標系で計算
	return mtxView;
}
/*
void Camera::TitleUpdate() {

	//CameraAutoMove();

}*/


bool Camera::CheckView(D3DXVECTOR3 Position,float cullingScale) {

	D3DXMATRIX vp, invvp;

	vp = m_ViewMatrix * m_ProjectionMatrix;
	D3DXMatrixInverse(&invvp,NULL,&vp);//逆行列

	D3DXVECTOR3 vpos[4];
	D3DXVECTOR3 wpos[4];

	//視錐台奥の４頂点セット
	vpos[0] = D3DXVECTOR3(-1.0f,1.0f,1.0f);//左上
	vpos[1] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);//右上
	vpos[2] = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);//左下
	vpos[3] = D3DXVECTOR3(1.0f, -1.0f, 1.0f);//右下

	//座標変換
	D3DXVec3TransformCoord(&wpos[0], &vpos[0], &invvp);
	D3DXVec3TransformCoord(&wpos[1], &vpos[1], &invvp);
	D3DXVec3TransformCoord(&wpos[2], &vpos[2], &invvp);
	D3DXVec3TransformCoord(&wpos[3], &vpos[3], &invvp);

	D3DXVECTOR3 v, v1, v2, n;

	v = Position - m_Position;

	//D3DXVec3Normalize(&v,&v);

	//左側
	v1 = wpos[0] - m_Position;
	v2 = wpos[2] - m_Position;
	D3DXVec3Cross(&n,&v1,&v2);//内積

	//面について外側であったらfalse(描画しない)
	if (D3DXVec3Dot(&n, &v) < 0) return false;
	/**/
	//右側
	v1 = wpos[1] - m_Position;
	v2 = wpos[3] - m_Position;
	D3DXVec3Cross(&n, &v1, &v2);//内積

	//面について外側であったらfalse(描画しない)
	if (D3DXVec3Dot(&n, &v) > 0) return false;

	//上側
	v1 = wpos[0] - m_Position;
	v2 = wpos[1] - m_Position;
	D3DXVec3Cross(&n, &v1, &v2);//内積

	//面について外側であったらfalse(描画しない)
	if (D3DXVec3Dot(&n, &v) > 0) return false;
		
	//下側
	v1 = wpos[2] - m_Position;
	v2 = wpos[3] - m_Position;
	D3DXVec3Cross(&n, &v1, &v2);//内積

	//面について外側であったらfalse(描画しない)
	if (D3DXVec3Dot(&n, &v) < 0) return false;



	return true;
}

void Camera::CameraZoom() {

	Area cameraArea;
	Area cameraAreaMax;
	float areaMaxCorrect = 3.5f;
	float safety = 1.0f;


	float cameraDistance;//注視点からカメラまでの距離
	cameraDistance = fabsf(m_Position.z);

	//生存エリアからちょっと内側に設定する
	cameraArea = StageSystem::GetInStageArea();
	cameraAreaMax.downMax = cameraArea.downMax + areaMaxCorrect;
	cameraAreaMax.leftMax = cameraArea.leftMax + areaMaxCorrect;
	cameraAreaMax.rightMax = cameraArea.rightMax - areaMaxCorrect;
	cameraAreaMax.upMax = cameraArea.upMax - areaMaxCorrect;
	cameraArea.downMax = 10;
	cameraArea.leftMax = 10;
	cameraArea.rightMax = -10;
	cameraArea.upMax = -10;

	std::list<AnimationPlayer*> playerList = PlayerManager::GetPlayerList();

	for (AnimationPlayer* pP:playerList) {

		//ストックがないキャラは追わない
		if (pP->GetLifeStock() <= 0) continue;

		//プレイヤーの中での最広位置を取り、カメラ規格と比較
		/*
		if (cameraArea.rightMax > pP->GetPosition().x + safety) {
			cameraArea.rightMax = pP->GetPosition().x + safety;
		}
		if (cameraArea.upMax > pP->GetPosition().y + safety) {
			cameraArea.upMax = pP->GetPosition().y + safety;
		}
		if (cameraArea.leftMax < pP->GetPosition().x - safety) {
			cameraArea.leftMax = pP->GetPosition().x - safety;
		}
		if (cameraArea.downMax < pP->GetPosition().y - safety) {
			cameraArea.downMax = pP->GetPosition().y - safety;
		}
		*/
		if (cameraArea.rightMax < pP->GetPosition().x + safety) {
			cameraArea.rightMax = pP->GetPosition().x + safety;
		
			//上限を超えないようにする
			if (cameraArea.rightMax > cameraAreaMax.rightMax) cameraArea.rightMax = cameraAreaMax.rightMax;

		}
		if (cameraArea.upMax < pP->GetPosition().y + safety) {
			cameraArea.upMax = pP->GetPosition().y + safety;

			//上限を超えないようにする
			if (cameraArea.upMax > cameraAreaMax.upMax) cameraArea.upMax = cameraAreaMax.upMax;

		}
		if (cameraArea.leftMax > pP->GetPosition().x - safety) {
			cameraArea.leftMax = pP->GetPosition().x - safety;
		
			//上限を超えないようにする
			if (cameraArea.leftMax < cameraAreaMax.leftMax) cameraArea.leftMax = cameraAreaMax.leftMax;

		}
		if (cameraArea.downMax > pP->GetPosition().y - safety) {
			cameraArea.downMax = pP->GetPosition().y - safety;

			//上限を超えないようにする
			if (cameraArea.downMax < cameraAreaMax.downMax) cameraArea.downMax = cameraAreaMax.downMax;

		}

	}

	//注視点の算出
	m_Target.x = cameraArea.GetCenter().x;
	m_Target.y = cameraArea.GetCenter().y;

	//座標の算出
	m_Position.x = cameraArea.GetCenter().x;

	//視野角の算出

	//横と縦の広い方を採用する
	float cR = cameraArea.GetRange().x;
	if (cR < cameraArea.GetRange().y) cR = cameraArea.GetRange().y;

	//viewAngle = atan2(cameraArea.GetRange().x / 2,cameraDistance);
	//viewAngle = atan2(cR / 2, cameraDistance);

	//補正
	//viewAngle *= 2;
	//viewAngle += 0.2f;

	targetViewAngle = atan2(cR / 2, cameraDistance);

	//補正
	targetViewAngle *= 2;
	targetViewAngle += 0.2f;

	
	//線形補完
	CameraAngleLerp();

	//デバッグ用
	debugCheckcameraArea = cameraArea;

}

void Camera::CameraAngleLerp() {

	float plusVA = (targetViewAngle - viewAngle) / CAMERALINEAR_TIME;
	viewAngle += plusVA;

}
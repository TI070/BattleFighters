#include "stageSystem.h"
#include "backGround.h"
#include "MeshFieldBox.h"
#include "meshFieldBoxManager.h"
#include "audioManager.h"
#include "gameSystem.h"
#include "BgObjectManager.h"


Area StageSystem::centerArea;

Area StageSystem::inStageArea;

Area StageSystem::groundArea;

Area StageSystem::cameraZoomLimitArea;

BackGround* StageSystem::skyDome = nullptr;
BackGround* StageSystem::skyDome_bottom = nullptr;

void StageSystem::SetStage(STAGEKIND s_sk) {

	MeshFieldBox* mfbP;

	//ステージごとの設定
	switch (s_sk)
	{
	case SK_BATTLEFIELD:
		

		//mfbP = MeshFieldBoxManager::SetMeshFieldBox({ 0,0,0 }, 0.6f, 1.0f, 2, 0.5f);
		mfbP = MeshFieldBoxManager::SetMeshFieldBox({ 0,0,0 }, 0.6f, 1.0f, 2, 0.2f);
		mfbP->SetTexture(TEX_FIELD2);
		mfbP->SetThroughFlag(false);
		mfbP->SetCliffFlag(true);
		mfbP = MeshFieldBoxManager::SetMeshFieldBox({ 0,0,1.0f }, 0.6f, 1.0f, 2, 0.04f);
		mfbP->SetTexture(TEX_FIELD_PURPLE);
		mfbP->SetThroughFlag(false);
		mfbP = MeshFieldBoxManager::SetMeshFieldBox({ 0,0,-1.0f }, 0.6f, 1.0f, 2, 0.04f);
		mfbP->SetTexture(TEX_FIELD_PURPLE);
		mfbP->SetThroughFlag(false);
		mfbP = MeshFieldBoxManager::SetMeshFieldBox({ 0,0,1.72f }, 0.6f, 1.0f, 2, 0.15f);
		mfbP->SetTexture(TEX_FIELD3);
		mfbP = MeshFieldBoxManager::SetMeshFieldBox({ 0,0,-1.72f }, 0.6f, 1.0f, 2, 0.15f);
		mfbP->SetTexture(TEX_FIELD3);

		//mfbP->SetThroughFlag(true);
		//mfbP->SetCliffFlag(true);

		mfbP = MeshFieldBoxManager::SetMeshFieldBox({ 3,2,0 }, 0.2f, 1.0f, 2, 1);
		mfbP->SetThroughFlag(true);
		mfbP->SetCliffFlag(false);
		mfbP->SetTexture(TEX_FIELD2);
		mfbP = MeshFieldBoxManager::SetMeshFieldBox({ 3,2,2.0f }, 0.2f, 1.0f, 2, 0.04f);
		mfbP->SetTexture(TEX_FIELD_PURPLE);
		mfbP->SetCliffFlag(false);
		mfbP = MeshFieldBoxManager::SetMeshFieldBox({ 3,2,-2.0f }, 0.2f, 1.0f, 2, 0.04f);
		mfbP->SetTexture(TEX_FIELD_PURPLE);
		mfbP->SetCliffFlag(false);


		mfbP = MeshFieldBoxManager::SetMeshFieldBox({ -3,2,0 }, 0.2f, 1.0f, 2, 1);
		mfbP->SetThroughFlag(true);
		mfbP->SetCliffFlag(false);
		mfbP->SetTexture(TEX_FIELD2);
		mfbP = MeshFieldBoxManager::SetMeshFieldBox({ -3,2,-2.0f }, 0.2f, 1.0f, 2, 0.04f);
		mfbP->SetTexture(TEX_FIELD_PURPLE);
		mfbP->SetCliffFlag(false);
		mfbP = MeshFieldBoxManager::SetMeshFieldBox({ -3,2,2.0f }, 0.2f, 1.0f, 2, 0.04f);
		mfbP->SetTexture(TEX_FIELD_PURPLE);
		mfbP->SetCliffFlag(false);


		mfbP = MeshFieldBoxManager::SetMeshFieldBox({ 0,4,0 }, 0.2f, 1.0f, 2, 1);
		mfbP->SetThroughFlag(true);
		mfbP->SetCliffFlag(false);
		mfbP->SetTexture(TEX_FIELD2);
		mfbP = MeshFieldBoxManager::SetMeshFieldBox({ 0,4,2.0f }, 0.2f, 1.0f, 2, 0.04f);
		mfbP->SetTexture(TEX_FIELD_PURPLE);
		mfbP->SetCliffFlag(false);
		mfbP = MeshFieldBoxManager::SetMeshFieldBox({ 0,4,-2.0f }, 0.2f, 1.0f, 2, 0.04f);
		mfbP->SetTexture(TEX_FIELD_PURPLE);
		mfbP->SetCliffFlag(false);

		//背景オブジェクト
		BgObjectManager::SetBgObject({ 2,0,2 }, { 0,0,3.14f / 6 }, { 0.4f,0.4f,0.4f});
		BgObjectManager::SetBgObject({ 3.5f,0,2 }, { 0,0,-3.14f / 6 * 1.5f }, { 0.35f,0.35f,0.35f });
		BgObjectManager::SetBgObject({ 0,0,2 }, { 0,0,3.14f / 12 }, { 0.45f,0.45f,0.45f });

		BgObjectManager::SetBgObject({ -2.5f,0,2 }, { 0,0,-3.14f / 8 }, { 0.35f,0.35f,0.35f });
		BgObjectManager::SetBgObject({ -4.0f,0,2 }, { 0,0,3.14f / 6 * 1.5f }, { 0.3f,0.3f,0.3f });


		SetCenterArea({ 5.0f,-5.0f,1.0f,0.0f });
		SetGroundArea({ 5.0f,-5.0f,3.0f,0.0f });
		SetInStageArea({ 10.0f,-10.0f,10.0f,-10.0f });
		SetCameraZoomLimitArea({ 6.0f,-6.0f,3.0f,-3.0f });

		//BGMセット
		AudioManager::AudioPlay(AK_BGM_BATTLE, true);

		break;

	case SK_LASTSTOP:


		mfbP = MeshFieldBoxManager::SetMeshFieldBox({ 0,0,0 }, 0.6f, 1.0f, 2, 0.5f);
		mfbP->SetThroughFlag(false);
		//mfbP->SetThroughFlag(true);
		mfbP->SetCliffFlag(true);

		SetCenterArea({ 5.0f,-5.0f,1.0f,0.0f });
		SetGroundArea({ 5.0f,-5.0f,3.0f,0.0f });
		SetInStageArea({ 10.0f,-10.0f,10.0f,-10.0f });
		SetCameraZoomLimitArea({ 6.0f,-6.0f,3.0f,-3.0f });

		//BGMセット
		AudioManager::AudioPlay(AK_BGM_BATTLE, true);


	default:
		break;
	}


	//汎用
	skyDome = new BackGround;
	skyDome->Init();

	skyDome_bottom = new BackGround;
	skyDome_bottom->Init();
	skyDome_bottom->SetRotation({ 0,1.57f,3.14f });
	skyDome_bottom->SetPosition({ 0,0.5f,0 });

	//バトル開始準備
	ReadyBattle();



}

void StageSystem::Draw() {

	if (skyDome == nullptr) return;

	skyDome->Draw();
	skyDome_bottom->Draw();


}

//エリアの中に入っていたらtrueを返す
bool StageSystem::CheckInArea(Area s_area, D3DXVECTOR3 s_pos) {

	if (s_area.rightMax >= s_pos.x && s_area.leftMax <= s_pos.x &&
		s_area.upMax >= s_pos.y && s_area.downMax <= s_pos.y) {

		return true;
	}

	return false;

}

void StageSystem::ReadyBattle() {

	//bool* spP = GameSystem::GetStagePlayingP();
	//*spP = true;

}
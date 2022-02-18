
#include "result.h"
#include "texture.h"
#include "config.h"
#include "writestring.h"
#include "key_manager.h"
#include "scene.h"
#include "writestring.h"
#include "audio.h"
#include "imageManager.h"
#include "UIwindow.h"
#include "stageSystem.h"
#include "gameSystem.h"
#include "title.h"
#include "audioManager.h"
#include "meshFieldBoxManager.h"
#include "MeshFieldBox.h"
#include "controller.h"

#include <stdio.h>

#define TITLELOGO_WIDTH (SCREEN_WIDTH)
#define TITLELOGO_HEIGHT (SCREEN_HEIGHT)





int Result::select;

TexString* Result::resultLogoText;
UIWindow* Result::resultLogoWindow;
UIWindow* Result::resultDataWindows[4];
TexString* Result::resultRankText[4];

Polygon2D* Result::resultBg;
Polygon2D* Result::resultBgParts[9];

AnimationModel* Result::pModel[4];

ID3D11Buffer* Result::m_VertexBuffer = nullptr;
ID3D11ShaderResourceView* Result::m_Texture = nullptr;

ID3D11VertexShader* Result::m_VertexShader = nullptr;
ID3D11PixelShader* Result::m_PixelShader = nullptr;
ID3D11InputLayout* Result::m_VertxeLayout = nullptr;

int Result::modelFrame[4];

Polygon2D* Result::resultDataWindowsTex[4];

void Result::Initialize() {

	//BGM
	AudioManager::AudioStop(AK_BGM_BATTLE);
	AudioManager::AudioPlay(AK_BGM_RESULT,1);


	char buf[64];

	select = 0;

	//背景
	//resultBg = ImageManager::SetImage({ SCREEN_WIDTH / 2 + 0.600,SCREEN_HEIGHT / 2 }, { SCREEN_WIDTH,SCREEN_HEIGHT }, ImageManager::GetTexId(TEX_UI_TITLEMENUBG), DEPTH_THREE);


	resultLogoWindow = ImageManager::SetUIWindow({ SCREEN_WIDTH / 2.0f ,SCREEN_HEIGHT / 2 - 200 }, { 600,200 }, 0);
	//一人クリア時には違うテ[クスチャ貼る
	if (GameSystem::GetCurrentStage() >= STAGEEND && GameSystem::GetResultData(0).rank == 1) {
		resultLogoWindow->SetWindowTex(TEX_UI_CONGRA);
	}
	else {
		resultLogoWindow->SetWindowTex(TEX_UI_RESULT);
	}
	//sprintf(buf, "Result");
	//resultLogoText = resultLogoWindow->SetTextTex(buf, { 100,50 }, { 96,96 }, { 0,0,0,1 });
	//resultLogoText->SetColor({ 1,0,0,1 });



	//リザルトデータ表示
	for (int i = 0; i < 4; i++) {

		if (!GameSystem::GetisJoin(i)) continue;

		resultDataWindows[i] = ImageManager::SetUIWindow({30 + (SCREEN_WIDTH / 2.0f) + (i - 2) * RESULTDATA_WINDOW_WIDTH + 50 * i,-10 + SCREEN_HEIGHT * 3.0f / 4}, { RESULTDATA_WINDOW_WIDTH,RESULTDATA_WINDOW_HEIGHT }, 0);
		//resultDataWindowsTex[i] = resultDataWindows[i]->SetWindowTex(TEX_UI_RESULTWINDOW,1);
		resultDataWindowsTex[i] = resultDataWindows[i]->SetWindowTex(TEX_UI_PLAYERSELECTWINDOW, 1);

		

		if (i == 0) resultDataWindowsTex[i]->SetColor({255 / 255.0f,76 / 255.0f,76 / 255.0f,255/255.0f});
		else if (i == 1) resultDataWindowsTex[i]->SetColor({   76 / 255.0f, 76 / 255.0f,255 / 255.0f,255 / 255.0f });
		else if (i == 2) resultDataWindowsTex[i]->SetColor({ 255 / 255.0f,255/ 255.0f,76 / 255.0f,255 / 255.0f });
		else if (i == 3) resultDataWindowsTex[i]->SetColor({  76 / 255.0f, 255 / 255.0f, 76/ 255.0f,255 / 255.0f });
		resultDataWindowsTex[i]->UpdateVertex();

		//キャラ画像
		resultDataWindows[i]->SetPolygon2D({ RESULTDATA_WINDOW_WIDTH / 2,RESULTDATA_WINDOW_HEIGHT / 2 - 75 }, { 120,120 }, ImageManager::GetTexId((TEX_KIND)(TEX_UI_SELECTIMAGE_ARCHER + (int)GameSystem::GetSelectChara(i))));

		//プレイヤーナンバー
		sprintf(buf, "%dP", i + 1);
		resultDataWindows[i]->SetTextTex(buf, { 20,140 }, { 64,64 }, { 0,0,0,1 });

		//順位
		sprintf(buf, "rank%d",GameSystem::GetResultData(i).rank);
		resultRankText[i] = resultDataWindows[i]->SetTextTex(buf, { 20,190 }, { 64,64 }, { 0,0,0,1 });
		//resultRankText[i]->SetColor({ 1,0,0,1 });

		//生存タイム
		//勝者は-:--で表示
		if(GameSystem::GetResultData(i).surviveTime == 0)sprintf(buf, "-:--");
		else sprintf(buf, "%d:%02d", GameSystem::GetResultData(i).surviveTime / 60, GameSystem::GetResultData(i).surviveTime % 60);
		resultDataWindows[i]->SetTextTex(buf, { 20,240 }, { 64,64 }, { 0,0,0,1 });


	}

	//ResultBgInit();

	MakeField();

	
	//勝者のキャラ取得
	CHARA_KIND ck;
	if (GameSystem::GetResultData(0).rank == 1) ck = GameSystem::GetSelectChara(0);
	else if (GameSystem::GetResultData(1).rank == 1) ck = GameSystem::GetSelectChara(1);
	else if (GameSystem::GetResultData(2).rank == 1) ck = GameSystem::GetSelectChara(2);
	else if (GameSystem::GetResultData(3).rank == 1) ck = GameSystem::GetSelectChara(3);


	ModelLoad(ck,RPAUSE_WINNER);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertxeLayout, "charaShaderVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "charaShaderPS.cso");




}


void Result::Draw() {



	ModelView();


}

void Result::Uninit() {

	AudioManager::AudioStop(AK_BGM_RESULT);

}

void Result::Update() {

	//ResultBgUpdate();

	//とりあえずタイトルに戻す
	if (KeyManager_Trigger(KM_DECIDE) || ControllerManager::GetButtonState(0).aButton == BS_TRIGGER) {
		
		//currenStageを参照して次のステージに行くかを判断する
		
		if(GameSystem::GetCurrentStage() == 0) Scene::SetCurrentScene(SCENE_TITLE);
		else if(GameSystem::GetCurrentStage() >= STAGEEND) Scene::SetCurrentScene(SCENE_TITLE);
		else {

			//勝っていたら次のステージへ
			if (GameSystem::GetResultData(0).rank == 1) Scene::SetCurrentScene(SCENE_GAME);

			//負けていたらタイトルへ
			else Scene::SetCurrentScene(SCENE_TITLE);

		}
		GameSystem::Init();

	}

	pModel[0]->Update("Win", "Win", 0.3f, modelFrame[0]);
	modelFrame[0]++;

	//MenuSelect();

}

void Result::ResultBgInit() {

	for (int i = 0; i < 9; i++) {
		resultBgParts[i] = ImageManager::SetImageD({ SCREEN_WIDTH / 2.0f + (MENUBGPARTS_DISTANCE * (i - 4.0f)),SCREEN_HEIGHT / 2.0f }, { 1200,675 }, ImageManager::GetTexId(TEX_UI_TITLEMENUBGPARTS), DEPTH_TWO);
		resultBgParts[i]->SetColor({ 1,1,1,0.5f });
		resultBgParts[i]->UpdateVertex();
		resultBgParts[i]->SetIsUse(true);
	}

}


void Result::ResultBgUpdate() {

	for (int i = 0; i < 9; i++) {
		D3DXVECTOR3 spos;
		spos = resultBgParts[i]->GetPosition();
		spos.x += 2.0f;
		if (spos.x > SCREEN_WIDTH * 3 / 2.0f + 960 / 9) spos.x = -SCREEN_WIDTH / 2.0f - 960 / 9;
		resultBgParts[i]->SetPosition(spos);
		resultBgParts[i]->UpdateVertex();
	}

}


void Result::ModelView(){

	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertxeLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, 0.05f, 0.05f, 0.05f);
	D3DXMatrixRotationYawPitchRoll(&rot,0, 0, 0);
	D3DXMatrixTranslation(&trans, 0, -4, 0);

	world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);

	//モデルを表示する
	pModel[0]->Draw();
	
	

}

void Result::ModelLoad(CHARA_KIND s_CK, RESULT_PAUSE s_RP) {


	for (int i = 0; i < 4;i++) {
		pModel[i] = new AnimationModel;
		modelFrame[i] = 0;
	}

	//勝ったキャラのモデルを読み込む
	switch (s_CK)
	{
	case CK_TEST:
		pModel[0]->Load("asset/model/Player1.fbx");

		//勝敗で読み込みアニメーションを変える
		pModel[0]->LoadAnimation("asset/model/animation/player1Win.fbx", "Win");


		break;
	case CK_PALADIN:
		pModel[0]->Load("asset/model/paladin.fbx");

		//勝敗で読み込みアニメーションを変える
		pModel[0]->LoadAnimation("asset/model/animation/paladin/paladinWin.fbx", "Win");


		break;
	case CK_FIGTHER:
		pModel[0]->Load("asset/model/animation/fighter/Mma Kickbase.fbx");

		//勝敗で読み込みアニメーションを変える
		pModel[0]->LoadAnimation("asset/model/animation/fighter/figtherWin.fbx", "Win");


		break;
	case CK_WIZARD:
		pModel[0]->Load("asset/model/animation/wizard/magic_Idle 02.fbx");
		pModel[0]->LoadAnimation("asset/model/animation/wizard/magicWin.fbx", "Win");

		//勝敗で読み込みアニメーションを変える


		break;
	case CHARAKIND_MAX:
		break;
	default:
		break;
	}

	



}


void Result::MakeField() {

	MeshFieldBox* mfbP;

	mfbP = MeshFieldBoxManager::SetMeshFieldBox({ 0,-4,0 }, 2.0f, 1.0f, 2, 1.0f);
	mfbP->SetThroughFlag(false);

}
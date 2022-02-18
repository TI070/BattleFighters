
#include "gameSystem.h"
#include "main.h"
#include "imageManager.h"
#include "scene.h"
#include "polygon2d.h"
#include "playerManager.h"
#include "camera.h"


#define READYTIME (1)
#define GOTIME (1)



bool GameSystem::debugFlag[DF_MAX];
int GameSystem::charaStartStock[4];
int* GameSystem::charaCurrentStock[4];//各キャラの現在残機
bool GameSystem::stagePlaying;
int GameSystem::timer;//タイマー（秒単位）
int GameSystem::addTimerCnt;
ResultData GameSystem::resultData[4];
CHARA_KIND GameSystem::selectChara[4];
CHARA_MODE GameSystem::selectPlayerMode[4] = {CM_PLAYER,CM_CPU,CM_CPU,CM_CPU };
bool GameSystem::isJoin[4] = {true,true,false,false};
int GameSystem::setStockNum = 1;

GAMEMODE_BATTLE GameSystem::currentGameMode = GM_BATTLEROYAL;
int GameSystem::currentStage = 0;

int GameSystem::readyGoSw = 0;
Polygon2D* GameSystem::readyGo;
Polygon2D* GameSystem::stageNumTex = nullptr;

int GameSystem::alphaCount = 0;

void GameSystem::Init() {

	for (int i = 0; i < DF_MAX; i++) {
		debugFlag[i] = false;
	}

	for (int i = 0; i < 4; i++) {

		//１人用モードの時は選択キャラが初期化されないようにする
		if (i == 0 && currentStage >= 1) continue;

		charaCurrentStock[i] = nullptr;
		selectChara[i] = CHARAKIND_MAX;
		//isJoin[i] = false;
	}

	//一時対処
	//isJoin[0] = true;
	//isJoin[1] = true;

	//SetStartStockAll(3);

	//timer = 0;
	//addTimerCnt = 0;

	BattleSet();

}


void GameSystem::Update() {

	ReadyGoUpdate();

	TimerUpdate();

	if(stagePlaying)JudgeGameSet();
}

//ゲームセット判定　ゲームセットならtrueを返す
bool GameSystem::JudgeGameSet() {

	int lifeCharaNum = 0;
	
	//ストックが複数人残っているかを判定する
	for (int i = 0; i < 4; i++) {
		if (charaCurrentStock[i] == nullptr) continue;
		if (*charaCurrentStock[i] > 0) lifeCharaNum++;
	}

	//二人以上残っていたら終わらない
	if (lifeCharaNum >= 2) return false;

	//残り０人なら引き分け 後で考えます

	//一人なら勝ち
	if (lifeCharaNum == 1) {
		GameSet();
	}


	return true;

}


void GameSystem::GameSet() {

	//ステージプレイを止める
	stagePlaying = false;

	//ゲームセットを示すメッセージ画像を出す
	ImageManager::SetImage({ SCREEN_WIDTH / 2, SCREEN_HEIGHT/2 - 100 }, { 450,150 }, ImageManager::GetTexId(TEX_UI_GAMESET), 0);

	//順位付け 仮置き
	SetRankStock();

	//画面遷移を呼ぶ
	//currentStageを参照して、１人モードか対戦モードかで処理を分ける
	if (currentStage == 0) {
		Scene::SetCurrentScene(SCENE_RESULT, 180);
	}
	else {
		currentStage++;
		Scene::SetCurrentScene(SCENE_RESULT, 180);

	}
}

void GameSystem::BattleSet() {

	for (int i = 0; i < 4; i++) {
		//参加しているプレイヤーのステータスを初期化する
		if (charaCurrentStock[i] == nullptr) continue;
		resultData[i].rank = 0;
		resultData[i].score = 0;
		resultData[i].surviveTime = 0;
	}

	stagePlaying = true;
	
	readyGoSw = 0;

	SetStartStockAll(setStockNum);

	timer = 0;
	addTimerCnt = 0;

}

//ストック制での順位セット
void GameSystem::SetRankStock() {

	//これが呼び出されたタイミングで生存時間を参照して順位付けをする

	struct stockData {
		int d_rank;
		int playerNum;
		int surviveTime;
	};

	stockData sData[4];
	stockData dummy;
	int cnt = 0;
	//生存時間をセット
	for (int i = 0; i < 4; i++) {
		if (charaCurrentStock[i] == nullptr) continue;
		sData[cnt].surviveTime = resultData[i].surviveTime;
		sData[cnt].playerNum = i;
		cnt++;
	}

	//ソート
	for (int i = 0; i < cnt; i++) {
		for (int j = i + 1; j < cnt; j++) {
			if (sData[i].surviveTime > sData[j].surviveTime) {
			 	dummy = sData[i];
				sData[i] = sData[j];
				sData[j] = dummy;
			}
		}
	}

	//順位付け
	for (int i = 0; i < cnt; i++) {
		sData[i].d_rank = i + 1;
	}

	//resultDataに返す
	for (int i = 0; i < 4; i++) {
		//if (i == sData[i].playerNum) {
		if (charaCurrentStock[i] == nullptr) continue;
		resultData[sData[i].playerNum].rank = sData[i].d_rank;
		//}
	}

}


void GameSystem::TimerUpdate() {

	if(addTimerCnt > 60){
		addTimerCnt = 0;
		timer++;
	}
	else addTimerCnt++;

}

void GameSystem::SetSurviveTime(int playerNum) {

	resultData[playerNum].surviveTime = timer;

}

void GameSystem::PlayerModeChange(int playerNum) {

	int r;
	r = (selectPlayerMode[playerNum] + 1) % CHARAMODE_MAX;
	selectPlayerMode[playerNum] = (CHARA_MODE)r;

}

void GameSystem::OnePlayerInit() {

	//プレイヤーのセット
	AnimationPlayer* ap;
	AnimationPlayer* eAP;
	AnimationPlayer* eAP2;
	AnimationPlayer* eAP3;

	if (GameSystem::GetSelectChara(0) == CK_TEST) {
		ap = PlayerManager::SetAnimationPlayer<AnimationPlayer>(1);
	}
	else if (GameSystem::GetSelectChara(0) == CK_PALADIN) {
		ap = PlayerManager::SetAnimationPlayer<AP_Paradin>(1);
	}
	else if (GameSystem::GetSelectChara(0) == CK_FIGTHER) {
		ap = PlayerManager::SetAnimationPlayer<AP_Fighter>(1);
	}
	else if (GameSystem::GetSelectChara(0) == CK_WIZARD) {
		ap = PlayerManager::SetAnimationPlayer<AP_Wizard>(1);
	}

	ap->SetCharacterMode(CM_PLAYER);
	ap->Init();

	//ap[i]->SetCharacterMode(CM_PLAYER);//プレーヤーモード割り当て
	//ap->SetCharacterMode(CM_CPU);//プレーヤーモード割り当て

	//UIWindow* pUI = ImageManager::SetUIWindow({ 100,100 }, { 100,100 }, 0);

	//カメラ設定
	Camera* pC = Manager::GetCamera();
	pC->SetPlayer(ap);
	pC->Init();

	//テスト用ステージ割り当て
	StageSystem::SetStage(SK_BATTLEFIELD);


	switch (currentStage)
	{

	case 1:

		//敵の選択、生成

		//敵のセット
		eAP = PlayerManager::SetAnimationPlayer<AnimationPlayer>(2);


		//ゲームシステムにセットする
		GameSystem::SetSelectChara(1, CK_TEST);

		GameSystem::SetisJoin(1, true);
		GameSystem::SetisJoin(2, false);
		GameSystem::SetisJoin(3, false);

		eAP->SetCharacterMode(CM_CPU);
		eAP->Init();

		//画像の生成
		stageNumTex = ImageManager::SetImageD({ SCREEN_WIDTH / 2,70 }, { 300,60 }, ImageManager::GetTexId(TEX_UI_STAGE1), 0);

		break;

	case 2:
		//敵のセット
		eAP = PlayerManager::SetAnimationPlayer<AP_Paradin>(2);


		//ゲームシステムにセットする
		GameSystem::SetSelectChara(1, CK_PALADIN);

		GameSystem::SetisJoin(1, true);
		GameSystem::SetisJoin(2, false);
		GameSystem::SetisJoin(3, false);

		eAP->SetCharacterMode(CM_CPU);
		eAP->Init();

		//画像の生成
		stageNumTex = ImageManager::SetImageD({ SCREEN_WIDTH / 2,70 }, { 300,60 }, ImageManager::GetTexId(TEX_UI_STAGE2), 0);


		break;

	case 3:
		//敵のセット
		eAP = PlayerManager::SetAnimationPlayer<AP_Wizard>(2);
		//ゲームシステムにセットする
		GameSystem::SetSelectChara(1, CK_WIZARD);

		eAP2 = PlayerManager::SetAnimationPlayer<AP_Fighter>(3);
		//ゲームシステムにセットする
		GameSystem::SetSelectChara(2, CK_FIGTHER);


		GameSystem::SetisJoin(1, true);
		GameSystem::SetisJoin(2, true);
		GameSystem::SetisJoin(3, false);

		eAP->SetCharacterMode(CM_CPU);
		eAP->Init();

		eAP2->SetCharacterMode(CM_CPU);
		eAP2->Init();


		//画像の生成
		stageNumTex = ImageManager::SetImageD({ SCREEN_WIDTH / 2,70 }, { 300,60 }, ImageManager::GetTexId(TEX_UI_STAGE3), 0);


		break;

	case 4:
		//敵のセット
		eAP = PlayerManager::SetAnimationPlayer<AP_Paradin>(2);
		//ゲームシステムにセットする
		GameSystem::SetSelectChara(1, CK_TEST);

		eAP2 = PlayerManager::SetAnimationPlayer<AP_Fighter>(3);
		//ゲームシステムにセットする
		GameSystem::SetSelectChara(2, CK_FIGTHER);

		eAP3 = PlayerManager::SetAnimationPlayer<AnimationPlayer>(4);
		//ゲームシステムにセットする
		GameSystem::SetSelectChara(3, CK_PALADIN);

		GameSystem::SetisJoin(1, true);
		GameSystem::SetisJoin(2, true);
		GameSystem::SetisJoin(3, true);

		eAP->SetCharacterMode(CM_CPU);
		eAP->Init();

		eAP2->SetCharacterMode(CM_CPU);
		eAP2->Init();

		eAP3->SetCharacterMode(CM_CPU);
		eAP3->Init();

		//画像の生成
		stageNumTex = ImageManager::SetImageD({ SCREEN_WIDTH / 2,70 }, { 300,60 }, ImageManager::GetTexId(TEX_UI_STAGE4), 0);



		break;

	case STAGEEND:

		///クリア画像を出す


		//とりあえず戻す
		Scene::SetCurrentScene(SCENE_TITLE, 180);

		break;
	}


}

void GameSystem::ReadyGoUpdate() {

	switch (readyGoSw)
	{

	case 0:
		
		stagePlaying = false;

		readyGo = ImageManager::SetImageD({ SCREEN_WIDTH / 2,200 }, { 500,150 }, ImageManager::GetTexId(TEX_UI_READY), 0);

		readyGoSw = 1;

		break;
	case 1:

		if (stageNumTex != nullptr) {
			float alpha = 1 - (alphaCount / 360.0f);
			stageNumTex->SetColor({ 1,1,1,alpha });
			stageNumTex->UpdateVertex();
			alphaCount++;
		}

		if (timer > READYTIME) {
			readyGo->SetTexture(ImageManager::GetTexId(TEX_UI_GO));

			readyGoSw = 2;

			stagePlaying = true;

			if (stageNumTex != nullptr) {
				alphaCount = 60;
				float alpha = 1 - (alphaCount / 360.0f);
				stageNumTex->SetColor({ 1,1,1,alpha });
				stageNumTex->UpdateVertex();

			}

		}
		break;
	case 2:

		if (stageNumTex != nullptr) {
			float alpha = 1 - (alphaCount / 120.0f);
			stageNumTex->SetColor({ 1,1,1,alpha });
			stageNumTex->UpdateVertex();
			alphaCount++;
		}

		if (timer > READYTIME + GOTIME) {
			readyGo->SetIsUse(false);

			readyGoSw = 3;

		}

		break;
	default:
		break;
	}




}
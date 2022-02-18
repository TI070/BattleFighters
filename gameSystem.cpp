
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
int* GameSystem::charaCurrentStock[4];//�e�L�����̌��ݎc�@
bool GameSystem::stagePlaying;
int GameSystem::timer;//�^�C�}�[�i�b�P�ʁj
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

		//�P�l�p���[�h�̎��͑I���L����������������Ȃ��悤�ɂ���
		if (i == 0 && currentStage >= 1) continue;

		charaCurrentStock[i] = nullptr;
		selectChara[i] = CHARAKIND_MAX;
		//isJoin[i] = false;
	}

	//�ꎞ�Ώ�
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

//�Q�[���Z�b�g����@�Q�[���Z�b�g�Ȃ�true��Ԃ�
bool GameSystem::JudgeGameSet() {

	int lifeCharaNum = 0;
	
	//�X�g�b�N�������l�c���Ă��邩�𔻒肷��
	for (int i = 0; i < 4; i++) {
		if (charaCurrentStock[i] == nullptr) continue;
		if (*charaCurrentStock[i] > 0) lifeCharaNum++;
	}

	//��l�ȏ�c���Ă�����I���Ȃ�
	if (lifeCharaNum >= 2) return false;

	//�c��O�l�Ȃ�������� ��ōl���܂�

	//��l�Ȃ珟��
	if (lifeCharaNum == 1) {
		GameSet();
	}


	return true;

}


void GameSystem::GameSet() {

	//�X�e�[�W�v���C���~�߂�
	stagePlaying = false;

	//�Q�[���Z�b�g���������b�Z�[�W�摜���o��
	ImageManager::SetImage({ SCREEN_WIDTH / 2, SCREEN_HEIGHT/2 - 100 }, { 450,150 }, ImageManager::GetTexId(TEX_UI_GAMESET), 0);

	//���ʕt�� ���u��
	SetRankStock();

	//��ʑJ�ڂ��Ă�
	//currentStage���Q�Ƃ��āA�P�l���[�h���ΐ탂�[�h���ŏ����𕪂���
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
		//�Q�����Ă���v���C���[�̃X�e�[�^�X������������
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

//�X�g�b�N���ł̏��ʃZ�b�g
void GameSystem::SetRankStock() {

	//���ꂪ�Ăяo���ꂽ�^�C�~���O�Ő������Ԃ��Q�Ƃ��ď��ʕt��������

	struct stockData {
		int d_rank;
		int playerNum;
		int surviveTime;
	};

	stockData sData[4];
	stockData dummy;
	int cnt = 0;
	//�������Ԃ��Z�b�g
	for (int i = 0; i < 4; i++) {
		if (charaCurrentStock[i] == nullptr) continue;
		sData[cnt].surviveTime = resultData[i].surviveTime;
		sData[cnt].playerNum = i;
		cnt++;
	}

	//�\�[�g
	for (int i = 0; i < cnt; i++) {
		for (int j = i + 1; j < cnt; j++) {
			if (sData[i].surviveTime > sData[j].surviveTime) {
			 	dummy = sData[i];
				sData[i] = sData[j];
				sData[j] = dummy;
			}
		}
	}

	//���ʕt��
	for (int i = 0; i < cnt; i++) {
		sData[i].d_rank = i + 1;
	}

	//resultData�ɕԂ�
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

	//�v���C���[�̃Z�b�g
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

	//ap[i]->SetCharacterMode(CM_PLAYER);//�v���[���[���[�h���蓖��
	//ap->SetCharacterMode(CM_CPU);//�v���[���[���[�h���蓖��

	//UIWindow* pUI = ImageManager::SetUIWindow({ 100,100 }, { 100,100 }, 0);

	//�J�����ݒ�
	Camera* pC = Manager::GetCamera();
	pC->SetPlayer(ap);
	pC->Init();

	//�e�X�g�p�X�e�[�W���蓖��
	StageSystem::SetStage(SK_BATTLEFIELD);


	switch (currentStage)
	{

	case 1:

		//�G�̑I���A����

		//�G�̃Z�b�g
		eAP = PlayerManager::SetAnimationPlayer<AnimationPlayer>(2);


		//�Q�[���V�X�e���ɃZ�b�g����
		GameSystem::SetSelectChara(1, CK_TEST);

		GameSystem::SetisJoin(1, true);
		GameSystem::SetisJoin(2, false);
		GameSystem::SetisJoin(3, false);

		eAP->SetCharacterMode(CM_CPU);
		eAP->Init();

		//�摜�̐���
		stageNumTex = ImageManager::SetImageD({ SCREEN_WIDTH / 2,70 }, { 300,60 }, ImageManager::GetTexId(TEX_UI_STAGE1), 0);

		break;

	case 2:
		//�G�̃Z�b�g
		eAP = PlayerManager::SetAnimationPlayer<AP_Paradin>(2);


		//�Q�[���V�X�e���ɃZ�b�g����
		GameSystem::SetSelectChara(1, CK_PALADIN);

		GameSystem::SetisJoin(1, true);
		GameSystem::SetisJoin(2, false);
		GameSystem::SetisJoin(3, false);

		eAP->SetCharacterMode(CM_CPU);
		eAP->Init();

		//�摜�̐���
		stageNumTex = ImageManager::SetImageD({ SCREEN_WIDTH / 2,70 }, { 300,60 }, ImageManager::GetTexId(TEX_UI_STAGE2), 0);


		break;

	case 3:
		//�G�̃Z�b�g
		eAP = PlayerManager::SetAnimationPlayer<AP_Wizard>(2);
		//�Q�[���V�X�e���ɃZ�b�g����
		GameSystem::SetSelectChara(1, CK_WIZARD);

		eAP2 = PlayerManager::SetAnimationPlayer<AP_Fighter>(3);
		//�Q�[���V�X�e���ɃZ�b�g����
		GameSystem::SetSelectChara(2, CK_FIGTHER);


		GameSystem::SetisJoin(1, true);
		GameSystem::SetisJoin(2, true);
		GameSystem::SetisJoin(3, false);

		eAP->SetCharacterMode(CM_CPU);
		eAP->Init();

		eAP2->SetCharacterMode(CM_CPU);
		eAP2->Init();


		//�摜�̐���
		stageNumTex = ImageManager::SetImageD({ SCREEN_WIDTH / 2,70 }, { 300,60 }, ImageManager::GetTexId(TEX_UI_STAGE3), 0);


		break;

	case 4:
		//�G�̃Z�b�g
		eAP = PlayerManager::SetAnimationPlayer<AP_Paradin>(2);
		//�Q�[���V�X�e���ɃZ�b�g����
		GameSystem::SetSelectChara(1, CK_TEST);

		eAP2 = PlayerManager::SetAnimationPlayer<AP_Fighter>(3);
		//�Q�[���V�X�e���ɃZ�b�g����
		GameSystem::SetSelectChara(2, CK_FIGTHER);

		eAP3 = PlayerManager::SetAnimationPlayer<AnimationPlayer>(4);
		//�Q�[���V�X�e���ɃZ�b�g����
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

		//�摜�̐���
		stageNumTex = ImageManager::SetImageD({ SCREEN_WIDTH / 2,70 }, { 300,60 }, ImageManager::GetTexId(TEX_UI_STAGE4), 0);



		break;

	case STAGEEND:

		///�N���A�摜���o��


		//�Ƃ肠�����߂�
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
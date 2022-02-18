#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "polygon2d.h"

#include "field.h"
#include "camera.h"
#include "wall.h"

#include "key_manager.h"
#include "keyboard.h"

#include "imageManager.h"
#include "damageObjectManager.h"
#include "collisionSystem.h"

#include "billboard.h"
#include "time.h"

#include "texture.h"
#include "explosion.h"
#include "effectManager.h"
#include "writestring.h"

#include "scene.h"
#include "title.h"
#include "result.h"
#include "fade.h"

#include "audio.h"
#include "rendererManager.h"

#include "filecontroll.h"
#include "gameMenu.h"

#include "systemMessage.h"
#include "gameSystem.h"

#include "backGround.h"

#include "animationModel.h"
#include "animationPlayer.h"
#include "meshFieldBoxManager.h"
#include "playerManager.h"
#include "UIwindow.h"

#include "stageSystem.h"
#include "audioManager.h"
#include "BgObjectManager.h"
#include "controller.h"

#include <list>


std::list<GameObject*> g_GameObject;//STL�̃��X�g�\�� �f�X�g���N�^���Ă΂�Ȃ��Ə�����Ȃ��Ƃ�

Camera* Manager::pCamera;
Player* Manager::pPlayer;
BackGround* Manager::pBackGround;
GAMEMODE Manager::gameMode;
Audio* Manager::testA;

AnimationModel* Manager::aModel;
int Manager::aModel_frame = 0;

//AnimationPlayer* Manager::pAPlayer;
//MeshFieldBoxManager* Manager::mfbManager;

void Manager::Init()
{

	//�V�X�e���̏�����
	Keyboard_Initialize();
	KeyManager_Initilalize();
	Texture_Initialize();

	srand((unsigned)time(NULL));

	Audio::InitMaster();
	AudioManager::Init();

	ControllerManager::Init();

	writestring::Initialize();

	Renderer::Init();
	
	RendererManager::Init();

	ImageManager::Init();

	GameSystem::Init();

	//StageSystem::SetStage(SK_BATTLEFIELD);

	Title::Initialize();




	pCamera = new Camera;

	DamageObjectManager::Init();
	DamageObjectManager::SetCamera(pCamera);

	//�r���{�[�h�Ƀr���[�s���ݒ肷��
	Billboard::BillBoard_SetViewMatrix(pCamera->GetViewMatrix());

	srand((unsigned int)time(NULL)); // ���ݎ����̏��ŏ�����

	PlayerManager::SetCamera(pCamera);
	//pAPlayer = new AnimationPlayer(pCamera);


	aModel_frame = 0;

	Fade::Fade_Initialize();
}


void Manager::Uninit()
{
	
	
}

void Manager::Update()
{
	////���ʏ���
	//�V�X�e������
	KeyManager_Update();
	ControllerManager::Update();

	switch (Scene::GetCurrentScene())
	{
	case SCENE_TITLE:

		//ControllerManager::Update();

		Title::Update();

		ImageManager::Update();
		EffectManager::Update();


		break;

	case SCENE_GAME:

		//�I�u�W�F�N�g
		pCamera->Update();

		//pAPlayer->Update();
		//ControllerManager::Update();

		PlayerManager::Update();

		DamageObjectManager::Update();

		CollisionSystem::Update();

		EffectManager::Update();

		//UI

		ImageManager::Update();

		//�V�X�e��
		GameSystem::Update();

		break;

	case SCENE_RESULT:

		pCamera->Update();

		Result::Update();



		break;


	default:
		break;
	}



	Scene::ChangeSceneFade();


	//aModel_frame++;

}

void Manager::Draw()
{
	Renderer::Begin();

	switch (Scene::GetCurrentScene())
	{
	case SCENE_TITLE:

		ControllerManager::DebugDraw();

		Title::Draw();


		EffectManager::Draw();

		ImageManager::Draw();

		break;

	case SCENE_GAME:



		pCamera->Draw();

		Billboard::BillBoard_SetViewMatrix(pCamera->GetViewMatrix());

		StageSystem::Draw();
		//pAPlayer->Draw();
		PlayerManager::Draw();

		MeshFieldBoxManager::Draw();
		DamageObjectManager::Draw();

		BgObjectManager::Draw();


		EffectManager::Draw();

		ImageManager::Draw();



		break;

	case SCENE_RESULT:

		pCamera->Draw();

		StageSystem::Draw();

		Result::Draw();

		MeshFieldBoxManager::Draw();

		ImageManager::Draw();

		break;


	default:
		break;
	}

	Fade::Fade_Draw();

	Renderer::End();

}

void Manager::GameInit() {


	//�L�����Z���N�g�𔽉f
	AnimationPlayer* ap[4];
	for (int i = 0; i < 4; i++) {

		//�Q�����Ă��Ȃ���Ώ������Ȃ�
		if (!GameSystem::GetisJoin(i)) continue;

		if (GameSystem::GetSelectChara(i) == CK_TEST) {
			ap[i] = PlayerManager::SetAnimationPlayer<AnimationPlayer>(i + 1);
		}
		else if (GameSystem::GetSelectChara(i) == CK_PALADIN) {
			ap[i] = PlayerManager::SetAnimationPlayer<AP_Paradin>(i + 1);
		}
		else if (GameSystem::GetSelectChara(i) == CK_FIGTHER) {
			ap[i] = PlayerManager::SetAnimationPlayer<AP_Fighter>(i + 1);
		}
		else if (GameSystem::GetSelectChara(i) == CK_WIZARD) {
			ap[i] = PlayerManager::SetAnimationPlayer<AP_Wizard>(i + 1);
		}

		else if (GameSystem::GetSelectChara(i) == CHARAKIND_MAX) {
			int r = rand() % CHARAKIND_MAX;

			//�Q�[���V�X�e���ɃZ�b�g����
			GameSystem::SetSelectChara(i, (CHARA_KIND)r);

			switch (r)
			{
			case CK_TEST:
				ap[i] = PlayerManager::SetAnimationPlayer<AnimationPlayer>(i + 1);
				break;
			case CK_PALADIN:
				ap[i] = PlayerManager::SetAnimationPlayer<AP_Paradin>(i + 1);
				break;
			case CK_FIGTHER:
				ap[i] = PlayerManager::SetAnimationPlayer<AP_Fighter>(i + 1);
				break;
			case CK_WIZARD:
				ap[i] = PlayerManager::SetAnimationPlayer<AP_Wizard>(i + 1);
				break;
			default:
				break;
			}
		}

		//ap[i]->SetCharacterMode(CM_PLAYER);//�v���[���[���[�h���蓖��
		//ap->SetCharacterMode(CM_CPU);//�v���[���[���[�h���蓖��
		ap[i]->SetCharacterMode(GameSystem::GetPlayerMode(i));
	}

	pCamera->SetPlayer(ap[0]);
	pCamera->Init();


	for (int i = 0; i < 4; i++) {

		//�Q�����Ă��Ȃ���Ώ������Ȃ�
		if (!GameSystem::GetisJoin(i)) continue;
		ap[i]->Init();
	}
	//UIWindow* pUI = ImageManager::SetUIWindow({ 100,100 }, { 100,100 }, 0);

	//�e�X�g�p�X�e�[�W���蓖��
	StageSystem::SetStage(SK_BATTLEFIELD);
	//StageSystem::SetStage(SK_LASTSTOP);


}
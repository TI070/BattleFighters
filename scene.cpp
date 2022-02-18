
#include "scene.h"

#include "fade.h"
#include "imageManager.h"
#include "effectManager.h"
#include "Manager.h"
#include "title.h"
#include "result.h"
#include "backGround.h"
#include "systemMessage.h"
#include "playerManager.h"
#include "gameSystem.h"
#include "meshFieldBoxManager.h"


SCENE_KIND Scene::CurrentScene = SCENE_TITLE;
SCENE_KIND Scene::reserveScene;
int Scene::mode;
int Scene::changeDelay = 0;


SCENE_KIND Scene::GetCurrentScene() {
	return CurrentScene;
}


void Scene::SetCurrentScene(SCENE_KIND SetScene) {

	mode = 1;

	//CurrentScene = SetScene;

	//�\��
	reserveScene = SetScene;

	//�t�F�[�h�J�n
	Fade::Fade_Start(FADE_OUT, 1, 1, 1, 30);
	
}

void Scene::SetCurrentScene(SCENE_KIND SetScene, int s_delay) {

	mode = 1;

	//�\��
	reserveScene = SetScene;

	//�t�F�[�h�J�n
	Fade::Fade_Start(FADE_OUT, 1, 1, 1, s_delay);

	//�f�B���C�ݒ�
	//changeDelay = s_delay;

}

void Scene::ChangeSceneFade() {

	if (mode == 1) {//�t�F�[�h�C��
		if (CurrentScene != reserveScene) {//��v���Ă��Ȃ�������

			Fade::Fade_Update();
			if (!Fade::Fade_IsFade()) {//����

				//if (CurrentScene == TITLE) stage::UnInit();

				SceneEndFunc(CurrentScene);
				SceneStartFunc(reserveScene);

				CurrentScene = reserveScene;
				Fade::Fade_Start(FADE_IN, 1, 1, 1, 30);
				mode = 2;

			}
		}
	}
	else if (mode == 2) {//�t�F�[�h�A�E�g

		Fade::Fade_Update();
		if (!Fade::Fade_IsFade()) {//����

			mode = 0;
		}

	}


}

void Scene::SceneEndFunc(SCENE_KIND endScene) {

	switch (endScene)
	{
	case SCENE_TITLE:
		SystemMessage::Uninit();
		ImageManager::Uninit();
		Title::Uninit();

		EffectManager::Uninit();
		break;
	case SCENE_GAME:
		//GameSystem::Init();
		SystemMessage::Uninit();
		ImageManager::Uninit();
		EffectManager::Uninit();

		Manager::GameUninit();
		PlayerManager::Uninit();
		MeshFieldBoxManager::Uninit();

		break;
	case SCENE_RESULT:
		ImageManager::Uninit();
		Result::Uninit();
		MeshFieldBoxManager::Uninit();

		break;
	case SCENE_MAX:
		break;
	default:
		break;
	}

}

void Scene::SceneStartFunc(SCENE_KIND endScene) {

	switch (endScene)
	{
	case SCENE_TITLE:
		Title::Initialize();
		Manager::TitleInit();

		//�Ƃ肠����
		GameSystem::Init();

		break;
	case SCENE_GAME:

		if(GameSystem::GetCurrentStage() == 0) Manager::GameInit();
		else GameSystem::OnePlayerInit();

		break;
	case SCENE_RESULT:
		Result::Initialize();

		break;
	case SCENE_MAX:
		break;
	default:
		break;
	}

}

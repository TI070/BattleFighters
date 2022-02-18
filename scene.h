#pragma once

#include "manager.h"

enum SCENE_KIND {
	SCENE_TITLE,
	SCENE_GAME,
	SCENE_RESULT,
	SCENE_MAX
};

class Scene {//static�N���X

private:

	static SCENE_KIND CurrentScene;
	static SCENE_KIND reserveScene;
	//static SCENE_KIND endScene;
	static int mode;
	static int changeDelay;

public:

	static SCENE_KIND GetCurrentScene();
	static SCENE_KIND GetReserveScene() { return reserveScene; }

	static void SetCurrentScene(SCENE_KIND SetScene);
	static void SetCurrentScene(SCENE_KIND SetScene,int s_delay);
	static void ChangeSceneFade();

	static void SceneEndFunc(SCENE_KIND endScene);//�V�[���؂�ւ�莞�̌㏈��
	static void SceneStartFunc(SCENE_KIND endScene);//�V�[���؂�ւ�莞�̌㏈��

};
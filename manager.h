#pragma once

#include <vector>
#include <typeinfo>

class Camera;
class Player;
class AnimationPlayer;
class BackGround;
class Audio;
class AnimationModel;
class MeshFieldBoxManager;

enum GAMEMODE {

	GM_NEWGAME,
	GM_LOADGAME

};

class Manager
{

private:

	static Camera* pCamera;
	static Player* pPlayer;
	static BackGround* pBackGround;

	static GAMEMODE gameMode;
	static Audio* testA;

	static AnimationModel* aModel;

	static int aModel_frame;


public:

	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static void GameInit();
	static void GameUninit() {};

	static void SetGameMode(GAMEMODE s_gm) { gameMode = s_gm; }
	
	static void TitleInit() {};

	static Camera* GetCamera() { return pCamera; }

	//template <typename T>

	//std::vector<T*> GetGameObjects() {
	//
	//	std::vector<T*> objects;//STL‚Ì”z—ñ
	//	for (GameObject* object : m_GameObject) {
	//
	//		if (typeid(*object) == typeid(T)) {
	//			objects.push_back((T*)object);
	//
	//		}
	//
	//		return objects;
	//
	//	}
	//}



};
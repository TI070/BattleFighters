#pragma once

#include <list>
#include "animationPlayer.h"
#include "managerBase.h"
#include "stageSystem.h"

class PlayerManager : public ManagerBase {

private:
	static std::list<AnimationPlayer*> pList;
	static Camera* pCamera;

public:
	static void Draw();
	static void Update();
	static void Uninit();

	//AnimationPlayer* SetAnimationPlayer();
	static void SetCamera(Camera* s_camera) { pCamera = s_camera; }
	static std::list<AnimationPlayer*> GetPlayerList() { return pList; }

	template <typename T>
	static AnimationPlayer* SetAnimationPlayer(int s_playerNum){

		//Bullet* pBullet = new T(s_pos, s_size, BULLET_TEST_PASS, s_vec, s_owner);
		//pList.push_back(pBullet);

		//return pBullet;

		AnimationPlayer* ap = new T(pCamera);
		ap->SetplayerNumber(s_playerNum);
		//ap->Init();
		pList.push_back(ap);
		return ap;

	}

	//��ԋ߂����v���C���[��T��
	static AnimationPlayer* GetNearPlayer(AnimationPlayer* s_player);

	//�G���A���ɂ���v���[���[���擾����
	static AnimationPlayer* GetPlayerinArea(Area s_area, AnimationPlayer* me);

};

#include "playerManager.h"


Camera* PlayerManager::pCamera;
std::list<AnimationPlayer*> PlayerManager::pList;

void PlayerManager::Draw() {

	for (AnimationPlayer* object : pList) {

		object->Draw();

	}
}


void PlayerManager::Update() {

	for (AnimationPlayer* object : pList) {

		object->Update();

	}

	RemoveMember<AnimationPlayer>(&pList);

}

void PlayerManager::Uninit() {

	for (AnimationPlayer* object : pList) {

		object->Uninit();
		delete object;
	}
	pList.clear();//���X�g�̍폜

}

/*
AnimationPlayer* PlayerManager::SetAnimationPlayer() {

	AnimationPlayer* ap = new AnimationPlayer(pCamera);
	ap->Init();
	pList.push_back(ap);
	return mb;
}*/

AnimationPlayer* PlayerManager::GetNearPlayer(AnimationPlayer* s_player){

	float distance = 999999;
	AnimationPlayer* nearPlayer = nullptr;

	for (AnimationPlayer* object : pList) {

		//�����������珈�����Ȃ�
		if (object == s_player) continue;

		D3DXVECTOR3 vec = s_player->GetPosition() - object->GetPosition();
		float dis = D3DXVec3Length(&vec);

		if (distance > dis) {
			distance = dis;
			nearPlayer = object;
		}

	}

	return nearPlayer;

}

AnimationPlayer* PlayerManager::GetPlayerinArea(Area s_area, AnimationPlayer* me) {

	AnimationPlayer* inAreaPlayer = nullptr;

	for (AnimationPlayer* object : pList) {

		//�����������珈�����Ȃ�
		if (object == me) continue;

		//�G���A�̒����`�F�b�N
		if (s_area.CheckInArea({ object->GetPosition().x,object->GetPosition().y })) {

			inAreaPlayer = object;
			break;
		}

	}

	return inAreaPlayer;

}
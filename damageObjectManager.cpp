
#include "damageObjectManager.h"
#include "damageObject.h"
#include "animationPlayer.h"

std::list<DamageObject*> DamageObjectManager::pList;
Camera* DamageObjectManager::pCamera;

void DamageObjectManager::Init() {


}

void DamageObjectManager::Uninit() {


}

void DamageObjectManager::Update() {

	for (DamageObject* object : pList) {

		object->Update();

	}

	RemoveMember<DamageObject>(&pList);

}

void DamageObjectManager::Draw() {

	for (DamageObject* object : pList) {

		object->Draw();

	}

}

DamageObject* DamageObjectManager::SetDamageObject(D3DXVECTOR3 s_pos, AnimationPlayer* s_owner) {

	DamageObject* pObj = new DamageObject(s_pos,s_owner);
	pObj->Init();
	pObj->SetCamera(pCamera);
	pList.push_back(pObj);

	return pObj;

}

ShieldObject* DamageObjectManager::SetShieldObject(D3DXVECTOR3 s_pos, AnimationPlayer* s_owner) {

	ShieldObject* pObj = new ShieldObject(s_pos, s_owner);
	pObj->Init();
	pObj->SetCamera(pCamera);
	pList.push_back(pObj);

	return pObj;

}




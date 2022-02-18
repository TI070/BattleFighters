
#include "BgObjectManager.h"

std::list<BgObject*> BgObjectManager::pList;

void BgObjectManager::Init() {



}

void BgObjectManager::Uninit() {

	for (BgObject* object : pList) {

		object->Uninit();
		delete object;
	}
	pList.clear();//ƒŠƒXƒg‚Ìíœ

}

void BgObjectManager::Draw() {

	for (BgObject* object : pList) {

		object->Draw();

	}

}

BgObject* BgObjectManager::SetBgObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale) {

	BgObject* pObj = new BgObject;
	pObj->Init();
	pObj->SetPosition(pos);
	pObj->SetRotation(rot);
	pObj->SetScale(scale);

	pList.push_back(pObj);

	return pObj;

}
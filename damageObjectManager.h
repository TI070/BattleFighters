#pragma once

#include <list>
#include "config.h"
#include "managerBase.h"
#include "main.h"

class DamageObject;
class ShieldObject;
class Character;
class AnimationPlayer;
class Camera;

class DamageObjectManager : public ManagerBase
{

private:

	static std::list<DamageObject*> pList;
	static Camera* pCamera;
	//static Field* pField;

public:

	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static DamageObject* SetDamageObject(D3DXVECTOR3 pos, AnimationPlayer* s_owner);
	static ShieldObject* SetShieldObject(D3DXVECTOR3 pos, AnimationPlayer* s_owner);

	static void SetCamera(Camera* s_camera) { pCamera = s_camera; }
	static std::list<DamageObject*> GetDamageObjectList() { return pList; }

	//static void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 s_vec,Character* s_owner);

	/*
	template <typename T>
	static Bullet* SetBulletT(D3DXVECTOR3 s_pos, D3DXVECTOR3 s_vec,float s_size ,Character* s_owner) {

		Bullet* pBullet = new T(s_pos, s_size, BULLET_TEST_PASS, s_vec, s_owner);
		pList.push_back(pBullet);

		return pBullet;
	}

	static std::list<Bullet*> GetBulletList();
	*/

	template <typename T>
	static DamageObject* SetDObjT(D3DXVECTOR3 s_pos, AnimationPlayer* s_owner) {

		T* pObj = new T(s_pos, s_owner);
		pObj->Init();
		pObj->SetCamera(pCamera);
		pList.push_back(pObj);

		return pObj;
	}

};
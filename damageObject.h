#pragma once

#include "gameObject.h"
#include "model.h"
#include "audioManager.h"


enum DAMAGEOBJ_KIND {

	DOBJ_NORMAL,
	DOBJ_SHIELD,
	DOBJ_FIRE,
	DOBJ_THUNDER,
	DOBJ_ICE,
	DOBJ_WATER
};

enum DAMAGEOBJ_DIR {
	DOD_NONE,
	DOD_RIGHT,
	DOD_LEFT

};


class AnimationPlayer;
class Model;
class Camera;

class DamageObject : public GameObject {

protected:
	
	AnimationPlayer* owner;
	Model pModel;

	int damage;//ダメージ量
	float blowPower = 0;//吹っ飛ばし力
	int lifeTime;//生存時間
	Camera* pCamera;

	D3DXVECTOR3 velocity = {0,0,0};
	D3DXVECTOR2 blowVec = {0,0};//吹っ飛ばしベクトル

	bool followOwner = false;//オーナーの位置を追従するか
	D3DXVECTOR3 followOffset;//追従する場合のオフセット

	D3DXVECTOR3 rollSpeed = { 0,0,0 };
	bool rollFlag;

	bool isDraw = false;
	int delayTime = 0;//有効になるまでの時間

	AnimationPlayer* target = nullptr;

	DAMAGEOBJ_DIR direction = DOD_NONE;

	AUDIO_KIND hitSoundKind = AUDIOKIND_MAX;

public:

	DamageObject(D3DXVECTOR3 s_pos, AnimationPlayer* s_AP) {

		m_Position = s_pos;
		owner = s_AP;

	}

	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	void SetCamera(Camera* s_camera) { pCamera = s_camera; }
	void SetDamage(int s_damage) { damage = s_damage; }
	void SetLifeTime(int s_lifeTime) { lifeTime = s_lifeTime; }
	void SetVelocity(D3DXVECTOR3 s_v) { velocity = s_v; }
	void SetBlowPower(float s_bp) { blowPower = s_bp; }
	void SetBlowVec(D3DXVECTOR2 s_v) { blowVec = s_v; }
	void SetRollSpeed(D3DXVECTOR3 s_r) { 
		rollSpeed = s_r;
		rollFlag = true;
	}
	void SetDelayTime(int s_time) { delayTime = s_time; }
	void SetisDraw(bool s_bool) { isDraw = s_bool; }
	void SetDirection(DAMAGEOBJ_DIR s_dir) { direction = s_dir; }

	float GetBlowPower() { return blowPower; }
	float GetDamage() { return damage; }
	D3DXVECTOR2 GetBlowVec() { return blowVec; }

	AnimationPlayer* GetOwner() { return owner; }

	bool CheckLifeTime();//lifeが0より上ならtrue
	virtual void Move();
	void FollowOwnerOn();
	void FollowOwner();
	virtual DAMAGEOBJ_KIND GetObjKind() { return DOBJ_NORMAL; }

	bool CheckExist() { 
		if (delayTime > 0) return false;
		return true;
	}

	void SetTarget(AnimationPlayer* s_target) { target = s_target; }

	void SetSoundKind(AUDIO_KIND s_ak) { hitSoundKind = s_ak; }
	AUDIO_KIND GetSoundKind() { return hitSoundKind; }

};

class ShieldObject : public DamageObject {

public:

	ShieldObject(D3DXVECTOR3 s_pos, AnimationPlayer* s_AP) : DamageObject(s_pos, s_AP) {


	}

	void Init();
	void Draw();
	DAMAGEOBJ_KIND GetObjKind() { return DOBJ_SHIELD; }

	

};


class ArrowObject : public DamageObject {

public:
	ArrowObject(D3DXVECTOR3 s_pos, AnimationPlayer* s_AP) : DamageObject(s_pos, s_AP) {


	}

	void Init();
	//void Draw();
	DAMAGEOBJ_KIND GetObjKind() { return DOBJ_NORMAL; }

};

class FireballObject : public DamageObject {

private:
	

public:
	FireballObject(D3DXVECTOR3 s_pos, AnimationPlayer* s_AP) : DamageObject(s_pos, s_AP) {
		
	}

	void Init();
	void Update();
	//void Draw();
	void Move();
	DAMAGEOBJ_KIND GetObjKind() { return DOBJ_FIRE; }

};

class ThunderObject : public DamageObject {

private:


public:
	ThunderObject(D3DXVECTOR3 s_pos, AnimationPlayer* s_AP) : DamageObject(s_pos, s_AP) {

	}

	void Init();
	//void Draw();
	DAMAGEOBJ_KIND GetObjKind() { return DOBJ_THUNDER; }

};

class IceObject : public DamageObject {

private:


public:
	IceObject(D3DXVECTOR3 s_pos, AnimationPlayer* s_AP) : DamageObject(s_pos, s_AP) {

	}

	void Init();
	void Move();
	void Draw();
	DAMAGEOBJ_KIND GetObjKind() { return DOBJ_THUNDER; }

};

class WaterObject : public DamageObject {

private:


public:
	WaterObject(D3DXVECTOR3 s_pos, AnimationPlayer* s_AP) : DamageObject(s_pos, s_AP) {

	}

	void Init();
	//void Draw();
	DAMAGEOBJ_KIND GetObjKind() { return DOBJ_THUNDER; }

};
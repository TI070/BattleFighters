#pragma once
#include "gameObject.h"
#include "animationModel.h"
#include "Model.h"
#include "config.h"

#define CHARA_MOVESPEED_DEFAULT (0.05f)
#define CHARA_ROLLSPEED_DEFAULT (0.05f)

#define CHARA_CLIMEABLE_HEIGHT (0.1f)

//吹っ飛ばし速度減速係数
#define BLOWN_DEC_COEFFICIENT (0.95f)

//ダメージ吹っ飛ばし補正の基準値
#define BLOWN_CORRECT_BASE (100.0f)

//吹っ飛ばし操作不能速度
#define BLOWN_INOPERABLE_SPEED (0.0f)//未設定

//ダメージを受けた時の無敵時間
#define DAMAGE_INVINCIBLETIME (10)

//復活時の無敵時間
#define RESPAWN_INVINCIBLETIME (60)

//崖つかみクールタイム
#define CRIFFGRAP_COOLTIME (90)


//復活位置
#define RESPAWN_POINT {0,5,0}

//床バウンド閾値
#define BOUND_LIMIT (0.2f)

//シールドブレイク復帰時間
#define SHIELDBREAK_TIME (300)

//崖上がり回避無敵時間
#define CRIFFAVOID_INVINCIBLETIME (20)

//緊急回避無敵時間
#define AVOID_INVINCIBLETIME (20)

//緊急回避硬直時間
#define AVOID_COOLTIME (30)

//UIダメージイージング関数
#define EASING_DAMAGETEXT_TIME (60.0f)

//ダッシュ受付時間
#define DASH_RECEPTIONTIME (10)

//つかみ持続時間
#define GRABHOLD_TIMEMAX (180)

class MeshFieldBoxManager;
class MeshFieldBox;
class Camera;
class Model;
class StatusWindow;
class TexString;
class BattleAI;
class DamageObject;

struct AttackData {
	float range;
	int damage;
	int delay;
	int lifetime;
	D3DXVECTOR3 velocity;
	D3DXVECTOR3 rollSpeed;
	D3DXVECTOR3 scale;
	float blowPower;
	D3DXVECTOR2 blowVec;
	D3DXVECTOR3 offsetPos;
	bool follow;
};


struct CliffGrapData {//つかまっている崖のデータ

	MeshFieldBox* pMBox;
	DIRECTION dir;
};


enum CHARA_DIRECTION {

	CD_RIGHT,
	CD_LEFT

};


enum CHARA_KIND {

	CK_TEST,
	CK_PALADIN,
	CK_FIGTHER,
	CK_WIZARD,
	CHARAKIND_MAX
};


enum CHARA_MODE {

	CM_PLAYER,//プレイヤー操作
	CM_CPU,//CPU操作
	CM_SANDBAG,//動かない
	CHARAMODE_MAX
};

enum CHARA_CONDITION {//特殊状態
	CC_NORMAL,
	CC_SHIELDBREAK,//シールドブレイク
	CC_CLLIFGRAP,//崖つかまり
	CC_AVOID,//緊急回避
	CC_GRABBED,//つかまれ
	CC_GRAB//つかみ
};

enum CHARA_KEY {//操作キー
	CK_UP,
	CK_DOWN,
	CK_RIGHT,
	CK_LEFT,
	CK_JUMP,
	CK_ATTACK,
	CK_SHIELD,

};

enum KEYINPUT_TYPE {

	KEY_PRESS,
	KEY_TRIGGER,
	KEY_RELEASE

};

enum ANIMATION_STATE {

	AS_WAIT,
	AS_RUN,
	AS_JUMP,
	AS_AVOID,
	AS_CLIFF,
	AS_THROW,
	AS_BLOWN,
	AS_NATTACK,
	AS_DOWNATTACK,
	AS_DIRATTACK,
	AS_UPATTACK,
	AS_BREAK

};

enum ATTACK_KIND {

	AK_NEUTRAL,
	AK_DOWN,
	AK_UP,
	AK_DIR,
	AK_MAX
};


class AnimationPlayer : public GameObject {



	
protected:

	Camera* pCamera = nullptr;
	Model pHitBoxModel;
	AnimationModel* a_Model = nullptr;
	ANIMATION_STATE AS = AS_WAIT;
	ANIMATION_STATE PAST_AS = AS_WAIT;

	const char* as_name = "Idle";
	const char* pastAs_name = "Idle";

	int aModel_frame = 0;

	//MeshFieldBoxManager* mfbm;

	bool isJump;

	D3DXVECTOR3 velocity;//移動速度
	D3DXVECTOR3 blownVelocity = {};//吹っ飛ばされ速度

	float landHeight;//設置判定に使う高さ

	D3DXVECTOR4 shaderPlusColor = {0,0,0,0};

	BattleAI* battleAI = nullptr;

	AttackData atkData[AK_MAX];

	//１フレーム前の位置
	D3DXVECTOR3 pastPos;

	int ShieldBreakTime;

	D3DXVECTOR3 modelOffset;
	D3DXVECTOR3 modelOffsetVelocity;
	D3DXVECTOR3 modelRotate;

	bool modelFrameStop;

	int actionCharge = 0;//ため技に使う汎用変数

	
	int shieldLife;
	int shieldLifeMax;

	float moveSpeed;
	float rollSpeed;
	int addJumpCnt;
	int addJumpMax;

	float jumpPower;
	bool throughFlag;
	float weight;

	D3DXVECTOR3 hitScale;
	//D3DXVECTOR3 hitPosition;
	CHARA_KIND charaKind = CK_TEST;
	CHARA_MODE charaMode = CM_SANDBAG;
	CHARA_DIRECTION charaDir = CD_RIGHT;
	CHARA_CONDITION charaCon = CC_NORMAL;

	int playerNumber;//プレイヤー識別番号
	bool isHuman;

	int accumulationDamage = 0;//蓄積ダメージ
	int lifeStock;//残機

	int invincibleTime = 0;//無敵時間

	StatusWindow* StatesWindow;
	TexString* DamageText;

	bool upAttackAble = true;//上技が出せるかどうか
	bool invincibleColorSw = false;
	bool useShield = false;

	int actionCooltime = 0;//硬直
	int jumpCooltime = 0;//ジャンプ不可能時間
	int grapCooltime = 0;//崖つかみクールタイム

	CliffGrapData cliffGrapData = { nullptr,RIGHT};

	//つかんでいる相手
	AnimationPlayer* GrapingPlayer = nullptr;

	//つかみ経過時間
	int grabHoldTime = 0;

	virtual void ModelDataLoad();

	DamageObject* attackObj = nullptr;//攻撃中の攻撃判定

	//UI表示用
	RendererSet UIRenset;
	int damageTextEasingTime = EASING_DAMAGETEXT_TIME;

	//ダッシュ
	bool dashFlag = false;
	int dashReceptionTime = true;


public:

	enum CLIFFACTION {

		CA_UP,//ただ上がる
		CA_ATTACK,//攻撃しながら上がる
		CA_AVOID//回避しながら上がる
	};

	AnimationPlayer(Camera* pC) {

		//moveSpeed = CHARA_MOVESPEED_DEFAULT;
		//rollSpeed = CHARA_ROLLSPEED_DEFAULT;

		pCamera = pC;
		//Init();
	}

	void Init();
	void Uninit();
	void Update();
	void Draw();

	virtual void PlayerUpdate();
	void CPUUpdate();
	void CPUThink();
	void SandbagUpdate();
	void HitBoxUpdate();
	void InvincibleUpdate();

	bool AnimationCharacterMove();
	void CharacterRoll();

	void Jump();
	bool CheckJumping();
	void AddGravity();

	//void SetMeshField(MeshField* s_mf) { mf = s_mf; }
	//void SetMeshFieldBox(MeshFieldBox* s_mfb) { mfb = s_mfb; }
	//void SetMeshFieldBoxManager(MeshFieldBoxManager* s_mfbm) { mfbm = s_mfbm; }
	bool ClimeAbleCheck();

	virtual void Attack();//ニュートラル
	virtual void SubAttack();//ニュートラル 特殊
	virtual void UpAttack();//上
	virtual void SubUpAttack() {};//上 特殊
	virtual void DownAttack();//下
	virtual void SubDownAttack() {};//下 特殊
	virtual void DirAttack();//横
	virtual void SubDirAttack() {};//横 特殊
	virtual void CliffAttack();//崖上がり攻撃
	virtual void SetStates();

	void SetplayerNumber(int s_num) { playerNumber = s_num; }
	int GetplayerNumber() { return playerNumber; }

	void SetIsHuman(bool s_bool) { isHuman = s_bool; }

	void SetCharacterMode(CHARA_MODE s_cm);
	void SetCharacterKind(CHARA_KIND s_ck) { charaKind = s_ck; }
	void SetLifeStock(int s_stock) { lifeStock = s_stock; }
	int GetLifeStock() { return lifeStock; }

	//D3DXVECTOR3 GetHitScale() { return hitScale; }
	//D3DXVECTOR3 GetHitPosition() { return hitPosition; }
	Model* GetHitBox() { return &pHitBoxModel; }
	D3DXVECTOR3 GetHitScale() { return pHitBoxModel.GetScale(); }
	D3DXVECTOR3 GetHitPosition() { return pHitBoxModel.GetPosition(); }


	void BlownCharacter(D3DXVECTOR2 s_vec);//吹っ飛ばされる

	void AddDamage(int s_add) { 
		accumulationDamage += s_add; 
		if (accumulationDamage > 999) accumulationDamage = 999;
		else if (accumulationDamage < 0) accumulationDamage = 0;
	}

	void DamageFunction(int s_add);

	float GetBlownCorrect();//現在のダメージ量から吹っ飛ばし係数を算出し返す
	bool CheckInvincible();//無敵時間中ならtrueを返す
	void SetInvincibleColor();
	CHARA_DIRECTION GetCharaDir() { return charaDir; }
	bool CheckJumpAble();

	D3DXVECTOR3 GetBlownVelocity() { return blownVelocity; }

	void SetCharaCon(CHARA_CONDITION s_cc) { charaCon = s_cc; }
	
	void DamageUIUpdate();

	void DeathFunction();
	void BlownEffect();
	void Respawn();

	void SetCharacterParameter();
	//シールド
	void ShieldFunction();
	void ShieldUp();//シールドを張る
	bool CheckShield();//シールドが有効状態ならtrueを返す
	void ShieldDamage(int s_damage);
	void ShieldBreakUpdate();
	void ShieldBreak();

	void HitBoxInit();

	//プレイヤーナンバーごとに任意の操作キーを押せているかを返す
	bool CheckIputKey(CHARA_KEY cK,KEYINPUT_TYPE KT);

	void SetVelocity(D3DXVECTOR3 s_vec) { velocity = s_vec; }
	D3DXVECTOR3 GetVelocity() { return velocity; }
	float GetMoveSpeed() { return moveSpeed; }
	void SetDirection(CHARA_DIRECTION s_dir) { charaDir = s_dir; }
	//void SetAnimation(ANIMATION_STATE s_anm) { AS = s_anm; }
	void ThroughFloor();
	void SetThroughFlag(bool s_f) { throughFlag = s_f; }
	void SetCharaDir(CHARA_DIRECTION s_cd) { 
		charaDir = s_cd;
		float dirN = 1;
		if (s_cd == CD_LEFT) dirN *= -1;
		DirectionRoll({ dirN,0 }, 270);
	}

	void SetActionCooltime(int s_num) { actionCooltime = s_num; }
	bool ActionCooltimeUpdate();
	bool CheckActionCooltime() {
		if (actionCooltime > 0) {
			return true;
		}
		else return false;
	};

	void RewindPos() { m_Position = pastPos; }//１フレーム位置を戻す

	CHARA_CONDITION GetCharaCon() { return charaCon; }

	//衝突によるバウンド
	void CharacterBound(D3DXVECTOR3 s_pos);

	//崖つかみ
	void CheckGrap();
	void CliffGrap();
	void CliffAction(CLIFFACTION s_ca);
	void SetCliffData(MeshFieldBox* s_box,DIRECTION s_dir) {
		cliffGrapData.pMBox = s_box;
		cliffGrapData.dir = s_dir;
	}
	void SetCliffGrapCooltime(int s_time) { grapCooltime = s_time; }
	
	//緊急回避
	void AvoidEmergency();
	void AvoidUpdate();

	//つかみ
	void GrabChara();
	void GrabRelease();
	void SetGrab();
	void GrabingAction();

	//技データ
	virtual void SetAttackData();
	AttackData* GetAttackData() { return atkData; }

	//デバッグ用
	void DebugDraw();
	void HitBoxDraw();

	//アニメーション設定
	void SetAnimationState(ANIMATION_STATE s_as);
	void SetAnimationState(ANIMATION_STATE s_as,int s_startFrame);
	virtual void AnimationStateUpdate();

	void SetStartStates();

	////チャージ
	//チャージ時間が引数を上回っていればtrueを返す
	bool CheckActionCharge(int chargeCheckNum);
	void ActionCharge();
	void ChargeReset();
	int GetActionCharge() { return actionCharge; }

	//攻撃判定
	void AttackObjDeath();//攻撃中の攻撃判定を消す

	//UI
	void PlayerSignInit();
	void PlayerSignDraw();

	virtual CHARA_KIND GetCharaKind() { return CK_TEST; }


	float shieldWParameter = -0.5f;


};

class AP_Paradin : public AnimationPlayer {

private:
	void ModelDataLoad();
	void AnimationStateUpdate();

public:

	AP_Paradin(Camera* pC) : AnimationPlayer(pC){}

	void PlayerUpdate();

	void Attack();//ニュートラル
	void SubAttack() {};//ニュートラル 特殊
	void UpAttack();//上
	void SubUpAttack() {};//上 特殊
	void DownAttack();//下
	void SubDownAttack() {};//下 特殊
	void DirAttack();//横
	void SubDirAttack() {};//横 特殊
	void CliffAttack() { AnimationPlayer::CliffAttack(); }//崖上がり攻撃
	void SetStates();

	void SetAttackData();

	CHARA_KIND GetCharaKind() { return CK_PALADIN; }


};


class AP_Fighter : public AnimationPlayer {

private:
	void ModelDataLoad();
	void AnimationStateUpdate();

public:

	AP_Fighter(Camera* pC) : AnimationPlayer(pC) {}

	void PlayerUpdate();

	void Attack();//ニュートラル
	void SubAttack() {};//ニュートラル 特殊
	void UpAttack();//上
	void SubUpAttack() {};//上 特殊
	void DownAttack();//下
	void SubDownAttack() {};//下 特殊
	void DirAttack();//横
	void SubDirAttack() {};//横 特殊
	void CliffAttack() { AnimationPlayer::CliffAttack(); }//崖上がり攻撃
	void SetStates();

	void SetAttackData();

	CHARA_KIND GetCharaKind() { return CK_FIGTHER; }


};

class AP_Wizard : public AnimationPlayer {

private:
	void ModelDataLoad();
	void AnimationStateUpdate();

public:

	AP_Wizard(Camera* pC) : AnimationPlayer(pC) {}

	void PlayerUpdate();

	void Attack();//ニュートラル
	void SubAttack() {};//ニュートラル 特殊
	void UpAttack();//上
	void SubUpAttack() {};//上 特殊
	void DownAttack();//下
	void SubDownAttack() {};//下 特殊
	void DirAttack();//横
	void SubDirAttack() {};//横 特殊
	void CliffAttack() { AnimationPlayer::CliffAttack(); }//崖上がり攻撃
	void SetStates();

	void SetAttackData();

	CHARA_KIND GetCharaKind() { return CK_WIZARD; }


};
#pragma once
#include "gameObject.h"
#include "animationModel.h"
#include "Model.h"
#include "config.h"

#define CHARA_MOVESPEED_DEFAULT (0.05f)
#define CHARA_ROLLSPEED_DEFAULT (0.05f)

#define CHARA_CLIMEABLE_HEIGHT (0.1f)

//������΂����x�����W��
#define BLOWN_DEC_COEFFICIENT (0.95f)

//�_���[�W������΂��␳�̊�l
#define BLOWN_CORRECT_BASE (100.0f)

//������΂�����s�\���x
#define BLOWN_INOPERABLE_SPEED (0.0f)//���ݒ�

//�_���[�W���󂯂����̖��G����
#define DAMAGE_INVINCIBLETIME (10)

//�������̖��G����
#define RESPAWN_INVINCIBLETIME (60)

//�R���݃N�[���^�C��
#define CRIFFGRAP_COOLTIME (90)


//�����ʒu
#define RESPAWN_POINT {0,5,0}

//���o�E���h臒l
#define BOUND_LIMIT (0.2f)

//�V�[���h�u���C�N���A����
#define SHIELDBREAK_TIME (300)

//�R�オ���𖳓G����
#define CRIFFAVOID_INVINCIBLETIME (20)

//�ً}��𖳓G����
#define AVOID_INVINCIBLETIME (20)

//�ً}����d������
#define AVOID_COOLTIME (30)

//UI�_���[�W�C�[�W���O�֐�
#define EASING_DAMAGETEXT_TIME (60.0f)

//�_�b�V����t����
#define DASH_RECEPTIONTIME (10)

//���ݎ�������
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


struct CliffGrapData {//���܂��Ă���R�̃f�[�^

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

	CM_PLAYER,//�v���C���[����
	CM_CPU,//CPU����
	CM_SANDBAG,//�����Ȃ�
	CHARAMODE_MAX
};

enum CHARA_CONDITION {//������
	CC_NORMAL,
	CC_SHIELDBREAK,//�V�[���h�u���C�N
	CC_CLLIFGRAP,//�R���܂�
	CC_AVOID,//�ً}���
	CC_GRABBED,//���܂�
	CC_GRAB//����
};

enum CHARA_KEY {//����L�[
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

	D3DXVECTOR3 velocity;//�ړ����x
	D3DXVECTOR3 blownVelocity = {};//������΂��ꑬ�x

	float landHeight;//�ݒu����Ɏg������

	D3DXVECTOR4 shaderPlusColor = {0,0,0,0};

	BattleAI* battleAI = nullptr;

	AttackData atkData[AK_MAX];

	//�P�t���[���O�̈ʒu
	D3DXVECTOR3 pastPos;

	int ShieldBreakTime;

	D3DXVECTOR3 modelOffset;
	D3DXVECTOR3 modelOffsetVelocity;
	D3DXVECTOR3 modelRotate;

	bool modelFrameStop;

	int actionCharge = 0;//���ߋZ�Ɏg���ėp�ϐ�

	
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

	int playerNumber;//�v���C���[���ʔԍ�
	bool isHuman;

	int accumulationDamage = 0;//�~�σ_���[�W
	int lifeStock;//�c�@

	int invincibleTime = 0;//���G����

	StatusWindow* StatesWindow;
	TexString* DamageText;

	bool upAttackAble = true;//��Z���o���邩�ǂ���
	bool invincibleColorSw = false;
	bool useShield = false;

	int actionCooltime = 0;//�d��
	int jumpCooltime = 0;//�W�����v�s�\����
	int grapCooltime = 0;//�R���݃N�[���^�C��

	CliffGrapData cliffGrapData = { nullptr,RIGHT};

	//����ł��鑊��
	AnimationPlayer* GrapingPlayer = nullptr;

	//���݌o�ߎ���
	int grabHoldTime = 0;

	virtual void ModelDataLoad();

	DamageObject* attackObj = nullptr;//�U�����̍U������

	//UI�\���p
	RendererSet UIRenset;
	int damageTextEasingTime = EASING_DAMAGETEXT_TIME;

	//�_�b�V��
	bool dashFlag = false;
	int dashReceptionTime = true;


public:

	enum CLIFFACTION {

		CA_UP,//�����オ��
		CA_ATTACK,//�U�����Ȃ���オ��
		CA_AVOID//������Ȃ���オ��
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

	virtual void Attack();//�j���[�g����
	virtual void SubAttack();//�j���[�g���� ����
	virtual void UpAttack();//��
	virtual void SubUpAttack() {};//�� ����
	virtual void DownAttack();//��
	virtual void SubDownAttack() {};//�� ����
	virtual void DirAttack();//��
	virtual void SubDirAttack() {};//�� ����
	virtual void CliffAttack();//�R�オ��U��
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


	void BlownCharacter(D3DXVECTOR2 s_vec);//������΂����

	void AddDamage(int s_add) { 
		accumulationDamage += s_add; 
		if (accumulationDamage > 999) accumulationDamage = 999;
		else if (accumulationDamage < 0) accumulationDamage = 0;
	}

	void DamageFunction(int s_add);

	float GetBlownCorrect();//���݂̃_���[�W�ʂ��琁����΂��W�����Z�o���Ԃ�
	bool CheckInvincible();//���G���Ԓ��Ȃ�true��Ԃ�
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
	//�V�[���h
	void ShieldFunction();
	void ShieldUp();//�V�[���h�𒣂�
	bool CheckShield();//�V�[���h���L����ԂȂ�true��Ԃ�
	void ShieldDamage(int s_damage);
	void ShieldBreakUpdate();
	void ShieldBreak();

	void HitBoxInit();

	//�v���C���[�i���o�[���ƂɔC�ӂ̑���L�[�������Ă��邩��Ԃ�
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

	void RewindPos() { m_Position = pastPos; }//�P�t���[���ʒu��߂�

	CHARA_CONDITION GetCharaCon() { return charaCon; }

	//�Փ˂ɂ��o�E���h
	void CharacterBound(D3DXVECTOR3 s_pos);

	//�R����
	void CheckGrap();
	void CliffGrap();
	void CliffAction(CLIFFACTION s_ca);
	void SetCliffData(MeshFieldBox* s_box,DIRECTION s_dir) {
		cliffGrapData.pMBox = s_box;
		cliffGrapData.dir = s_dir;
	}
	void SetCliffGrapCooltime(int s_time) { grapCooltime = s_time; }
	
	//�ً}���
	void AvoidEmergency();
	void AvoidUpdate();

	//����
	void GrabChara();
	void GrabRelease();
	void SetGrab();
	void GrabingAction();

	//�Z�f�[�^
	virtual void SetAttackData();
	AttackData* GetAttackData() { return atkData; }

	//�f�o�b�O�p
	void DebugDraw();
	void HitBoxDraw();

	//�A�j���[�V�����ݒ�
	void SetAnimationState(ANIMATION_STATE s_as);
	void SetAnimationState(ANIMATION_STATE s_as,int s_startFrame);
	virtual void AnimationStateUpdate();

	void SetStartStates();

	////�`���[�W
	//�`���[�W���Ԃ������������Ă����true��Ԃ�
	bool CheckActionCharge(int chargeCheckNum);
	void ActionCharge();
	void ChargeReset();
	int GetActionCharge() { return actionCharge; }

	//�U������
	void AttackObjDeath();//�U�����̍U�����������

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

	void Attack();//�j���[�g����
	void SubAttack() {};//�j���[�g���� ����
	void UpAttack();//��
	void SubUpAttack() {};//�� ����
	void DownAttack();//��
	void SubDownAttack() {};//�� ����
	void DirAttack();//��
	void SubDirAttack() {};//�� ����
	void CliffAttack() { AnimationPlayer::CliffAttack(); }//�R�オ��U��
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

	void Attack();//�j���[�g����
	void SubAttack() {};//�j���[�g���� ����
	void UpAttack();//��
	void SubUpAttack() {};//�� ����
	void DownAttack();//��
	void SubDownAttack() {};//�� ����
	void DirAttack();//��
	void SubDirAttack() {};//�� ����
	void CliffAttack() { AnimationPlayer::CliffAttack(); }//�R�オ��U��
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

	void Attack();//�j���[�g����
	void SubAttack() {};//�j���[�g���� ����
	void UpAttack();//��
	void SubUpAttack() {};//�� ����
	void DownAttack();//��
	void SubDownAttack() {};//�� ����
	void DirAttack();//��
	void SubDirAttack() {};//�� ����
	void CliffAttack() { AnimationPlayer::CliffAttack(); }//�R�オ��U��
	void SetStates();

	void SetAttackData();

	CHARA_KIND GetCharaKind() { return CK_WIZARD; }


};
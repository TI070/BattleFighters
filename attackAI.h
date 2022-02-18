#pragma once

#include "animationPlayer.h"
#include "battleAI.h"

enum ATTACKAI_KIND{

	ATKAI_TARGET,
	ATKAI_SURVIVE

};

class AttackAI {

protected:
	AnimationPlayer* owner;//�{��
	AnimationPlayer* target;//�Ώ�
	BattleAI* parentAI;//�eAI
	int atkDecideTime = 0;
	int atkDecideTimeLimit = 60;
	int interruptRange = 1;

public:
	AttackAI(BattleAI* s_ai) {
		parentAI = s_ai;
		owner = s_ai->GetOwner();
		target = s_ai->GetTarget();
		if (s_ai->GetAttackAI() != nullptr) {//null�Ƃ�
			atkDecideTime = s_ai->GetAttackAI()->GetAtkDecideTime();
			atkDecideTimeLimit = s_ai->GetAttackAI()->GetAtkDecideTimeLimit();
		}
		else {
			atkDecideTime = 0;
			atkDecideTimeLimit = 60;
		}
	}

	virtual void Attack() = 0;
	virtual void Think() = 0;

	void ChangeAI(AttackAI* s_ai);
	int GetAtkDecideTime() { return atkDecideTime; }
	int GetAtkDecideTimeLimit() { return atkDecideTime; }
	void SetAtkDecideTimeLimit(int s_lim) { atkDecideTimeLimit = s_lim; }
	void SetTarget(AnimationPlayer* s_p) { target = s_p; }

	bool CheckAtkDecideTime();
	void SetDecideTimeLimit(int s_t) { atkDecideTimeLimit = s_t; }
	void SetDecideTimeLimitRandom(int s_t1,int s_t2) { 
		int s_t = abs(s_t1 - s_t2);
		int r = rand() % s_t;

		if (s_t1 < s_t2) {
			atkDecideTimeLimit = s_t1 + r;

			return;
		}

		atkDecideTimeLimit = s_t2 + r;

	}

	//����̕���������
	void TurnAround();

	void Update();
	virtual ATTACKAI_KIND GetType() = 0;

	//���荞�ݍU�����f�@�����𖞂������瑦���U�����s���悤�ɂ���
	bool InterruptAttackDecide();

};

class AttackAI_TargetAttack :public AttackAI{

private:
	int chargeLimit = 0;

	void SetChargeLimit();

public:
	AttackAI_TargetAttack(BattleAI* s_ai) :AttackAI(s_ai) {}
	void Attack();
	void Think();

	ATTACKAI_KIND GetType() { return ATKAI_TARGET; }

};

//�L�������ƂɍX�ɍU���p�N���X�����
class AttackAI_TagetAttack_Paladin :public AttackAI_TargetAttack {

public:
	AttackAI_TagetAttack_Paladin(BattleAI* s_ai) :AttackAI_TargetAttack(s_ai) {}
	void Attack();

};

class AttackAI_TagetAttack_Fighter :public AttackAI_TargetAttack {

public:
	AttackAI_TagetAttack_Fighter(BattleAI* s_ai) :AttackAI_TargetAttack(s_ai) {}
	void Attack();

};

class AttackAI_TagetAttack_Wizard :public AttackAI_TargetAttack {

public:
	AttackAI_TagetAttack_Wizard(BattleAI* s_ai) :AttackAI_TargetAttack(s_ai) {}
	void Attack();

};

class AttackAI_Survive :public AttackAI {

public:
	AttackAI_Survive(BattleAI* s_ai) :AttackAI(s_ai) {}
	void Attack();
	void Think();

	ATTACKAI_KIND GetType() { return ATKAI_SURVIVE; }
};

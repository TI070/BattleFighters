#pragma once

#include "animationPlayer.h"

enum AI_LEVEL {

	AI_LEVEL1


};


enum AI_ATTACKMODE {

	AI_ATTACKMODE_TARGET//�^�[�Q�b�g�����G�ɍU������

};


enum AI_MOVEMODE {

	AI_MOVEMODE_STOP,//
	AI_MOVEMODE_TOCENTER,//�����Ɍ������E�������ێ�����
	AI_MOVEMODE_TOENEMY,//�G�̕��Ɍ�����

};

enum TT_RANGE {

	SHORT_RANGE,
	MIDDLE_RANGE,
	LONG_RANGE,
	TTRANGE_MAX
};

class AnimationPlayer;
class MoveAI;
class AttackAI;

class BattleAI {

private:

	AI_MOVEMODE aiMoveMode = AI_MOVEMODE_TOCENTER;
	AI_ATTACKMODE aiAttackMode = AI_ATTACKMODE_TARGET;
	AnimationPlayer* owner;
	AnimationPlayer* target;
	float toTargetDistance;//�G�ɑ΂��ĕۂƂ��Ƃ��鋗��

	float decideTime;//�s���𔻒f����܂ł̎���
	float thinkTime;//�l��������
	//�v�l���ԐU�ꕝ
	float decideTimeMax;
	float decideTimeMin;

	bool thinkReset;

	AttackData* atkData;
	ATTACK_KIND selectAtk;

	//AI�X�e�[�g
	MoveAI* moveAI = nullptr;
	AttackAI* attackAI = nullptr;

	float ttRange[TTRANGE_MAX];

public:

	BattleAI(AnimationPlayer* s_Ap) {
		owner = s_Ap;
	}

	void Init();
	void Think();
	void Update();

	void ToCenterArea();
	void ToTarget();
	void SetAIMoveMode(AI_MOVEMODE s_mode) { aiMoveMode = s_mode; }

	void OwnerMove();
	void OwnerStop();
	float GetToTargetDistance() { return toTargetDistance; }
	AnimationPlayer* GetOwner() { return owner; }
	AnimationPlayer* GetTarget() { return target; }
	ATTACK_KIND GetSelectAtk() { return selectAtk; }
	MoveAI* GetMoveAI() { return moveAI; }
	AttackAI* GetAttackAI() { return attackAI; }

	virtual void OwnerAttack() {};

	void AvoidCrisis();

	void SetDecideTime(float s_time) { decideTime = s_time; }

	bool CheckDecideAble() { //�v�l���Ԃ����f�\���Ԃ���������true��Ԃ�
		if (thinkTime >= decideTime) {

			thinkReset = true;

			//����v�l���Ԃ�ݒ�
			int rng = decideTimeMax - decideTimeMin;
			decideTime = decideTimeMin + rand() % rng;

			return true;
		}
		return false;
	}

	//�G���^�[�Q�b�g����
	void TargetGet();

	//�Z�I��
	void SelectAtkAction();

	void SetMoveAI(MoveAI* s_ai) { moveAI = s_ai; }
	void SetAttackAI(AttackAI* s_ai) { attackAI = s_ai; }

	void GenerateTTDistanceBase();
	void ThinktoTargetDistance();//����Ǝ�낤�Ƃ��鋗���𐶐�����

	//�O����Think�����Ă�
	void MoveAIThink();

	//�I�𒆂̋Z�͈͓̔��Ƀ^�[�Q�b�g�������Ă��邩�ǂ������m�F
	//�����Ă�����true��Ԃ�
	bool CheckInAtkArea(ATTACK_KIND s_ak);

};

class Knight : BattleAI {

};
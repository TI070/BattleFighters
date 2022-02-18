#include "attackAI.h"
#include "stageSystem.h"
#include "playerManager.h"
#include "battleAI.h"
#include "moveAI.h"
#include <typeinfo>

void AttackAI::ChangeAI(AttackAI* s_ai) {

	//�eAI�̈ړ�AI��ύX����
	parentAI->SetAttackAI(s_ai);

	//���E
	delete this;
}

bool AttackAI::InterruptAttackDecide() {

	ATTACK_KIND selectAtk = parentAI->GetSelectAtk();
	//�Z��������悤�ȋ����ɂ�����U��

	//�G���A���Q��
	if (parentAI->CheckInAtkArea(selectAtk)) return true;
	return false;

}

void AttackAI_TargetAttack::Attack() {

	

	//��莞�Ԃ��ƂɍU������
	if (CheckAtkDecideTime()) {
		atkDecideTime = 0;
		SetDecideTimeLimitRandom(30, 60);
		//����̂ق�������
		TurnAround();

		//�Z�̒��I
		parentAI->SelectAtkAction();
		ATTACK_KIND selectAtk = parentAI->GetSelectAtk();

		//�I�����ꂽ�U��������
		if (selectAtk == AK_NEUTRAL) {
			if (chargeLimit == 0) {//�ŏ��@���ߖڕW�l��ݒ肵�ĂP�񒙂߂�
				SetChargeLimit();
				owner->Attack();
			}
			else if (chargeLimit > owner->GetActionCharge()) {//���ߖڕW�l�܂ł��ߑ�����
				owner->Attack();
			}
			else {//���ߖڕW�l�܂ł��܂����猂��
				owner->SubAttack();
				chargeLimit = 0;
			}
		}
		else if (selectAtk == AK_DIR) {
			owner->DirAttack();
		}
		else if (selectAtk == AK_DOWN) {
			owner->DownAttack();
		}
		else if (selectAtk == AK_UP) {
			owner->UpAttack();
		}
	}
	else {//����s���������@�����ł̓`���[�W�|

		if (chargeLimit == 0) return;

		if (chargeLimit > owner->GetActionCharge()) {//���ߖڕW�l�܂ł��ߑ�����
			owner->Attack();
		}
		else {//���ߖڕW�l�܂ł��܂����猂��
			owner->SubAttack();
			chargeLimit = 0;
		}
	}


}

void AttackAI_TagetAttack_Paladin::Attack() {



	//��莞�Ԃ��ƂɍU������
	if (CheckAtkDecideTime()) {
		atkDecideTime = 0;
		SetDecideTimeLimitRandom(30, 60);

		//����̂ق�������
		TurnAround();

		//�Z�̒��I
		parentAI->SelectAtkAction();
		ATTACK_KIND selectAtk = parentAI->GetSelectAtk();

		//�I�����ꂽ�U��������
		if (selectAtk == AK_NEUTRAL) {
			owner->Attack();
		}
		else if (selectAtk == AK_DIR) {
			owner->DirAttack();
		}
		else if (selectAtk == AK_DOWN) {
			owner->DownAttack();
		}
		else if (selectAtk == AK_UP) {
			owner->UpAttack();
		}
	}



}

void AttackAI_TagetAttack_Fighter::Attack() {



	//��莞�Ԃ��ƂɍU������
	if (CheckAtkDecideTime()) {
		atkDecideTime = 0;
		SetDecideTimeLimitRandom(30, 60);

		//����̂ق�������
		TurnAround();

		//�Z�̒��I
		parentAI->SelectAtkAction();
		ATTACK_KIND selectAtk = parentAI->GetSelectAtk();

		//�I�����ꂽ�U��������
		if (selectAtk == AK_NEUTRAL) {
			owner->Attack();
		}
		else if (selectAtk == AK_DIR) {
			owner->DirAttack();
		}
		else if (selectAtk == AK_DOWN) {
			owner->DownAttack();
		}
		else if (selectAtk == AK_UP) {
			owner->UpAttack();
		}
	}



}

void AttackAI_TagetAttack_Wizard::Attack() {


	//��莞�Ԃ��ƂɍU������
	if (CheckAtkDecideTime()) {
		atkDecideTime = 0;
		SetDecideTimeLimitRandom(30, 60);

		//����̂ق�������
		TurnAround();

		//�Z�̒��I
		parentAI->SelectAtkAction();
		ATTACK_KIND selectAtk = parentAI->GetSelectAtk();

		//�I�����ꂽ�U��������
		if (selectAtk == AK_NEUTRAL) {
			owner->Attack();
		}
		else if (selectAtk == AK_DIR) {
			owner->DirAttack();
		}
		else if (selectAtk == AK_DOWN) {
			owner->DownAttack();
		}
		else if (selectAtk == AK_UP) {
			owner->UpAttack();
		}
	}



}

void AttackAI::Update() {

	//�v�l���Ԃ̑���
	atkDecideTime++;


}

bool AttackAI::CheckAtkDecideTime() {

	if (atkDecideTime >= atkDecideTimeLimit) return true;
	return false;
}

void AttackAI::TurnAround() {

	float d = target->GetPosition().x - owner->GetPosition().x;

	if (d > 0) {
		owner->SetCharaDir(CD_RIGHT);
	}
	else {
		owner->SetCharaDir(CD_LEFT);
	}
}

void AttackAI_TargetAttack::Think() {

	//�s���`�Ȃ琶���D�惂�[�h�ɂ���
	if (parentAI->GetMoveAI()->GetType() == MOVEAI_CENTER) {
		AttackAI_Survive* AI_TT = new AttackAI_Survive(parentAI);
		ChangeAI(AI_TT);
	}

}

void AttackAI_TargetAttack::SetChargeLimit() {

	int r = rand() % 10;

	switch (r)
	{
	case 0:
		chargeLimit = 5;
		break;
	case 1:
		chargeLimit = 10;
		break;
	case 2:
		chargeLimit = 12;
		break;
	case 3:
		chargeLimit = 20;
		break;
	case 4:
		chargeLimit = 30;
		break;
	case 5:
		chargeLimit = 35;
		break;
	case 6:
		chargeLimit = 40;
		break;
	case 7:
		chargeLimit = 50;
		break;
	case 8:
		chargeLimit = 65;
		break;
	case 9:
		chargeLimit = 80;
		break;
	default:
		chargeLimit = 5;
		break;
	}

}


void AttackAI_Survive::Attack() {

	//�U�����Ȃ�

	//�W�����v���g���؂����畜�A�Z���g��
	if (owner->CheckJumpAble()) {
		owner->UpAttack();
	}

}

void AttackAI_Survive::Think() {

	//�G���^�[�Q�b�g����]�T���ł�����U�����[�h�ɂ���
	if (parentAI->GetMoveAI()->GetType() != MOVEAI_CENTER) {
		
		AttackAI_TargetAttack* AI_TT;
		if (owner->GetCharaKind() == CK_TEST) {
			AI_TT = new AttackAI_TargetAttack(parentAI);
		}
		else if (owner->GetCharaKind() == CK_PALADIN) {
			AI_TT = new AttackAI_TagetAttack_Paladin(parentAI);
		}
		else if (owner->GetCharaKind() == CK_FIGTHER) {
			AI_TT = new AttackAI_TagetAttack_Fighter(parentAI);
		}
		else if (owner->GetCharaKind() == CK_WIZARD) {
			AI_TT = new AttackAI_TagetAttack_Wizard(parentAI);
		}

		ChangeAI(AI_TT);
	}

}
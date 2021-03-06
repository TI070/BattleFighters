#include "attackAI.h"
#include "stageSystem.h"
#include "playerManager.h"
#include "battleAI.h"
#include "moveAI.h"
#include <typeinfo>

void AttackAI::ChangeAI(AttackAI* s_ai) {

	//親AIの移動AIを変更する
	parentAI->SetAttackAI(s_ai);

	//自殺
	delete this;
}

bool AttackAI::InterruptAttackDecide() {

	ATTACK_KIND selectAtk = parentAI->GetSelectAtk();
	//技が当たるような距離にいたら攻撃

	//エリアを参照
	if (parentAI->CheckInAtkArea(selectAtk)) return true;
	return false;

}

void AttackAI_TargetAttack::Attack() {

	

	//一定時間ごとに攻撃する
	if (CheckAtkDecideTime()) {
		atkDecideTime = 0;
		SetDecideTimeLimitRandom(30, 60);
		//相手のほうを向く
		TurnAround();

		//技の抽選
		parentAI->SelectAtkAction();
		ATTACK_KIND selectAtk = parentAI->GetSelectAtk();

		//選択された攻撃をする
		if (selectAtk == AK_NEUTRAL) {
			if (chargeLimit == 0) {//最初　ため目標値を設定して１回貯める
				SetChargeLimit();
				owner->Attack();
			}
			else if (chargeLimit > owner->GetActionCharge()) {//ため目標値までため続ける
				owner->Attack();
			}
			else {//ため目標値までたまったら撃つ
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
	else {//特殊行動だけ許可　ここではチャージ弓

		if (chargeLimit == 0) return;

		if (chargeLimit > owner->GetActionCharge()) {//ため目標値までため続ける
			owner->Attack();
		}
		else {//ため目標値までたまったら撃つ
			owner->SubAttack();
			chargeLimit = 0;
		}
	}


}

void AttackAI_TagetAttack_Paladin::Attack() {



	//一定時間ごとに攻撃する
	if (CheckAtkDecideTime()) {
		atkDecideTime = 0;
		SetDecideTimeLimitRandom(30, 60);

		//相手のほうを向く
		TurnAround();

		//技の抽選
		parentAI->SelectAtkAction();
		ATTACK_KIND selectAtk = parentAI->GetSelectAtk();

		//選択された攻撃をする
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



	//一定時間ごとに攻撃する
	if (CheckAtkDecideTime()) {
		atkDecideTime = 0;
		SetDecideTimeLimitRandom(30, 60);

		//相手のほうを向く
		TurnAround();

		//技の抽選
		parentAI->SelectAtkAction();
		ATTACK_KIND selectAtk = parentAI->GetSelectAtk();

		//選択された攻撃をする
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


	//一定時間ごとに攻撃する
	if (CheckAtkDecideTime()) {
		atkDecideTime = 0;
		SetDecideTimeLimitRandom(30, 60);

		//相手のほうを向く
		TurnAround();

		//技の抽選
		parentAI->SelectAtkAction();
		ATTACK_KIND selectAtk = parentAI->GetSelectAtk();

		//選択された攻撃をする
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

	//思考時間の操作
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

	//ピンチなら生存優先モードにする
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

	//攻撃しない

	//ジャンプを使い切ったら復帰技を使う
	if (owner->CheckJumpAble()) {
		owner->UpAttack();
	}

}

void AttackAI_Survive::Think() {

	//敵をターゲットする余裕ができたら攻撃モードにする
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
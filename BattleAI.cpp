
#include "main.h"
#include "battleAI.h"
#include "moveAI.h"
#include "attackAI.h"
#include "stageSystem.h"
#include "animationPlayer.h"
#include "playerManager.h"


void BattleAI::Init() {

	//���u��
	aiMoveMode = AI_MOVEMODE_TOCENTER;
	aiAttackMode = AI_ATTACKMODE_TARGET;

	toTargetDistance = 1.0f;

	//�p�����[�^������
	thinkTime = 0;
	decideTime = 60;
	thinkReset = false;
	decideTimeMax = 60;
	decideTimeMin = 30;

	//�Z�f�[�^�擾
	atkData = owner->GetAttackData();

	//�Ƃ肠�����^�[�Q�b�g�ݒ�
	TargetGet();

	//�Ƃ肠���������W�ݒ�
	GenerateTTDistanceBase();

	//�Ƃ肠�����g���Z�ݒ�
	SelectAtkAction();

	//�Ƃ肠���������W�I��
	ThinktoTargetDistance();



	//�Ƃ肠����AI����
	//moveAI = new MoveAI_ToCenter(this);
	moveAI = new MoveAI_ToTarget(this);

	//�e�̃L�����ŕύX
	if (owner->GetCharaKind() == CK_TEST) {
		attackAI = new AttackAI_TargetAttack(this);
	}
	else if (owner->GetCharaKind() == CK_PALADIN) {
		attackAI = new AttackAI_TagetAttack_Paladin(this);
	}
	else if (owner->GetCharaKind() == CK_FIGTHER) {
		attackAI = new AttackAI_TagetAttack_Fighter(this);
	}
	else if (owner->GetCharaKind() == CK_WIZARD) {
		attackAI = new AttackAI_TagetAttack_Wizard(this);
	}

	//�f�o�b�O
	//selectAtk = AK_NEUTRAL;

}

void BattleAI::Think() {

	//�^�[�Q�b�g�����Ȃ���Ύ�蒼��
	if (target == nullptr) {
		TargetGet();
		attackAI->SetTarget(target);
		moveAI->SetTarget(target);

	}
	if (!owner->CheckActionCooltime()) {
		//���x���Z�b�g
		D3DXVECTOR3 s_vec = { 0,0,0 };
		s_vec.y = owner->GetVelocity().y;
		owner->SetVelocity(s_vec);
	}


	owner->SetThroughFlag(false);

	//�ړ�AI
	moveAI->Update();
	moveAI->Think();

	//�U��AI
	attackAI->Update();
	attackAI->Think();

	//�v�l���ԃv���X
	thinkTime++;
	if (thinkReset) {
		thinkTime = 0;
		thinkReset = false;
	}

}

void BattleAI::Update() {


	//�ړ�AI
	//�`���[�W���͈ړ��ł��Ȃ�
	if (owner->GetActionCharge() == 0) {
		moveAI->Move();
	}

	//�U��AI

	attackAI->Attack();



	//�f�o�b�O
	/*
	if (moveAI->GetType() == MOVEAI_CENTER) {
		OutputDebugString("Move_Tocenter\n");
	}
	else if (moveAI->GetType() == MOVEAI_TARGET) {
		OutputDebugString("Move_Totarget\n");
	}
	else if (moveAI->GetType() == MOVEAI_STOP){
		OutputDebugString("Move_Stop\n");
	}
	else if (moveAI->GetType() == MOVEAI_CLIFF) {
		OutputDebugString("Move_Cliff\n");
	}

	if (attackAI->GetType() == ATKAI_TARGET) {
		OutputDebugString("AttackTarget\n");
	}
	else if (attackAI->GetType() == ATKAI_SURVIVE) {
		OutputDebugString("AttackSurvive\n");
	}
	*/

}



void BattleAI::ToTarget( ) {

	//���f�\���`�F�b�N
	if (!CheckDecideAble()) return;

	//�G�Ƃ̋����Z�b�g
	D3DXVECTOR3 vec = owner->GetPosition() - target->GetPosition();
	float dis = D3DXVec3Length(&vec);

	//��苗���ȏ㗣��Ă�����ړ�����
	if (toTargetDistance < dis) {


		//�G�̂�������Ɍ������Ĉړ�����
		if (owner->GetPosition().x > target->GetPosition().x) {
			//OwnerMove(CD_LEFT);
			owner->SetCharaDir(CD_LEFT);
		}
		else if (owner->GetPosition().x < target->GetPosition().x) {
			//OwnerMove(CD_RIGHT);
			owner->SetCharaDir(CD_RIGHT);
		}
	}
	else {//�����łȂ���Έړ����Ȃ�

		OwnerStop();

	}



	//������������̂ق������ȏ㍂����΃W�����v ���u��
	if (owner->GetPosition().y < 0.0f + target->GetPosition().y) owner->Jump();

	//������������̂ق������ȏ�Ⴍ�Ă��蔲���\����ɂ���Ȃ炷�蔲�� ���u��
	if (owner->GetPosition().y > 0.0f + target->GetPosition().y) {
		owner->ThroughFloor();
	}

}


void BattleAI::OwnerStop() {

	owner->SetAnimationState(AS_WAIT);

}

void BattleAI::SelectAtkAction() {

	////�Z�̃f�[�^���Q�Ƃ��A�K���ȋZ��I��

	std::vector<int> pAtkOption;

	//�Z�͈̔͂��Q�Ƃ��A�^�[�Q�b�g�����ɂ��邩�m�F
	for (int i = 0; i < 4;i++) {

		//�^�[�Q�b�g�����Ȃ������珈�����Ȃ�
		if (target == nullptr) return;

		//�G���A�̐ݒ�
		Area atkArea;
		D3DXVECTOR3 sPos = owner->GetPosition() + atkData[i].offsetPos;
		CHARA_DIRECTION cDir = owner->GetCharaDir();
		float vecX,vecY;
		
		//�����ɂ�郌���W�ݒ�̕ύX
		if (cDir == CD_RIGHT) {
			vecX = atkData[i].velocity.x * atkData[i].lifetime * 1;
			atkArea.rightMax = sPos.x + atkData[i].scale.x / 2 + vecX;
			atkArea.leftMax =  sPos.x + atkData[i].scale.x / 2 ;
		}
		else {
			vecX = atkData[i].velocity.x * atkData[i].lifetime * -1;
			atkArea.rightMax = sPos.x + atkData[i].scale.x / 2 ;
			atkArea.leftMax  = sPos.x + atkData[i].scale.x / 2 + vecX;
		}

		vecY = atkData[i].velocity.y * atkData[i].lifetime;
		if (vecY > 0) {
			atkArea.upMax = sPos.y + atkData[i].scale.y / 2 + vecY;
			atkArea.downMax = sPos.y + atkData[i].scale.y / 2;
		}
		else {
			atkArea.upMax = sPos.y + atkData[i].scale.y / 2;
			atkArea.downMax = sPos.y + atkData[i].scale.y / 2 + vecY;
		}

		//�͈͓��Ƀ^�[�Q�b�g�������Ă��邩�`�F�b�N
		if (atkArea.CheckInArea({ target->GetPosition().x,target->GetPosition().y })) {

			//�����Ă������⃊�X�g�ɒǉ�
			pAtkOption.push_back(i);
		}


	}

	//����������⃊�X�g���烉���_���ŋZ������
	if (pAtkOption.size() > 0) {
		int r = rand() % pAtkOption.size();
		selectAtk = (ATTACK_KIND)pAtkOption[r];

	}
	//��⃊�X�g�����Ȃ�������ǂ����悤���킩��Ȃ����ǂƂ肠���������_���Őݒ�
	else {
		selectAtk = (ATTACK_KIND)(rand() % 4);

		//�����̊m���ŗ�O����
		if (rand() % 2 == 0) selectAtk = AK_MAX;
	}


}

void BattleAI::TargetGet() {
	
	//���ۂɂ͋����Ƃ��X�g�b�N�Ƃ��_���[�W�Ƃ����Q�Ƃ��ă^�[�Q�b�g���߂�΂�������
	//�Ƃ肠�����߂��̓z�K���Ɏw���Ƃ���

	target = PlayerManager::GetNearPlayer(owner);

}

void BattleAI::ThinktoTargetDistance() {



	//�ǂ�ɂ��邩�I�ԁ@���u�������_��

	int r = rand() % TTRANGE_MAX;

	toTargetDistance = ttRange[r];
	//toTargetDistance = ttRange[3];

}

void BattleAI::GenerateTTDistanceBase() {

	//�Z�����W���Q�Ƃ��ĒZ�E���E���̒����𐶐�����
	float plusR = 0.2f;//�␳�l

	std::vector<int> atkRangeNum;

	for (int i = 0; i < 4; i++) {
		atkRangeNum.push_back(atkData[i].range);
	}

	std::sort(atkRangeNum.begin(), atkRangeNum.end());

	ttRange[SHORT_RANGE] = atkRangeNum[0];
	ttRange[MIDDLE_RANGE] = (atkRangeNum[0] + atkRangeNum[1] + atkRangeNum[2] + atkRangeNum[3]) / 4.0f;
	ttRange[LONG_RANGE] = atkRangeNum[3];

	//�␳
	ttRange[SHORT_RANGE] -= plusR;
	ttRange[MIDDLE_RANGE] -= plusR;
	ttRange[LONG_RANGE] -= plusR;
}

void BattleAI::MoveAIThink() {

	moveAI->Think();
}

bool BattleAI::CheckInAtkArea(ATTACK_KIND s_ak) {

	//��O�Ȃ�false
	if (s_ak == AK_MAX)return false;

	//�G���A�̐ݒ�
	Area atkArea;
	D3DXVECTOR3 sPos = owner->GetPosition() + atkData[s_ak].offsetPos;
	CHARA_DIRECTION cDir = owner->GetCharaDir();
	float vecX, vecY;

	//�����ɂ�郌���W�ݒ�̕ύX
	if (cDir == CD_RIGHT) {
		vecX = atkData[s_ak].velocity.x * atkData[s_ak].lifetime * 1;
		atkArea.rightMax = sPos.x + atkData[s_ak].scale.x / 2 + vecX;
		atkArea.leftMax = sPos.x + atkData[s_ak].scale.x / 2;
	}
	else {
		vecX = atkData[s_ak].velocity.x * atkData[s_ak].lifetime * -1;
		atkArea.rightMax = sPos.x + atkData[s_ak].scale.x / 2;
		atkArea.leftMax = sPos.x + atkData[s_ak].scale.x / 2 + vecX;
	}

	vecY = atkData[s_ak].velocity.y * atkData[s_ak].lifetime;
	if (vecY > 0) {
		atkArea.upMax = sPos.y + atkData[s_ak].scale.y / 2 + vecY;
		atkArea.downMax = sPos.y + atkData[s_ak].scale.y / 2;
	}
	else {
		atkArea.upMax = sPos.y + atkData[s_ak].scale.y / 2;
		atkArea.downMax = sPos.y + atkData[s_ak].scale.y / 2 + vecY;
	}

	//�͈͓��Ƀ^�[�Q�b�g�������Ă��邩�`�F�b�N
	if (atkArea.CheckInArea({ target->GetPosition().x,target->GetPosition().y })) {

		return true;
	}
	return false;
}

#include "moveAI.h"
#include "stageSystem.h"
#include "playerManager.h"
#include "battleAI.h"
#include <typeinfo>

void MoveAI::ChangeAI(MoveAI* s_ai) {

	//�eAI�̈ړ�AI��ύX����
	parentAI->SetMoveAI(s_ai);

	//���E
	delete this;
}
void MoveAI::Update() {

	//�R���܂��ԂȂ��ԑJ��
	if (owner->GetCharaCon() == CC_CLLIFGRAP && typeid(this) != typeid(MoveAI_Cliff)) {
		MoveAI_Cliff* AI_TT = new MoveAI_Cliff(parentAI);
		ChangeAI(AI_TT);
	}
	else {
		//����O�Ȃ�߂�悤�ɂ���
		AvoidCrisis();
	}
}

float MoveAI::GetTargetDistance() {

	D3DXVECTOR3 vec = owner->GetPosition() - target->GetPosition();
	float dis = D3DXVec3Length(&vec);
	return dis;

}

void MoveAI::ChaseTargetHeight() {

	//������������̂ق������ȏ㍂����΃W�����v
	if (owner->GetPosition().y < 0.0f + target->GetPosition().y) owner->Jump();

	//������������̂ق������ȏ�Ⴍ�Ă��蔲���\����ɂ���Ȃ炷�蔲��
	if (owner->GetPosition().y > 0.0f + target->GetPosition().y) {
		owner->ThroughFloor();
	}
}


void MoveAI::RandomJump(int rnd) {

	int r = rand() % rnd;

	if (r == 0) owner->Jump();

}

void MoveAI::AvoidCrisis() {

	//���f�\���`�F�b�N
	if (!parentAI->CheckDecideAble()) return;

	//�����̈ʒu�������G���A�ɂ��邩�ǂ����𔻒f���A�����G���A�ɂ��Ȃ���Β����Ɍ��������Ƃ���
	Area center = StageSystem::GetCenterArea();
	D3DXVECTOR3 ownerPosition = owner->GetPosition();

	if (center.rightMax < ownerPosition.x || center.leftMax > ownerPosition.x) {
		MoveAI_ToCenter* AI_TT = new MoveAI_ToCenter(parentAI);
		ChangeAI(AI_TT);
	}

}

void MoveAI::AvoidAttack() {

	//�G�̍U���������悤�ɓw�͂���

	//�W�����v


	//�V�[���h


}

void MoveAI_Stop::Move() {

	owner->SetVelocity({0,owner->GetVelocity().y,0});
	owner->SetAnimationState(AS_WAIT);

	//�X�g�b�v�Ƃ͂������������킹�邭�炢�͂���
	ChaseTargetHeight();

}

void MoveAI_Stop::Think() {

	//��ԉ������鉽�����l����
	//�����W���ɂ��Ȃ������烌���W���Ē��I����
	if (parentAI->GetToTargetDistance() < GetTargetDistance()) {

		parentAI->ThinktoTargetDistance();

		//�^�[�Q�b�g�ړ��ɂ���H
		MoveAI_ToTarget* AI_TT = new MoveAI_ToTarget(parentAI);
		ChangeAI(AI_TT);

	}

	//���߂�ꂽ�X�g�b�v���Ԃ𒴂�����^�[�Q�b�g�ړ��ɂ���
	if (stopCnt >= stopLimit) {
		MoveAI_ToTarget* AI_TT = new MoveAI_ToTarget(parentAI);
		ChangeAI(AI_TT);
	}
	else {
		stopCnt++;
	}

}




void MoveAI_ToCenter::Move() {

	int dirN = 1;
	if (owner->GetCharaDir() == CD_LEFT) dirN *= -1;

	owner->SetVelocity({ owner->GetMoveSpeed() * dirN,owner->GetVelocity().y,0 });
	owner->SetAnimationState(AS_RUN);

}

void MoveAI_ToCenter::Think() {

	//���f�\���`�F�b�N
	if (!parentAI->CheckDecideAble()) return;

	//�����̈ʒu�������G���A�ɂ��邩�ǂ����𔻒f���A�����G���A�ɂ��Ȃ���Β����Ɍ��������Ƃ���
	Area center = StageSystem::GetCenterArea();
	D3DXVECTOR3 ownerPosition = owner->GetPosition();

	//���E�̔��f
	if (center.rightMax < ownerPosition.x) {
		owner->SetCharaDir(CD_LEFT);
	}
	else if (center.leftMax > ownerPosition.x) {
		owner->SetCharaDir(CD_RIGHT);
	}
	else {//�����G���A���ɂ���

		//�G���^�[�Q�b�g
		AnimationPlayer* s_t = PlayerManager::GetNearPlayer(owner);
		target = s_t;

		MoveAI_ToTarget* AI_TT = new MoveAI_ToTarget(parentAI);
		ChangeAI(AI_TT);

		return;
	}
}

void MoveAI_ToTarget::Move() {

	int dirN = 1;
	if (owner->GetCharaDir() == CD_LEFT) dirN *= -1;

	owner->SetVelocity({ owner->GetMoveSpeed() * dirN,owner->GetVelocity().y,0 });
	owner->SetAnimationState(AS_RUN);
}

void MoveAI_ToTarget::Think() {

	//���f�\���`�F�b�N
	if (!parentAI->CheckDecideAble()) return;

	//�G�Ƃ̋����Z�b�g


	//��苗���ȏ㗣��Ă�����ړ�����
	if (parentAI->GetToTargetDistance() < GetTargetDistance()) {


		//�G�̂�������Ɍ������Ĉړ�����
		if (owner->GetPosition().x > target->GetPosition().x) {
			owner->SetCharaDir(CD_LEFT);
		}
		else if (owner->GetPosition().x < target->GetPosition().x) {
			owner->SetCharaDir(CD_RIGHT);
		}
	}
	else {//�����łȂ���Έړ����Ȃ�

		//parentAI->OwnerStop();
		MoveAI_Stop* AI_TT = new MoveAI_Stop(parentAI);
		ChangeAI(AI_TT);

	}

	ChaseTargetHeight();

}

void MoveAI_Cliff::Move() {



}

void MoveAI_Cliff::Think() {

	//���f�\���`�F�b�N
	if (!parentAI->CheckDecideAble()) return;

	//���A
	owner->CliffAction(AnimationPlayer::CA_UP);

	//�R���܂��Ԃ�����
	owner->SetCharaCon(CC_NORMAL);
	owner->SetCliffGrapCooltime(CRIFFGRAP_COOLTIME);

	//���A�����璆����ڎw��
	MoveAI_ToTarget* AI_TT = new MoveAI_ToTarget(parentAI);
	ChangeAI(AI_TT);
}
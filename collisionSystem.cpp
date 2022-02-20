
#include "collisionSystem.h"
#include "damageObjectManager.h"
#include "MeshFieldBox.h"
#include "meshFieldBoxManager.h"
#include "damageObject.h"
#include "animationPlayer.h"
#include "playerManager.h"
#include "effectManager.h"
#include "imageManager.h"
#include "audioManager.h"

#include "audio.h"

#include <list>
#include <typeinfo.h>

#define COLLISION_RANGE (0.1f)

//Audio* CollisionSystem::AttackSE;
Player* CollisionSystem::pP;

void CollisionSystem::Update() {

	CollisionDamageObjectandDamageObject();
	CollisionDamageObjectandCharacter();
	CollisionMeshFieldBoxandCharacter();
}

void CollisionSystem::CollisionDamageObjectandCharacter() {

	std::list<DamageObject*> dObjList = DamageObjectManager::GetDamageObjectList();
	std::list<AnimationPlayer*> playerList = PlayerManager::GetPlayerList();

	for (AnimationPlayer* pPlayer : playerList) {

		//���G���ԂȂ珈�����Ȃ�
		if (pPlayer->CheckInvincible()) continue;

		for (DamageObject* pDamageObj : dObjList) {

			//�����̒e�������珈�����Ȃ�
			if (pPlayer == pDamageObj->GetOwner()) continue;

			//�V�[���h�Ȃ珈�����Ȃ�
			if (pDamageObj->GetObjKind() == DOBJ_SHIELD) continue;

			//�܂����݂��Ă��Ȃ���Ώ������Ȃ�
			if (!pDamageObj->CheckExist()) return;

			if (CheckObbCollision(pDamageObj,pPlayer->GetHitBox())) {

				////�_���[�W�̉��Z
				pPlayer->DamageFunction(pDamageObj->GetDamage());

				////�ӂ��Ƃ΂�
				D3DXVECTOR2 b_vec;
				float blowPower = pDamageObj->GetBlowPower() * pPlayer->GetBlownCorrect();//�ӂ��Ƃ΂���  �Z�̐�����΂��� * �ݐσ_���[�W�ɂ�鐁����΂��W��
				D3DXVECTOR2 dObjtochara_Vec;//�U���ʒu����G�ʒu�ւ̃x�N�g��
				dObjtochara_Vec.x = pPlayer->GetHitPosition().x - pDamageObj->GetPosition().x;
				dObjtochara_Vec.y = pPlayer->GetHitPosition().y - pDamageObj->GetPosition().y;
				D3DXVec2Normalize(&dObjtochara_Vec, &dObjtochara_Vec);

				b_vec = (dObjtochara_Vec + pDamageObj->GetBlowVec()) * blowPower;//(�U���ʒu����G�ʒu�ւ̃x�N�g�� + �Z���Ƃ̕����␳�x�N�g��) * �ӂ��Ƃ΂���
				pPlayer->BlownCharacter(b_vec);

				////�G�t�F�N�g�̔���
				//�G�t�F�N�g�T�C�Y�ݒ�
				float effSize = 2.0f;

				//�G�t�F�N�g�ʒu�ݒ�
				D3DXVECTOR3 effPos = pPlayer->GetPosition();
				//�O�ɕ\�����邽�߂ɓ�����
				effPos.z -= 1.0f;
				effPos.y += 1.5f;

				EffectManager::SetAnimationEffect(effPos, effSize, ImageManager::GetTexId(TEX_EFF_BLOW), BLOWEFFECT_LIFE, BLOWEFFECT_ENUM);

				//�ӂ��Ƃ΂��G�t�F�N�g
				pPlayer->BlownEffect();

				//�Փˉ���炷
				AudioManager::AudioPlay(pDamageObj->GetSoundKind(),false);


				//��������
				pDamageObj->SetDestroy();




			}

		}
	}

}

//�������Ă�����true��Ԃ�
bool CollisionSystem::CheckObbCollision_DamageObjectandCharacter(GameObject* pChara, GameObject* pdObj) {

	D3DXVECTOR3 charaPos = pChara->GetPosition();
	D3DXVECTOR3 dObjPos = pdObj->GetPosition();

	D3DXVECTOR3 direction = dObjPos - charaPos;

	D3DXVECTOR3 obbx, obbz;//����(dObj)��X�����AZ�����̒����x�N�g��
	float obbLenx, obbLenz;

	obbx = pChara->GetObbX();
	obbLenx = D3DXVec3Length(&obbx);
	obbx /= obbLenx;//���K��

	obbz = pChara->GetObbZ();
	obbLenz = D3DXVec3Length(&obbz);
	obbz /= obbLenz;//���K��


	float lenX, lenZ;
	lenX = D3DXVec3Dot(&obbx,&direction);//������dObj��player�Ƃ�X�����̋������o��
	lenZ = D3DXVec3Dot(&obbz, &direction);//Z����

	//X�����AZ�����Ƃ��ɕ��̂�X.Z�����̑傫���x�N�g����菬������Γ������Ă���
	if (fabs(lenX) < obbLenx && fabs(lenZ) < obbLenz) {
		return true;
	}

	return false;
}

bool CollisionSystem::CheckObbCollision(GameObject* pBox, GameObject* pPoint) {

	D3DXVECTOR3 boxPos = pBox->GetPosition();
	D3DXVECTOR3 pointPos = pPoint->GetPosition();

	D3DXVECTOR3 direction = pointPos - boxPos;

	D3DXVECTOR3 obbx, obbz,obby;//����(dObj)��X�����AZ�����̒����x�N�g��
	float obbLenx, obbLenz,obbLeny;

	obbx = pBox->GetObbX();
	obbLenx = D3DXVec3Length(&obbx);
	obbx /= obbLenx;//���K��

	obbz = pBox->GetObbZ();
	obbLenz = D3DXVec3Length(&obbz);
	obbz /= obbLenz;//���K��

	obby = pBox->GetObbY();
	obbLeny = D3DXVec3Length(&obby);
	obby /= obbLeny;//���K��


	float lenX, lenZ,lenY;
	lenX = D3DXVec3Dot(&obbx, &direction);//������dObj��player�Ƃ�X�����̋������o��
	lenZ = D3DXVec3Dot(&obbz, &direction);//Z����
	lenY = D3DXVec3Dot(&obby, &direction);//Y����

	//X�����AZ�����Ƃ��ɕ��̂�X.Z�����̑傫���x�N�g����菬������Γ������Ă���
	if (fabs(lenX) < obbLenx && fabs(lenZ) < obbLenz && fabs(lenY) < obbLeny) {
		return true;
	}


	return false;


}

bool CollisionSystem::CheckObbObbCollision(GameObject* pBoxA, GameObject* pBoxB) {

	/*
	D3DXVECTOR3 boxAPos = pBoxA->GetPosition();
	D3DXVECTOR3 boxBPos = pBoxB->GetPosition();

	//������ boxA_X ��ݒ�
	D3DXVECTOR3 ax_vec = pBoxA->GetObbX();

	//OBB B�̎����擾
	D3DXVECTOR3 bx_vec = pBoxB->GetObbX();
	D3DXVECTOR3 by_vec = pBoxB->GetObbY();
	D3DXVECTOR3 bz_vec = pBoxB->GetObbZ();

	//�������Z�o


	D3DXVECTOR3 direction = pointPos - boxPos;

	D3DXVECTOR3 obbx, obbz, obby;//����(dObj)��X�����AZ�����̒����x�N�g��
	float obbLenx, obbLenz, obbLeny;

	obbx = pBox->GetObbX();
	obbLenx = D3DXVec3Length(&obbx);
	obbx /= obbLenx;//���K��

	obbz = pBox->GetObbZ();
	obbLenz = D3DXVec3Length(&obbz);
	obbz /= obbLenz;//���K��

	obby = pBox->GetObbY();
	obbLeny = D3DXVec3Length(&obby);
	obby /= obbLeny;//���K��


	float lenX, lenZ, lenY;
	lenX = D3DXVec3Dot(&obbx, &direction);//������dObj��player�Ƃ�X�����̋������o��
	lenZ = D3DXVec3Dot(&obbz, &direction);//Z����
	lenY = D3DXVec3Dot(&obby, &direction);//Y����

	//X�����AZ�����Ƃ��ɕ��̂�X.Z�����̑傫���x�N�g����菬������Γ������Ă���
	if (fabs(lenX) < obbLenx && fabs(lenZ) < obbLenz && fabs(lenY) < obbLeny) {
		return true;
	}

	

	*/
	return false;
	


}


void CollisionSystem::CollisionDamageObjectandDamageObject() {

	std::list<DamageObject*> dObjList = DamageObjectManager::GetDamageObjectList();

	int cnt = 0;


	for (DamageObject* pDamageObj : dObjList) {

		cnt++;
		//�C�e���[�^�Z�b�g
		//auto itr = dObjList.begin();
		//itr = std::next(itr,cnt);
		//DamageObject* pDamageObj2 = *itr;
		for (DamageObject* pDamageObj2 : dObjList) {

		//for (; itr != dObjList.end();) {
			//itr = std::next(itr, 1);
			//pDamageObj2 = *itr;

			//�������L�҂̒e�������珈�����Ȃ�
			if (pDamageObj2->GetOwner() == pDamageObj->GetOwner()) continue;

			//�V�[���h���m�Ȃ珈�����Ȃ�
			if (pDamageObj2->GetObjKind() == DOBJ_SHIELD &&  pDamageObj2->GetObjKind() == DOBJ_SHIELD) continue;

			if (CheckObbCollision(pDamageObj, pDamageObj2)) {


				//�e�ƒe
				if (pDamageObj2->GetObjKind() == DOBJ_NORMAL && pDamageObj2->GetObjKind() == DOBJ_NORMAL) {

					//��ő��E�Ƃ�������

				}


				//�e�ƃV�[���h
				if (pDamageObj2->GetObjKind() == DOBJ_SHIELD && pDamageObj->GetObjKind() == DOBJ_NORMAL) {

					//�V�[���h���̑ϋv�l�����
					//�_���[�W��
					int sDamage = pDamageObj->GetDamage();
					pDamageObj2->GetOwner()->ShieldDamage(sDamage);
					pDamageObj2->GetOwner()->shieldWParameter = -0.8f;

					//�U�����̔��������
					pDamageObj->SetDestroy();

					AudioManager::AudioPlay(AK_SE_SHIELDDAMAGE, false);


				}
				else if (pDamageObj->GetObjKind() == DOBJ_SHIELD && pDamageObj2->GetObjKind() == DOBJ_NORMAL) {

					//�V�[���h���̑ϋv�l�����
					//�_���[�W��
					int sDamage = pDamageObj2->GetDamage();
					pDamageObj->GetOwner()->ShieldDamage(sDamage);
					pDamageObj->GetOwner()->shieldWParameter = -0.8f;

					//�U�����̔��������
					pDamageObj2->SetDestroy();

					AudioManager::AudioPlay(AK_SE_SHIELDDAMAGE, false);


				}



			}

		}
	}

}


void CollisionSystem::CollisionMeshFieldBoxandCharacter() {

	std::list<MeshFieldBox*> dObjList = MeshFieldBoxManager::GetMeshBoxList();
	std::list<AnimationPlayer*> playerList = PlayerManager::GetPlayerList();


	for (AnimationPlayer* pPlayer : playerList) {

		for (MeshFieldBox* pMBoxObj : dObjList) {

			//���蔲�����Ȃ���ꏈ��
			if (pMBoxObj->GetThroughFlag()) {
				if (!CheckObbCollision(pMBoxObj, pPlayer)) continue;

				//�ォ�炶��Ȃ���΃o�E���h���Ȃ�
				if (pPlayer->GetPosition().y < pMBoxObj->GetPosition().y && pPlayer->GetBlownVelocity() < 0) continue;

				pPlayer->CharacterBound(pMBoxObj->GetPosition());



				continue;
			}

			//���ƃv���C���[���Փ˂��Ă����珈��
			if (CheckObbCollision(pMBoxObj, pPlayer)) {

				//�v���C���[�̈ʒu�����ɖ߂�
				//�������ォ�����Ă��鎞�͖߂��Ȃ�
				if (pPlayer->GetPosition().y - pPlayer->GetVelocity().y + 0.01f > pMBoxObj->GetPosition().y) continue;
				pPlayer->RewindPos();


				//���x���o�Ă��鎞�̓o�E���h����
				pPlayer->CharacterBound(pMBoxObj->GetPosition());


				//���x���������s���Ӗ��͂Ȃ��͂��Ȃ̂�break
				break;

			}

		}
	}
}
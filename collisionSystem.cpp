
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

		//無敵時間なら処理しない
		if (pPlayer->CheckInvincible()) continue;

		for (DamageObject* pDamageObj : dObjList) {

			//自分の弾だったら処理しない
			if (pPlayer == pDamageObj->GetOwner()) continue;

			//シールドなら処理しない
			if (pDamageObj->GetObjKind() == DOBJ_SHIELD) continue;

			//まだ存在していなければ処理しない
			if (!pDamageObj->CheckExist()) return;

			if (CheckObbCollision(pDamageObj,pPlayer->GetHitBox())) {

				////ダメージの加算
				pPlayer->DamageFunction(pDamageObj->GetDamage());

				////ふっとばし
				D3DXVECTOR2 b_vec;
				float blowPower = pDamageObj->GetBlowPower() * pPlayer->GetBlownCorrect();//ふっとばし力  技の吹っ飛ばし力 * 累積ダメージによる吹っ飛ばし係数
				D3DXVECTOR2 dObjtochara_Vec;//攻撃位置から敵位置へのベクトル
				dObjtochara_Vec.x = pPlayer->GetHitPosition().x - pDamageObj->GetPosition().x;
				dObjtochara_Vec.y = pPlayer->GetHitPosition().y - pDamageObj->GetPosition().y;
				D3DXVec2Normalize(&dObjtochara_Vec, &dObjtochara_Vec);

				b_vec = (dObjtochara_Vec + pDamageObj->GetBlowVec()) * blowPower;//(攻撃位置から敵位置へのベクトル + 技ごとの方向補正ベクトル) * ふっとばし力
				pPlayer->BlownCharacter(b_vec);

				////エフェクトの発生
				//エフェクトサイズ設定
				float effSize = 2.0f;

				//エフェクト位置設定
				D3DXVECTOR3 effPos = pPlayer->GetPosition();
				//前に表示するために動かす
				effPos.z -= 1.0f;
				effPos.y += 1.5f;

				EffectManager::SetAnimationEffect(effPos, effSize, ImageManager::GetTexId(TEX_EFF_BLOW), BLOWEFFECT_LIFE, BLOWEFFECT_ENUM);

				//ふっとばしエフェクト
				pPlayer->BlownEffect();

				//衝突音を鳴らす
				AudioManager::AudioPlay(pDamageObj->GetSoundKind(),false);


				//消す処理
				pDamageObj->SetDestroy();




			}

		}
	}

}

//当たっていたらtrueを返す
bool CollisionSystem::CheckObbCollision_DamageObjectandCharacter(GameObject* pChara, GameObject* pdObj) {

	D3DXVECTOR3 charaPos = pChara->GetPosition();
	D3DXVECTOR3 dObjPos = pdObj->GetPosition();

	D3DXVECTOR3 direction = dObjPos - charaPos;

	D3DXVECTOR3 obbx, obbz;//物体(dObj)のX方向、Z方向の長さベクトル
	float obbLenx, obbLenz;

	obbx = pChara->GetObbX();
	obbLenx = D3DXVec3Length(&obbx);
	obbx /= obbLenx;//正規化

	obbz = pChara->GetObbZ();
	obbLenz = D3DXVec3Length(&obbz);
	obbz /= obbLenz;//正規化


	float lenX, lenZ;
	lenX = D3DXVec3Dot(&obbx,&direction);//ここでdObjとplayerとのX方向の距離を出す
	lenZ = D3DXVec3Dot(&obbz, &direction);//Z方向

	//X方向、Z方向ともに物体のX.Z方向の大きさベクトルより小さければ当たっている
	if (fabs(lenX) < obbLenx && fabs(lenZ) < obbLenz) {
		return true;
	}

	return false;
}

bool CollisionSystem::CheckObbCollision(GameObject* pBox, GameObject* pPoint) {

	D3DXVECTOR3 boxPos = pBox->GetPosition();
	D3DXVECTOR3 pointPos = pPoint->GetPosition();

	D3DXVECTOR3 direction = pointPos - boxPos;

	D3DXVECTOR3 obbx, obbz,obby;//物体(dObj)のX方向、Z方向の長さベクトル
	float obbLenx, obbLenz,obbLeny;

	obbx = pBox->GetObbX();
	obbLenx = D3DXVec3Length(&obbx);
	obbx /= obbLenx;//正規化

	obbz = pBox->GetObbZ();
	obbLenz = D3DXVec3Length(&obbz);
	obbz /= obbLenz;//正規化

	obby = pBox->GetObbY();
	obbLeny = D3DXVec3Length(&obby);
	obby /= obbLeny;//正規化


	float lenX, lenZ,lenY;
	lenX = D3DXVec3Dot(&obbx, &direction);//ここでdObjとplayerとのX方向の距離を出す
	lenZ = D3DXVec3Dot(&obbz, &direction);//Z方向
	lenY = D3DXVec3Dot(&obby, &direction);//Y方向

	//X方向、Z方向ともに物体のX.Z方向の大きさベクトルより小さければ当たっている
	if (fabs(lenX) < obbLenx && fabs(lenZ) < obbLenz && fabs(lenY) < obbLeny) {
		return true;
	}


	return false;


}

bool CollisionSystem::CheckObbObbCollision(GameObject* pBoxA, GameObject* pBoxB) {

	/*
	D3DXVECTOR3 boxAPos = pBoxA->GetPosition();
	D3DXVECTOR3 boxBPos = pBoxB->GetPosition();

	//分離軸 boxA_X を設定
	D3DXVECTOR3 ax_vec = pBoxA->GetObbX();

	//OBB Bの軸を取得
	D3DXVECTOR3 bx_vec = pBoxB->GetObbX();
	D3DXVECTOR3 by_vec = pBoxB->GetObbY();
	D3DXVECTOR3 bz_vec = pBoxB->GetObbZ();

	//何かを算出


	D3DXVECTOR3 direction = pointPos - boxPos;

	D3DXVECTOR3 obbx, obbz, obby;//物体(dObj)のX方向、Z方向の長さベクトル
	float obbLenx, obbLenz, obbLeny;

	obbx = pBox->GetObbX();
	obbLenx = D3DXVec3Length(&obbx);
	obbx /= obbLenx;//正規化

	obbz = pBox->GetObbZ();
	obbLenz = D3DXVec3Length(&obbz);
	obbz /= obbLenz;//正規化

	obby = pBox->GetObbY();
	obbLeny = D3DXVec3Length(&obby);
	obby /= obbLeny;//正規化


	float lenX, lenZ, lenY;
	lenX = D3DXVec3Dot(&obbx, &direction);//ここでdObjとplayerとのX方向の距離を出す
	lenZ = D3DXVec3Dot(&obbz, &direction);//Z方向
	lenY = D3DXVec3Dot(&obby, &direction);//Y方向

	//X方向、Z方向ともに物体のX.Z方向の大きさベクトルより小さければ当たっている
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
		//イテレータセット
		//auto itr = dObjList.begin();
		//itr = std::next(itr,cnt);
		//DamageObject* pDamageObj2 = *itr;
		for (DamageObject* pDamageObj2 : dObjList) {

		//for (; itr != dObjList.end();) {
			//itr = std::next(itr, 1);
			//pDamageObj2 = *itr;

			//同じ所有者の弾だったら処理しない
			if (pDamageObj2->GetOwner() == pDamageObj->GetOwner()) continue;

			//シールド同士なら処理しない
			if (pDamageObj2->GetObjKind() == DOBJ_SHIELD &&  pDamageObj2->GetObjKind() == DOBJ_SHIELD) continue;

			if (CheckObbCollision(pDamageObj, pDamageObj2)) {


				//弾と弾
				if (pDamageObj2->GetObjKind() == DOBJ_NORMAL && pDamageObj2->GetObjKind() == DOBJ_NORMAL) {

					//後で相殺とかを入れる

				}


				//弾とシールド
				if (pDamageObj2->GetObjKind() == DOBJ_SHIELD && pDamageObj->GetObjKind() == DOBJ_NORMAL) {

					//シールド側の耐久値を削る
					//ダメージ分
					int sDamage = pDamageObj->GetDamage();
					pDamageObj2->GetOwner()->ShieldDamage(sDamage);
					pDamageObj2->GetOwner()->shieldWParameter = -0.8f;

					//攻撃側の判定を消す
					pDamageObj->SetDestroy();

					AudioManager::AudioPlay(AK_SE_SHIELDDAMAGE, false);


				}
				else if (pDamageObj->GetObjKind() == DOBJ_SHIELD && pDamageObj2->GetObjKind() == DOBJ_NORMAL) {

					//シールド側の耐久値を削る
					//ダメージ分
					int sDamage = pDamageObj2->GetDamage();
					pDamageObj->GetOwner()->ShieldDamage(sDamage);
					pDamageObj->GetOwner()->shieldWParameter = -0.8f;

					//攻撃側の判定を消す
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

			//すり抜け床なら特殊処理
			if (pMBoxObj->GetThroughFlag()) {
				if (!CheckObbCollision(pMBoxObj, pPlayer)) continue;

				//上からじゃなければバウンドしない
				if (pPlayer->GetPosition().y < pMBoxObj->GetPosition().y && pPlayer->GetBlownVelocity() < 0) continue;

				pPlayer->CharacterBound(pMBoxObj->GetPosition());



				continue;
			}

			//床とプレイヤーが衝突していたら処理
			if (CheckObbCollision(pMBoxObj, pPlayer)) {

				//プレイヤーの位置を元に戻す
				//ただし上から乗っている時は戻さない
				if (pPlayer->GetPosition().y - pPlayer->GetVelocity().y + 0.01f > pMBoxObj->GetPosition().y) continue;
				pPlayer->RewindPos();


				//速度が出ている時はバウンドする
				pPlayer->CharacterBound(pMBoxObj->GetPosition());


				//何度も処理を行う意味はないはずなのでbreak
				break;

			}

		}
	}
}
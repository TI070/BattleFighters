
#include "moveAI.h"
#include "stageSystem.h"
#include "playerManager.h"
#include "battleAI.h"
#include <typeinfo>

void MoveAI::ChangeAI(MoveAI* s_ai) {

	//親AIの移動AIを変更する
	parentAI->SetMoveAI(s_ai);

	//自殺
	delete this;
}
void MoveAI::Update() {

	//崖つかまり状態なら状態遷移
	if (owner->GetCharaCon() == CC_CLLIFGRAP && typeid(this) != typeid(MoveAI_Cliff)) {
		MoveAI_Cliff* AI_TT = new MoveAI_Cliff(parentAI);
		ChangeAI(AI_TT);
	}
	else {
		//足場外なら戻るようにする
		AvoidCrisis();
	}
}

float MoveAI::GetTargetDistance() {

	D3DXVECTOR3 vec = owner->GetPosition() - target->GetPosition();
	float dis = D3DXVec3Length(&vec);
	return dis;

}

void MoveAI::ChaseTargetHeight() {

	//自分よりも相手のほうが一定以上高ければジャンプ
	if (owner->GetPosition().y < 0.0f + target->GetPosition().y) owner->Jump();

	//自分よりも相手のほうが一定以上低くてすり抜け可能足場にいるならすり抜け
	if (owner->GetPosition().y > 0.0f + target->GetPosition().y) {
		owner->ThroughFloor();
	}
}


void MoveAI::RandomJump(int rnd) {

	int r = rand() % rnd;

	if (r == 0) owner->Jump();

}

void MoveAI::AvoidCrisis() {

	//判断可能かチェック
	if (!parentAI->CheckDecideAble()) return;

	//自分の位置が中央エリアにいるかどうかを判断し、中央エリアにいなければ中央に向かおうとする
	Area center = StageSystem::GetCenterArea();
	D3DXVECTOR3 ownerPosition = owner->GetPosition();

	if (center.rightMax < ownerPosition.x || center.leftMax > ownerPosition.x) {
		MoveAI_ToCenter* AI_TT = new MoveAI_ToCenter(parentAI);
		ChangeAI(AI_TT);
	}

}

void MoveAI::AvoidAttack() {

	//敵の攻撃を避けるように努力する

	//ジャンプ


	//シールド


}

void MoveAI_Stop::Move() {

	owner->SetVelocity({0,owner->GetVelocity().y,0});
	owner->SetAnimationState(AS_WAIT);

	//ストップとはいえ高さを合わせるくらいはする
	ChaseTargetHeight();

}

void MoveAI_Stop::Think() {

	//状態解除する何かを考える
	//レンジ内にいなかったらレンジを再抽選する
	if (parentAI->GetToTargetDistance() < GetTargetDistance()) {

		parentAI->ThinktoTargetDistance();

		//ターゲット移動にする？
		MoveAI_ToTarget* AI_TT = new MoveAI_ToTarget(parentAI);
		ChangeAI(AI_TT);

	}

	//決められたストップ時間を超えたらターゲット移動にする
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

	//判断可能かチェック
	if (!parentAI->CheckDecideAble()) return;

	//自分の位置が中央エリアにいるかどうかを判断し、中央エリアにいなければ中央に向かおうとする
	Area center = StageSystem::GetCenterArea();
	D3DXVECTOR3 ownerPosition = owner->GetPosition();

	//左右の判断
	if (center.rightMax < ownerPosition.x) {
		owner->SetCharaDir(CD_LEFT);
	}
	else if (center.leftMax > ownerPosition.x) {
		owner->SetCharaDir(CD_RIGHT);
	}
	else {//中央エリア内にいる

		//敵をターゲット
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

	//判断可能かチェック
	if (!parentAI->CheckDecideAble()) return;

	//敵との距離セット


	//一定距離以上離れていたら移動する
	if (parentAI->GetToTargetDistance() < GetTargetDistance()) {


		//敵のいる方向に向かって移動する
		if (owner->GetPosition().x > target->GetPosition().x) {
			owner->SetCharaDir(CD_LEFT);
		}
		else if (owner->GetPosition().x < target->GetPosition().x) {
			owner->SetCharaDir(CD_RIGHT);
		}
	}
	else {//そうでなければ移動しない

		//parentAI->OwnerStop();
		MoveAI_Stop* AI_TT = new MoveAI_Stop(parentAI);
		ChangeAI(AI_TT);

	}

	ChaseTargetHeight();

}

void MoveAI_Cliff::Move() {



}

void MoveAI_Cliff::Think() {

	//判断可能かチェック
	if (!parentAI->CheckDecideAble()) return;

	//復帰
	owner->CliffAction(AnimationPlayer::CA_UP);

	//崖つかまり状態を解除
	owner->SetCharaCon(CC_NORMAL);
	owner->SetCliffGrapCooltime(CRIFFGRAP_COOLTIME);

	//復帰したら中央を目指す
	MoveAI_ToTarget* AI_TT = new MoveAI_ToTarget(parentAI);
	ChangeAI(AI_TT);
}
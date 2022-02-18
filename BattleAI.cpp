
#include "main.h"
#include "battleAI.h"
#include "moveAI.h"
#include "attackAI.h"
#include "stageSystem.h"
#include "animationPlayer.h"
#include "playerManager.h"


void BattleAI::Init() {

	//仮置き
	aiMoveMode = AI_MOVEMODE_TOCENTER;
	aiAttackMode = AI_ATTACKMODE_TARGET;

	toTargetDistance = 1.0f;

	//パラメータ初期化
	thinkTime = 0;
	decideTime = 60;
	thinkReset = false;
	decideTimeMax = 60;
	decideTimeMin = 30;

	//技データ取得
	atkData = owner->GetAttackData();

	//とりあえずターゲット設定
	TargetGet();

	//とりあえずレンジ設定
	GenerateTTDistanceBase();

	//とりあえず使う技設定
	SelectAtkAction();

	//とりあえずレンジ選ぶ
	ThinktoTargetDistance();



	//とりあえずAI生成
	//moveAI = new MoveAI_ToCenter(this);
	moveAI = new MoveAI_ToTarget(this);

	//親のキャラで変更
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

	//デバッグ
	//selectAtk = AK_NEUTRAL;

}

void BattleAI::Think() {

	//ターゲットがいなければ取り直す
	if (target == nullptr) {
		TargetGet();
		attackAI->SetTarget(target);
		moveAI->SetTarget(target);

	}
	if (!owner->CheckActionCooltime()) {
		//速度リセット
		D3DXVECTOR3 s_vec = { 0,0,0 };
		s_vec.y = owner->GetVelocity().y;
		owner->SetVelocity(s_vec);
	}


	owner->SetThroughFlag(false);

	//移動AI
	moveAI->Update();
	moveAI->Think();

	//攻撃AI
	attackAI->Update();
	attackAI->Think();

	//思考時間プラス
	thinkTime++;
	if (thinkReset) {
		thinkTime = 0;
		thinkReset = false;
	}

}

void BattleAI::Update() {


	//移動AI
	//チャージ中は移動できない
	if (owner->GetActionCharge() == 0) {
		moveAI->Move();
	}

	//攻撃AI

	attackAI->Attack();



	//デバッグ
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

	//判断可能かチェック
	if (!CheckDecideAble()) return;

	//敵との距離セット
	D3DXVECTOR3 vec = owner->GetPosition() - target->GetPosition();
	float dis = D3DXVec3Length(&vec);

	//一定距離以上離れていたら移動する
	if (toTargetDistance < dis) {


		//敵のいる方向に向かって移動する
		if (owner->GetPosition().x > target->GetPosition().x) {
			//OwnerMove(CD_LEFT);
			owner->SetCharaDir(CD_LEFT);
		}
		else if (owner->GetPosition().x < target->GetPosition().x) {
			//OwnerMove(CD_RIGHT);
			owner->SetCharaDir(CD_RIGHT);
		}
	}
	else {//そうでなければ移動しない

		OwnerStop();

	}



	//自分よりも相手のほうが一定以上高ければジャンプ 仮置き
	if (owner->GetPosition().y < 0.0f + target->GetPosition().y) owner->Jump();

	//自分よりも相手のほうが一定以上低くてすり抜け可能足場にいるならすり抜け 仮置き
	if (owner->GetPosition().y > 0.0f + target->GetPosition().y) {
		owner->ThroughFloor();
	}

}


void BattleAI::OwnerStop() {

	owner->SetAnimationState(AS_WAIT);

}

void BattleAI::SelectAtkAction() {

	////技のデータを参照し、適当な技を選ぶ

	std::vector<int> pAtkOption;

	//技の範囲を参照し、ターゲットが中にいるか確認
	for (int i = 0; i < 4;i++) {

		//ターゲットがいなかったら処理しない
		if (target == nullptr) return;

		//エリアの設定
		Area atkArea;
		D3DXVECTOR3 sPos = owner->GetPosition() + atkData[i].offsetPos;
		CHARA_DIRECTION cDir = owner->GetCharaDir();
		float vecX,vecY;
		
		//方向によるレンジ設定の変更
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

		//範囲内にターゲットが入っているかチェック
		if (atkArea.CheckInArea({ target->GetPosition().x,target->GetPosition().y })) {

			//入っていたら候補リストに追加
			pAtkOption.push_back(i);
		}


	}

	//完成した候補リストからランダムで技を決定
	if (pAtkOption.size() > 0) {
		int r = rand() % pAtkOption.size();
		selectAtk = (ATTACK_KIND)pAtkOption[r];

	}
	//候補リストが作れなかったらどうしようかわからないけどとりあえずランダムで設定
	else {
		selectAtk = (ATTACK_KIND)(rand() % 4);

		//半分の確率で例外を代入
		if (rand() % 2 == 0) selectAtk = AK_MAX;
	}


}

void BattleAI::TargetGet() {
	
	//実際には距離とかストックとかダメージとかを参照してターゲット決めればいいけど
	//とりあえず近くの奴適当に指しとくか

	target = PlayerManager::GetNearPlayer(owner);

}

void BattleAI::ThinktoTargetDistance() {



	//どれにするか選ぶ　仮置きランダム

	int r = rand() % TTRANGE_MAX;

	toTargetDistance = ttRange[r];
	//toTargetDistance = ttRange[3];

}

void BattleAI::GenerateTTDistanceBase() {

	//技レンジを参照して短・中・長の長さを生成する
	float plusR = 0.2f;//補正値

	std::vector<int> atkRangeNum;

	for (int i = 0; i < 4; i++) {
		atkRangeNum.push_back(atkData[i].range);
	}

	std::sort(atkRangeNum.begin(), atkRangeNum.end());

	ttRange[SHORT_RANGE] = atkRangeNum[0];
	ttRange[MIDDLE_RANGE] = (atkRangeNum[0] + atkRangeNum[1] + atkRangeNum[2] + atkRangeNum[3]) / 4.0f;
	ttRange[LONG_RANGE] = atkRangeNum[3];

	//補正
	ttRange[SHORT_RANGE] -= plusR;
	ttRange[MIDDLE_RANGE] -= plusR;
	ttRange[LONG_RANGE] -= plusR;
}

void BattleAI::MoveAIThink() {

	moveAI->Think();
}

bool BattleAI::CheckInAtkArea(ATTACK_KIND s_ak) {

	//例外ならfalse
	if (s_ak == AK_MAX)return false;

	//エリアの設定
	Area atkArea;
	D3DXVECTOR3 sPos = owner->GetPosition() + atkData[s_ak].offsetPos;
	CHARA_DIRECTION cDir = owner->GetCharaDir();
	float vecX, vecY;

	//方向によるレンジ設定の変更
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

	//範囲内にターゲットが入っているかチェック
	if (atkArea.CheckInArea({ target->GetPosition().x,target->GetPosition().y })) {

		return true;
	}
	return false;
}
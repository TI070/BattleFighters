#pragma once

#include "animationPlayer.h"

enum AI_LEVEL {

	AI_LEVEL1


};


enum AI_ATTACKMODE {

	AI_ATTACKMODE_TARGET//ターゲットした敵に攻撃する

};


enum AI_MOVEMODE {

	AI_MOVEMODE_STOP,//
	AI_MOVEMODE_TOCENTER,//中央に向かう・中央を維持する
	AI_MOVEMODE_TOENEMY,//敵の方に向かう

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
	float toTargetDistance;//敵に対して保とうとする距離

	float decideTime;//行動を判断するまでの時間
	float thinkTime;//考えた時間
	//思考時間振れ幅
	float decideTimeMax;
	float decideTimeMin;

	bool thinkReset;

	AttackData* atkData;
	ATTACK_KIND selectAtk;

	//AIステート
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

	bool CheckDecideAble() { //思考時間が判断可能時間を上回ったらtrueを返す
		if (thinkTime >= decideTime) {

			thinkReset = true;

			//次回思考時間を設定
			int rng = decideTimeMax - decideTimeMin;
			decideTime = decideTimeMin + rand() % rng;

			return true;
		}
		return false;
	}

	//敵をターゲットする
	void TargetGet();

	//技選び
	void SelectAtkAction();

	void SetMoveAI(MoveAI* s_ai) { moveAI = s_ai; }
	void SetAttackAI(AttackAI* s_ai) { attackAI = s_ai; }

	void GenerateTTDistanceBase();
	void ThinktoTargetDistance();//相手と取ろうとする距離を生成する

	//外からThinkだけ呼ぶ
	void MoveAIThink();

	//選択中の技の範囲内にターゲットが入っているかどうかを確認
	//入っていたらtrueを返す
	bool CheckInAtkArea(ATTACK_KIND s_ak);

};

class Knight : BattleAI {

};
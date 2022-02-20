#pragma once

#include "animationPlayer.h"
#include "battleAI.h"

enum MOVEAI_KIND {

	MOVEAI_CENTER,
	MOVEAI_STOP,
	MOVEAI_TARGET,
	MOVEAI_CLIFF

};


class MoveAI {

private:


protected:
	AnimationPlayer* owner;//本体
	AnimationPlayer* target;//対象
	BattleAI* parentAI;//親AI

	//ターゲットと高さを合わせようとする動き
	void ChaseTargetHeight();

	//何の前触れもなくジャンプする
	void RandomJump(int rnd);
	void AvoidCrisis();

	int AI_Level = 1;//てきのつよさ

public:
	MoveAI(BattleAI* s_ai) { 
		parentAI = s_ai;
		owner = s_ai->GetOwner();
		target = s_ai->GetTarget();

	}


	virtual void Move() = 0;
	virtual void Think() = 0;
	void SetOwner(AnimationPlayer* s_p) { owner = s_p; }
	void SetTarget(AnimationPlayer* s_p) { target = s_p; }

	void ChangeAI(MoveAI* s_ai);
	void Update();//共通アップデート
	float GetTargetDistance();

	virtual MOVEAI_KIND GetType() = 0;

	void AvoidAttack();

};

class MoveAI_Stop : public MoveAI{
private:
	int stopLimit = 60;
	int stopCnt = 0;
public:
	MoveAI_Stop(BattleAI* s_ai):MoveAI(s_ai) {}
	void Move();
	void Think();

	MOVEAI_KIND GetType() { return MOVEAI_STOP; }
};

class MoveAI_ToCenter : public MoveAI {
public:
	MoveAI_ToCenter(BattleAI* s_ai) :MoveAI(s_ai) {}
	void Move();
	void Think();
	MOVEAI_KIND GetType() { return MOVEAI_CENTER; }
};

class MoveAI_ToTarget : public MoveAI {
public:
	MoveAI_ToTarget(BattleAI* s_ai) :MoveAI(s_ai) {}
	void Move();
	void Think();
	MOVEAI_KIND GetType() { return MOVEAI_TARGET; }
};

//崖つかまり移動AI
class MoveAI_Cliff : public MoveAI {
public:
	MoveAI_Cliff(BattleAI* s_ai) :MoveAI(s_ai) {}
	void Move();
	void Think();
	MOVEAI_KIND GetType() { return MOVEAI_CLIFF; }
};
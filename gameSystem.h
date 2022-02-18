#pragma once

#define STOCKMAX (5)
#define STAGEEND (5)

struct ResultData {

	int rank;//順位
	int score;
	int surviveTime;//生存時間
};

enum DEBUG_FUNCTION {
	
	DF_HITBOX,
	DF_MAX

};

enum GAMEMODE_BATTLE {

	GM_BATTLEROYAL,
	GM_ONEPLAYER

};

enum CHARA_KIND;
enum CHARA_MODE;

class Polygon2D;

class GameSystem{

private:
	static bool debugFlag[DF_MAX];
	static int charaStartStock[4];//各キャラの初期残機
	static int* charaCurrentStock[4];//各キャラの現在残機
	static bool stagePlaying;
	static ResultData resultData[4];

	static int timer;//タイマー（秒単位）
	static int addTimerCnt;

	static CHARA_KIND selectChara[4];
	static CHARA_MODE selectPlayerMode[4];

	static bool isJoin[4];
	static int setStockNum;

	static GAMEMODE_BATTLE currentGameMode;

	static int currentStage;//一人モード専用

	static int readyGoSw;

	static Polygon2D* readyGo;
	static Polygon2D* stageNumTex;

	static int alphaCount;

public:

	static void Init();
	static void DebugFlagOn(DEBUG_FUNCTION s_df) { debugFlag[s_df] = true; }
	static void DebugFlagOff(DEBUG_FUNCTION s_df) { debugFlag[s_df] = false; };
	static bool GetDebugFlag(DEBUG_FUNCTION s_df) { return debugFlag[s_df]; }
	static int GetStartStock(int charaNum) { return charaStartStock[charaNum]; }
	static void SetStartStock(int charaNum, int s_stockNum) { charaStartStock[charaNum] = s_stockNum; }
	static void SetStartStockAll(int s_stockNum) { for (int i = 0; i < 4; i++) charaStartStock[i] = s_stockNum;}
	static void SetCurrentStock(int charaNum,int* s_pStock) { charaCurrentStock[charaNum] = s_pStock; }
	static bool GetStagePlaing() { return stagePlaying; }
	static bool* GetStagePlayingP() { return &stagePlaying; }

	static void Update();

	//対戦システム
	static bool JudgeGameSet();
	static void GameSet();

	static void BattleSet();
	static void SetRankStock();
	static void SetSurviveTime(int playerNum);

	static void TimerUpdate();

	static ResultData GetResultData(int s_num) { return resultData[s_num]; }

	static void SetSelectChara(int playernum, CHARA_KIND s_ck) { selectChara[playernum] = s_ck; }
	static CHARA_KIND GetSelectChara(int playerNum) { return selectChara[playerNum]; }


	static void SetPlayerMode(int playerNum,CHARA_MODE s_cm) { selectPlayerMode[playerNum] = s_cm; }
	static CHARA_MODE GetPlayerMode(int playerNum) { return selectPlayerMode[playerNum]; }


	static void PlayerModeChange(int playerNum);

	static void SetisJoin(int playerNum, bool s_bool) { isJoin[playerNum] = s_bool; }
	static int GetisJoin(int playerNum) { return isJoin[playerNum]; }

	static void SetStockNum(int s_num) { 
		setStockNum = s_num;
		SetStartStockAll(setStockNum);
	}
	static int GetStockNum() { return setStockNum; }
	static void AddStockNum(int add) { 
		setStockNum += add;
		if (setStockNum <= 0) setStockNum = 1;
		else if (setStockNum > STOCKMAX) setStockNum = STOCKMAX;
	
		SetStartStockAll(setStockNum);
	}

	static void SetGameMode(GAMEMODE_BATTLE s_gm) { currentGameMode = s_gm; }

	static void OnePlayerInit();

	static void ReadyGoUpdate();
	static void SetCurrentStage(int sNum) { currentStage = sNum; }
	static int GetCurrentStage() { return currentStage;}


};
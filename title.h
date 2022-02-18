#pragma once
#ifndef TITLE_H
#define TITLE_H

#include "polygon2d.h"
#include "stageSystem.h"
#include <list>

#define CHARASELECT_XNUM (2)
#define CHARASELECT_YNUM (2)

#define CHARA_KINDNUM (CHARASELECT_XNUM * CHARASELECT_YNUM)

#define CHARASELECT_WIDTH (140)
#define CHARASELECT_HEIGHT (140)

#define SELECTMAKER_HEIGHT (64)
#define SELECTMAKER_WIDTH (64)

#define SELECTHAND_HEIGHT (96)
#define SELECTHAND_WIDTH (96)

#define PLAYERMODEPLATE_HEIGHT (64)
#define PLAYERMODEPLATE_WIDTH (128)


#define MARKER_MOVESPEED (6)

#define PARTICLEGENERATE_TIME (60)

#define MENUBGPARTS_DISTANCE (960 / 4.0f)

enum TITLE_MODE {

	TM_TITLE,
	TM_MENU,
	TM_CHARASELECT,
	TM_ONEPLAYER,
	TM_KEYCONFIG,
	TM_MAX

};

enum TITLEMENU_MENU {

	TMM_ONCE,
	TMM_BATTLE,
	TMM_KEYCONFIG,
	TMM_MAX
};

enum KEYCONFIG_MENU {

	KCM_BUTTON1,
	KCM_BUTTON2,
	KCM_BUTTON3,
	KCM_BUTTON4,
	KCM_BUTTONMAX

};


struct SelectHand {
	Polygon2D* image;
	int catchMarkerNum;
};

struct BoundPolygon2D {
	Polygon2D* image;
	D3DXVECTOR2 vec;
	float size;
	bool breakFlag;
	bool exist;
};

class Audio;
class UIWindow;
class TexStringAlphabet;
class ParticleSystem;

class Title {



private:

	static int TitleTexId;

	static Polygon2D* titleLogo;

	static Audio* TitleBgm;
	static Audio* cursorMoveSE;
	static Audio* cursorDecideSE;

	static UIWindow* pTitleWindow;
	static UIWindow* pUIMessageWindow;
	static TexString* pUIMessageText;
	static bool pUIMessageSw;
	static int pUIMessageTimer;

	static int cursor;

	static TexString* gameStartText;
	static UIWindow* gameStartWindow;

	static UIWindow* gameMenuWindow_Once;
	static UIWindow* gameMenuWindow_Battle;
	static UIWindow* gameMenuWindow_KeyConfig;

	static Polygon2D* onceMenuText;
	static Polygon2D* battleMenuText;
	static Polygon2D* keyconfigMenuText;


	static TITLE_MODE currentMode;

	static UIWindow* gameMenuSelectWindow;

	static UIWindow* charaSelectWindows[CHARA_KINDNUM];
	static UIWindow* playerSelectWindows[4];
	static Polygon2D* playerSelectWindowTex[4];
	static Polygon2D* playerSelectImages[4];
	static Polygon2D* playerModeImages[4];

	static Polygon2D* selectMaker[4];

	static SelectHand P1Hand;
	static SelectHand P2Hand;


	static Area charaSelectArea[CHARA_KINDNUM];

	static Polygon2D* titleBg;

	static bool titleEffectOn;
	static std::list<ParticleSystem*> particleSystemList;
	static int particleGenerateCnt;

	static UIWindow* stockControllWindow;
	static TexString* stockControllText;
	static Polygon2D* leftSankaku;
	static Polygon2D* rightSankaku;

	static D3DXVECTOR4 tlShaderParameter;

	//タイトルバウンド用オブジェクト
	static BoundPolygon2D boundPolygon[4];

	static Polygon2D* titleMenuBg;

	//タイトルメニュー賑やかしポリゴン
	static Polygon2D* titleMenuParts[9];
	static Polygon2D* charaSelectBg;

	static Polygon2D* stockTex;

	static int acceptCooltime;

	static int cursorKeyConfig;


	//キーコンフィグウインドウ
	static UIWindow* keyConfigLogoWindow;
	static UIWindow* keyConfigWindow[4];
	static Polygon2D* keyConfigWindowTex[4];
	static Polygon2D* keyConfigWindowTexString[4];

	static bool keyconSelect;
	static TexString* keyconWindowString[4];

	static TexString* keyconPlayerNumString;
	static int keyconfigControllerNum;

public:

	static void Initialize();
	static void Draw();
	static void Uninit();
	static void Update();
	static void MenuSelect();
	static void MenuColorUpdate();

	static void MenuDecide();

	static void TitleModeSet(TITLE_MODE s_tm);

	static void MarkerMove(int s_num);
	static void HandMove();

	static void JoinPlayer(int playerNum);
	static void SetJoinPlayer(int playerNum,bool s_bool);
	static int GetJoinPlayer(int playerNum);

	static void TitleEffectFunction();
	static bool ParticleGenerateUpdate();

	static void TitleFire(D3DXVECTOR3 s_pos,float s_size);

	static void TitleBreakEffect(float s_size,int index);


	static void BoundPolygonInit();
	static void BoundPolygonUpdate();
	static void BoundPolygonDraw();
	static void BoundPolygonCollision();

	static void BoundPolygonSpdSet(int index);

	static void BoundPolygonBreak(int index);

	static void TitleMenuBgInit();
	static void TitleMenuBgUpdate();

	static void UIMessageUpdate();

};
#endif
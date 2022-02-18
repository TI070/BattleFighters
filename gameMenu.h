#pragma once

#include "unitConfig.h"

enum GAMEMENU_KIND {

	GAMEMENU_UNITSELECT,
	GAMEMENU_UNITMOVE,
	GAMEMENU_MAKEROAD,
	GAMEMENU_UNITCLASSCHANGE,
	GAMEMENU_DATASAVE,
	GAMEMENU_TOTITLE,
	GAMEMENU_END

};

enum GAMEMENU_ICON {

	GAMEMENUICON_ROAD,
	GAMEMENUICON_STATUE,
	GAMEMENUICON_BREAK,
	GAMEMENUICON_SWORD,
	GAMEMENUICON_WIZARD,
	GAMEMENUICON_PRIEST,
	GAMEMENUICON_END
	
};




class Polygon2D;
class UIWindow;
class Player;
class Audio;

class GameMenu {

private:

	static int cursor;

	static Polygon2D* activeF_p2;
	static Polygon2D* activeIconF_p2;

	static Polygon2D* QIcon;
	static Polygon2D* EIcon;

	static Polygon2D* gameMenuTex[GAMEMENU_END];
	static Polygon2D* gameMenuIconTex[GAMEMENUICON_END];

	static UIWindow* menuExpWindow;//メニューの説明ウインドウ

	static bool MenuExpDisplayFlag;

	static Audio* cursorMoveSE;
	static Audio* cursorDecideSE;

	static Player* pP;

	static UNIT_TYPE_DETAIL changeJob;

	static int gmIcon;

	static GAMEMENU_KIND iconMode;

	static UIWindow* menuCostString;//使用コスト

public:

	static void Init();
	static void Update();
	static void ActiveFrameUpdate();
	static void Draw();
	static void Uninit();

	static void SelectMenu();

	static void CursorMove(int num);
	static void IconCursorMove(int num);

	static void RealUnitSelect();
	static void UnitSelect();
	static void UnitClassChange();
	static void DataSave();
	static void DataLoad();
	static void ToTitle();
	static void MakeRoad();

	static void ColorUpdate();

	static UNIT_TYPE_DETAIL GetChangeJob() { return changeJob; }

	static void IconUpdate();

	static int GetSelectIcon() { return gmIcon; }

	static void MenuExpSet();

	static void SetJMessage(int row, int col, const char* mes);
	static void SetAMessage(int row, int col, const char* mes);

	static void SetMenuCostString(int s_num);

};
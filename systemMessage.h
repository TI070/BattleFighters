#pragma once

#include <list>
#include "managerBase.h"


#define MESSAGEWINDOW_SIZE_X (600)
#define MESSAGEWINDOW_SIZE_Y (150)

#define MESSAGEWINDOW_POS_X (500)

#define MESSAGEWINDOW_SIZE {MESSAGEWINDOW_SIZE_X,MESSAGEWINDOW_SIZE_Y}



class UIWindow;


class SystemMessage : public ManagerBase{

private:
	static UIWindow* pMessageWindow;

	static bool windowAble;
	static std::list<UIWindow*> pList;

public:
	static void SetMessageWindow();

	static void Init();
	static void Uninit();

	//ウインドウを非表示にして
	static void WindowReset();
	static void SetJMessage(int row, int col, const char* mes);
	static void SetAMessage(int row, int col, const char* mes);

	//現在設定中のメッセージウインドウをリストに追加
	static void SetList();

	//リストの先頭を削除し詰める
	static void TurnPage();

	static void Update();
	static void Draw();


	//使用するメッセージウインドウのリスト
	static void SetTutorialMessage_1();
	static void TestMessage();


};
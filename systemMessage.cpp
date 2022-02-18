
#include "systemMessage.h"
#include "imageManager.h"
#include "polygon2d.h"
#include "UIwindow.h"
#include "key_manager.h"



bool SystemMessage::windowAble = false;
UIWindow* SystemMessage::pMessageWindow = nullptr;
std::list<UIWindow*> SystemMessage::pList;

void SystemMessage::Init() {

	windowAble = false;
}

void SystemMessage::SetMessageWindow() {


	//残っているものがあれば消す
	//if (pMessageWindow != nullptr) {
	//	WindowReset();
	//}

	//pMessageWindow = ImageManager::SetUIWindow({SCREEN_WIDTH,SCREEN_HEIGHT - MESSAGEWINDOW_SIZE_Y/2 }, MESSAGEWINDOW_SIZE, ImageManager::GetTexId(TEX_UI_MESSAGEWINDOW), DEPTH_ZERO);
	//pMessageWindow = new UIWindow({ MESSAGEWINDOW_POS_X,SCREEN_HEIGHT - MESSAGEWINDOW_SIZE_Y / 2 }, MESSAGEWINDOW_SIZE, ImageManager::GetTexId(TEX_UI_MESSAGEWINDOW), DEPTH_ZERO);
	//pMessageWindow->SetWindowTex(new Polygon2D({ MESSAGEWINDOW_POS_X,SCREEN_HEIGHT - MESSAGEWINDOW_SIZE_Y / 2 }, MESSAGEWINDOW_SIZE, ImageManager::GetTexId(TEX_UI_MESSAGEWINDOW), DEPTH_ZERO));

}

void SystemMessage::WindowReset() {

	pMessageWindow->Uninit();
	pMessageWindow = nullptr;
	windowAble = false;

}

void SystemMessage::SetAMessage(int row, int col, const char* mes) {

	if (pMessageWindow == nullptr) return;

	//pMessageWindow->SetATextTex(new TexStringAlphabet({220.0f + 32 * (col - 1),410.0f + 32 * (row - 1) }, 32, ImageManager::GetTexId(TEX_JAPANESE_FONT), DEPTH_ZERO, mes, { 0.0f,0.0f,0.0f,1.0f }));


}

void SystemMessage::SetJMessage(int row, int col, const char* mes) {

	if (pMessageWindow == nullptr) return;

	//pMessageWindow->SetJTextTex(new TexStringJapanese({ 220.0f + 32 * (col - 1),410.0f + 32 * (row - 1) }, 32, ImageManager::GetTexId(TEX_JAPANESE_FONT), DEPTH_ZERO, mes, { 0.0f,0.0f,0.0f,1.0f }));

}

void SystemMessage::SetList() {


	//追加位置が先頭でなければisUseをfalseにする
	if(pList.size() != 0) pMessageWindow->SetIsUse(false);

	//リストの後ろに追加
	pList.push_back(pMessageWindow);


}

void SystemMessage::TurnPage() {

	std::list<UIWindow*>::iterator  itr = pList.begin();
	
	UIWindow* pUW = *itr;

	pUW->SetDestroy();
	

	itr++;

	if (pList.size() <= 1) return;

	pUW = *itr;

	pUW->SetIsUse(true);
}

void SystemMessage::Update() {

	if (pList.size() == 0) return;

	//仮
	if (KeyManager_Trigger(KM_K)) {
		TurnPage();
	}

	ManagerBase::RemoveMember<UIWindow>(&pList);

}

void SystemMessage::Draw() {

	if (pList.size() == 0) return;

	std::list<UIWindow*>::iterator  itr = pList.begin();

	UIWindow* pUW = *itr;

	pUW->Draw();

}

void SystemMessage::SetTutorialMessage_1() {

	//メッセージ１
	SetMessageWindow();
	SetJMessage(1, 1, "このゲぁムは　みちをひいてむらをつくり");
	SetJMessage(2, 1, "ひとをそだてて　モンスタぁをたおすゲぁムです");
	SetList();

	//メッセージ２
	SetMessageWindow();
	SetJMessage(1, 1, "プレイヤぁは　カぁソルをそうさしてさまざまなアクションをします");
	SetJMessage(2, 1, "カぁソルいどう");
	SetAMessage(2, 5, "WASD key");
	SetList();

	//メッセージ3
	SetMessageWindow();
	SetJMessage(1, 1, "アクションをするときは　がめんうえのメニュぁからそうさをえらびます");
	SetJMessage(2, 1, "メニューぁの");
	SetAMessage(2, 4, "MAKEROAD");
	SetJMessage(2, 8, "をえらんでみましょう");
	SetJMessage(3, 1, "メニュぁいどう");
	SetAMessage(3, 5, "QE key");
	SetJMessage(4, 1, "メニュぁけってい");
	SetAMessage(4, 5, "L key");
	SetList();

	//メッセージ4
	SetMessageWindow();
	SetJMessage(1, 1, "みどりのタイルのうえでスペぁスキぁをおすと");
	SetJMessage(2, 1, "みちをつくることができます");
	SetJMessage(3, 1, "このとき　みぎうえのすうじがげんしょうします");
	SetJMessage(4, 1, "このすうじを　プレイヤぁパワぁとよびます");

	SetList();

	//メッセージ5
	SetMessageWindow();
	SetJMessage(1, 1, "これらのアクションをつかって");
	SetJMessage(2, 1, "ひとをみちびき　おくちにいるボスをたおしましょう");

	SetList();




}


void SystemMessage::TestMessage() {
	
	SetMessageWindow();
	SetJMessage(1, 1, "アイウエオカキクケコサシスセソタチツテト");
	SetJMessage(2, 1, "ナニヌネノハヒフヘホマミムメモヤユヨラリルレロ");
	SetJMessage(3, 1, "ワヲンぁァィゥェォャュョ");
	SetList();

}

void SystemMessage::Uninit() {

	for (UIWindow* pUI: pList) {
		pUI->Uninit();
		delete pUI;
	}
	pList.clear();

}
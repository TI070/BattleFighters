
#include "gameMenu.h"
#include "key_manager.h"
#include "scene.h"
#include "imageManager.h"
#include "polygon2d.h"
#include "filecontroll.h"
#include "audio.h"
#include "systemMessage.h"
#include "UIwindow.h"


#define GAMEMENU_STARTPOS_X (100.0f)
#define GAMEMENU_SIZE_X (120.0f)
#define GAMEMENU_SIZE_Y (50.0f)
#define GAMEMENU_SIZE {GAMEMENU_SIZE_X,GAMEMENU_SIZE_Y}

#define GAMEMENUICON_SIZE_X (48)
#define GAMEMENUICON_SIZE_Y (GAMEMENUICON_SIZE_X)
#define GAMEMENUICON_SIZE {GAMEMENUICON_SIZE_X,GAMEMENUICON_SIZE_X}

#define GM_SELECT_COLOR 1,1,1,1
#define GM_NOTSELECT_COLOR 64.0/255,64.0f/255,64.0f/255,255/255.0f


int GameMenu::cursor;

Polygon2D* GameMenu::gameMenuTex[GAMEMENU_END];
Polygon2D* GameMenu::gameMenuIconTex[GAMEMENUICON_END];

Polygon2D* GameMenu::QIcon;
Polygon2D* GameMenu::EIcon;

Audio* GameMenu::cursorMoveSE;
Audio* GameMenu::cursorDecideSE;

Player* GameMenu::pP;
UNIT_TYPE_DETAIL GameMenu::changeJob;
Polygon2D* GameMenu::activeF_p2;
Polygon2D* GameMenu::activeIconF_p2;
int GameMenu::gmIcon;
GAMEMENU_KIND GameMenu::iconMode;

UIWindow* GameMenu::menuExpWindow = nullptr;

UIWindow* GameMenu::menuCostString = nullptr;

/*
void GameMenu::Init() {

	//ゲームメニューUIを生成する
	gameMenuTex[GAMEMENU_UNITSELECT] = ImageManager::SetImageD({ GAMEMENU_STARTPOS_X,GAMEMENU_SIZE_Y / 2 }, GAMEMENU_SIZE, ImageManager::GetTexId(TEX_GAMEMENU_UNITSELECT), 0);
	gameMenuTex[GAMEMENU_UNITMOVE] = ImageManager::SetImageD({ GAMEMENU_STARTPOS_X + GAMEMENU_SIZE_X * 1,GAMEMENU_SIZE_Y/2 }, GAMEMENU_SIZE, ImageManager::GetTexId(TEX_GAMEMENU_UNITMOVE), 0);
	gameMenuTex[GAMEMENU_MAKEROAD] = ImageManager::SetImageD({ GAMEMENU_STARTPOS_X + GAMEMENU_SIZE_X * 2,GAMEMENU_SIZE_Y / 2 }, GAMEMENU_SIZE, ImageManager::GetTexId(TEX_GAMEMENU_MAKEROAD), 0);
	gameMenuTex[GAMEMENU_UNITCLASSCHANGE] = ImageManager::SetImageD({ GAMEMENU_STARTPOS_X + GAMEMENU_SIZE_X * 3,GAMEMENU_SIZE_Y / 2 }, GAMEMENU_SIZE, ImageManager::GetTexId(TEX_GAMEMENU_CLASSCHANGE), 0);
	gameMenuTex[GAMEMENU_DATASAVE] = ImageManager::SetImageD({ GAMEMENU_STARTPOS_X + GAMEMENU_SIZE_X * 4,GAMEMENU_SIZE_Y/2 }, GAMEMENU_SIZE, ImageManager::GetTexId(TEX_GAMEMENU_DATASAVE), 0);
	gameMenuTex[GAMEMENU_TOTITLE] = ImageManager::SetImageD({ GAMEMENU_STARTPOS_X + GAMEMENU_SIZE_X * 5,GAMEMENU_SIZE_Y / 2 }, GAMEMENU_SIZE, ImageManager::GetTexId(TEX_GAMEMENU_TOTITLE), 0);

	//ゲームメニューアイコンを生成する
	gameMenuIconTex[GAMEMENUICON_ROAD] = ImageManager::SetImageD({ GAMEMENU_STARTPOS_X + GAMEMENU_SIZE_X * 2 - GAMEMENU_SIZE_X / 2 + GAMEMENUICON_SIZE_X/2,GAMEMENUICON_SIZE_Y / 2 + GAMEMENU_SIZE_Y }, GAMEMENUICON_SIZE, ImageManager::GetTexId(TEX_ICON_ROAD), 0);
	gameMenuIconTex[GAMEMENUICON_STATUE] = ImageManager::SetImageD({ GAMEMENU_STARTPOS_X + GAMEMENU_SIZE_X * 2 + GAMEMENUICON_SIZE_X - GAMEMENU_SIZE_X / 2 + GAMEMENUICON_SIZE_X / 2,GAMEMENUICON_SIZE_Y / 2 + GAMEMENU_SIZE_Y }, GAMEMENUICON_SIZE, ImageManager::GetTexId(TEX_ICON_STATUE), 0);
	gameMenuIconTex[GAMEMENUICON_BREAK] = ImageManager::SetImageD({ GAMEMENU_STARTPOS_X + GAMEMENU_SIZE_X * 2 + GAMEMENUICON_SIZE_X * 2 - GAMEMENU_SIZE_X / 2 + GAMEMENUICON_SIZE_X / 2,GAMEMENUICON_SIZE_Y / 2 + GAMEMENU_SIZE_Y }, GAMEMENUICON_SIZE, ImageManager::GetTexId(TEX_ICON_BREAK), 0);
	gameMenuIconTex[GAMEMENUICON_SWORD] = ImageManager::SetImageD({ GAMEMENU_STARTPOS_X + GAMEMENU_SIZE_X * 3 - GAMEMENU_SIZE_X / 2 + GAMEMENUICON_SIZE_X / 2,GAMEMENUICON_SIZE_Y / 2 + GAMEMENU_SIZE_Y }, GAMEMENUICON_SIZE, ImageManager::GetTexId(TEX_ICON_SWORD), 0);
	gameMenuIconTex[GAMEMENUICON_WIZARD] = ImageManager::SetImageD({ GAMEMENU_STARTPOS_X + GAMEMENU_SIZE_X * 3 - GAMEMENU_SIZE_X / 2 + GAMEMENUICON_SIZE_X / 2 + GAMEMENUICON_SIZE_X,GAMEMENUICON_SIZE_Y / 2 + GAMEMENU_SIZE_Y }, GAMEMENUICON_SIZE, ImageManager::GetTexId(TEX_ICON_WIZARD), 0);
	gameMenuIconTex[GAMEMENUICON_PRIEST] = ImageManager::SetImageD({ GAMEMENU_STARTPOS_X + GAMEMENU_SIZE_X * 3 - GAMEMENU_SIZE_X / 2 + GAMEMENUICON_SIZE_X / 2 + GAMEMENUICON_SIZE_X * 2,GAMEMENUICON_SIZE_Y / 2 + GAMEMENU_SIZE_Y }, GAMEMENUICON_SIZE, ImageManager::GetTexId(TEX_ICON_PRIEST), 0);

	//ガイドアイコンの生成
	QIcon = ImageManager::SetImage({ GAMEMENU_STARTPOS_X - GAMEMENU_SIZE_X * 0.5 - 25,GAMEMENU_SIZE_Y / 2 }, { 50,50}, ImageManager::GetTexId(TEX_ICON_Q), 0);
	EIcon = ImageManager::SetImage({ GAMEMENU_STARTPOS_X + GAMEMENU_SIZE_X * 5.5 + 25,GAMEMENU_SIZE_Y / 2 }, { 50,50 }, ImageManager::GetTexId(TEX_ICON_E), 0);

	//メニュー説明ウインドウの生成
	menuExpWindow = ImageManager::SetUIWindow({ MESSAGEWINDOW_POS_X,SCREEN_HEIGHT - MESSAGEWINDOW_SIZE_Y / 2 }, MESSAGEWINDOW_SIZE, ImageManager::GetTexId(TEX_UI_MESSAGEWINDOWBASE), DEPTH_ONE);
	menuExpWindow->SetWindowTex(new Polygon2D ({ MESSAGEWINDOW_POS_X,SCREEN_HEIGHT - MESSAGEWINDOW_SIZE_Y / 2 }, MESSAGEWINDOW_SIZE, ImageManager::GetTexId(TEX_UI_MESSAGEWINDOWBASE), DEPTH_ONE));

	MenuExpSet();

	//コスト表示
	menuCostString = ImageManager::SetUIWindow({ MESSAGEWINDOW_POS_X,SCREEN_HEIGHT - MESSAGEWINDOW_SIZE_Y / 2 }, MESSAGEWINDOW_SIZE, ImageManager::GetTexId(TEX_UI_MESSAGEWINDOWBASE), DEPTH_ONE);
	char buf[64];
	sprintf(buf, "%d", 1);

	TexStringAlphabet* menuCostS = new TexStringAlphabet({ PLAYERPOWER_UI_POS_X - 10,PLAYERPOWER_UI_POS_Y - 30 }, 32, ImageManager::GetTexId(TEX_ALPHABET_FONT), DEPTH_ZERO, buf, { 0.0f,0.0f,0.0f,1.0f });
	menuCostS->SetColor({ 1,0,0,1 });
	menuCostString->SetATextTex(menuCostS);
	menuCostString->SetIsUse(false);

	for (int i = 0; i < GAMEMENUICON_END; i++) {
		gameMenuIconTex[i]->SetIsUse(false);
	}


	//activeF_p2 = ImageManager::SetImageD({ GAMEMENU_STARTPOS_X + GAMEMENU_SIZE_X * 0,GAMEMENU_SIZE_Y / 2 }, GAMEMENU_SIZE, ImageManager::GetTexId(TEX_GAMEMENU_ACTIVEF), 0);
	activeIconF_p2 = ImageManager::SetImageD({ GAMEMENU_STARTPOS_X + GAMEMENU_SIZE_X * 0,GAMEMENU_SIZE_Y / 2 }, GAMEMENU_SIZE, ImageManager::GetTexId(TEX_GAMEMENU_ACTIVEF), 0);

	cursor = 0;

	pP = UnitManager::GetPlayer();

	changeJob = UTD_PRIEST;

	gmIcon = GAMEMENUICON_END;
	iconMode = GAMEMENU_END;

	ColorUpdate();
	ActiveFrameUpdate();

	cursorMoveSE = new Audio;
	cursorMoveSE->Load("asset/sound/se/cursormove.wav");

	cursorDecideSE = new Audio;
	cursorDecideSE->Load("asset/sound/se/kettei.wav");


}


void GameMenu::Draw() {




}

void GameMenu::Uninit() {


	cursorMoveSE->Uninit();
	cursorDecideSE->Uninit();



}


void GameMenu::Update() {

	//ボタンは後で調整
	if (KeyManager_Trigger(KM_L)) {
		
		cursorDecideSE->Play();

		SelectMenu();


		ActiveFrameUpdate();
		IconUpdate();
	}

	if (KeyManager_Trigger(KM_Q)) {

		cursorMoveSE->Play();

		if (iconMode == GAMEMENU_END) {
			CursorMove(-1);
		}
		else {

			IconCursorMove(-1);
		}
	}
	else if (KeyManager_Trigger(KM_E)) {

		cursorMoveSE->Play();

		if (iconMode == GAMEMENU_END) {
			CursorMove(1);
		}
		else {

			IconCursorMove(1);
		}
	}



}

void GameMenu::CursorMove(int num) {
	

	cursor += num;
	
	if (cursor < 0) cursor = GAMEMENU_END - 1;
	else cursor %= GAMEMENU_END;

	if (cursor == GAMEMENU_UNITSELECT) RealUnitSelect();
	else if (cursor == GAMEMENU_UNITMOVE) UnitSelect();
	else pP->SetUSM(P_NOTSELECTMENU);



	ColorUpdate();

	MenuExpSet();
}


void GameMenu::SelectMenu() {

	menuCostString->SetIsUse(false);

	switch ((GAMEMENU_KIND)cursor)
	{
	case GAMEMENU_UNITSELECT:
		//RealUnitSelect();
		break;
	case GAMEMENU_UNITMOVE:
		//UnitSelect();
		break;
	case GAMEMENU_UNITCLASSCHANGE:

		if (iconMode == GAMEMENU_UNITCLASSCHANGE) {
			//UnitClassChange();
			iconMode = GAMEMENU_END;
			gmIcon = GAMEMENUICON_END;
			//IconCursorMove(0);
		}
		else {
			iconMode = GAMEMENU_UNITCLASSCHANGE;
			gmIcon = GAMEMENUICON_SWORD;
			IconCursorMove(0);
		}
		UnitClassChange();
		break;
	case GAMEMENU_DATASAVE:
		DataSave();
		break;
	case GAMEMENU_MAKEROAD:

		if (iconMode == GAMEMENU_MAKEROAD) {
			//MakeRoad();
			iconMode = GAMEMENU_END;
			gmIcon = GAMEMENUICON_END;
		}
		else {
			iconMode = GAMEMENU_MAKEROAD;
			gmIcon = GAMEMENUICON_ROAD;
			IconCursorMove(0);
		}
		MakeRoad();
		break;
	case GAMEMENU_TOTITLE:
		ToTitle();
		break;
	default:
		break;
	}

}

void GameMenu::RealUnitSelect() {

	if (pP->GetUSM() != P_UNITSELECT) {
		pP->SetUSM(P_UNITSELECT);
	}
	else {
		pP->SetUSM(P_NOTSELECTMENU);
	}
}

void GameMenu::UnitSelect() {

	if (pP->GetUSM() != P_UNITMOVE) {
		pP->SetUSM(P_UNITMOVE);
	}
	else {
		pP->SetUSM(P_NOTSELECTMENU);
	}

}

void GameMenu::UnitClassChange() {

	if (pP->GetUSM() != P_CLASSCHANGE) {
		pP->SetUSM(P_CLASSCHANGE);
	}
	else {
		pP->SetUSM(P_NOTSELECTMENU);
	}

}

void GameMenu::MakeRoad() {

	if (pP->GetUSM() != P_MAKEROAD) {
		pP->SetUSM(P_MAKEROAD);
	}
	else {
		pP->SetUSM(P_NOTSELECTMENU);
	}

}

void GameMenu::DataSave() {

	FileControll::GameDataSave();
}

void GameMenu::DataLoad() {

	Scene::SetCurrentScene(SCENE_GAME);

	FileControll::GameDataLoad();

}

void GameMenu::ToTitle() {

	Scene::SetCurrentScene(SCENE_TITLE);
}

void GameMenu::ColorUpdate() {

	for (int i = 0; i < GAMEMENU_END;i++) {

		if (i == cursor) gameMenuTex[i]->SetColor({ GM_SELECT_COLOR });
		else gameMenuTex[i]->SetColor({ GM_NOTSELECT_COLOR });

		gameMenuTex[i]->UpdateVertex({ GAMEMENU_STARTPOS_X + i * GAMEMENU_SIZE_X,GAMEMENU_SIZE_Y / 2 },GAMEMENU_SIZE);
	}



}

void GameMenu::ActiveFrameUpdate() {

	//アクティブフレーム描画
	PLAYER_UNITSELECTMENU s_usm = pP->GetUSM();

	//メニュー部分
	
	if (s_usm == P_UNITMOVE) {
		activeF_p2->SetColor({ 1,1,1,1 });
		activeF_p2->UpdateVertex({ GAMEMENU_STARTPOS_X + 1 * GAMEMENU_SIZE_X,GAMEMENU_SIZE_Y / 2 }, GAMEMENU_SIZE);
	}
	else if (s_usm == P_CLASSCHANGE) {
		activeF_p2->SetColor({ 1,1,1,1 });
		activeF_p2->UpdateVertex({ GAMEMENU_STARTPOS_X + 3 * GAMEMENU_SIZE_X,GAMEMENU_SIZE_Y / 2 }, GAMEMENU_SIZE);
	}
	else if (s_usm == P_MAKEROAD) {
		activeF_p2->SetColor({ 1,1,1,1 });
		activeF_p2->UpdateVertex({ GAMEMENU_STARTPOS_X + 2 * GAMEMENU_SIZE_X,GAMEMENU_SIZE_Y / 2 }, GAMEMENU_SIZE);
	}
	else {
		activeF_p2->SetColor({ 1,1,1,0 });
		activeF_p2->UpdateVertex({ GAMEMENU_STARTPOS_X + 0 * GAMEMENU_SIZE_X,GAMEMENU_SIZE_Y / 2 }, GAMEMENU_SIZE);
	}
	

	//アイコン部分
	if (gmIcon == GAMEMENUICON_ROAD) {

		activeIconF_p2->SetColor({ 1,1,1,1 });
		activeIconF_p2->UpdateVertex({ GAMEMENU_STARTPOS_X + GAMEMENU_SIZE_X * 2 - GAMEMENU_SIZE_X / 2 + GAMEMENUICON_SIZE_X / 2, GAMEMENUICON_SIZE_Y / 2 + GAMEMENU_SIZE_Y }, GAMEMENUICON_SIZE);

	}
	else if (gmIcon == GAMEMENUICON_STATUE) {

		activeIconF_p2->SetColor({ 1,1,1,1 });
		activeIconF_p2->UpdateVertex({ GAMEMENU_STARTPOS_X + GAMEMENU_SIZE_X * 2 + GAMEMENUICON_SIZE_X - GAMEMENU_SIZE_X / 2 + GAMEMENUICON_SIZE_X / 2,GAMEMENUICON_SIZE_Y / 2 + GAMEMENU_SIZE_Y }, GAMEMENUICON_SIZE);

	}
	else if (gmIcon == GAMEMENUICON_BREAK) {

		activeIconF_p2->SetColor({ 1,1,1,1 });
		activeIconF_p2->UpdateVertex({ GAMEMENU_STARTPOS_X + GAMEMENU_SIZE_X * 2 + GAMEMENUICON_SIZE_X * 2 - GAMEMENU_SIZE_X / 2 + GAMEMENUICON_SIZE_X / 2,GAMEMENUICON_SIZE_Y / 2 + GAMEMENU_SIZE_Y }, GAMEMENUICON_SIZE);

	}
	else if (gmIcon == GAMEMENUICON_SWORD) {

		activeIconF_p2->SetColor({ 1,1,1,1 });
		activeIconF_p2->UpdateVertex({ GAMEMENU_STARTPOS_X + GAMEMENU_SIZE_X * 3 - GAMEMENU_SIZE_X / 2 + GAMEMENUICON_SIZE_X / 2,GAMEMENUICON_SIZE_Y / 2 + GAMEMENU_SIZE_Y }, GAMEMENUICON_SIZE);

	}
	else if (gmIcon == GAMEMENUICON_WIZARD) {

		activeIconF_p2->SetColor({ 1,1,1,1 });
		activeIconF_p2->UpdateVertex({ GAMEMENU_STARTPOS_X + GAMEMENU_SIZE_X * 3 - GAMEMENU_SIZE_X / 2 + GAMEMENUICON_SIZE_X / 2 + GAMEMENUICON_SIZE_X,GAMEMENUICON_SIZE_Y / 2 + GAMEMENU_SIZE_Y }, GAMEMENUICON_SIZE);

	}
	else if (gmIcon == GAMEMENUICON_PRIEST) {

		activeIconF_p2->SetColor({ 1,1,1,1 });
		activeIconF_p2->UpdateVertex({ GAMEMENU_STARTPOS_X + GAMEMENU_SIZE_X * 3 - GAMEMENU_SIZE_X / 2 + GAMEMENUICON_SIZE_X / 2 + GAMEMENUICON_SIZE_X * 2,GAMEMENUICON_SIZE_Y / 2 + GAMEMENU_SIZE_Y }, GAMEMENUICON_SIZE);

	}
	else {
		activeIconF_p2->SetColor({ 1,1,1,0 });
		activeIconF_p2->UpdateVertex({ GAMEMENU_STARTPOS_X + GAMEMENU_SIZE_X * 1 - GAMEMENU_SIZE_X / 2 + GAMEMENUICON_SIZE_X / 2, GAMEMENUICON_SIZE_Y / 2 + GAMEMENU_SIZE_Y }, GAMEMENUICON_SIZE);

	}

	

}


void GameMenu::IconUpdate() {

	//一旦全部非表示
	for (int i = 0; i < GAMEMENUICON_END; i++) {
		gameMenuIconTex[i]->SetIsUse(false);
	}


	switch ((GAMEMENU_KIND)iconMode)
	{

		case GAMEMENU_UNITMOVE:
			break;
		case GAMEMENU_UNITCLASSCHANGE:

			//クラスチェンジアイコンを表示
			gameMenuIconTex[GAMEMENUICON_SWORD]->SetIsUse(true);
			gameMenuIconTex[GAMEMENUICON_WIZARD]->SetIsUse(true);
			gameMenuIconTex[GAMEMENUICON_PRIEST]->SetIsUse(true);

			break;
		case GAMEMENU_DATASAVE:
			break;
		case GAMEMENU_MAKEROAD:
			gameMenuIconTex[GAMEMENUICON_ROAD]->SetIsUse(true);
			gameMenuIconTex[GAMEMENUICON_STATUE]->SetIsUse(true);
			gameMenuIconTex[GAMEMENUICON_BREAK]->SetIsUse(true);

			break;
		case GAMEMENU_TOTITLE:
			break;
		default:
			break;

	}


}

void GameMenu::IconCursorMove(int num) {

	menuCostString->SetIsUse(false);

	gmIcon += num;

	if (iconMode == GAMEMENU_MAKEROAD) {

		if (gmIcon > GAMEMENUICON_BREAK) gmIcon = GAMEMENUICON_ROAD;
		else if (gmIcon < GAMEMENUICON_ROAD) gmIcon = GAMEMENUICON_BREAK;


		//コスト表示
		if(gmIcon == GAMEMENUICON_ROAD) SetMenuCostString(PPCOST_MAKEROAD);
		else if (gmIcon == GAMEMENUICON_STATUE) SetMenuCostString(PPCOST_STATUE);
	
	}
	else if(iconMode == GAMEMENU_UNITCLASSCHANGE){

		if (gmIcon > GAMEMENUICON_PRIEST) gmIcon = GAMEMENUICON_SWORD;
		else if (gmIcon < GAMEMENUICON_SWORD) gmIcon = GAMEMENUICON_PRIEST;

		//職業の決定
		if(gmIcon == GAMEMENUICON_SWORD) changeJob = UTD_SOLDIER;
		else if (gmIcon == GAMEMENUICON_WIZARD) changeJob = UTD_WIZARD;
		else if (gmIcon == GAMEMENUICON_PRIEST) changeJob = UTD_PRIEST;

		SetMenuCostString(PPCOST_CLASSCHANGE);

	}

	ActiveFrameUpdate();
}


void GameMenu::MenuExpSet() {

	//ウインドウ破棄して再生成
	if (menuExpWindow != nullptr) menuExpWindow->Uninit();
	menuExpWindow = ImageManager::SetUIWindow({ MESSAGEWINDOW_POS_X,SCREEN_HEIGHT - MESSAGEWINDOW_SIZE_Y / 2 }, MESSAGEWINDOW_SIZE, ImageManager::GetTexId(TEX_UI_MESSAGEWINDOWBASE), DEPTH_ONE);
	menuExpWindow->SetWindowTex(new Polygon2D({ MESSAGEWINDOW_POS_X,SCREEN_HEIGHT - MESSAGEWINDOW_SIZE_Y / 2 }, MESSAGEWINDOW_SIZE, ImageManager::GetTexId(TEX_UI_MESSAGEWINDOWBASE), DEPTH_ONE));


	switch ((GAMEMENU_KIND)cursor)
	{
	case GAMEMENU_UNITSELECT:
		SetJMessage(1, 1, "カぁソルのいちのユニットをせんたくじょうたいにします");
		SetJMessage(2, 1, "せんたくじょうたいのユニットはステぁタスがひょうじされます");
		SetJMessage(3, 1, "てきのステぁタスもみることができます");

		break;
	case GAMEMENU_UNITMOVE:
		SetJMessage(1, 1, "せんたくじょうたいのユニットをカぁソルいちにいどうさせます");
		SetJMessage(2, 1, "みちじゃないばしょやとおすぎるばしょにはいどうできません");

		break;
	case GAMEMENU_UNITCLASSCHANGE:
		SetJMessage(1, 1, "プレイヤぁパワぁをつかって");
		SetJMessage(2, 1, "せんたくじょうたいのユニットをクラスチェンジします");

		break;
	case GAMEMENU_DATASAVE:
		SetJMessage(1, 1, "デぁタをほぞんします");

		break;
	case GAMEMENU_MAKEROAD:
		SetJMessage(1, 1, "プレイヤぁパワぁをつかって");
		SetJMessage(2, 1, "みちをひいたりこわしたり");
		SetJMessage(3, 1, "せきぞうをたてることができます");
		SetJMessage(4, 1, "せきぞうのまわりにはたてものがたちます");

		break;
	case GAMEMENU_TOTITLE:
		SetJMessage(1, 1, "タイトルにもどります");
		SetJMessage(2, 1, "デぁタはほぞんされません");
		break;
	default:

		break;
	}


}


void GameMenu::SetAMessage(int row, int col, const char* mes) {

	if (menuExpWindow == nullptr) return;

	menuExpWindow->SetATextTex(new TexStringAlphabet({ 220.0f + 32 * (col - 1),410.0f + 32 * (row - 1) }, 32, ImageManager::GetTexId(TEX_JAPANESE_FONT), DEPTH_ZERO, mes, { 0.0f,0.0f,0.0f,1.0f }));


}

void GameMenu::SetJMessage(int row, int col, const char* mes) {

	if (menuExpWindow == nullptr) return;

	menuExpWindow->SetJTextTex(new TexStringJapanese({ 220.0f + 32 * (col - 1),410.0f + 32 * (row - 1) }, 32, ImageManager::GetTexId(TEX_JAPANESE_FONT), DEPTH_ZERO, mes, { 0.0f,0.0f,0.0f,1.0f }));

}


void GameMenu::SetMenuCostString(int s_num) {

	if (menuCostString != nullptr) menuCostString->Uninit();


	menuCostString = ImageManager::SetUIWindow({ MESSAGEWINDOW_POS_X,SCREEN_HEIGHT - MESSAGEWINDOW_SIZE_Y / 2 }, MESSAGEWINDOW_SIZE, ImageManager::GetTexId(TEX_UI_MESSAGEWINDOWBASE), DEPTH_ONE);
	char buf[64];
	sprintf(buf, "%d", s_num);

	TexStringAlphabet* menuCostS = new TexStringAlphabet({ PLAYERPOWER_UI_POS_X - 10,PLAYERPOWER_UI_POS_Y - 30 }, 32, ImageManager::GetTexId(TEX_ALPHABET_FONT), DEPTH_ZERO, buf, { 0.0f,0.0f,0.0f,1.0f });
	menuCostS->SetColor({ 1,0,0,1 });
	menuCostString->SetATextTex(menuCostS);
	menuCostString->SetIsUse(true);

}
*/
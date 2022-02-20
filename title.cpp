
#include "title.h"
#include "texture.h"
#include "config.h"
#include "writestring.h"
#include "key_manager.h"
#include "scene.h"
#include "writestring.h"
#include "imageManager.h"
#include "audio.h"
#include "UIwindow.h"
#include "manager.h"
#include "gameSystem.h"
#include "effectManager.h"
#include "particle.h"
#include "rendererManager.h"
#include "imgui.h"
#include "audioManager.h"
#include "controller.h"

#include <stdio.h>

#define TITLELOGO_WIDTH (550)
#define TITLELOGO_HEIGHT (170)

#define TITLEMENU_SIZE_X (300)
#define TITLEMENU_SIZE_Y (200)

#define TITLEMENU_STARTPOS_X (SCREEN_WIDTH/ 2)
#define TITLEMENU_STARTPOS_Y (400)

#define BOUNDPOLYGON_BASEWIDTH  (100)
#define BOUNDPOLYGON_BASEHEIGHT (100)


//#define TM_SELECT_COLOR 0,255,128,255
//#define TM_NOTSELECT_COLOR 128,128,128,255

#define TM_SELECT_COLOR 0,255/255,0,255/255
#define TM_NOTSELECT_COLOR 64.0/255,64.0f/255,64.0f/255,255/255.0f

int Title::TitleTexId;

Polygon2D* Title::titleLogo = nullptr;
Audio* Title::TitleBgm = nullptr;
Audio* Title::cursorMoveSE = nullptr;
Audio* Title::cursorDecideSE = nullptr;

UIWindow* Title::pTitleWindow = nullptr;
int Title::cursor;

TexString* Title::gameStartText = nullptr;
UIWindow* Title::gameStartWindow = nullptr;

TITLE_MODE Title::currentMode = TM_TITLE;

UIWindow* Title::gameMenuWindow_Once = nullptr;
UIWindow* Title::gameMenuWindow_Battle = nullptr;

UIWindow* Title::gameMenuSelectWindow = nullptr;

UIWindow* Title::charaSelectWindows[CHARA_KINDNUM];

Polygon2D* Title::onceMenuText;
Polygon2D* Title::battleMenuText;

Polygon2D* Title::selectMaker[4];


UIWindow* Title::playerSelectWindows[4];
Area Title::charaSelectArea[CHARA_KINDNUM];
Polygon2D* Title::playerSelectImages[4];
UIWindow* Title::pUIMessageWindow;
TexString* Title::pUIMessageText;

SelectHand Title::P1Hand;
SelectHand Title::P2Hand;

Polygon2D* Title::playerModeImages[4];
Polygon2D* Title::titleBg;

bool Title::titleEffectOn = true;

int Title::particleGenerateCnt = 60;

UIWindow* Title::stockControllWindow;
TexString* Title::stockControllText;

Polygon2D* Title::leftSankaku;
Polygon2D* Title::rightSankaku;

D3DXVECTOR4 Title::tlShaderParameter = { 0,0,0,0 };

BoundPolygon2D Title::boundPolygon[4];

Polygon2D* Title::titleMenuBg;

Polygon2D* Title::titleMenuParts[9];

Polygon2D* Title::charaSelectBg;

bool Title::pUIMessageSw;
int Title::pUIMessageTimer;

Polygon2D* Title::playerSelectWindowTex[4];

Polygon2D* Title::stockTex;

int Title::acceptCooltime = 0;

UIWindow* Title::gameMenuWindow_KeyConfig = nullptr;

Polygon2D* Title::keyconfigMenuText = nullptr;

int Title::cursorKeyConfig = 0;

UIWindow* Title::keyConfigWindow[4];
Polygon2D* Title::keyConfigWindowTex[4];
UIWindow* Title::keyConfigLogoWindow;
Polygon2D* Title::keyConfigWindowTexString[4];
bool Title::keyconSelect = false;
TexString* Title::keyconWindowString[4];
TexString* Title::keyconPlayerNumString;

int Title::keyconfigControllerNum = 0;

void Title::Initialize() {

	//背景カラー設定
	Renderer::SetClearColor({255,239,192,255});


	char buf[64];

	BoundPolygonInit();


	////タイトル用意
	//背景
	titleBg = ImageManager::SetImage({ SCREEN_WIDTH / 2 + 0.600,SCREEN_HEIGHT / 2 }, { SCREEN_WIDTH,SCREEN_HEIGHT }, ImageManager::GetTexId(TEX_UI_TITLEBG), 0);

	titleMenuBg = ImageManager::SetImage({ SCREEN_WIDTH / 2 + 0.600,SCREEN_HEIGHT / 2 }, { SCREEN_WIDTH,SCREEN_HEIGHT }, ImageManager::GetTexId(TEX_UI_TITLEMENUBG), DEPTH_THREE);
	titleMenuBg->SetIsUse(false);

	//ロゴ
	TitleTexId = Texture_SetTextureLoadFile("asset/title.png");
	titleLogo = ImageManager::SetImageD({ SCREEN_WIDTH / 2,125 }, { TITLELOGO_WIDTH,TITLELOGO_HEIGHT }, ImageManager::GetTexId(TEX_LOGO_TITLE), 0);
	titleLogo->SetColor({0, 0, 0, 1});
	titleLogo->UpdateVertex();

	titleLogo->SetVertexShader(RendererManager::GetGdVS());
	titleLogo->SetPixelShader(RendererManager::GetGdPS());
	titleLogo->SetVertxeLayout(RendererManager::GetGdIL());


	//文字表示
	gameStartWindow = ImageManager::SetUIWindow({ SCREEN_WIDTH / 2.0f ,SCREEN_HEIGHT - 150 }, { 370,40 }, 0);
	gameStartWindow->SetWindowTex(TEX_UI_TITLESTART);
	//sprintf(buf, "Z GameStart");
	//gameStartText = gameStartWindow->SetTextTex(buf, { 50,50 }, { 64,64 }, { 0,0,0,1 });

	titleLogo->SetIsUse(true);
	gameStartWindow->SetIsUse(true);



	////メニュー用意
	//文字表示
	gameMenuWindow_Once = ImageManager::SetUIWindow({ SCREEN_WIDTH / 2.0f ,SCREEN_HEIGHT/2 - 150 }, { 500,100 }, 0);
	onceMenuText = gameMenuWindow_Once->SetWindowTex(TEX_UI_MENUWINDOW,true);
	gameMenuWindow_Once->SetPolygon2D({ 230,50 }, { 430,40 }, ImageManager::GetTexId(TEX_UI_STRONEPLAYER),true);
	//sprintf(buf, "1 PLAYER");
	//onceMenuText = gameMenuWindow_Once->SetTextTex(buf, { 50,30 }, { 64,64 }, { 0,0,0,1 });
	//onceMenuText->SetColor({0,0,0,1});

	gameMenuWindow_Battle = ImageManager::SetUIWindow({ SCREEN_WIDTH / 2.0f ,SCREEN_HEIGHT/2 + 000 }, { 500,100 }, true);
	battleMenuText = gameMenuWindow_Battle->SetWindowTex(TEX_UI_MENUWINDOW, true);
	gameMenuWindow_Battle->SetPolygon2D({ 230,50 }, { 430,40 }, ImageManager::GetTexId(TEX_UI_STRBATTLEROYALE));

	gameMenuWindow_KeyConfig = ImageManager::SetUIWindow({ SCREEN_WIDTH / 2.0f ,SCREEN_HEIGHT / 2 + 150 }, { 500,100 }, true);
	keyconfigMenuText = gameMenuWindow_KeyConfig->SetWindowTex(TEX_UI_MENUWINDOW, true);
	gameMenuWindow_KeyConfig->SetPolygon2D({ 250,50 }, { 430,40 }, ImageManager::GetTexId(TEX_UI_MENU_KEYCONFIG));


	//sprintf(buf, "BATTLE ROYALE");
	//battleMenuText = gameMenuWindow_Battle->SetTextTex(buf, { 50,30 }, { 64,64 }, { 0,0,0,1 });
	//battleMenuText->SetColor({ 0,0,0,1 });

	gameMenuWindow_Once->SetIsUse(false);
	gameMenuWindow_Battle->SetIsUse(false);
	gameMenuWindow_KeyConfig->SetIsUse(false);

	//メニューセレクト用
	gameMenuSelectWindow = ImageManager::SetUIWindow({ SCREEN_WIDTH / 2.0f ,SCREEN_HEIGHT / 2 - 100 }, { 500,100 }, 0);
	gameMenuSelectWindow->SetWindowTex(TEX_PAINTBLOCK);

	gameMenuSelectWindow->SetIsUse(false);

	////キャラセレ用意
	for (int i = 0; i < CHARASELECT_YNUM; i++) {
		for (int j = 0; j < CHARASELECT_XNUM; j++) {
			charaSelectWindows[i * CHARASELECT_XNUM + j] = ImageManager::SetUIWindow({ (SCREEN_WIDTH / 2.0f - CHARASELECT_HEIGHT * CHARASELECT_XNUM / 2.0f) + j * CHARASELECT_WIDTH ,(SCREEN_HEIGHT / 2.0f - CHARASELECT_HEIGHT * CHARASELECT_YNUM / 2.0f) + i * CHARASELECT_HEIGHT }, { CHARASELECT_WIDTH,CHARASELECT_HEIGHT }, 0);
			charaSelectWindows[i * CHARASELECT_XNUM + j]->SetWindowTex(TEX_PAINTBLOCK);
			charaSelectWindows[i * CHARASELECT_XNUM + j]->SetIsUse(false);

			charaSelectArea[i * CHARASELECT_XNUM + j].upMax = ((SCREEN_HEIGHT / 2.0f - CHARASELECT_HEIGHT * CHARASELECT_YNUM / 2.0f) + i * CHARASELECT_HEIGHT) - CHARASELECT_HEIGHT / 2;
			charaSelectArea[i * CHARASELECT_XNUM + j].downMax = ((SCREEN_HEIGHT / 2.0f - CHARASELECT_HEIGHT * CHARASELECT_YNUM / 2.0f) + i * CHARASELECT_HEIGHT) + CHARASELECT_HEIGHT / 2;
			charaSelectArea[i * CHARASELECT_XNUM + j].rightMax = ((SCREEN_WIDTH / 2.0f - CHARASELECT_HEIGHT * CHARASELECT_XNUM / 2.0f) + j * CHARASELECT_WIDTH) + CHARASELECT_WIDTH / 2;
			charaSelectArea[i * CHARASELECT_XNUM + j].leftMax = ((SCREEN_WIDTH / 2.0f - CHARASELECT_HEIGHT * CHARASELECT_XNUM / 2.0f) + j * CHARASELECT_WIDTH) - CHARASELECT_WIDTH / 2;

		}
	}
	/*
	charaSelectWindows[CK_TEST]->SetPolygon2D({ CHARASELECT_WIDTH / 2,CHARASELECT_HEIGHT / 2 }, { CHARASELECT_WIDTH,CHARASELECT_HEIGHT }, ImageManager::GetTexId(TEX_UI_SELECTIMAGE_ARCHER));
	Polygon2D* f = charaSelectWindows[CK_TEST]->SetPolygon2D({ CHARASELECT_WIDTH / 2,CHARASELECT_HEIGHT / 2 }, { CHARASELECT_WIDTH,CHARASELECT_HEIGHT }, ImageManager::GetTexId(TEX_WINDOW_FRAME));
	f->SetColor({ 0.2f,0.2f, 0.2f, 1.0f });


	charaSelectWindows[CK_PALADIN]->SetPolygon2D({ CHARASELECT_WIDTH / 2,CHARASELECT_HEIGHT / 2 }, { CHARASELECT_WIDTH,CHARASELECT_HEIGHT }, ImageManager::GetTexId(TEX_UI_SELECTIMAGE_PALADIN));
	f = charaSelectWindows[CK_PALADIN]->SetPolygon2D({ CHARASELECT_WIDTH / 2,CHARASELECT_HEIGHT / 2 }, { CHARASELECT_WIDTH,CHARASELECT_HEIGHT }, ImageManager::GetTexId(TEX_WINDOW_FRAME));
	f->SetColor({ 0.2f,0.2f, 0.2f, 1.0f });
	*/
	for (int i = 0; i < CHARA_KINDNUM;i++) {
		charaSelectWindows[i]->SetPolygon2D({ CHARASELECT_WIDTH / 2,CHARASELECT_HEIGHT / 2 }, { CHARASELECT_WIDTH,CHARASELECT_HEIGHT }, ImageManager::GetTexId((TEX_KIND)((int)TEX_UI_SELECTIMAGE_ARCHER + i)));
		Polygon2D* f = charaSelectWindows[i]->SetPolygon2D({ CHARASELECT_WIDTH / 2,CHARASELECT_HEIGHT / 2 }, { CHARASELECT_WIDTH,CHARASELECT_HEIGHT }, ImageManager::GetTexId(TEX_WINDOW_FRAME));
		f->SetColor({ 0.2f,0.2f, 0.2f, 1.0f });
	}

	for (int i = 0; i < 4; i++) {

		playerSelectWindows[i] = ImageManager::SetUIWindow({120.0f + (SCREEN_WIDTH / 4) * i,SCREEN_HEIGHT - 100.0f}, { 160,200 }, 0);
		playerSelectWindowTex[i] = playerSelectWindows[i]->SetWindowTex(TEX_UI_PLAYERSELECTWINDOW,true);
		playerSelectWindows[i]->SetIsUse(false);
		playerSelectImages[i] = playerSelectWindows[i]->SetPolygon2D({ 80,100 }, { CHARASELECT_WIDTH,CHARASELECT_HEIGHT }, NULL,true);
		//playerSelectImages[i]->SetIsUse(true);
		playerSelectImages[i]->SetIsUse(false);
		//参加状態によってによって変更
		if (!GameSystem::GetisJoin(i)) {
			//playerSelectImages[i]->SetColor({0.7f,0.7f, 0.7f, 1.0f});
			//playerSelectImages[i]->UpdateVertex();

		}

		if (i == 0) {
			if(!GameSystem::GetisJoin(0)) playerSelectWindowTex[i]->SetColor({ 184 / 255.0f,184 / 255.0f,184 / 255.0f,255 / 255.0f, });
			else playerSelectWindowTex[i]->SetColor({ 255 / 255.0f,0 / 255.0f,0 / 255.0f,255 / 255.0f, });
		}
		else if (i == 1) {
			if (GameSystem::GetisJoin(1)) playerSelectWindowTex[i]->SetColor({ 0 / 255.0f,0 / 255.0f,255 / 255.0f,255 / 255.0f, });
			else playerSelectWindowTex[i]->SetColor({ 184 / 255.0f,184 / 255.0f,184 / 255.0f,255 / 255.0f, });
		}
		else if (i == 2) {		
			if (GameSystem::GetisJoin(2)) playerSelectWindowTex[i]->SetColor({ 255 / 255.0f,255 / 255.0f,0 / 255.0f,255 / 255.0f, });
			else playerSelectWindowTex[i]->SetColor({ 184 / 255.0f,184 / 255.0f,184 / 255.0f,255 / 255.0f, });
		}
		else if (i == 3) {
			if (GameSystem::GetisJoin(3)) playerSelectWindowTex[i]->SetColor({ 0 / 255.0f,255 / 255.0f,0 / 255.0f,255 / 255.0f, });
			else playerSelectWindowTex[i]->SetColor({ 184 / 255.0f,184 / 255.0f,184 / 255.0f,255 / 255.0f, });
		}
		playerSelectWindowTex[i]->UpdateVertex();


		//プレイヤーモード表示
		playerModeImages[i] = playerSelectWindows[i]->SetPolygon2D({ CHARASELECT_WIDTH - 60,CHARASELECT_HEIGHT}, { PLAYERMODEPLATE_WIDTH,PLAYERMODEPLATE_HEIGHT }, ImageManager::GetTexId(TEX_UI_PLAYERPLATE));
		playerModeImages[i]->SetTexture(ImageManager::GetTexId((TEX_KIND)(TEX_UI_PLAYERPLATE + GameSystem::GetPlayerMode(i))));

	}

	//メッセージ表示用ウインドウ
	pUIMessageWindow = ImageManager::SetUIWindow({ SCREEN_WIDTH/2 ,200 }, { 624,60 }, 0);
	pUIMessageWindow->SetWindowTex(TEX_UI_BATTLESTART);
	//sprintf(buf, "C BattleStart");
	//pUIMessageText = pUIMessageWindow->SetTextTex(buf, { -100 ,50 }, { 96,96 }, { 0,0,0,1 });
	//pUIMessageText->SetColor({255/255.0f,170/255.0f,79/255.0f,1});
	pUIMessageWindow->SetIsUse(false);
	pUIMessageSw = false;

	//ストック操作
	stockControllWindow = ImageManager::SetUIWindow({ SCREEN_WIDTH - 200 ,50 }, { 300,100 }, 0);
	stockControllWindow->SetWindowTex(NULL);
	stockTex = stockControllWindow->SetPolygon2D({ 150,28 }, { 200,50 }, ImageManager::GetTexId(TEX_UI_STOCK));
	sprintf(buf, "%d",GameSystem::GetStockNum());
	stockControllText = stockControllWindow->SetTextTex(buf, { 50 + 32 * 6,12 }, { 64,64 }, { 0,0,0,1 });
	stockControllText->SetColor({255/255.0f,128 / 255.0f, 128 / 255.0f, 1});
	stockControllWindow->SetIsUse(false);
	leftSankaku = stockControllWindow->SetPolygon2D({10,30}, { 64,64 }, ImageManager::GetTexId(TEX_ICON_LEFTSANKAKU));
	rightSankaku = stockControllWindow->SetPolygon2D({ 300,30 }, { 64,64 }, ImageManager::GetTexId(TEX_ICON_RIGHTSANKAKU));

	currentMode = TM_TITLE;

	cursor = 0;

	//セレクトマーカー
	selectMaker[0] = ImageManager::SetImageD({ 100,100 }, { SELECTMAKER_WIDTH,SELECTMAKER_HEIGHT }, ImageManager::GetTexId(TEX_UI_SELECTMARKER1), 0);
	selectMaker[1] = ImageManager::SetImageD({ SCREEN_WIDTH - 100,100 }, { SELECTMAKER_WIDTH,SELECTMAKER_HEIGHT }, ImageManager::GetTexId(TEX_UI_SELECTMARKER2), 0);
	selectMaker[2] = ImageManager::SetImageD({ 100,300 }, { SELECTMAKER_WIDTH,SELECTMAKER_HEIGHT }, ImageManager::GetTexId(TEX_UI_SELECTMARKER3), 0);
	selectMaker[3] = ImageManager::SetImageD({ SCREEN_WIDTH - 100,300 }, { SELECTMAKER_WIDTH,SELECTMAKER_HEIGHT }, ImageManager::GetTexId(TEX_UI_SELECTMARKER4), 0);
	
	for (int i = 0; i < 4; i++)selectMaker[i]->SetIsUse(false);

	//セレクトハンド
	P1Hand.image = ImageManager::SetImageD({ 100,400 }, { SELECTHAND_WIDTH,SELECTHAND_HEIGHT }, ImageManager::GetTexId(TEX_ICON_SELECTHAND_OPEN), 0);
	P1Hand.catchMarkerNum = -1;
	P1Hand.image->SetIsUse(false);

	//2P 該当コントローラーを認識したら出てくるようにするのが理想
	P2Hand.image = ImageManager::SetImageD({ 300,400 }, { SELECTHAND_WIDTH,SELECTHAND_HEIGHT }, ImageManager::GetTexId(TEX_UI_SELECTHAND2_OPEN), 0);
	P2Hand.catchMarkerNum = -1;
	P2Hand.image->SetIsUse(false);



	//キャラセレ背景
	charaSelectBg = ImageManager::SetImageD({ SCREEN_WIDTH/2.0f,SCREEN_HEIGHT / 2.0f }, { SCREEN_WIDTH ,SCREEN_HEIGHT }, ImageManager::GetTexId(TEX_UI_CHARASELECTBG), DEPTH_ONE);
	charaSelectBg->SetColor({ 179/255.0f,174 / 255.0f,161 / 255.0f,1 });
	charaSelectBg->UpdateVertex();
	charaSelectBg->SetIsUse(false);

	TitleMenuBgInit();

	//キーコンフィグ用設定
	for (int i = 0; i < 4;i++) {

		keyConfigWindow[i] = ImageManager::SetUIWindow({ 200.0f , 150.0f + i * 100 }, { 80,80 }, 0);
		keyConfigWindowTexString[i] = keyConfigWindow[i]->SetPolygon2D({ 200,35 }, {200,50}, ImageManager::GetTexId((TEX_KIND)(TEX_UI_KEYCONFIG_STRING1 + i)), true);
		keyConfigWindowTex[i] = keyConfigWindow[i]->SetWindowTex((TEX_KIND)(TEX_UI_PLAYERSELECTWINDOW), true);
		keyConfigWindow[i]->SetIsUse(false);

		sprintf(buf, "0");
		keyconWindowString[i] = keyConfigWindow[i]->SetTextTex(buf, { 20,20 }, { 64,64 }, { 0,0,0,1 });
		keyconWindowString[i]->SetColor({ 0 / 255.0f,0 / 255.0f, 0 / 255.0f, 1 });


	}

	keyConfigLogoWindow = ImageManager::SetUIWindow({ SCREEN_WIDTH / 2.0f ,60 }, { 500,80 }, 0);
	keyConfigLogoWindow->SetWindowTex(TEX_UI_KEYCONFIG_LOGO);
	keyConfigLogoWindow->SetIsUse(false);
	
	sprintf(buf, "1P");
	keyconPlayerNumString = keyConfigLogoWindow->SetTextTex(buf, { -200,0 }, { 64,64 }, { 0,0,0,1 });



	//BGM
	AudioManager::AudioPlay(AK_BGM_TITLE,1);

}


void Title::Draw() {

	return;

	int testB = keyconSelect;

	ImGui::Begin("test");

	ImGui::InputInt("keyconSelect", &testB);

	ImGui::End();
	
}

void Title::Uninit() {


}

void Title::Update() {

	TitleEffectFunction();

	MenuSelect();

	BoundPolygonUpdate();

	TitleMenuBgUpdate();

	if(currentMode == TM_CHARASELECT) UIMessageUpdate();

	//デバッグ
	if (KeyManager_Trigger(KM_L)) {//飛ばしてゲームへ
		Scene::SetCurrentScene(SCENE_GAME);
	}

}

void Title::MenuSelect() {

	if (acceptCooltime > 0) acceptCooltime--;

	switch (currentMode)
	{
	case TM_TITLE:
		if (KeyManager_Trigger(KM_DECIDE) || ControllerManager::GetButtonState(0).aButton == BS_TRIGGER) {

			//決定音とか
			AudioManager::AudioPlay(AK_SE_GAMESTART,false);

			//モード移動
			TitleModeSet(TM_MENU);

		}
		break;
	case TM_MENU:

		//if (acceptCooltime > 0) acceptCooltime--;

		if (KeyManager_Trigger(KM_DECIDE) || ControllerManager::GetButtonState(0).aButton == BS_TRIGGER) {

			switch (cursor)
			{
			case TMM_ONCE:

				AudioManager::AudioPlay(AK_SE_KETTEI, false);

				
				TitleModeSet(TM_ONEPLAYER);

				break;

			case TMM_BATTLE:

				AudioManager::AudioPlay(AK_SE_KETTEI, false);

			
				TitleModeSet(TM_CHARASELECT);

				break;

			case TMM_KEYCONFIG:

				//コントローラーが接続されていなかったらはじく
				if (ControllerManager::GetCDevice(0) == nullptr) {

					AudioManager::AudioPlay(AK_SE_CANCEL, false);
					return;

				}

				AudioManager::AudioPlay(AK_SE_KETTEI, false);


				TitleModeSet(TM_KEYCONFIG);

				break;

			default:
				break;
			}

		}
		else if (KeyManager_Press(KM_W) || ControllerManager::GetButtonState(0).LstickY == -1000) {

			if (acceptCooltime > 0) break;

			//カーソル移動
			cursor--;
			AudioManager::AudioPlay(AK_SE_CURSORMOVE, false);

			if (cursor < 0) cursor = TMM_MAX - 1;

			//選択箇所の色を変える
			if (cursor == TMM_ONCE) {
				onceMenuText->SetColor({ 255 / 255.0f,255 / 255.0f,111 / 255.0f,255 / 255.0f });
				battleMenuText->SetColor({ 255 / 255.0f,255 / 255.0f,255 / 255.0f,255 / 255.0f });
				onceMenuText->UpdateVertex();
				battleMenuText->UpdateVertex();
				keyconfigMenuText->SetColor({ 255 / 255.0f,255 / 255.0f,255 / 255.0f,255 / 255.0f });
				keyconfigMenuText->UpdateVertex();

			}
			else if (cursor == TMM_BATTLE) {
				onceMenuText->SetColor({ 255 / 255.0f,255 / 255.0f,255 / 255.0f,255 / 255.0f });
				battleMenuText->SetColor({ 255 / 255.0f,255 / 255.0f,111 / 255.0f,255 / 255.0f });
				onceMenuText->UpdateVertex();
				battleMenuText->UpdateVertex();
				keyconfigMenuText->SetColor({ 255 / 255.0f,255 / 255.0f,255 / 255.0f,255 / 255.0f });
				keyconfigMenuText->UpdateVertex();

			}
			else if (cursor == TMM_KEYCONFIG) {
				onceMenuText->SetColor({ 255 / 255.0f,255 / 255.0f,255 / 255.0f,255 / 255.0f });
				battleMenuText->SetColor({ 255 / 255.0f,255 / 255.0f,255 / 255.0f,255 / 255.0f });
				onceMenuText->UpdateVertex();
				battleMenuText->UpdateVertex();
				keyconfigMenuText->SetColor({ 255 / 255.0f,255 / 255.0f,111 / 255.0f,255 / 255.0f });
				keyconfigMenuText->UpdateVertex();
			}

			acceptCooltime = 20;
		}
		else if (KeyManager_Press(KM_S) || ControllerManager::GetButtonState(0).LstickY == 1000) {

			if (acceptCooltime > 0) break;


			//カーソル移動
			cursor++;
			AudioManager::AudioPlay(AK_SE_CURSORMOVE, false);


			if (cursor >= TMM_MAX) cursor = 0;

			//選択箇所の色を変える
			if (cursor == TMM_ONCE) {
				onceMenuText->SetColor({ 255 / 255.0f,255 / 255.0f,111 / 255.0f,255 / 255.0f });
				battleMenuText->SetColor({ 255 / 255.0f,255 / 255.0f,255 / 255.0f,255 / 255.0f });
				keyconfigMenuText->SetColor({ 255 / 255.0f,255 / 255.0f,255 / 255.0f,255 / 255.0f });
				onceMenuText->UpdateVertex();
				battleMenuText->UpdateVertex();
				keyconfigMenuText->UpdateVertex();

			}
			else if (cursor == TMM_BATTLE) {
				onceMenuText->SetColor({ 255 / 255.0f,255 / 255.0f,255 / 255.0f,255 / 255.0f });
				battleMenuText->SetColor({ 255 / 255.0f,255 / 255.0f,111 / 255.0f,255 / 255.0f });
				onceMenuText->UpdateVertex();
				battleMenuText->UpdateVertex();
				keyconfigMenuText->SetColor({ 255 / 255.0f,255 / 255.0f,255 / 255.0f,255 / 255.0f });
				keyconfigMenuText->UpdateVertex();
			}
			else if (cursor == TMM_KEYCONFIG) {
				onceMenuText->SetColor({ 255 / 255.0f,255 / 255.0f,255 / 255.0f,255 / 255.0f });
				battleMenuText->SetColor({ 255 / 255.0f,255 / 255.0f,255 / 255.0f,255 / 255.0f });
				onceMenuText->UpdateVertex();
				battleMenuText->UpdateVertex();
				keyconfigMenuText->SetColor({ 255 / 255.0f,255 / 255.0f,111 / 255.0f,255 / 255.0f });
				keyconfigMenuText->UpdateVertex();
			}

			acceptCooltime = 20;
		}
		else if (KeyManager_Trigger(KM_CANCEL) || ControllerManager::GetButtonState(0).bButton == BS_TRIGGER) {//戻る

			AudioManager::AudioPlay(AK_SE_CANCEL, false);
			AudioManager::AudioStop(AK_BGM_MENU);

			TitleModeSet(TM_TITLE);

		}



		break;
	case TM_CHARASELECT:


		////キャラセレ
		//マーカー
		HandMove();


		//プレイヤーのキャラが選択されていたらバトルを始める
		if (KeyManager_Trigger(KM_DECIDE) || ControllerManager::GetButtonState(0).aButton == BS_TRIGGER ||
			KeyManager_Trigger(KM_SPACE) || ControllerManager::GetButtonState(1).aButton == BS_TRIGGER
			) {

			//エリア内に入っていたら該当キャラを選択
			for (int i = 0; i < CHARA_KINDNUM; i++) {
				for (int j = 0; j < 4; j++) {
					//プレイヤー１
					if (charaSelectArea[i].CheckInArea2D({ selectMaker[j]->GetPosition().x,selectMaker[j]->GetPosition().y })) {

						playerSelectImages[j]->SetTexture(ImageManager::GetTexId(TEX_UI_SELECTIMAGE_ARCHER) + i);
						playerSelectImages[j]->SetIsUse(true);
						GameSystem::SetSelectChara(j, (CHARA_KIND)i);

						AudioManager::AudioPlay(AK_SE_KETTEI, false);


					}
				}
			}


			//決定音とか

			
		}

		//プレイヤー１がセレクトした状態なら開始ガイドを表示
		if (GameSystem::GetSelectChara(0) != CHARAKIND_MAX) {

			pUIMessageWindow->SetIsUse(true);

			//プレイヤー１がセレクトした状態でCを押すと開始
			if (KeyManager_Trigger(KM_CAMERA) || ControllerManager::GetButtonState(0).yButton == BS_TRIGGER) {

			//モード移動
				AudioManager::AudioPlay(AK_SE_GAMESTART, false);
				//BGMセット
				AudioManager::AudioStop(AK_BGM_MENU);

				GameSystem::SetCurrentStage(0);

				Scene::SetCurrentScene(SCENE_GAME);
			}

		}
		else {
			pUIMessageWindow->SetIsUse(false);
		}

		if (KeyManager_Trigger(KM_CANCEL) || ControllerManager::GetButtonState(0).bButton == BS_TRIGGER) {//戻る

			AudioManager::AudioPlay(AK_SE_CANCEL, false);

			TitleModeSet(TM_MENU);

		}

		break;

	case TM_ONEPLAYER:

		////キャラセレ
		//マーカー
		HandMove();


		//プレイヤーのキャラが選択されていたらバトルを始める
		if (KeyManager_Trigger(KM_DECIDE) || ControllerManager::GetButtonState(0).aButton == BS_TRIGGER) {

			//エリア内に入っていたら該当キャラを選択
			for (int i = 0; i < CHARA_KINDNUM; i++) {
				for (int j = 0; j < 4; j++) {
					//プレイヤー１
					if (charaSelectArea[i].CheckInArea2D({ selectMaker[j]->GetPosition().x,selectMaker[j]->GetPosition().y })) {

						playerSelectImages[j]->SetTexture(ImageManager::GetTexId(TEX_UI_SELECTIMAGE_ARCHER) + i);
						GameSystem::SetSelectChara(j, (CHARA_KIND)i);
					}
				}
			}


			//決定音とか


		}

		//プレイヤー１がセレクトした状態なら開始ガイドを表示
		if (GameSystem::GetSelectChara(0) != CHARAKIND_MAX) {

			pUIMessageWindow->SetIsUse(true);

			//プレイヤー１がセレクトした状態でCを押すと開始
			if (KeyManager_Trigger(KM_CAMERA) || ControllerManager::GetButtonState(0).yButton == BS_TRIGGER) {

				//モード移動
				//Scene::SetCurrentScene(SCENE_GAME);
				AudioManager::AudioPlay(AK_SE_GAMESTART, false);
				AudioManager::AudioStop(AK_BGM_MENU);

				//あとで一人用のやつをつくる
				Scene::SetCurrentScene(SCENE_GAME);
				GameSystem::SetCurrentStage(1);
			}

		}
		else {
			pUIMessageWindow->SetIsUse(false);
		}


		if (KeyManager_Trigger(KM_CANCEL) || ControllerManager::GetButtonState(0).bButton == BS_TRIGGER) {//戻る

			AudioManager::AudioPlay(AK_SE_CANCEL, false);

			TitleModeSet(TM_MENU);

		}

		break;

	case TM_KEYCONFIG:


		//対象コントローラーの変更
		if (!keyconSelect && ControllerManager::GetButtonState(keyconfigControllerNum).rButton == BS_TRIGGER) {

			int dummy = keyconfigControllerNum + 1;

			if (dummy >= 4) dummy = 0;

			if (ControllerManager::GetCDevice(dummy) != nullptr) {

				keyconfigControllerNum = dummy;
			}
			else keyconfigControllerNum = 0;

			char buf[32];
			sprintf(buf,"%dP", keyconfigControllerNum + 1);
			keyconPlayerNumString->TextUpdate(buf);

			for (int i = 0; i < 4; i++) {

				char buf[64];
				if (i == 0) sprintf(buf, "%d", (int)ControllerManager::GetConfigStates(keyconfigControllerNum, BK_BUTTON_A));
				else if (i == 1) sprintf(buf, "%d", (int)ControllerManager::GetConfigStates(keyconfigControllerNum, BK_BUTTON_B));
				else if (i == 2) sprintf(buf, "%d", (int)ControllerManager::GetConfigStates(keyconfigControllerNum, BK_BUTTON_X));
				else if (i == 3) sprintf(buf, "%d", (int)ControllerManager::GetConfigStates(keyconfigControllerNum, BK_BUTTON_R));

				keyconWindowString[i]->TextUpdate(buf);

			}

		}


		//選択状態だとカーソル移動しない
		if (keyconSelect) {

			//コントローラーの入力を取得
			BUTTON_KIND setBK = ControllerManager::GetTriggerButton(keyconfigControllerNum);
			if (setBK == BK_BUTTON_NONE || ControllerManager::GetButtonState(keyconfigControllerNum).aButton == BS_PRESS);
			else {
				//コントローラー番号と選択しているカーソル位置をチェックする
				BUTTON_KIND bk;
				int keyNum;
				if (cursorKeyConfig == KCM_BUTTON1) {
					//bk = BK_BUTTON_A;
					//bk = ControllerManager::GetCS(keyconfigControllerNum).aButtonSet;
					bk = ControllerManager::GetConfigStates(keyconfigControllerNum, BK_BUTTON_A);
					//setBK = BK_BUTTON_A;
					keyNum = BK_BUTTON_A;
				}
				else if (cursorKeyConfig == KCM_BUTTON2) {
					//bk = BK_BUTTON_B;
					//bk = ControllerManager::GetCS(keyconfigControllerNum).bButtonSet;
					bk = ControllerManager::GetConfigStates(keyconfigControllerNum, BK_BUTTON_B);
					//setBK = BK_BUTTON_B;
					keyNum = BK_BUTTON_B;
				}
				else if (cursorKeyConfig == KCM_BUTTON3) {
					//bk = BK_BUTTON_X;
					//bk = ControllerManager::GetCS(keyconfigControllerNum).xButtonSet;
					bk = ControllerManager::GetConfigStates(keyconfigControllerNum, BK_BUTTON_X);
					//setBK = BK_BUTTON_X;
					keyNum = BK_BUTTON_X;
				}
				else if (cursorKeyConfig == KCM_BUTTON4) {
					//bk = BK_BUTTON_R;
					//bk = ControllerManager::GetCS(keyconfigControllerNum).rButtonSet;
					bk = ControllerManager::GetConfigStates(keyconfigControllerNum, BK_BUTTON_R);
					//setBK = BK_BUTTON_R;
					keyNum = BK_BUTTON_R;
				}

				//setBK = ControllerManager::GetConfigStates(keyconfigControllerNum, setBK);

				//ControllerManager::KeyConfig(setBK, bk, 0);
				//ControllerManager::KeyConfig(bk,setBK , keyconfigControllerNum);
				ControllerManager::KeyConfig(bk, setBK,keyNum, keyconfigControllerNum);

				AudioManager::AudioPlay(AK_SE_CURSORMOVE, false);

				keyconSelect = false;
				//return;
				acceptCooltime = 20;


				for (int i = 0; i < 4; i++) {

					if (i == cursorKeyConfig) {
						keyConfigWindowTex[i]->SetColor({ 255 / 255.0f,255 / 255.0f,111 / 255.0f,255 / 255.0f });
						keyConfigWindowTex[i]->UpdateVertex();
					}
					else {
						keyConfigWindowTex[i]->SetColor({ 255 / 255.0f,255 / 255.0f,255 / 255.0f,255 / 255.0f });
						keyConfigWindowTex[i]->UpdateVertex();
					}

					char buf[64];
					if (i == 0) sprintf(buf, "%d", (int)ControllerManager::GetConfigStates(keyconfigControllerNum, BK_BUTTON_A));
					else if (i == 1) sprintf(buf, "%d", (int)ControllerManager::GetConfigStates(keyconfigControllerNum, BK_BUTTON_B));
					else if (i == 2) sprintf(buf, "%d", (int)ControllerManager::GetConfigStates(keyconfigControllerNum, BK_BUTTON_X));
					else if (i == 3) sprintf(buf, "%d", (int)ControllerManager::GetConfigStates(keyconfigControllerNum, BK_BUTTON_R));

					keyconWindowString[i]->TextUpdate(buf);


				}

			}

			return;
		}


		//選択
		if (KeyManager_Trigger(KM_DECIDE) || ControllerManager::GetButtonState(keyconfigControllerNum).aButton == BS_TRIGGER) {

			if (acceptCooltime > 0) break;

			AudioManager::AudioPlay(AK_SE_CURSORMOVE, false);


			//非選択状態
			if (!keyconSelect) {

				keyconSelect = true;

			}
			//選択状態
			else {
				return;
			}

		}

		if (KeyManager_Press(KM_W) || ControllerManager::GetButtonState(keyconfigControllerNum).LstickY == -1000) {

			if (acceptCooltime > 0) break;


			//カーソル移動
			cursorKeyConfig--;
			AudioManager::AudioPlay(AK_SE_CURSORMOVE, false);

			if (cursorKeyConfig < 0) cursorKeyConfig = KCM_BUTTONMAX - 1;

			//選択箇所の色を変える
			for (int i = 0; i < 4; i++) {

				if (i == cursorKeyConfig) {
					keyConfigWindowTex[i]->SetColor({ 255 / 255.0f,255 / 255.0f,111 / 255.0f,255 / 255.0f });
					keyConfigWindowTex[i]->UpdateVertex();
				}
				else {
					keyConfigWindowTex[i]->SetColor({ 255 / 255.0f,255 / 255.0f,255 / 255.0f,255 / 255.0f });
					keyConfigWindowTex[i]->UpdateVertex();
				}

			}

			acceptCooltime = 20;
		}
		else if (KeyManager_Press(KM_S) || ControllerManager::GetButtonState(keyconfigControllerNum).LstickY == 1000) {

			if (acceptCooltime > 0) break;


			//カーソル移動
			cursorKeyConfig++;
			AudioManager::AudioPlay(AK_SE_CURSORMOVE, false);

			if (cursorKeyConfig >= KCM_BUTTONMAX) cursorKeyConfig = 0;

			//選択箇所の色を変える
			for (int i = 0; i < 4; i++) {

				if (i == cursorKeyConfig) {
					keyConfigWindowTex[i]->SetColor({ 255 / 255.0f,255 / 255.0f,111 / 255.0f,255 / 255.0f });
					keyConfigWindowTex[i]->UpdateVertex();
				}
				else {
					keyConfigWindowTex[i]->SetColor({ 255 / 255.0f,255 / 255.0f,255 / 255.0f,255 / 255.0f });
					keyConfigWindowTex[i]->UpdateVertex();
				}

			}

			acceptCooltime = 20;
		}

		//戻る
		if (KeyManager_Trigger(KM_CANCEL) || ControllerManager::GetButtonState(0).bButton == BS_TRIGGER) {//戻る

			if (acceptCooltime > 0) break;

			AudioManager::AudioPlay(AK_SE_CANCEL, false);

			TitleModeSet(TM_MENU);

		}

		break;

	default:
		break;
	}




}

void Title::TitleModeSet(TITLE_MODE s_tm) {

	currentMode = s_tm;

	switch (s_tm)
	{
	case TM_TITLE:
		//ウインドウ状態変更
		titleLogo->SetIsUse(true);
		gameStartWindow->SetIsUse(true);

		gameMenuWindow_Once->SetIsUse(false);
		gameMenuWindow_Battle->SetIsUse(false);
		gameMenuWindow_KeyConfig->SetIsUse(false);
		pUIMessageWindow->SetIsUse(false);
		P1Hand.image->SetIsUse(false);
		P2Hand.image->SetIsUse(false);
		titleBg->SetIsUse(true);
		stockControllWindow->SetIsUse(false);
		charaSelectBg->SetIsUse(false);

		for (int i = 0; i < CHARA_KINDNUM; i++) charaSelectWindows[i]->SetIsUse(false);
		for (int i = 0; i < 4; i++) playerSelectWindows[i]->SetIsUse(false);
		for (int i = 0; i < 4; i++)selectMaker[i]->SetIsUse(false);

		//背景セット
		titleMenuBg->SetIsUse(false);
		for (int i = 0; i < 9; i++) titleMenuParts[i]->SetIsUse(false);
		for (int i = 0; i < 4; i++) boundPolygon[i].image->SetIsUse(true);

		//キーコンフィグ
		for (int i = 0; i < 4; i++) keyConfigWindow[i]->SetIsUse(false);
		keyConfigLogoWindow->SetIsUse(false);
		for (int i = 0; i < 4; i++) keyConfigWindowTexString[i]->SetIsUse(false);

		//BGMセット
		AudioManager::AudioStop(AK_BGM_MENU);
		AudioManager::AudioPlay(AK_BGM_TITLE,1);

		break;
	case TM_MENU:

		//選択箇所の色を変える
		if (cursor == TMM_ONCE) {
			onceMenuText->SetColor({ 196,0,0,255 });
			battleMenuText->SetColor({ 0,0,0,255 });
		}
		else if (cursor == TMM_BATTLE) {
			onceMenuText->SetColor({ 0,0,0,255 });
			battleMenuText->SetColor({ 196,0,0,255 });
		}

		//ウインドウ状態変更
		titleLogo->SetIsUse(false);
		gameStartWindow->SetIsUse(false);

		gameMenuWindow_Once->SetIsUse(true);
		gameMenuWindow_Battle->SetIsUse(true);
		gameMenuWindow_KeyConfig->SetIsUse(true);
		pUIMessageWindow->SetIsUse(false);
		P1Hand.image->SetIsUse(false);
		P2Hand.image->SetIsUse(false);
		titleBg->SetIsUse(false);
		stockControllWindow->SetIsUse(false);
		charaSelectBg->SetIsUse(false);

		//選択箇所の色を変える
		if (cursor == TMM_ONCE) {
			onceMenuText->SetColor({ 255 / 255.0f,255 / 255.0f,111 / 255.0f,255 / 255.0f });
			battleMenuText->SetColor({ 255 / 255.0f,255 / 255.0f,255 / 255.0f,255 / 255.0f });
			keyconfigMenuText->SetColor({ 255 / 255.0f,255 / 255.0f,255 / 255.0f,255 / 255.0f });
			onceMenuText->UpdateVertex();
			battleMenuText->UpdateVertex();
			keyconfigMenuText->UpdateVertex();

		}
		else if (cursor == TMM_BATTLE) {
			onceMenuText->SetColor({ 255 / 255.0f,255 / 255.0f,255 / 255.0f,255 / 255.0f });
			battleMenuText->SetColor({ 255 / 255.0f,255 / 255.0f,111 / 255.0f,255 / 255.0f });
			onceMenuText->UpdateVertex();
			battleMenuText->UpdateVertex();
			keyconfigMenuText->SetColor({ 255 / 255.0f,255 / 255.0f,255 / 255.0f,255 / 255.0f });
			keyconfigMenuText->UpdateVertex();
		}
		else if (cursor == TMM_KEYCONFIG) {
			onceMenuText->SetColor({ 255 / 255.0f,255 / 255.0f,255 / 255.0f,255 / 255.0f });
			battleMenuText->SetColor({ 255 / 255.0f,255 / 255.0f,255 / 255.0f,255 / 255.0f });
			onceMenuText->UpdateVertex();
			battleMenuText->UpdateVertex();
			keyconfigMenuText->SetColor({ 255 / 255.0f,255 / 255.0f,111 / 255.0f,255 / 255.0f });
			keyconfigMenuText->UpdateVertex();
		}


		for (int i = 0; i < CHARA_KINDNUM; i++) charaSelectWindows[i]->SetIsUse(false);
		for (int i = 0; i < 4; i++) playerSelectWindows[i]->SetIsUse(false);
		for (int i = 0; i < 4; i++)selectMaker[i]->SetIsUse(false);

		//背景セット
		titleMenuBg->SetIsUse(true);
		for (int i = 0; i < 9; i++) titleMenuParts[i]->SetIsUse(true);
		for (int i = 0; i < 4; i++) boundPolygon[i].image->SetIsUse(false);

		//キーコンフィグ
		for (int i = 0; i < 4; i++) keyConfigWindow[i]->SetIsUse(false);
		keyConfigLogoWindow->SetIsUse(false);
		for (int i = 0; i < 4; i++) keyConfigWindowTexString[i]->SetIsUse(false);

		//BGMセット
		AudioManager::AudioStop(AK_BGM_TITLE);
		AudioManager::AudioPlay(AK_BGM_MENU, 1);

		break;

	case TM_CHARASELECT:
		//ウインドウ状態変更
		titleLogo->SetIsUse(false);
		gameStartWindow->SetIsUse(false);

		gameMenuWindow_Once->SetIsUse(false);
		gameMenuWindow_Battle->SetIsUse(false);
		gameMenuWindow_KeyConfig->SetIsUse(false);
		P1Hand.image->SetIsUse(true);
		P2Hand.image->SetIsUse(true);
		titleBg->SetIsUse(false);
		stockControllWindow->SetIsUse(true);
		charaSelectBg->SetIsUse(true);

		for (int i = 0; i < CHARA_KINDNUM; i++) charaSelectWindows[i]->SetIsUse(true);
		for (int i = 0; i < 4; i++) playerSelectWindows[i]->SetIsUse(true);
		for (int i = 0; i < 4; i++)selectMaker[i]->SetIsUse(true);

		//背景セット
		titleMenuBg->SetIsUse(true);
		for (int i = 0; i < 9; i++) titleMenuParts[i]->SetIsUse(true);
		for (int i = 0; i < 4; i++) boundPolygon[i].image->SetIsUse(false);

		//キーコンフィグ
		for (int i = 0; i < 4; i++) keyConfigWindow[i]->SetIsUse(false);
		keyConfigLogoWindow->SetIsUse(false);
		for (int i = 0; i < 4; i++) keyConfigWindowTexString[i]->SetIsUse(false);

		break;

	case TM_ONEPLAYER:

		//ウインドウ状態変更
		titleLogo->SetIsUse(false);
		gameStartWindow->SetIsUse(false);

		gameMenuWindow_Once->SetIsUse(false);
		gameMenuWindow_Battle->SetIsUse(false);
		gameMenuWindow_KeyConfig->SetIsUse(false);
		P1Hand.image->SetIsUse(true);
		P2Hand.image->SetIsUse(false);
		titleBg->SetIsUse(false);
		stockControllWindow->SetIsUse(true);
		charaSelectBg->SetIsUse(true);

		for (int i = 0; i < CHARA_KINDNUM; i++) charaSelectWindows[i]->SetIsUse(true);
		for (int i = 0; i < 1; i++) playerSelectWindows[i]->SetIsUse(true);
		for (int i = 0; i < 1; i++)selectMaker[i]->SetIsUse(true);

		//背景セット
		titleMenuBg->SetIsUse(true);
		for (int i = 0; i < 9; i++) titleMenuParts[i]->SetIsUse(true);
		for (int i = 0; i < 4; i++) boundPolygon[i].image->SetIsUse(false);

		//キーコンフィグ
		for (int i = 0; i < 4; i++) keyConfigWindow[i]->SetIsUse(false);
		keyConfigLogoWindow->SetIsUse(false);
		for (int i = 0; i < 4; i++) keyConfigWindowTexString[i]->SetIsUse(false);

		break;

	case TM_KEYCONFIG:

		//ウインドウ状態変更
		titleLogo->SetIsUse(false);
		gameStartWindow->SetIsUse(false);

		gameMenuWindow_Once->SetIsUse(false);
		gameMenuWindow_Battle->SetIsUse(false);
		gameMenuWindow_KeyConfig->SetIsUse(false);
		P1Hand.image->SetIsUse(false);
		P2Hand.image->SetIsUse(false);
		titleBg->SetIsUse(false);
		stockControllWindow->SetIsUse(false);
		charaSelectBg->SetIsUse(false);

		for (int i = 0; i < CHARA_KINDNUM; i++) charaSelectWindows[i]->SetIsUse(false);
		for (int i = 0; i < 1; i++) playerSelectWindows[i]->SetIsUse(false);
		for (int i = 0; i < 1; i++)selectMaker[i]->SetIsUse(false);

		//背景セット
		titleMenuBg->SetIsUse(true);
		for (int i = 0; i < 9; i++) titleMenuParts[i]->SetIsUse(true);
		for (int i = 0; i < 4; i++) boundPolygon[i].image->SetIsUse(false);

		//キーコンフィグ
		for (int i = 0; i < 4; i++) keyConfigWindow[i]->SetIsUse(true);
		keyConfigLogoWindow->SetIsUse(true);
		for (int i = 0; i < 4; i++) keyConfigWindowTexString[i]->SetIsUse(true);

		for (int i = 0; i < 4; i++) {

			if (i == cursorKeyConfig) {
				keyConfigWindowTex[i]->SetColor({ 255 / 255.0f,255 / 255.0f,111 / 255.0f,255 / 255.0f });
				keyConfigWindowTex[i]->UpdateVertex();
			}
			else {
				keyConfigWindowTex[i]->SetColor({ 255 / 255.0f,255 / 255.0f,255 / 255.0f,255 / 255.0f });
				keyConfigWindowTex[i]->UpdateVertex();
			}

			char buf[64];
			if (i == 0) sprintf(buf, "%d",(int)ControllerManager::GetConfigStates(0,BK_BUTTON_A));
			else if (i == 1) sprintf(buf, "%d", (int)ControllerManager::GetConfigStates(0, BK_BUTTON_B));
			else if (i == 2) sprintf(buf, "%d", (int)ControllerManager::GetConfigStates(0, BK_BUTTON_X));
			else if (i == 3) sprintf(buf, "%d", (int)ControllerManager::GetConfigStates(0, BK_BUTTON_R));

			keyconWindowString[i]->TextUpdate(buf);


		}


		

		break;

	default:
		break;
	}


}

void Title::MarkerMove(int s_num) {

	if (KeyManager_Press(KM_D) || ControllerManager::GetButtonState(0).LstickX == 1000) {
		D3DXVECTOR3 sPos = selectMaker[s_num]->GetPosition();
		sPos.x += MARKER_MOVESPEED;
		selectMaker[s_num]->SetPosition(sPos);
		selectMaker[s_num]->UpdateVertex({ sPos.x,sPos.y }, { SELECTMAKER_WIDTH,SELECTMAKER_HEIGHT });
	}
	if (KeyManager_Press(KM_A) || ControllerManager::GetButtonState(0).LstickX == -1000) {
		D3DXVECTOR3 sPos = selectMaker[s_num]->GetPosition();
		sPos.x -= MARKER_MOVESPEED;
		selectMaker[s_num]->SetPosition(sPos);
		selectMaker[s_num]->UpdateVertex({ sPos.x,sPos.y }, { SELECTMAKER_WIDTH,SELECTMAKER_HEIGHT });

	}
	if (KeyManager_Press(KM_W) || ControllerManager::GetButtonState(0).LstickY == -1000) {
		D3DXVECTOR3 sPos = selectMaker[s_num]->GetPosition();
		sPos.y -= MARKER_MOVESPEED;
		selectMaker[s_num]->SetPosition(sPos);
		selectMaker[s_num]->UpdateVertex({ sPos.x,sPos.y }, { SELECTMAKER_WIDTH,SELECTMAKER_HEIGHT });

	}
	if (KeyManager_Press(KM_S) || ControllerManager::GetButtonState(0).LstickY == 1000) {
		D3DXVECTOR3 sPos = selectMaker[s_num]->GetPosition();
		sPos.y += MARKER_MOVESPEED;
		selectMaker[s_num]->SetPosition(sPos);
		selectMaker[s_num]->UpdateVertex({ sPos.x,sPos.y }, { SELECTMAKER_WIDTH,SELECTMAKER_HEIGHT });

	}

	//プレイヤー２
	if (KeyManager_Press(KM_K) || ControllerManager::GetButtonState(1).LstickX == 1000) {
		D3DXVECTOR3 sPos = selectMaker[s_num]->GetPosition();
		sPos.x += MARKER_MOVESPEED;
		selectMaker[s_num]->SetPosition(sPos);
		selectMaker[s_num]->UpdateVertex({ sPos.x,sPos.y }, { SELECTMAKER_WIDTH,SELECTMAKER_HEIGHT });
	}
	if (KeyManager_Press(KM_H) || ControllerManager::GetButtonState(1).LstickX == -1000) {
		D3DXVECTOR3 sPos = selectMaker[s_num]->GetPosition();
		sPos.x -= MARKER_MOVESPEED;
		selectMaker[s_num]->SetPosition(sPos);
		selectMaker[s_num]->UpdateVertex({ sPos.x,sPos.y }, { SELECTMAKER_WIDTH,SELECTMAKER_HEIGHT });

	}
	if (KeyManager_Press(KM_U) || ControllerManager::GetButtonState(1).LstickY == -1000) {
		D3DXVECTOR3 sPos = selectMaker[s_num]->GetPosition();
		sPos.y -= MARKER_MOVESPEED;
		selectMaker[s_num]->SetPosition(sPos);
		selectMaker[s_num]->UpdateVertex({ sPos.x,sPos.y }, { SELECTMAKER_WIDTH,SELECTMAKER_HEIGHT });

	}
	if (KeyManager_Press(KM_J) || ControllerManager::GetButtonState(1).LstickY == 1000) {
		D3DXVECTOR3 sPos = selectMaker[s_num]->GetPosition();
		sPos.y += MARKER_MOVESPEED;
		selectMaker[s_num]->SetPosition(sPos);
		selectMaker[s_num]->UpdateVertex({ sPos.x,sPos.y }, { SELECTMAKER_WIDTH,SELECTMAKER_HEIGHT });

	}

	//画面端ブロック入れる


}

void Title::HandMove() {


	//手の移動
	if (KeyManager_Press(KM_D) || ControllerManager::GetButtonState(0).LstickX == 1000) {
		D3DXVECTOR3 sPos = P1Hand.image->GetPosition();
		sPos.x += MARKER_MOVESPEED;
		P1Hand.image->SetPosition(sPos);
		P1Hand.image->UpdateVertex({ sPos.x,sPos.y }, { SELECTHAND_WIDTH,SELECTHAND_HEIGHT });
	}
	if (KeyManager_Press(KM_A) || ControllerManager::GetButtonState(0).LstickX == -1000) {
		D3DXVECTOR3 sPos = P1Hand.image->GetPosition();
		sPos.x -= MARKER_MOVESPEED;
		P1Hand.image->SetPosition(sPos);
		P1Hand.image->UpdateVertex({ sPos.x,sPos.y }, { SELECTHAND_WIDTH,SELECTHAND_HEIGHT });

	}
	if (KeyManager_Press(KM_W) || ControllerManager::GetButtonState(0).LstickY == -1000) {
		D3DXVECTOR3 sPos = P1Hand.image->GetPosition();
		sPos.y -= MARKER_MOVESPEED;
		P1Hand.image->SetPosition(sPos);
		P1Hand.image->UpdateVertex({ sPos.x,sPos.y }, { SELECTHAND_WIDTH,SELECTHAND_HEIGHT });

	}
	if (KeyManager_Press(KM_S) || ControllerManager::GetButtonState(0).LstickY == 1000) {
		D3DXVECTOR3 sPos = P1Hand.image->GetPosition();
		sPos.y += MARKER_MOVESPEED;
		P1Hand.image->SetPosition(sPos);
		P1Hand.image->UpdateVertex({ sPos.x,sPos.y }, { SELECTHAND_WIDTH,SELECTHAND_HEIGHT });

	}
	if(P1Hand.catchMarkerNum != -1) {//持っている状態ならマーカーも移動
		MarkerMove(P1Hand.catchMarkerNum);

	}


	//プレイヤ－２
	if (KeyManager_Press(KM_K) || ControllerManager::GetButtonState(1).LstickX == 1000) {
		D3DXVECTOR3 sPos = P2Hand.image->GetPosition();
		sPos.x += MARKER_MOVESPEED;
		P2Hand.image->SetPosition(sPos);
		P2Hand.image->UpdateVertex({ sPos.x,sPos.y }, { SELECTHAND_WIDTH,SELECTHAND_HEIGHT });
	}
	if (KeyManager_Press(KM_H) || ControllerManager::GetButtonState(1).LstickX == -1000) {
		D3DXVECTOR3 sPos = P2Hand.image->GetPosition();
		sPos.x -= MARKER_MOVESPEED;
		P2Hand.image->SetPosition(sPos);
		P2Hand.image->UpdateVertex({ sPos.x,sPos.y }, { SELECTHAND_WIDTH,SELECTHAND_HEIGHT });

	}
	if (KeyManager_Press(KM_U) || ControllerManager::GetButtonState(1).LstickY == -1000) {
		D3DXVECTOR3 sPos = P2Hand.image->GetPosition();
		sPos.y -= MARKER_MOVESPEED;
		P2Hand.image->SetPosition(sPos);
		P2Hand.image->UpdateVertex({ sPos.x,sPos.y }, { SELECTHAND_WIDTH,SELECTHAND_HEIGHT });

	}
	if (KeyManager_Press(KM_J) || ControllerManager::GetButtonState(1).LstickY == 1000) {
		D3DXVECTOR3 sPos = P2Hand.image->GetPosition();
		sPos.y += MARKER_MOVESPEED;
		P2Hand.image->SetPosition(sPos);
		P2Hand.image->UpdateVertex({ sPos.x,sPos.y }, { SELECTHAND_WIDTH,SELECTHAND_HEIGHT });

	}
	if (P2Hand.catchMarkerNum != -1) {//持っている状態ならマーカーも移動
		MarkerMove(P2Hand.catchMarkerNum);

	}


	//画面端ブロック入れる



	////キャッチ/リリース
	if (KeyManager_Trigger(KM_DECIDE) || ControllerManager::GetButtonState(0).aButton == BS_TRIGGER) {
		//すでに持っていたらリリース
		if (P1Hand.catchMarkerNum != -1) {

			P1Hand.catchMarkerNum = -1;
			P1Hand.image->SetTexture(ImageManager::GetTexId(TEX_ICON_SELECTHAND_OPEN));

		}
		//ハンドの位置がマーカーのエリア内ならキャッチ
		else {


			for (int i = 0; i < 4; i++) {
				//チェック
				Area markerArea;
				markerArea.rightMax = selectMaker[i]->GetPosition().x + SELECTMAKER_WIDTH / 2;
				markerArea.leftMax = selectMaker[i]->GetPosition().x - SELECTMAKER_WIDTH / 2;
				markerArea.downMax = selectMaker[i]->GetPosition().y + SELECTMAKER_HEIGHT / 2;
				markerArea.upMax = selectMaker[i]->GetPosition().y - SELECTMAKER_HEIGHT / 2;

				if (markerArea.CheckInArea2D({ P1Hand.image->GetPosition().x,P1Hand.image->GetPosition().y })) {

					//キャッチ
					P1Hand.catchMarkerNum = i;

					P1Hand.image->SetTexture(ImageManager::GetTexId(TEX_ICON_SELECTHAND_CATCH));

					break;

				}
			}


			//ついでにプレイヤーモードも変える
			for (int i = 0; i < 4; i++) {
				//チェック
				Area playerModeArea;
				playerModeArea.rightMax = playerModeImages[i]->GetPosition().x + PLAYERMODEPLATE_WIDTH / 2;
				playerModeArea.leftMax = playerModeImages[i]->GetPosition().x - PLAYERMODEPLATE_WIDTH / 2;
				playerModeArea.downMax = playerModeImages[i]->GetPosition().y + PLAYERMODEPLATE_HEIGHT / 2;
				playerModeArea.upMax = playerModeImages[i]->GetPosition().y - PLAYERMODEPLATE_HEIGHT / 2;

				if (playerModeArea.CheckInArea2D({ P1Hand.image->GetPosition().x,P1Hand.image->GetPosition().y })) {

					//チェンジ
					GameSystem::PlayerModeChange(i);

					//イメチェン
					//playerModeImages[i]->SetTexture(TEX_UI_PLAYERPLATE + GameSystem::GetPlayerMode(i));
					playerModeImages[i]->SetTexture(ImageManager::GetTexId((TEX_KIND)(TEX_UI_PLAYERPLATE + GameSystem::GetPlayerMode(i))));

					break;

				}
			}


			//ストック増減　一旦ここに置く
			Area leftSankakuArea, rightSankakuArea;
			leftSankakuArea.rightMax = leftSankaku->GetPosition().x + 64 / 2;
			leftSankakuArea.leftMax = leftSankaku->GetPosition().x - 64 / 2;
			leftSankakuArea.downMax = leftSankaku->GetPosition().y + 64 / 2;
			leftSankakuArea.upMax = leftSankaku->GetPosition().y - 64 / 2;

			if (leftSankakuArea.CheckInArea2D({ P1Hand.image->GetPosition().x,P1Hand.image->GetPosition().y })) {
				//キャッチ
				GameSystem::AddStockNum(-1);
				char buf[64];
				sprintf(buf, "%d",GameSystem::GetStockNum());
				stockControllText->TextUpdate(buf);
			}

			rightSankakuArea.rightMax = rightSankaku->GetPosition().x + 64 / 2;
			rightSankakuArea.leftMax = rightSankaku->GetPosition().x - 64 / 2;
			rightSankakuArea.downMax = rightSankaku->GetPosition().y + 64 / 2;
			rightSankakuArea.upMax = rightSankaku->GetPosition().y - 64 / 2;

			if (rightSankakuArea.CheckInArea2D({ P1Hand.image->GetPosition().x,P1Hand.image->GetPosition().y })) {
				//キャッチ
				GameSystem::AddStockNum(1);
				char buf[64];
				sprintf(buf, "%d", GameSystem::GetStockNum());
				stockControllText->TextUpdate(buf);
			}


			//参加ONOFF
			Area p3Area, p4Area;
			p3Area.rightMax = playerSelectWindows[2]->GetPosition().x + 160 / 2;
			p3Area.leftMax = playerSelectWindows[2]->GetPosition().x - 160 / 2;
			p3Area.downMax = playerSelectWindows[2]->GetPosition().y + 200 / 2;
			p3Area.upMax = playerSelectWindows[2]->GetPosition().y - 200 / 2;

			bool jp3 = GetJoinPlayer(2);

			//範囲内なら処理
			if (p3Area.CheckInArea2D({ P1Hand.image->GetPosition().x,P1Hand.image->GetPosition().y })) {
				

				SetJoinPlayer(2,!jp3);

			}

			if (jp3) {

				p4Area.rightMax = playerSelectWindows[3]->GetPosition().x + 160 / 2;
				p4Area.leftMax = playerSelectWindows[3]->GetPosition().x - 160 / 2;
				p4Area.downMax = playerSelectWindows[3]->GetPosition().y + 200 / 2;
				p4Area.upMax = playerSelectWindows[3]->GetPosition().y - 200 / 2;

				bool jp4 = GetJoinPlayer(3);

				if (p4Area.CheckInArea2D({ P1Hand.image->GetPosition().x,P1Hand.image->GetPosition().y })) {


					SetJoinPlayer(3, !jp4);

				}
			}

		}

	}


	////2Pキャッチ/リリース
	if (KeyManager_Trigger(KM_SPACE) || ControllerManager::GetButtonState(1).aButton == BS_TRIGGER) {
		//すでに持っていたらリリース
		if (P2Hand.catchMarkerNum != -1) {

			P2Hand.catchMarkerNum = -1;
			P2Hand.image->SetTexture(ImageManager::GetTexId(TEX_UI_SELECTHAND2_OPEN));

		}
		//ハンドの位置がマーカーのエリア内ならキャッチ
		else {


			for (int i = 0; i < 4; i++) {
				//チェック
				Area markerArea;
				markerArea.rightMax = selectMaker[i]->GetPosition().x + SELECTMAKER_WIDTH / 2;
				markerArea.leftMax = selectMaker[i]->GetPosition().x - SELECTMAKER_WIDTH / 2;
				markerArea.downMax = selectMaker[i]->GetPosition().y + SELECTMAKER_HEIGHT / 2;
				markerArea.upMax = selectMaker[i]->GetPosition().y - SELECTMAKER_HEIGHT / 2;

				if (markerArea.CheckInArea2D({ P2Hand.image->GetPosition().x,P2Hand.image->GetPosition().y })) {

					//キャッチ
					P2Hand.catchMarkerNum = i;

					P2Hand.image->SetTexture(ImageManager::GetTexId(TEX_UI_SELECTHAND2_CATCH));

					break;

				}
			}


			//ついでにプレイヤーモードも変える
			for (int i = 0; i < 4; i++) {
				//チェック
				Area playerModeArea;
				playerModeArea.rightMax = playerModeImages[i]->GetPosition().x + PLAYERMODEPLATE_WIDTH / 2;
				playerModeArea.leftMax = playerModeImages[i]->GetPosition().x - PLAYERMODEPLATE_WIDTH / 2;
				playerModeArea.downMax = playerModeImages[i]->GetPosition().y + PLAYERMODEPLATE_HEIGHT / 2;
				playerModeArea.upMax = playerModeImages[i]->GetPosition().y - PLAYERMODEPLATE_HEIGHT / 2;

				if (playerModeArea.CheckInArea2D({ P2Hand.image->GetPosition().x,P2Hand.image->GetPosition().y })) {

					//チェンジ
					GameSystem::PlayerModeChange(i);

					//イメチェン
					//playerModeImages[i]->SetTexture(TEX_UI_PLAYERPLATE + GameSystem::GetPlayerMode(i));
					playerModeImages[i]->SetTexture(ImageManager::GetTexId((TEX_KIND)(TEX_UI_PLAYERPLATE + GameSystem::GetPlayerMode(i))));

					break;

				}
			}


			//ストック増減　一旦ここに置く
			/*
			Area leftSankakuArea, rightSankakuArea;
			leftSankakuArea.rightMax = leftSankaku->GetPosition().x + 64 / 2;
			leftSankakuArea.leftMax = leftSankaku->GetPosition().x - 64 / 2;
			leftSankakuArea.downMax = leftSankaku->GetPosition().y + 64 / 2;
			leftSankakuArea.upMax = leftSankaku->GetPosition().y - 64 / 2;

			if (leftSankakuArea.CheckInArea2D({ P1Hand.image->GetPosition().x,P1Hand.image->GetPosition().y })) {
				//キャッチ
				GameSystem::AddStockNum(-1);
				char buf[64];
				sprintf(buf, "%d", GameSystem::GetStockNum());
				stockControllText->TextUpdate(buf);
			}

			rightSankakuArea.rightMax = rightSankaku->GetPosition().x + 64 / 2;
			rightSankakuArea.leftMax = rightSankaku->GetPosition().x - 64 / 2;
			rightSankakuArea.downMax = rightSankaku->GetPosition().y + 64 / 2;
			rightSankakuArea.upMax = rightSankaku->GetPosition().y - 64 / 2;

			if (rightSankakuArea.CheckInArea2D({ P1Hand.image->GetPosition().x,P1Hand.image->GetPosition().y })) {
				//キャッチ
				GameSystem::AddStockNum(1);
				char buf[64];
				sprintf(buf, "%d", GameSystem::GetStockNum());
				stockControllText->TextUpdate(buf);
			}
			*/

		}

	}


}

void Title::JoinPlayer(int playerNum) {

	bool b = GameSystem::GetisJoin(playerNum);
	GameSystem::SetisJoin(playerNum, true);
	/*
	if (!GameSystem::GetisJoin(playerNum)) {
		playerSelectImages[playerNum]->SetColor({ 0.7f,0.7f, 0.7f, 1 });
		playerSelectImages[playerNum]->UpdateVertex();
	}
	else {
		playerSelectImages[playerNum]->SetColor({ 1.0f,1.0f, 1.0f, 1 });
		playerSelectImages[playerNum]->UpdateVertex();
	}*/

	if (playerNum == 0) {
		if (!GameSystem::GetisJoin(0)) playerSelectWindowTex[playerNum]->SetColor({ 184 / 255.0f,184 / 255.0f,184 / 255.0f,255 / 255.0f, });
		else playerSelectWindowTex[playerNum]->SetColor({ 255 / 255.0f,0 / 255.0f,0 / 255.0f,255 / 255.0f, });
	}
	else if (playerNum == 1) {
		if (GameSystem::GetisJoin(1)) playerSelectWindowTex[playerNum]->SetColor({ 0 / 255.0f,0 / 255.0f,255 / 255.0f,255 / 255.0f, });
		else playerSelectWindowTex[playerNum]->SetColor({ 184 / 255.0f,184 / 255.0f,184 / 255.0f,255 / 255.0f, });
	}
	else if (playerNum == 2) {
		if (GameSystem::GetisJoin(2)) playerSelectWindowTex[playerNum]->SetColor({ 255 / 255.0f,255 / 255.0f,0 / 255.0f,255 / 255.0f, });
		else playerSelectWindowTex[playerNum]->SetColor({ 184 / 255.0f,184 / 255.0f,184 / 255.0f,255 / 255.0f, });
	}
	else if (playerNum == 3) {
		if (GameSystem::GetisJoin(3)) playerSelectWindowTex[playerNum]->SetColor({ 0 / 255.0f,255 / 255.0f,0 / 255.0f,255 / 255.0f, });
		else playerSelectWindowTex[playerNum]->SetColor({ 184 / 255.0f,184 / 255.0f,184 / 255.0f,255 / 255.0f, });
	}

	playerSelectWindowTex[playerNum]->UpdateVertex();

}

void Title::SetJoinPlayer(int playerNum, bool s_bool) {

	bool b = GameSystem::GetisJoin(playerNum);
	GameSystem::SetisJoin(playerNum, s_bool);
	/*
	if (!GameSystem::GetisJoin(playerNum)) {
		playerSelectImages[playerNum]->SetColor({ 0.7f,0.7f, 0.7f, 1 });
		playerSelectImages[playerNum]->UpdateVertex();
	}
	else {
		playerSelectImages[playerNum]->SetColor({ 1.0f,1.0f, 1.0f, 1 });
		playerSelectImages[playerNum]->UpdateVertex();
	}*/

	if (playerNum == 0) {
		if (!GameSystem::GetisJoin(0)) playerSelectWindowTex[playerNum]->SetColor({ 184 / 255.0f,184 / 255.0f,184 / 255.0f,255 / 255.0f, });
		else playerSelectWindowTex[playerNum]->SetColor({ 255 / 255.0f,0 / 255.0f,0 / 255.0f,255 / 255.0f, });
	}
	else if (playerNum == 1) {
		if (GameSystem::GetisJoin(1)) playerSelectWindowTex[playerNum]->SetColor({ 0 / 255.0f,0 / 255.0f,255 / 255.0f,255 / 255.0f, });
		else playerSelectWindowTex[playerNum]->SetColor({ 184 / 255.0f,184 / 255.0f,184 / 255.0f,255 / 255.0f, });
	}
	else if (playerNum == 2) {
		if (GameSystem::GetisJoin(2)) playerSelectWindowTex[playerNum]->SetColor({ 255 / 255.0f,255 / 255.0f,0 / 255.0f,255 / 255.0f, });
		else playerSelectWindowTex[playerNum]->SetColor({ 184 / 255.0f,184 / 255.0f,184 / 255.0f,255 / 255.0f, });
	}
	else if (playerNum == 3) {
		if (GameSystem::GetisJoin(3)) playerSelectWindowTex[playerNum]->SetColor({ 0 / 255.0f,255 / 255.0f,0 / 255.0f,255 / 255.0f, });
		else playerSelectWindowTex[playerNum]->SetColor({ 184 / 255.0f,184 / 255.0f,184 / 255.0f,255 / 255.0f, });
	}

	playerSelectWindowTex[playerNum]->UpdateVertex();

}

void Title::TitleEffectFunction() {

	//タイトルロゴグラデーション
	tlShaderParameter.x -= 0.03f;
	titleLogo->SetShaderParameter(tlShaderParameter);

	//エフェクトONでなければ現在のパーティクルを非表示にして処理しない
	if (!titleEffectOn) {

		//非表示にする処理


		return;
	}

	//一定間隔ごとに生成を行う
	if (!ParticleGenerateUpdate()) return;


	TitleFire({ SCREEN_WIDTH / 2, SCREEN_HEIGHT - 100, 0 }, 3);

	TitleFire({ 100, SCREEN_HEIGHT - 100, 0 }, 5);
	TitleFire({ SCREEN_WIDTH - 100, SCREEN_HEIGHT - 100, 0 }, 4);

	TitleFire({ 200, SCREEN_HEIGHT - 50, 0 }, 7);
	TitleFire({ SCREEN_WIDTH - 200, SCREEN_HEIGHT - 150, 0 }, 6);


}

void Title::TitleFire(D3DXVECTOR3 s_pos, float s_size) {

	//生成
	D3DXVECTOR3 maxVec, minVec;
	maxVec = { 50.0f * s_size,5.0f * s_size ,5.0f * s_size };
	minVec = { -50.0f * s_size,-300.0f * s_size,-0.5f * s_size };

	//初期値設定
	D3DXVECTOR4 s_startColor, s_lastColor;
	s_startColor = { 255 / 255.0f,246 / 255.0f,102 / 255.0f,196 / 255.0f };
	s_lastColor = { 255 / 255.0f,102 / 255.0f,102 / 255.0f,128 / 255.0f };


	ParticleSystem2D* pPS = EffectManager::SetParticle2D();
	pPS->SetPosition(s_pos);
	pPS->SetColor(s_startColor);
	pPS->SetLastColor(s_lastColor);
	pPS->SetLife(128);
	pPS->SetParticleLife(32);
	pPS->SetVelocityMax(maxVec);
	pPS->SetVelocityMin(minVec);
	pPS->SetInterval(8);
	pPS->SetSpawnNum(32);
	pPS->SetSize(100 * s_size);
	pPS->SetLastSize({ 5 * s_size,5 * s_size });
	pPS->SetTexture(ImageManager::GetTexId(TEX_EFF_MOYA));
	pPS->SetisInstancing(true);
	pPS->Init();

}

void Title::TitleBreakEffect(float s_size, int index) {

	D3DXVECTOR3 spos = boundPolygon[index].image->GetPosition();

	//生成
	D3DXVECTOR3 maxVec, minVec;
	maxVec = { 50.0f * s_size,5.0f * s_size ,5.0f * s_size };
	minVec = { -50.0f * s_size,-300.0f * s_size,-0.5f * s_size };

	//位置によって速度を変える
	if (spos.x < 0) {
		maxVec.x = 300.0f;
		minVec.x = -5.0f;
	}
	else if (spos.x > SCREEN_WIDTH) {
		maxVec.x = -300.0f;
		minVec.x = 5.0f;
	}

	if (spos.y < 0) {
		maxVec.y = 300.0f;
		minVec.y = -5.0f;
	}
	else if (spos.x > SCREEN_WIDTH) {
		maxVec.y = -300.0f;
		minVec.y = 5.0f;
	}


	//プレイヤーごとに色変え
	D3DXVECTOR4 s_startColor, s_lastColor;
	s_startColor = { 255 / 255.0f,246 / 255.0f,102 / 255.0f,196 / 255.0f };
	s_lastColor = { 255 / 255.0f,102 / 255.0f,102 / 255.0f,128 / 255.0f };

	if (index == 0) {
		s_startColor = { 255 / 255.0f,64 / 255.0f,0 / 255.0f,196 / 255.0f };
		s_lastColor = { 255 / 255.0f,236 / 255.0f,230 / 255.0f,128 / 255.0f };
	}
	else if (index == 1) {
		s_startColor = { 48 / 255.0f,255 / 255.0f,0 / 255.0f,196 / 255.0f };
		s_lastColor = { 234 / 255.0f,255 / 255.0f,230 / 255.0f,128 / 255.0f };
	}
	else if (index == 2) {
		s_startColor = { 0 / 255.0f,80 / 255.0f,255 / 255.0f,196 / 255.0f };
		s_lastColor = { 230 / 255.0f,238 / 255.0f,255 / 255.0f,128 / 255.0f };
	}
	else if (index == 3) {
		s_startColor = { 255 / 255.0f,255 / 255.0f,64 / 255.0f,196 / 255.0f };
		s_lastColor = { 255 / 255.0f,255 / 255.0f,230 / 255.0f,128 / 255.0f };

	}

	ParticleSystem2D* pPS = EffectManager::SetParticle2D();
	pPS->SetPosition(spos);
	pPS->SetColor(s_startColor);
	pPS->SetLastColor(s_lastColor);
	pPS->SetLife(128);
	pPS->SetParticleLife(32);
	pPS->SetVelocityMax(maxVec);
	pPS->SetVelocityMin(minVec);
	pPS->SetInterval(8);
	pPS->SetSpawnNum(32);
	pPS->SetSize(100 * s_size);
	pPS->SetLastSize({ 5 * s_size,5 * s_size });
	pPS->SetTexture(ImageManager::GetTexId(TEX_PARTICLE_FLASH));
	pPS->SetisInstancing(true);
	pPS->Init();


}

//パーティクル生成可能ならtrueを返す
bool Title::ParticleGenerateUpdate() {

	if (particleGenerateCnt < PARTICLEGENERATE_TIME) {
		particleGenerateCnt++;
		return false;
	}
	
	particleGenerateCnt = 0;
	return true;

}


void Title::BoundPolygonInit() {

	//生成
	boundPolygon[0].image = ImageManager::SetImageD({ 50,100 }, { BOUNDPOLYGON_BASEWIDTH,BOUNDPOLYGON_BASEHEIGHT }, ImageManager::GetTexId(TEX_UI_BOUNDBALL), 0);
	boundPolygon[1].image = ImageManager::SetImageD({ SCREEN_WIDTH - 150,100 }, { BOUNDPOLYGON_BASEWIDTH,BOUNDPOLYGON_BASEHEIGHT }, ImageManager::GetTexId(TEX_UI_BOUNDBALL), 0);
	boundPolygon[2].image = ImageManager::SetImageD({ 50,SCREEN_HEIGHT - 100 }, { BOUNDPOLYGON_BASEWIDTH,BOUNDPOLYGON_BASEHEIGHT }, ImageManager::GetTexId(TEX_UI_BOUNDBALL), 0);
	boundPolygon[3].image = ImageManager::SetImageD({ SCREEN_WIDTH - 150,SCREEN_HEIGHT - 100 }, { BOUNDPOLYGON_BASEWIDTH,BOUNDPOLYGON_BASEHEIGHT }, ImageManager::GetTexId(TEX_UI_BOUNDBALL), 0);

	//パラメータ設定
	BoundPolygonSpdSet(0);
	BoundPolygonSpdSet(1);
	BoundPolygonSpdSet(2);
	BoundPolygonSpdSet(3);


	boundPolygon[0].size = BOUNDPOLYGON_BASEWIDTH;
	boundPolygon[1].size = BOUNDPOLYGON_BASEWIDTH;
	boundPolygon[2].size = BOUNDPOLYGON_BASEWIDTH;
	boundPolygon[3].size = BOUNDPOLYGON_BASEWIDTH;

	//シェーダー
	boundPolygon[0].image->SetVertexShader(RendererManager::GetGdColorVS(GC_RED));
	boundPolygon[0].image->SetPixelShader(RendererManager::GetGdColorPS(GC_RED));
	boundPolygon[0].image->SetVertxeLayout(RendererManager::GetGdColorIL(GC_RED));

	boundPolygon[1].image->SetVertexShader(RendererManager::GetGdColorVS(GC_GREEN));
	boundPolygon[1].image->SetPixelShader(RendererManager::GetGdColorPS(GC_GREEN));
	boundPolygon[1].image->SetVertxeLayout(RendererManager::GetGdColorIL(GC_GREEN));

	boundPolygon[2].image->SetVertexShader(RendererManager::GetGdColorVS(GC_BLUE));
	boundPolygon[2].image->SetPixelShader(RendererManager::GetGdColorPS(GC_BLUE));
	boundPolygon[2].image->SetVertxeLayout(RendererManager::GetGdColorIL(GC_BLUE));

	boundPolygon[3].image->SetVertexShader(RendererManager::GetGdColorVS(GC_YELLOW));
	boundPolygon[3].image->SetPixelShader(RendererManager::GetGdColorPS(GC_YELLOW));
	boundPolygon[3].image->SetVertxeLayout(RendererManager::GetGdColorIL(GC_YELLOW));

	boundPolygon[0].image->SetColor({1,1,1,0.9f});
	boundPolygon[1].image->SetColor({ 1,1,1,0.9f });
	boundPolygon[2].image->SetColor({ 1,1,1,0.9f });
	boundPolygon[3].image->SetColor({ 1,1,1,0.9f });

	boundPolygon[0].breakFlag = false;
	boundPolygon[1].breakFlag = false;
	boundPolygon[2].breakFlag = false;
	boundPolygon[3].breakFlag = false;

	boundPolygon[0].exist = true;
	boundPolygon[1].exist = true;
	boundPolygon[2].exist = true;
	boundPolygon[3].exist = true;

}

void Title::BoundPolygonUpdate() {

	//移動
	for (int i = 0; i < 4; i++) {

		if (!boundPolygon[i].exist) continue;

		D3DXVECTOR3 pos = boundPolygon[i].image->GetPosition();
		pos.x += boundPolygon[i].vec.x;
		pos.y += boundPolygon[i].vec.y;
		boundPolygon[i].image->SetPosition(pos);
		boundPolygon[i].image->UpdateVertex();

		/*
		if (pos.x > SCREEN_WIDTH || pos.x < 0) {
			//boundPolygon[i].vec.x *= -1;
			//pos.x += boundPolygon[i].vec.x;
		}
		
		if (pos.y > SCREEN_HEIGHT || pos.y < 0) {
			//boundPolygon[i].vec.y *= -1;
			//pos.y += boundPolygon[i].vec.y;
			BoundPolygonSpdSet(i);

		}*/

		
		if (pos.x > SCREEN_WIDTH) {
			BoundPolygonSpdSet(i);
			if (boundPolygon[i].vec.x > 0) boundPolygon[i].vec.x *= -1;
		
			BoundPolygonBreak(i);
		}
		else if (pos.x < 0) {
			BoundPolygonSpdSet(i);
			if (boundPolygon[i].vec.x < 0) boundPolygon[i].vec.x *= -1;
		
			BoundPolygonBreak(i);
		}

		else if (pos.y > SCREEN_HEIGHT) {
			BoundPolygonSpdSet(i);
			if (boundPolygon[i].vec.y > 0) boundPolygon[i].vec.y *= -1;
		
			BoundPolygonBreak(i);
		}
		else if (pos.y < 0) {
			BoundPolygonSpdSet(i);
			if (boundPolygon[i].vec.y < 0) boundPolygon[i].vec.y *= -1;
		
			BoundPolygonBreak(i);
		}


	}

	//物理反射
	BoundPolygonCollision();

	//画面外反射

}

void Title::BoundPolygonCollision() {

	float dx, dy,length;
	float e = 1.0f;//跳ね返り係数
	float	vx0_new, vy0_new, vx1_new, vy1_new;	// 衝突後の速度

	int cnt4 = 0;

	//コリジョンを計算
	for (int i = 0; i < 4 - 1; i++) {

		int cnt = 0;


		for (int j = i + 1; j < 4; j++) {

			// ボール同士のヒットチェック
			dx = boundPolygon[j].image->GetPosition().x - boundPolygon[i].image->GetPosition().x;				// ボール間のＸ方向差分
			dy = boundPolygon[j].image->GetPosition().y - boundPolygon[i].image->GetPosition().y;				// ボール間のＹ方向差分
			length = sqrtf(dx * dx + dy * dy);			// ボール間の距離を計算

			if (/*before_length > length &&*/ length < boundPolygon[j].size/2 + boundPolygon[i].size/2) {
				// 距離が短くなっており、かつ半径の合計よりも距離の方が短いので当たったと判定
				// はねかえりの法則 e=-(v1'-v2')/(v1-v2) と運動量保存則 m1v1'+m2v2' = m1v1+m2v2 を連立させる
				
				//存在していなければ処理しない
				if (!boundPolygon[j].exist) continue;
				//if (!boundPolygon[i].exist) continue;

				
				vx0_new = ((1 + e) * boundPolygon[j].vec.x) / 2;
				vy0_new = ((1 + e) * boundPolygon[j].vec.y) / 2;
				vx1_new = ((1 + e) * boundPolygon[i].vec.x) / 2;
				vy1_new = ((1 + e) * boundPolygon[i].vec.y) / 2;
				boundPolygon[i].vec.x = vx0_new;
				boundPolygon[i].vec.y = vy0_new;
				boundPolygon[j].vec.x = vx1_new;
				boundPolygon[j].vec.y = vy1_new;

				boundPolygon[i].breakFlag = true;
				boundPolygon[j].breakFlag = true;

				cnt++;
				if (j == 3) cnt4++;
			}
		


		}

		if (cnt == 0) {
			boundPolygon[i].exist = true;
		}
		if (cnt4 == 0) {
			boundPolygon[3].exist = true;
		}

	}

}

void Title::BoundPolygonSpdSet(int index) {

	float s_vecx, s_vecy;
	s_vecx = (rand() % 61 - 30) * 0.1f;
	if (0 < s_vecx && s_vecx < 1) s_vecx = 1.0f;
	else if (0 > s_vecx && s_vecx > -1) s_vecx = -1.0f;

	s_vecy = (rand() % 61 - 30) * 0.1f;
	if (0 < s_vecy && s_vecy < 1) s_vecy = 1.0f;
	else if (0 > s_vecy && s_vecy > -1) s_vecy = -1.0f;
	boundPolygon[index].vec = { s_vecx, s_vecy };

}

void Title::BoundPolygonBreak(int index) {

	//ブレイクフラッグが立っているかつ一定速度以上ならブレイク
	if (boundPolygon[index].breakFlag && D3DXVec2Length(&boundPolygon->vec) > 2.0f) {

		//撃破エフェクト
		TitleBreakEffect(1,index);

		//フラグ更新
		boundPolygon[index].exist = false;

		//リスポーン
		if (index == 0) {
			boundPolygon[index].image->SetPosition({ 50,100 ,0 });
		}
		else if (index == 1) {
			boundPolygon[index].image->SetPosition({ SCREEN_WIDTH - 150,100 ,0 });
		}
		else if (index == 2) {
			boundPolygon[index].image->SetPosition({ 50,SCREEN_HEIGHT - 100 ,0 });
		}
		else if (index == 3) {
			boundPolygon[index].image->SetPosition({ SCREEN_WIDTH - 150,SCREEN_HEIGHT - 100 ,0 });
		}

		
		boundPolygon[index].image->UpdateVertex();

		//復活位置に他のものがかぶっていたら


		return;
	}


	//速度が一定以上でなければフラグ解除
	boundPolygon[index].breakFlag = false;

}

void Title::TitleMenuBgInit() {

	for (int i = 0; i < 9;i++) {
		titleMenuParts[i] = ImageManager::SetImageD({ SCREEN_WIDTH/2.0f + (MENUBGPARTS_DISTANCE * (i - 4.0f)),SCREEN_HEIGHT/2.0f }, { 1200,675 }, ImageManager::GetTexId(TEX_UI_TITLEMENUBGPARTS), DEPTH_TWO);
		titleMenuParts[i]->SetColor({1,1,1,0.5f});
		titleMenuParts[i]->UpdateVertex();
		titleMenuParts[i]->SetIsUse(false);
	}

}

void Title::TitleMenuBgUpdate() {

	for (int i = 0; i < 9; i++) {
		D3DXVECTOR3 spos;
		spos = titleMenuParts[i]->GetPosition();
		spos.x += 2.0f;
		if (spos.x > SCREEN_WIDTH * 3 / 2.0f + 960/9) spos.x = -SCREEN_WIDTH / 2.0f - 960 / 9;
		titleMenuParts[i]->SetPosition(spos);
		titleMenuParts[i]->UpdateVertex();
	}

}

void Title::UIMessageUpdate() {




}

int Title::GetJoinPlayer(int playerNum) {
	return GameSystem::GetisJoin(playerNum); 
}

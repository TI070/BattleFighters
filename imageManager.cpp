
#include "imageManager.h"
#include "polygon2d.h"
#include "texture.h"
#include "UIwindow.h"
#include "fade.h"
#include "gauge.h"
#include "fieldConfig.h"



std::list<Polygon2D*> ImageManager::pImageList;

//Field* FieldManager::pField;

//ﾃｸｽﾁｬIDの羅列
unsigned short ImageManager::texId[TEXKIND_MAX];

void ImageManager::Init() {

	//テクスチャ読込
	texId[TEX_WHITE] = Texture_SetTextureLoadFile(TEX_WHITE_PASS);
	texId[TEX_PAINTBLOCK] = Texture_SetTextureLoadFile(TEX_PAINTBLOCK_PASS);
	texId[TEX_EXPLOSION] = Texture_SetTextureLoadFile(TEX_EXPLOSION_PASS);
	texId[TEX_BURST] = Texture_SetTextureLoadFile("asset//texture/effect/brust.png");
	texId[TEX_JAPANESE_FONT] = Texture_SetTextureLoadFile("asset/font/japanese.png");
	texId[TEX_ALPHABET_FONT] = Texture_SetTextureLoadFile("asset/font/alphabet.png");
	texId[TEX_WINDOW_FRAME] = Texture_SetTextureLoadFile("asset/texture/ui/frame.png");
	texId[TEX_LOGO_TITLE] = Texture_SetTextureLoadFile("asset/texture/logo/title_logo2.png");
	texId[TEX_LOGO_RESULT] = Texture_SetTextureLoadFile("asset/texture/logo/result_logo.png");
	texId[TEX_PARTICLE_FLASH] = Texture_SetTextureLoadFile("asset/texture/effect/particle_flash.png");
	texId[TEX_EX_UNITDEATH] = Texture_SetTextureLoadFile("asset/texture/effect/deathunit.png");
	texId[TEX_EFF_BLOW] = Texture_SetTextureLoadFile("asset/texture/effect/damage.png");
	texId[TEX_EFF_MOYA] = Texture_SetTextureLoadFile("asset/texture/effect/moya.png");
	texId[TEX_EFF_GROUNDSHOCK] = Texture_SetTextureLoadFile("asset/texture/effect/groundShock.png");

	texId[TEX_STOCKICON_BOW] = Texture_SetTextureLoadFile("asset/texture/icon/bow.png");
	texId[TEX_STOCKICON_SWORD] = Texture_SetTextureLoadFile("asset/texture/icon/sword.png");

	texId[TEX_FIELD_FIELD1] = Texture_SetTextureLoadFile("asset/texture/field/field1.png");


	texId[TEX_LOGO_STAGECLEAR] = Texture_SetTextureLoadFile("asset/texture/logo/clear_logo.png");
	texId[TEX_LOGO_PAUSE] = Texture_SetTextureLoadFile("asset/texture/logo/pause_logo.png");
	texId[TEX_LOGO_GAMESET] = Texture_SetTextureLoadFile("asset/texture/logo/gameSet.png");

	texId[TEX_UI_TITLEBG] = Texture_SetTextureLoadFile("asset/texture/ui/titleBg2.png");
	texId[TEX_UI_TITLEMENUBG] = Texture_SetTextureLoadFile("asset/texture/ui/titlemenuBg.png");
	texId[TEX_UI_TITLEMENUBGPARTS] = Texture_SetTextureLoadFile("asset/texture/ui/titlemenuBgParts.png");
	texId[TEX_UI_CHARASELECTBG] = Texture_SetTextureLoadFile("asset/texture/ui/charaSelectBg.png");


	texId[TEX_ICON_ROAD] = Texture_SetTextureLoadFile("asset/texture/icon/road.png");
	texId[TEX_ICON_STATUE] = Texture_SetTextureLoadFile("asset/texture/icon/statue.png");
	texId[TEX_ICON_BREAK] = Texture_SetTextureLoadFile("asset/texture/icon/break.png");
	texId[TEX_ICON_SWORD] = Texture_SetTextureLoadFile("asset/texture/icon/sword.png");
	texId[TEX_ICON_WIZARD] = Texture_SetTextureLoadFile("asset/texture/icon/wizard.png");
	texId[TEX_ICON_PRIEST] = Texture_SetTextureLoadFile("asset/texture/icon/priest.png");
	texId[TEX_ICON_Q] = Texture_SetTextureLoadFile("asset/texture/icon/Q_sk.png");
	texId[TEX_ICON_E] = Texture_SetTextureLoadFile("asset/texture/icon/E_sk.png");

	texId[TEX_ICON_SELECTHAND_OPEN] = Texture_SetTextureLoadFile("asset/texture/icon/selectHand1.png");
	texId[TEX_ICON_SELECTHAND_CATCH] = Texture_SetTextureLoadFile("asset/texture/icon/selectHand2.png");

	texId[TEX_UI_SELECTHAND2_OPEN] = Texture_SetTextureLoadFile("asset/texture/icon/selectHand1_2P.png");
	texId[TEX_UI_SELECTHAND2_CATCH] = Texture_SetTextureLoadFile("asset/texture/icon/selectHand2_2P.png");


	texId[TEX_ICON_LEFTSANKAKU] = Texture_SetTextureLoadFile("asset/texture/icon/leftSankaku.png");
	texId[TEX_ICON_RIGHTSANKAKU] = Texture_SetTextureLoadFile("asset/texture/icon/rightSankaku.png");


	texId[TEX_UI_PAUSEEX] = Texture_SetTextureLoadFile("asset/texture/ui/pauseEx.png");


	texId[TEX_UI_MESSAGEWINDOW] = Texture_SetTextureLoadFile("asset/texture/ui/messageWindow.png");
	texId[TEX_UI_PAUSEWINDOW] = Texture_SetTextureLoadFile("asset/texture/ui/pauseWindow.png");
	texId[TEX_UI_MESSAGEWINDOWBASE] = Texture_SetTextureLoadFile("asset/texture/ui/messageWindowBase.png");

	texId[TEX_UI_PLAYERPOWERWINDOW] = Texture_SetTextureLoadFile("asset/texture/ui/playerPowerUI.png");

	texId[TEX_UI_SELECTMARKER1] = Texture_SetTextureLoadFile("asset/texture/ui/marker1.png");
	texId[TEX_UI_SELECTMARKER2] = Texture_SetTextureLoadFile("asset/texture/ui/marker2.png");
	texId[TEX_UI_SELECTMARKER3] = Texture_SetTextureLoadFile("asset/texture/ui/marker3.png");
	texId[TEX_UI_SELECTMARKER4] = Texture_SetTextureLoadFile("asset/texture/ui/marker4.png");

	texId[TEX_UI_SELECTIMAGE_ARCHER] = Texture_SetTextureLoadFile("asset/texture/ui/marker4.png");


	texId[TEX_MES_GAMESET] = Texture_SetTextureLoadFile(TEX_PAINTBLOCK_PASS);


	texId[TEX_UI_SELECTIMAGE_ARCHER] = Texture_SetTextureLoadFile("asset/texture/ui/selectImage_archer.png");
	texId[TEX_UI_SELECTIMAGE_PALADIN] = Texture_SetTextureLoadFile("asset/texture/ui/selectImage_paladin.png");
	texId[TEX_UI_SELECTIMAGE_FIGTHER] = Texture_SetTextureLoadFile("asset/texture/ui/selectImage_fighter.png");
	texId[TEX_UI_SELECTIMAGE_WIZARD] = Texture_SetTextureLoadFile("asset/texture/ui/selectImage_wizard.png");


	texId[TEX_UI_PLAYERPLATE] = Texture_SetTextureLoadFile("asset/texture/ui/playerPlate.png");
	texId[TEX_UI_CPUPLATE] = Texture_SetTextureLoadFile("asset/texture/ui/cpuPlate.png");
	texId[TEX_UI_SANDBAGPLATE] = Texture_SetTextureLoadFile("asset/texture/ui/sandbagPlate.png");

	texId[TEX_UI_PLAYERSIGN1] = Texture_SetTextureLoadFile("asset/texture/ui/playerSign1.png");
	texId[TEX_UI_PLAYERSIGN2] = Texture_SetTextureLoadFile("asset/texture/ui/playerSign2.png");
	texId[TEX_UI_PLAYERSIGN3] = Texture_SetTextureLoadFile("asset/texture/ui/playerSign3.png");
	texId[TEX_UI_PLAYERSIGN4] = Texture_SetTextureLoadFile("asset/texture/ui/playerSign4.png");

	texId[TEX_UI_BOUNDBALL] = Texture_SetTextureLoadFile("asset/texture/ui/titleball.png");

	texId[TEX_WINDOW_FRAME2] = Texture_SetTextureLoadFile("asset/texture/ui/frame2.png");

	texId[TEX_STOCKICON_WAND] = Texture_SetTextureLoadFile("asset/texture/icon/wizard.png");
	texId[TEX_STOCKICON_FIGTHER] = Texture_SetTextureLoadFile("asset/texture/icon/figther.png");

	
	texId[TEX_UI_STRONEPLAYER] = Texture_SetTextureLoadFile("asset/texture/ui/onePlayer.png");
	texId[TEX_UI_STRBATTLEROYALE] = Texture_SetTextureLoadFile("asset/texture/ui/battleRoyale.png");
	texId[TEX_UI_MENUWINDOW] = Texture_SetTextureLoadFile("asset/texture/ui/titleMenuWindow.png");
	texId[TEX_UI_TITLESTART] = Texture_SetTextureLoadFile("asset/texture/ui/zGameStart.png");
	texId[TEX_UI_BATTLESTART] = Texture_SetTextureLoadFile("asset/texture/ui/cPressStart.png");
	texId[TEX_UI_PLAYERSELECTWINDOW] = Texture_SetTextureLoadFile("asset/texture/ui/playerSelectWindow.png");
	texId[TEX_UI_GAMESET] = Texture_SetTextureLoadFile("asset/texture/ui/gameSet.png");
	texId[TEX_UI_RESULT] = Texture_SetTextureLoadFile("asset/texture/ui/resultLogo.png");
	texId[TEX_UI_RESULTWINDOW] = Texture_SetTextureLoadFile("asset/texture/ui/resultWindow.png");
	texId[TEX_UI_STOCK] = Texture_SetTextureLoadFile("asset/texture/ui/stock.png");


	texId[TEX_FIELD_PURPLE] = Texture_SetTextureLoadFile("asset/texture/field/purple.png");
	texId[TEX_FIELD_CENTER] = Texture_SetTextureLoadFile("asset/texture/field/center.png");
	texId[TEX_FIELD2] = Texture_SetTextureLoadFile("asset/texture/field/field2.jpg");
	texId[TEX_FIELD3] = Texture_SetTextureLoadFile("asset/texture/field/field3.jpg");

	texId[TEX_UI_STAGE1] = Texture_SetTextureLoadFile("asset/texture/ui/stage1.png");
	texId[TEX_UI_STAGE2] = Texture_SetTextureLoadFile("asset/texture/ui/stage2.png");
	texId[TEX_UI_STAGE3] = Texture_SetTextureLoadFile("asset/texture/ui/stage3.png");
	texId[TEX_UI_STAGE4] = Texture_SetTextureLoadFile("asset/texture/ui/stage4.png");
	texId[TEX_UI_STAGE5] = Texture_SetTextureLoadFile("asset/texture/ui/stage5.png");
	texId[TEX_UI_CONGRA] = Texture_SetTextureLoadFile("asset/texture/ui/congra.png");

	texId[TEX_UI_READY] = Texture_SetTextureLoadFile("asset/texture/ui/ready.png");
	texId[TEX_UI_GO] = Texture_SetTextureLoadFile("asset/texture/ui/go.png");

	texId[TEX_UI_KEYCONFIG_LOGO] = Texture_SetTextureLoadFile("asset/texture/ui/keyconfig.png");
	texId[TEX_UI_KEYCONFIG_STRING1] = Texture_SetTextureLoadFile("asset/texture/ui/button1.png");
	texId[TEX_UI_KEYCONFIG_STRING2] = Texture_SetTextureLoadFile("asset/texture/ui/button2.png");
	texId[TEX_UI_KEYCONFIG_STRING3] = Texture_SetTextureLoadFile("asset/texture/ui/button3.png");
	texId[TEX_UI_KEYCONFIG_STRING4] = Texture_SetTextureLoadFile("asset/texture/ui/button4.png");

	texId[TEX_UI_MENU_KEYCONFIG] = Texture_SetTextureLoadFile("asset/texture/ui/keyconfigMenu.png");

	Texture_Load();
	
}

void ImageManager::Uninit() {

	for (Polygon2D* object : pImageList) {

		object->Uninit();
		delete object;
	}
	pImageList.clear();//リストの削除

}

void ImageManager::Update() {

	for (Polygon2D* object : pImageList) {

		object->Update();

	}

	RemoveMember<Polygon2D>(&pImageList);

}

void ImageManager::Draw() {

	//深さでソートする
	for (int i = DEPTH_MAX - 1; i >= 0; i--) {
		for (Polygon2D* object : pImageList) {

			//深いものから順番に描画する
			if (object->GetDepth() == i) {

				object->Draw();
			}
		}
	}

	//Fade::Fade_Draw();
}

Polygon2D* ImageManager::SetImage(D3DXVECTOR2 s_pos, float s_size, int texId, int depth) {

	Polygon2D* pPolygon2d = new Polygon2D(s_pos, s_size, texId,depth);
	//pField->Init();
	pImageList.push_back(pPolygon2d);

	return pPolygon2d;
}

Polygon2D* ImageManager::SetImage(D3DXVECTOR2 s_pos, D3DXVECTOR2 s_size, int texId, int depth) {

	Polygon2D* pPolygon2d = new Polygon2D(s_pos, s_size, texId, depth);
	pImageList.push_back(pPolygon2d);

	return pPolygon2d;
}

Polygon2D* ImageManager::SetImageD(D3DXVECTOR2 s_pos, D3DXVECTOR2 s_size, int texId, int depth) {

	Polygon2D* pPolygon2d = new Polygon2D(s_pos, s_size, texId, depth,true);
	pImageList.push_back(pPolygon2d);

	return pPolygon2d;
}

Polygon2D* ImageManager::SetStringJapaneseImage(D3DXVECTOR2 s_pos, float s_size, int texId, int depth, const char* string,D3DXVECTOR4 s_color) {

	Polygon2D* pPolygon2d = new TexStringJapanese(s_pos, s_size, texId, depth,string,s_color);
	//pField->Init();
	pImageList.push_back(pPolygon2d);

	return pPolygon2d;
}

Polygon2D* ImageManager::SetStringAlphabetImage(D3DXVECTOR2 s_pos, float s_size, int texId, int depth, const char* string, D3DXVECTOR4 s_color) {

	Polygon2D* pPolygon2d = new TexStringAlphabet(s_pos, s_size, texId, depth, string, s_color);
	//pField->Init();
	pImageList.push_back(pPolygon2d);

	return pPolygon2d;
}

Polygon2D* ImageManager::SetStringImage(D3DXVECTOR2 s_pos, float s_size, int texId, int depth, const char* string, D3DXVECTOR4 s_color) {

	Polygon2D* pPolygon2d = new TexString(s_pos, s_size, depth, string, s_color);
	//pField->Init();
	pImageList.push_back(pPolygon2d);

	return pPolygon2d;
}

Gauge* ImageManager::SetBuildGauge(float s_gaugeMax, float s_currentValue) {

	Gauge* pPolygon2d = new Gauge({ BUILDGAUGE_POS_X,BUILDGAUGE_POS_Y }, BUILDGAUGE_SIZE_X,BUILDGAUGE_SIZE_Y,s_gaugeMax ,s_currentValue);
	//pField->Init();
	pImageList.push_back(pPolygon2d);

	return pPolygon2d;
}

Gauge* ImageManager::SetGauge(D3DXVECTOR2 pos, float size_x, float size_y, float s_gaugeMax, float s_currentValue) {

	Gauge* pPolygon2d = new Gauge(pos, size_x, size_y, s_gaugeMax, s_currentValue);
	//pField->Init();
	pImageList.push_back(pPolygon2d);

	return pPolygon2d;
}

UIWindow* ImageManager::SetUIWindow(D3DXVECTOR2 s_pos, D3DXVECTOR2 s_size, int s_depth) {

	UIWindow* pPolygon2d = new UIWindow(s_pos, s_size,s_depth);
	//pField->Init();
	pImageList.push_back(pPolygon2d);

	return pPolygon2d;

}

StatusWindow* ImageManager::SetStatusWindow(D3DXVECTOR2 s_pos, D3DXVECTOR2 s_size, int s_depth, CHARA_KIND s_ck, int s_stockNum) {

	StatusWindow* pPolygon2d = new StatusWindow(s_pos, s_size, s_depth,s_ck,s_stockNum);
	//pField->Init();
	pImageList.push_back(pPolygon2d);

	return pPolygon2d;

}

unsigned short ImageManager::GetTexId(TEX_KIND texKind) {

	return texId[(int)texKind];

}

ID3D11ShaderResourceView* ImageManager::GetTex(TEX_KIND texKind) {

	return Texture_GetTexture(texId[(int)texKind]);
}
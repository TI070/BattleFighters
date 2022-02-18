#pragma once

#include <list>
#include "main.h" 
#include "managerBase.h"
#include "animationPlayer.h"

#define DEFAULT_UI_SIZE (200.0f)
#define UITEX_TEST_PASS "asset/texture/paintblock.png" 


#define TEX_WHITE_PASS "asset/texture/white.png" 
#define TEX_PAINTBLOCK_PASS "asset/texture/paintblock.png" 

#define TEX_PAINTBLOCK_PASS "asset/texture/paintblock.png" 
#define TEX_EXPLOSION_PASS "asset/texture/effect/explosion.png" 

#define BUILDGAUGE_SIZE_X (150)
#define BUILDGAUGE_SIZE_Y (20)

#define BUILDGAUGE_POS_X (SCREEN_WIDTH - BUILDGAUGE_SIZE_X/2 - 10)
#define BUILDGAUGE_POS_Y (60 + BUILDGAUGE_SIZE_Y/2 )

enum TEX_KIND {

	TEX_WHITE,
	TEX_PAINTBLOCK,
	TEX_EXPLOSION,
	TEX_BURST,
	TEX_JAPANESE_FONT,
	TEX_ALPHABET_FONT,
	TEX_WINDOW_FRAME,
	TEX_LOGO_TITLE,
	TEX_LOGO_RESULT,
	TEX_LOGO_GAMESET,

	TEX_PARTICLE_FLASH,
	TEX_EX_UNITDEATH,
	TEX_EFF_BLOW,
	TEX_EFF_MOYA,
	TEX_EFF_GROUNDSHOCK,

	TEX_STOCKICON_BOW,
	TEX_STOCKICON_SWORD,

	TEX_LOGO_STAGECLEAR,
	TEX_LOGO_PAUSE,

	TEX_FIELD_FIELD1,

	TEX_ICON_ROAD,
	TEX_ICON_STATUE,
	TEX_ICON_SWORD,
	TEX_ICON_WIZARD,
	TEX_ICON_PRIEST,
	TEX_ICON_Q,
	TEX_ICON_E,
	TEX_ICON_BREAK,
	TEX_ICON_SELECTHAND_OPEN,
	TEX_ICON_SELECTHAND_CATCH,
	TEX_ICON_LEFTSANKAKU,
	TEX_ICON_RIGHTSANKAKU,

	TEX_UI_PAUSEEX,

	TEX_UI_MESSAGEWINDOW,
	TEX_UI_MESSAGEWINDOWBASE,
	TEX_UI_PAUSEWINDOW,
	TEX_UI_TITLEBG,
	TEX_UI_TITLEMENUBG,
	TEX_UI_TITLEMENUBGPARTS,
	TEX_UI_CHARASELECTBG,


	TEX_UI_PLAYERPOWERWINDOW,

	TEX_MES_GAMESET,

	TEX_LOGO_RESULT_MESSAGE,

	TEX_UI_SELECTMARKER1,
	TEX_UI_SELECTMARKER2,
	TEX_UI_SELECTMARKER3,
	TEX_UI_SELECTMARKER4,
	TEX_UI_SELECTHAND1_OPEN,
	TEX_UI_SELECTHAND1_CATCH,
	TEX_UI_SELECTHAND2_OPEN,
	TEX_UI_SELECTHAND2_CATCH,


	TEX_UI_SELECTIMAGE_ARCHER,
	TEX_UI_SELECTIMAGE_PALADIN,
	TEX_UI_SELECTIMAGE_FIGTHER,
	TEX_UI_SELECTIMAGE_WIZARD,
	TEX_UI_PLAYERPLATE,
	TEX_UI_CPUPLATE,
	TEX_UI_SANDBAGPLATE,

	TEX_UI_PLAYERSIGN1,
	TEX_UI_PLAYERSIGN2,
	TEX_UI_PLAYERSIGN3,
	TEX_UI_PLAYERSIGN4,

	TEX_UI_BOUNDBALL,

	TEX_WINDOW_FRAME2,

	TEX_STOCKICON_WAND,
	TEX_STOCKICON_FIGTHER,

	TEX_UI_STRONEPLAYER,
	TEX_UI_STRBATTLEROYALE,

	TEX_UI_MENUWINDOW,
	TEX_UI_TITLESTART,
	TEX_UI_BATTLESTART,
	TEX_UI_PLAYERSELECTWINDOW,
	TEX_UI_GAMESET,
	TEX_UI_RESULT,
	TEX_UI_RESULTWINDOW,
	TEX_UI_STOCK,

	TEX_UI_STAGE1,
	TEX_UI_STAGE2,
	TEX_UI_STAGE3,
	TEX_UI_STAGE4,
	TEX_UI_STAGE5,
	TEX_UI_CONGRA,

	TEX_FIELD_PURPLE,
	TEX_FIELD_CENTER,
	TEX_FIELD2,
	TEX_FIELD3,

	TEX_UI_READY,
	TEX_UI_GO,

	TEX_UI_KEYCONFIG_LOGO,
	TEX_UI_KEYCONFIG_STRING1,
	TEX_UI_KEYCONFIG_STRING2,
	TEX_UI_KEYCONFIG_STRING3,
	TEX_UI_KEYCONFIG_STRING4,

	TEX_UI_MENU_KEYCONFIG,

	TEXKIND_MAX

};


class UIWindow;
class Polygon2D;
class Gauge;

class ImageManager : public ManagerBase
{

private:

	static std::list<Polygon2D*> pImageList;
	//static Field* pField;

public:

	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static Polygon2D* SetImage(D3DXVECTOR2 s_pos, float s_size, int texId,int depth);
	static Polygon2D* SetImage(D3DXVECTOR2 s_pos, D3DXVECTOR2 s_size, int texId,int depth);
	static Polygon2D* SetImageD(D3DXVECTOR2 s_pos, D3DXVECTOR2 s_size, int texId, int depth);

	static Polygon2D* SetStringJapaneseImage(D3DXVECTOR2 s_pos, float s_size, int texId, int depth,const char* string, D3DXVECTOR4 s_color);
	static Polygon2D* SetStringAlphabetImage(D3DXVECTOR2 s_pos, float s_size, int texId, int depth, const char* string, D3DXVECTOR4 s_color);
	static Polygon2D* SetStringImage(D3DXVECTOR2 s_pos, float s_size, int texId, int depth, const char* string, D3DXVECTOR4 s_color);
	static UIWindow* SetUIWindow(D3DXVECTOR2 s_pos, D3DXVECTOR2 s_size, int s_depth);
	static StatusWindow* SetStatusWindow(D3DXVECTOR2 s_pos, D3DXVECTOR2 s_size, int s_depth, CHARA_KIND s_ck, int s_stockNum);

	static Gauge* SetBuildGauge(float s_gaugeMax,float s_currentValue);
	static Gauge* SetGauge(D3DXVECTOR2 pos,float size_x, float size_y, float s_gaugeMax, float s_currentValue);

	static unsigned short texId[TEXKIND_MAX];

	static unsigned short GetTexId(TEX_KIND texKind);
	static ID3D11ShaderResourceView* GetTex(TEX_KIND texKind);

	





};
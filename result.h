#pragma once
#include "polygon2d.h"
#include "animationPlayer.h"

#define RESULTDATA_WINDOW_WIDTH (200)
#define RESULTDATA_WINDOW_HEIGHT (300)


enum RESULT_MENU {
	RESULT_START,
	RESULT_MAX
};

enum RESULT_PAUSE {

	RPAUSE_WINNER,
	RPAUSE_LOSER
};


class TexString;
class UIWindow;
class AnimationModel;

class Result {
private:

	static int select;

	static TexString* resultLogoText;
	static UIWindow* resultLogoWindow;

	static UIWindow* resultDataWindows[4];
	static Polygon2D* resultDataWindowsTex[4];

	static TexString* resultRankText[4];

	static Polygon2D* resultBg;
	static Polygon2D* resultBgParts[9];

	static AnimationModel* pModel[4];

	static ID3D11Buffer* m_VertexBuffer;
	static ID3D11ShaderResourceView* m_Texture;

	static ID3D11VertexShader* m_VertexShader;
	static ID3D11PixelShader* m_PixelShader;
	static ID3D11InputLayout* m_VertxeLayout;

	static Polygon2D* playerSelectImages[4];

	static int modelFrame[4];



public:

	static void Initialize();
	static void Draw();
	static void Uninit();
	static void Update();
	static void MenuSelect();

	static void ResultBgInit();
	static void ResultBgUpdate();

	static void ModelView();
	static void ModelLoad(CHARA_KIND s_CK, RESULT_PAUSE s_RP);

	static void MakeField();

};
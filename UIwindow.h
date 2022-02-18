#pragma once
#include "polygon2d.h"
#include "imageManager.h"
#include "animationPlayer.h"
#include <list>

#define STOCKIMAGE_SIZE (32)
#define STOCKIMAGE_OFFSETX (-30)
#define STOCKIMAGE_OFFSETY (-20)

class UIWindow : public Polygon2D {

protected:
	//ウインドウ構成要素
	Polygon2D* window = nullptr;
	std::list<TexString*> textList;
	std::list<Polygon2D*> imageList;

public:

	UIWindow(D3DXVECTOR2 s_pos, D3DXVECTOR2 s_size,int s_depth) {
		//Init();
		m_Position.x = s_pos.x;
		m_Position.y = s_pos.y;
		m_Position.z = 0;

		size = s_size;

		depth = s_depth;

		isUse = true;
	}
	//void Init() {};
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	void SetWindowTex(Polygon2D* s_window) { window = s_window;}
	Polygon2D* SetWindowTex(TEX_KIND s_tex);
	Polygon2D* SetWindowTex(TEX_KIND s_tex,bool dynamic);

	//void SetJTextTex(TexStringJapanese* jText) { JapaneseTexts.push_back(jText); }
	//void SetATextTex(TexStringAlphabet* aText) { AlphabetTexts.push_back(aText); }
	void SetTextTex(TexString* text) { textList.push_back(text); }
	TexString* SetTextTex(const char s_string[64],D3DXVECTOR2 s_pos,D3DXVECTOR2 s_size,D3DXVECTOR4 s_color);
	Polygon2D* SetPolygon2D(D3DXVECTOR2 s_pos, D3DXVECTOR2 s_size, unsigned short texId);
	Polygon2D* SetPolygon2D(D3DXVECTOR2 s_pos, D3DXVECTOR2 s_size, unsigned short texId,bool dummy);

};

class StatusWindow : public UIWindow {

	//ウインドウ構成要素
	//Polygon2D* window = nullptr;
	//std::list<TexStringJapanese*> JapaneseTexts;
	//std::list<TexStringAlphabet*> AlphabetTexts;
	//std::list<TexString*> textList;
	std::list<Polygon2D*> stockImageList;
	int stockNum;//ストックの数
	int stockTex;

public:

	StatusWindow(D3DXVECTOR2 s_pos, D3DXVECTOR2 s_size, int s_depth,CHARA_KIND s_ck,int s_stockNum): UIWindow(s_pos,s_size,s_depth)  {
		StockTexSet(s_ck);
		stockNum = s_stockNum;
		StockImageSet();
	}
	//void Init();
	//void Uninit();
	//void Update();
	void Draw();

	void StockImageSet();
	void StockTexSet(CHARA_KIND s_ck);
	void SetStockImageisUse(int stockNum, bool s_b);

};
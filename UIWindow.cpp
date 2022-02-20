
#include "UIwindow.h"
#include "imageManager.h"

void UIWindow::Uninit() {

	/*
	for (TexStringJapanese* object : JapaneseTexts) {

		object->Uninit();
		delete object;
	}
	JapaneseTexts.clear();//リストの削除
	
	for (TexStringAlphabet* object : AlphabetTexts) {

		object->Uninit();
		delete object;
	}
	AlphabetTexts.clear();//リストの削除
	*/

	for (TexString* object : textList) {

		object->Uninit();
		delete object;
	}
	textList.clear();//リストの削除

	//開放
	if(window != nullptr) window->Uninit();


}

void UIWindow::Update() {



}

void UIWindow::Draw() {

	if (!isUse) return;

	if (window) window->Draw();

	for (Polygon2D* object : imageList) {
		object->Draw();
	}

	for (TexString* object : textList) {
		object->Draw();
	}



}

Polygon2D* UIWindow::SetWindowTex(TEX_KIND s_tex){

	//大本ウインドウのパラメータを参照して場所を決める
	if (window != nullptr) {
		delete window;
	}
	window = new Polygon2D({ m_Position.x,m_Position.y }, {size.x,size.y},ImageManager::GetTexId(s_tex),depth);

	return window;
}

Polygon2D* UIWindow::SetWindowTex(TEX_KIND s_tex,bool dynamic) {

	//大本ウインドウのパラメータを参照して場所を決める
	if (window != nullptr) {
		delete window;
	}
	window = new Polygon2D({ m_Position.x,m_Position.y }, { size.x,size.y }, ImageManager::GetTexId(s_tex), depth,dynamic);

	return window;
}

TexString* UIWindow::SetTextTex(const char s_string[64], D3DXVECTOR2 s_pos, D3DXVECTOR2 s_size, D3DXVECTOR4 s_color) {

	s_pos.x += m_Position.x -  size.x / 2;
	s_pos.y += m_Position.y -  size.y / 2;

	TexString* text = new TexString(s_pos,s_size.x,depth,s_string,s_color);

	textList.push_back(text); 

	return text;

}

Polygon2D* UIWindow::SetPolygon2D(D3DXVECTOR2 s_pos, D3DXVECTOR2 s_size, unsigned short texId) {

	s_pos.x += m_Position.x - size.x / 2;
	s_pos.y += m_Position.y - size.y / 2;

	Polygon2D* image = new Polygon2D(s_pos, s_size,texId, 0);

	imageList.push_back(image);

	return image;

}

Polygon2D* UIWindow::SetPolygon2D(D3DXVECTOR2 s_pos, D3DXVECTOR2 s_size, unsigned short texId,bool dummy) {

	s_pos.x += m_Position.x - size.x / 2;
	s_pos.y += m_Position.y - size.y / 2;

	Polygon2D* image = new Polygon2D(s_pos, s_size, texId, 0,true);

	imageList.push_back(image);

	return image;

}

void StatusWindow::Draw() {

	if (!isUse) return;

	if (window) window->Draw();

	for (Polygon2D* object : stockImageList) {
		object->Draw();
	}

	for (TexString* object : textList) {
		object->Draw();
	}

}

void StatusWindow::StockImageSet() {

	//初期位置を決める
	D3DXVECTOR2 startPos;
	startPos.x = m_Position.x;
	startPos.y = m_Position.y;

	startPos.x += STOCKIMAGE_OFFSETX;
	startPos.y += STOCKIMAGE_OFFSETY;

	D3DXVECTOR2 setPos;

	//ストックの数分イメージを生成する
	for (int i = 0; i < stockNum;i++) {
		setPos = startPos;
		setPos.x += i * STOCKIMAGE_SIZE;
		Polygon2D* pol = new Polygon2D(setPos,STOCKIMAGE_SIZE,stockTex,depth);

		stockImageList.push_back(pol);
	}

}

void StatusWindow::StockTexSet(CHARA_KIND s_ck) {

	switch (s_ck)
	{
	case CK_TEST:
		stockTex = ImageManager::GetTexId(TEX_STOCKICON_BOW);
		break;
	case CK_PALADIN:
		stockTex = ImageManager::GetTexId(TEX_STOCKICON_SWORD);
		break;
	case CK_FIGTHER:
		stockTex = ImageManager::GetTexId(TEX_STOCKICON_FIGTHER);
		break;
	case CK_WIZARD:
		stockTex = ImageManager::GetTexId(TEX_STOCKICON_WAND);
		break;
	case CHARAKIND_MAX:
		break;
	default:
		break;
	}


}

void StatusWindow::SetStockImageisUse(int stockNum, bool s_b) {

	if (stockNum < 0) return;

	auto itr = stockImageList.begin();
	itr = std::next(itr, stockNum);
	Polygon2D* image = *itr;
	image->SetIsUse(s_b);

}
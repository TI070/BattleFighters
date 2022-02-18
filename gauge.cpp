
#include "gauge.h"
#include "billboard.h"
#include "texture.h"

/*
static int GaugeTexId;

void Gauge_Initialize(void) {

	GaugeTexId = Texture_SetTextureLoadFile("asset/white.png");

}


void Gauge_Draw(float posx, float posy, float size_x, float size_y, float gauge_max, float current_value) {


	//背面
	Sprite_SetColor(128,128,128,255);

	Sprite_Draw(GaugeTexId,posx,posy,size_x,size_y);


	//前面
	Sprite_SetColor(0, 255, 0, 255);

	Sprite_Draw(GaugeTexId, posx, posy, size_x * (current_value/gauge_max), size_y);


}

void Gauge_Draw(float posx, float posy,float gauge_max, float current_value) {


	//背面
	Sprite_SetColor(128, 128, 128, 255);

	Sprite_Draw(GaugeTexId, posx, posy, GAUGE_WIDTH, GAUGE_HEIGHT);


	//前面
	Sprite_SetColor(0, 255, 0, 255);

	Sprite_Draw(GaugeTexId, posx, posy, GAUGE_WIDTH * (current_value / gauge_max), GAUGE_HEIGHT);


}

void Gauge_Draw2(float posx, float posy,float gauge_max, float current_value, float charge_max, float charge_value) {


	//背面
	Sprite_SetColor(128, 128, 128, 255);

	Sprite_Draw(GaugeTexId, posx, posy, GAUGE_WIDTH, GAUGE_HEIGHT);


	//前面
	Sprite_SetColor(0, 255, 0, 255);

	Sprite_Draw(GaugeTexId, posx, posy, GAUGE_WIDTH * (current_value / gauge_max), GAUGE_HEIGHT);

	//チャージ
	Sprite_SetColor(255, 140, 0, 255);

	if (charge_value > charge_max) charge_value = charge_max;

	Sprite_Draw(GaugeTexId, posx, posy, GAUGE_WIDTH * (charge_value / charge_max), GAUGE_HEIGHT);

	Sprite_SetColorDefault();
}
*/

void Gauge::Init() {



}

void Gauge::BillBoardGaugeDraw(D3DXVECTOR3 s_pos, float size_x, float size_y, float gauge_max, float current_value) {

	int texId = ImageManager::GetTexId(TEX_PAINTBLOCK);

	//背面
	//Sprite_SetColor(128, 128, 128, 255);
	//Billboard::BillBoard_SetColor(bgColor);

	//Billboard::BillBorad_Draw(texId,s_pos,size_x,size_y,-0.1f);
	//Sprite_Draw(GaugeTexId, posx, posy, size_x, size_y);

	//前面
	//Sprite_SetColor(0, 255, 0, 255);
	Billboard::BillBoard_SetColor(mainColor);

	//Sprite_Draw(GaugeTexId, posx, posy, size_x * (current_value / gauge_max), size_y);
	s_pos.x -=(size_x - (current_value / gauge_max)) / 2;

	Billboard::BillBorad_Draw(texId, s_pos, size_x  * (current_value / gauge_max), size_y,-0.1f);


}

void Gauge::UIGaugeDraw() {

	int texId = ImageManager::GetTexId(TEX_PAINTBLOCK);

	//背面
	//Sprite_SetColor(128, 128, 128, 255);
	//Billboard::BillBoard_SetColor(255, 0, 0, 255);

	//Billboard::BillBorad_Draw(texId, s_pos, size_x, size_y, -0.1f);
	//Sprite_Draw(GaugeTexId, posx, posy, size_x, size_y);

	//Polygon2D::Draw();
	bgGauge->SetColor(bgColor);
	bgGauge->UpdateVertex(pos, { sizeX,sizeY });
	bgGauge->Draw();

	//前面
	//Sprite_SetColor(0, 255, 0, 255);
	//Billboard::BillBoard_SetColor(0, 255, 0, 255);

	//Sprite_Draw(GaugeTexId, posx, posy, size_x * (current_value / gauge_max), size_y);
	
	//値の調整
	D3DXVECTOR2 s_pos;
	D3DXVECTOR2 s_size;
	s_pos = pos;
	s_size.y = sizeY;
	s_size.x = sizeX * (currentValue / gaugeMax);
	s_pos.x -= (sizeX - sizeX * (currentValue / gaugeMax)) / 2;
	//size_x  * (current_value / gauge_max)
	//Billboard::BillBorad_Draw(texId, s_pos, size_x  * (current_value / gauge_max), size_y, -0.1f);
	
	//頂点データの更新
	mainGauge->SetColor(mainColor);
	mainGauge->UpdateVertex(s_pos, s_size);

	mainGauge->Draw();

}
void Gauge::Draw() {

	UIGaugeDraw();

}
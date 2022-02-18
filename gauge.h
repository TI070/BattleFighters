#pragma once
#ifndef GAUGE_H
#define GAUGE_H

#define GAUGE_HEIGHT (20)
#define GAUGE_WIDTH (100)

#include "main.h"
#include "polygon2d.h"
#include "imageManager.h"

/*
void Gauge_Initialize(void);
void Gauge_Draw(float posx, float posy, float size_x, float size_y, float gauge_max, float current_value);
void Gauge_Draw(float posx, float posy, float gauge_max, float current_value);
void Gauge_Draw2(float posx, float posy, float gauge_max, float current_value,float chargemax,float charge_value);
*/

class Gauge : public Polygon2D{

private:

	D3DXVECTOR2 pos;
	float sizeX;
	float sizeY;
	float gaugeMax;
	float currentValue;

	Polygon2D* bgGauge;
	Polygon2D* mainGauge;

	D3DXVECTOR4 bgColor = { 1.0f,0.0f,0.0f,1.0f };
	D3DXVECTOR4 mainColor = { 0.0f,1.0f,0.0f,1.0f };

public:

	Gauge() {};
	Gauge(D3DXVECTOR2 s_pos, float size_x, float size_y, float gauge_max, float current_value) {
		
		pos = s_pos;
		sizeX = size_x;
		sizeY = size_y;
		gaugeMax = gauge_max;
		currentValue = current_value;

		depth = 0;

		bgGauge = new Polygon2D(s_pos, {sizeX,sizeY},ImageManager::GetTexId(TEX_WHITE),0,true);
		mainGauge = new Polygon2D(s_pos, { sizeX,sizeY }, ImageManager::GetTexId(TEX_WHITE), 0, true);
	}
	void Init();
	void BillBoardGaugeDraw(D3DXVECTOR3 s_pos, float size_x, float size_y, float gauge_max, float current_value);
	void UIGaugeDraw();
	void Draw();

	Polygon2D* GetBgGauge() { return bgGauge; }
	Polygon2D* GetMainGauge() { return mainGauge; }

	void SetCurrentValue(float s_value) { currentValue = s_value; }


	void SetBgColor(D3DXVECTOR4 s_bgc) { bgColor = s_bgc; }
	void SetMainColor(D3DXVECTOR4 s_mc) { mainColor = s_mc; }

};

#endif
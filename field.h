#pragma once
#include "main.h"
#include "gameObject.h"
#include "fieldConfig.h"
#include "imageManager.h"

enum ROAD_TYPE {

	NOT_CONNECTION,//非接続
	HORIZON,//左+右
	VERTICAL,//奥+手前
	HORIZON_FRONT,//横+奥
	VERTICAL_RIGHT,//縦+右
	HORIZON_BACK,//横+手前

	VERTICAL_LEFT,//縦+左
	CROSS,//十字
	RT_BACK,
	RT_LEFT,
	RT_FRONT,
	RT_RIGHT,
	BACK_RIGHT,
	BACK_LEFT,
	FRONT_LEFT,
	FRONT_RIGHT

};

class Player;

class Field : public GameObject {

public:
	Field() {};

	Field(D3DXVECTOR3 s_pos, float s_size, const char* textureName) {
		Init(s_pos, s_size, textureName);
	}
	virtual void Init(D3DXVECTOR3 s_pos, float s_size, const char* textureName);
	virtual void Init(D3DXVECTOR3 s_pos, float s_size, unsigned short texId);
	virtual void Init() {};//仮置き
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	//引数とポジションが同じか確認し、同じならtrueを返す
	bool CheckPos(D3DXVECTOR3 s_pos);

	virtual void ChangeTex() {};
	virtual void CheckFieldType() {};
	virtual void CheckAroundRoad() {};

};

class SelecrtTile : public Field {

private:
	Player* pP;
	float size;

public:

	SelecrtTile() {};

	SelecrtTile(Player* pPlayer, float s_size, const char* textureName) {
		Init(pPlayer, s_size, textureName);
	}
	void Init(Player* pPlayer, float s_size, const char* textureName);
	//void Uninit();
	void Update();
	//void Draw();




};

class Grass : public Field {

private:


public:

	Grass() {};

	Grass(D3DXVECTOR3 s_pos, float s_size, const char* textureName) {
		textureName = FIELDTILE_GRASS_PASS;
		//Init(s_pos, s_size, ImageManager::GetTexId(TEX_FIELD_GRASS));

	}
	void Init(D3DXVECTOR3 s_pos, float s_size, unsigned short texId);

	//void Update();

};

class Road : public Field {

private:
	
	ROAD_TYPE roadType;

public:

	Road() {};

	Road(D3DXVECTOR3 s_pos, float s_size, const char* textureName) {
		textureName = FIELDTILE_ROAD_0_PASS;
		Init(s_pos,s_size, textureName);
		//FieldManager::SetFieldData(m_Position, ROAD);
		CheckAroundRoad();//タイプ取得
	}
	void Init(D3DXVECTOR3 s_pos, float s_size, const char* textureName);

	void Update();
	void ChangeTex();
	void CheckAroundRoad();
	void CheckFieldType();
};





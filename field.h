#pragma once
#include "main.h"
#include "gameObject.h"
#include "fieldConfig.h"
#include "imageManager.h"

enum ROAD_TYPE {

	NOT_CONNECTION,//��ڑ�
	HORIZON,//��+�E
	VERTICAL,//��+��O
	HORIZON_FRONT,//��+��
	VERTICAL_RIGHT,//�c+�E
	HORIZON_BACK,//��+��O

	VERTICAL_LEFT,//�c+��
	CROSS,//�\��
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
	virtual void Init() {};//���u��
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	//�����ƃ|�W�V�������������m�F���A�����Ȃ�true��Ԃ�
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
		CheckAroundRoad();//�^�C�v�擾
	}
	void Init(D3DXVECTOR3 s_pos, float s_size, const char* textureName);

	void Update();
	void ChangeTex();
	void CheckAroundRoad();
	void CheckFieldType();
};





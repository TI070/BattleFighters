#pragma once
#include "main.h"
#include "gameObject.h"
#include "effect.h"

#define EXPLOSION_LIFE (32)
#define EXPLOSION_SIZE_X (1024)
#define EXPLOSION_SIZE_Y (1024)

#define EXPLOSION_NUM_X (4)
#define EXPLOSION_NUM_Y (4)

#define UNITDEATH_LIFE (32)
#define UNITDEATH_NUM_X (3)
#define UNITDEATH_NUM_Y (4)

#define BURST_LIFE (60)


class Explosion : public Effect {

private:


protected:

	int eLife;
	int eSizeX;
	int eSizeY;

	int eNumX;
	int eNumY;


public:
	Explosion() {};
	Explosion(D3DXVECTOR3 s_pos, float s_size,unsigned short texId) {
		Init(s_pos, s_size, texId);

		eLife = EXPLOSION_LIFE;
		eNumX = EXPLOSION_NUM_X;
		eNumY = EXPLOSION_NUM_Y;
		//eSizeX = EXPLOSION_NUM_X;

		tex1_scale = eLife / (eNumX * eNumY);

		tcw = 1 / (float)eNumX;
		tch = 1 / (float)eNumY;

	}
	void Init(D3DXVECTOR3 s_pos, float s_size, unsigned short texId);
	void Init(D3DXVECTOR3 s_pos, D3DXVECTOR2 s_size, unsigned short texId);
	void Init() {};//仮置き
	void Uninit();
	void Update();
	void Draw();

	//テスト中
	void InstancingDraw();


};



class Ex_UnitDeath : public Explosion {

private:
	Ex_UnitDeath();

public:

	Ex_UnitDeath(D3DXVECTOR3 s_pos, float s_size, unsigned short texId) {
		Init(s_pos, s_size, texId);

		eLife = EXPLOSION_LIFE;
		eNumX = EXPLOSION_NUM_X;
		eNumY = EXPLOSION_NUM_Y;
		//eSizeX = EXPLOSION_NUM_X;

		tex1_scale = eLife / (eNumX * eNumY);

		tcw = 1 / (float)eNumX;
		tch = 1 / (float)eNumY;

	}

};

class AnimationEffect : public Explosion {

private:
	AnimationEffect();

public:

	AnimationEffect(D3DXVECTOR3 s_pos, float s_size, unsigned short texId,int s_life,D3DXVECTOR2 eNum) {
		Init(s_pos, s_size, texId);

		eLife = s_life;
		eNumX = eNum.x;
		eNumY = eNum.y;
		//eSizeX = EXPLOSION_NUM_X;

		tex1_scale = eLife / (eNumX * eNumY);

		tcw = 1 / (float)eNumX;
		tch = 1 / (float)eNumY;

	}

};

class BurstEffect : public Explosion {

private:
	BurstEffect();

public:

	BurstEffect(D3DXVECTOR3 s_pos, float s_size, unsigned short texId, int s_life) {
		
		D3DXVECTOR2 sSize = {15,3};

		Init(s_pos, sSize, texId);

		eLife = s_life;
		eNumX = 1;
		eNumY = 1;
		//eSizeX = EXPLOSION_NUM_X;

		tex1_scale = eLife / (eNumX * eNumY);

		tcw = 1 / (float)eNumX;
		tch = 1 / (float)eNumY;

	}

};



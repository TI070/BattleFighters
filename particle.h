#pragma once

#include "effect.h"
#include "main.h"
#include "texture.h"
#include "renderer.h"
#include <list>

#define PARTICLE_SPEEDCORRECT (1/60.0f)


class Particle : public Effect {


private:
	D3DXVECTOR4 C_ColorValue;//1フレームで変化する色の値
	float C_SizeValue;//1フレームで変化する色の値

public:

	//int maxLife;

	//パーティクルの速度
	D3DXVECTOR3 velocity;

	Particle(D3DXVECTOR3 s_pos, float s_size, int s_Life, D3DXVECTOR3 s_velocity, ID3D11ShaderResourceView* s_tex, D3DXVECTOR4 s_color, D3DXVECTOR4 s_lastColor) {

		////パーティクルに必要なものセット

		//生成位置
		m_Position = s_pos;
		//パーティクル個別の生存時間
		life = s_Life;
		//パーティクルの速度
		velocity = s_velocity;

		//サイズ
		size = s_size;

		//テクスチャ読込
		m_Texture = s_tex;

		color = s_color;
		lastColor = s_lastColor;

		Init();
		//必要な初期値
		//life = 0;

	}

	void Init(D3DXVECTOR3 s_pos, float s_size, unsigned short texId) {};
	void Init(D3DXVECTOR3 s_pos, D3DXVECTOR2 s_size, unsigned short texId) {};

	void Init();
	void Uninit();
	void Update();
	void Draw();

	//線形に色を変化させる
	void ChangeColorLinear();
	//線形に大きさを変化させる
	void ChangeSizeLinear();

	void SetC_SizeValue(){
		if (sizeChange) {
			C_SizeValue = size - lastSize.x;

			C_SizeValue /= life;
		}
	}

};

class ParticleSystem : public Effect {

protected:


	std::list<Particle*> particleList;

	//全体の生存時間
	//int particleSystemLifeMax;

	//パーティクル個別の生存時間
	int particleLife;

	//パーティクルの速度
	D3DXVECTOR3 velocityMax;//最大速度
	D3DXVECTOR3 velocityMin;//最小速度

	//パーティクル生成のタイミング(何フレームに１回生成するか)
	int spawnInterval;

	//パーティクル生成時に何個同時に生成するか
	int spawnNum;



public:
	ParticleSystem() {};

	ParticleSystem(D3DXVECTOR3 s_pos, float s_size,int s_systemLife ,int s_Life,D3DXVECTOR3 s_velocity,int s_interval,int s_spawnNum, unsigned short texId) {
		//Init(s_pos, s_size, texId);

		////パーティクルに必要なものセット

		//生成位置
		m_Position = s_pos;
		//全体の生存時間
		life = s_systemLife;
		//パーティクル個別の生存時間
		particleLife = s_Life;
		//パーティクルの速度
		velocityMax = s_velocity;

		//パーティクル生成のタイミング
		spawnInterval = s_interval;
		//パーティクル生成時に何個同時に生成するか
		spawnNum = s_spawnNum;

		//サイズ
		size = s_size;

		//テクスチャ読込
		m_Texture = Texture_GetTexture(texId);

		Init();

		//必要な初期値
		//life = 0;
	}

	void Init(D3DXVECTOR3 s_pos, float s_size, unsigned short texId) {};
	void Init(D3DXVECTOR3 s_pos, D3DXVECTOR2 s_size, unsigned short texId) {};

	void Init();
	void Uninit();
	void Update();
	void Draw();
	virtual void InstancingDraw();

	void SpawnParticle();

	void ParticleListRemove();

	//各種パラメータのセット
	void SetLife(int s_life) { life = s_life; }
	void SetParticleLife(int s_life) { particleLife = s_life; }
	void SetVelocityMin(D3DXVECTOR3 s_vel) { velocityMin = s_vel; }
	void SetVelocityMax(D3DXVECTOR3 s_vel) { velocityMax = s_vel; }
	void SetInterval(int s_itv) { spawnInterval = s_itv; }
	void SetSpawnNum(int s_num) { spawnNum = s_num; }
	void SetSize(float s_size) { size = s_size; }
	void SetTexture(unsigned short texId) { m_Texture = Texture_GetTexture(texId); }


};


class ParticleSystem2D : public ParticleSystem {

private:
	void InstancingDraw();
};
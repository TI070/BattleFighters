#pragma once

#include "effect.h"
#include "main.h"
#include "texture.h"
#include "renderer.h"
#include <list>

#define PARTICLE_SPEEDCORRECT (1/60.0f)


class Particle : public Effect {


private:
	D3DXVECTOR4 C_ColorValue;//1�t���[���ŕω�����F�̒l
	float C_SizeValue;//1�t���[���ŕω�����F�̒l

public:

	//int maxLife;

	//�p�[�e�B�N���̑��x
	D3DXVECTOR3 velocity;

	Particle(D3DXVECTOR3 s_pos, float s_size, int s_Life, D3DXVECTOR3 s_velocity, ID3D11ShaderResourceView* s_tex, D3DXVECTOR4 s_color, D3DXVECTOR4 s_lastColor) {

		////�p�[�e�B�N���ɕK�v�Ȃ��̃Z�b�g

		//�����ʒu
		m_Position = s_pos;
		//�p�[�e�B�N���ʂ̐�������
		life = s_Life;
		//�p�[�e�B�N���̑��x
		velocity = s_velocity;

		//�T�C�Y
		size = s_size;

		//�e�N�X�`���Ǎ�
		m_Texture = s_tex;

		color = s_color;
		lastColor = s_lastColor;

		Init();
		//�K�v�ȏ����l
		//life = 0;

	}

	void Init(D3DXVECTOR3 s_pos, float s_size, unsigned short texId) {};
	void Init(D3DXVECTOR3 s_pos, D3DXVECTOR2 s_size, unsigned short texId) {};

	void Init();
	void Uninit();
	void Update();
	void Draw();

	//���`�ɐF��ω�������
	void ChangeColorLinear();
	//���`�ɑ傫����ω�������
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

	//�S�̂̐�������
	//int particleSystemLifeMax;

	//�p�[�e�B�N���ʂ̐�������
	int particleLife;

	//�p�[�e�B�N���̑��x
	D3DXVECTOR3 velocityMax;//�ő呬�x
	D3DXVECTOR3 velocityMin;//�ŏ����x

	//�p�[�e�B�N�������̃^�C�~���O(���t���[���ɂP�񐶐����邩)
	int spawnInterval;

	//�p�[�e�B�N���������ɉ������ɐ������邩
	int spawnNum;



public:
	ParticleSystem() {};

	ParticleSystem(D3DXVECTOR3 s_pos, float s_size,int s_systemLife ,int s_Life,D3DXVECTOR3 s_velocity,int s_interval,int s_spawnNum, unsigned short texId) {
		//Init(s_pos, s_size, texId);

		////�p�[�e�B�N���ɕK�v�Ȃ��̃Z�b�g

		//�����ʒu
		m_Position = s_pos;
		//�S�̂̐�������
		life = s_systemLife;
		//�p�[�e�B�N���ʂ̐�������
		particleLife = s_Life;
		//�p�[�e�B�N���̑��x
		velocityMax = s_velocity;

		//�p�[�e�B�N�������̃^�C�~���O
		spawnInterval = s_interval;
		//�p�[�e�B�N���������ɉ������ɐ������邩
		spawnNum = s_spawnNum;

		//�T�C�Y
		size = s_size;

		//�e�N�X�`���Ǎ�
		m_Texture = Texture_GetTexture(texId);

		Init();

		//�K�v�ȏ����l
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

	//�e��p�����[�^�̃Z�b�g
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
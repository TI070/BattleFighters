#pragma once

#include <list>
#include "main.h"
#include "config.h"
#include "managerBase.h"

class Effect;
class ParticleSystem;
class ParticleSystem2D;

class EffectManager : public ManagerBase
{

private:

	static std::list<Effect*> pList;
	//static Field* pField;

public:

	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static void SetExplosion(D3DXVECTOR3 s_pos,float s_size);
	static void SetEx_UnitDeath(D3DXVECTOR3 s_pos, float s_size);
	static Effect* SetAnimationEffect(D3DXVECTOR3 s_pos, float s_size, unsigned short texId, int s_life, D3DXVECTOR2 eNum);
	static ParticleSystem* SetParticle(D3DXVECTOR3 s_pos, float s_size, int s_systemLife, int s_Life, D3DXVECTOR3 s_velocity, int s_interval, int s_spawnNum, unsigned short texId);
	static ParticleSystem* SetParticle();
	static ParticleSystem2D* SetParticle2D();
	static Effect* SetBurstEffect(D3DXVECTOR3 s_pos, float s_size);
	static std::list<Effect*> GetEffectList() { return pList; }

};
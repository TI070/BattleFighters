
#include "effectManager.h"
#include "imageManager.h"
#include "explosion.h"
#include "particle.h"

std::list<Effect*> EffectManager::pList;

void EffectManager::Init() {


}

void EffectManager::Uninit() {

	for (Effect* object : pList) {

		object->Uninit();
		delete object;
	}
	pList.clear();//ƒŠƒXƒg‚Ìíœ

}

void EffectManager::Update() {

	for (Effect* object : pList) {

		object->Update();

	}

	RemoveMember<Effect>(&pList);
}

void EffectManager::Draw() {

	for (Effect* object : pList) {

		object->Draw();

	}
}

void EffectManager::SetExplosion(D3DXVECTOR3 s_pos,float s_size) {

	Effect* pEffect = new Explosion(s_pos, s_size, ImageManager::GetTexId(TEX_EXPLOSION));
	pList.push_back(pEffect);

}

void EffectManager::SetEx_UnitDeath(D3DXVECTOR3 s_pos, float s_size) {

	Effect* pEffect = new Ex_UnitDeath(s_pos, s_size, ImageManager::GetTexId(TEX_EX_UNITDEATH));
	pList.push_back(pEffect);

}

Effect* EffectManager::SetAnimationEffect(D3DXVECTOR3 s_pos, float s_size, unsigned short texId, int s_life, D3DXVECTOR2 eNum) {

	Effect* pEffect = new AnimationEffect(s_pos, s_size,texId,s_life,eNum);
	pList.push_back(pEffect);

	return pEffect;

}


ParticleSystem* EffectManager::SetParticle(D3DXVECTOR3 s_pos, float s_size, int s_systemLife, int s_Life, D3DXVECTOR3 s_velocity, int s_interval, int s_spawnNum, unsigned short texId) {

	ParticleSystem* pEffect = new ParticleSystem(s_pos, s_size, s_systemLife,s_Life,s_velocity,s_interval,s_spawnNum,texId);
	pList.push_back(pEffect);

	return pEffect;
}

ParticleSystem* EffectManager::SetParticle() {

	ParticleSystem* pEffect = new ParticleSystem;
	pList.push_back(pEffect);

	return pEffect;
}

ParticleSystem2D* EffectManager::SetParticle2D() {

	ParticleSystem2D* pEffect = new ParticleSystem2D;
	pList.push_back(pEffect);

	return pEffect;
}

Effect* EffectManager::SetBurstEffect(D3DXVECTOR3 s_pos, float s_size) {

	Effect* pEffect = new BurstEffect(s_pos, s_size, ImageManager::GetTexId(TEX_BURST),BURST_LIFE);
	pList.push_back(pEffect);

	return pEffect;
}
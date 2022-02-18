#pragma once

#include "gameObject.h"

#define BLOWEFFECT_LIFE (100)
#define BLOWEFFECT_ENUM {5,2}




class Audio;
class Player;
class AnimationPlayer;
class DamageObject;


class CollisionSystem {

private:

	static void CollisionDamageObjectandCharacter();
	static void CollisionMeshFieldBoxandCharacter();
	static void CollisionDamageObjectandDamageObject();
	//static Audio* AttackSE;

public:

	static Player* pP;
	static void SetPlayer(Player* sP) { pP = sP; }
	static bool CheckObbCollision_DamageObjectandCharacter(GameObject* pEnemy, GameObject* pdObj);

	static bool CheckObbCollision(GameObject* pBox, GameObject* pPoint);

	static bool CheckObbObbCollision(GameObject* pBoxA, GameObject* pBoxB);

	static void Initilize();

	//ÉQÅ[ÉÄÇÃçXêV
	static void Update();

};


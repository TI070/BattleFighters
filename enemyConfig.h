#pragma once
#define ENEMY_MOVESPEED_DEFAULT (0.05f)
#define ENEMY_ROLLSPEED_DEFAULT (1.0f)
#define ENEMY_LIFE_DEFAULT (3)
#define ENEMY_DEFAULT_SIZE (1.0f) 
#define ENEMY_TEST_PASS "asset\\model\\torus.obj" 

#define ENEMY_SERACHRANGE (2) 

enum ENEMY_TYPE {
	ETYPE_ENEMY,
	ETYPE_TREE,
	ETYPE_SLIME,
	ETYPE_WING,
	ETYPE_MAGICMAN,
	ETYPE_SLIMEKING,
	ETYPE_MH

};


//��

#define TREE_NAME "��"

#define TREE_MOVESPEED_DEFAULT (0.00f)
#define TREE_ROLLSPEED_DEFAULT (0.0f)
#define TREE_LIFE_DEFAULT (3)
#define TREE_DEFAULT_SIZE (1.0f) 

#define TREE_ATK (0)
#define TREE_DEF (0) 
#define TREE_GOLD (2)
#define TREE_EXP (1) 

#define TREE_PASS "asset\\model\\tree.obj" 

//�X���C��

#define SLIME_NAME "�X���C��"

#define SLIME_MOVESPEED_DEFAULT (0.01f)
#define SLIME_ROLLSPEED_DEFAULT (0.0f)
#define SLIME_LIFE_DEFAULT (3)
#define SLIME_DEFAULT_SIZE (1.0f) 

#define SLIME_ATK (1)
#define SLIME_DEF (0) 
#define SLIME_GOLD (1)
#define SLIME_EXP (2) 

#define SLIME_PASS "asset\\model\\slime.obj" 

//�E�C���O

#define WING_NAME "�E�C���O"

#define WING_MOVESPEED_DEFAULT (0.03f)
#define WING_ROLLSPEED_DEFAULT (0.0f)
#define WING_LIFE_DEFAULT (8)
#define WING_DEFAULT_SIZE (1.0f) 

#define WING_ATK (2)
#define WING_DEF (0) 
#define WING_GOLD (1)
#define WING_EXP (4) 

#define WING_PASS "asset\\model\\wing.obj" 

//�}�W�b�N�}��

#define MAGICMAN_NAME "�}�W�b�N�}��"

#define MAGICMAN_MOVESPEED_DEFAULT (0.01f)
#define MAGICMAN_ROLLSPEED_DEFAULT (0.0f)
#define MAGICMAN_LIFE_DEFAULT (10)
#define MAGICMAN_DEFAULT_SIZE (1.0f) 

#define MAGICMAN_ATK (3)
#define MAGICMAN_DEF (0) 
#define MAGICMAN_GOLD (5)
#define MAGICMAN_EXP (10) 

#define MAGICMAN_PASS "asset\\model\\magicman.obj" 

//�X���C���L���O

#define SLIMEKING_NAME "�X���C���L���O"

#define SLIMEKING_MOVESPEED_DEFAULT (0.01f)
#define SLIMEKING_ROLLSPEED_DEFAULT (0.0f)
#define SLIMEKING_LIFE_DEFAULT (200)
#define SLIMEKING_DEFAULT_SIZE (1.0f) 

#define SLIMEKING_ATK (5)
#define SLIMEKING_DEF (0) 
#define SLIMEKING_GOLD (100)
#define SLIMEKING_EXP (200) 

#define SLIMEKING_PASS "asset\\model\\slimeking.obj" 

//�����̑�

#define MH_NAME "�����̑�"

#define MH_MOVESPEED_DEFAULT (0.00f)
#define MH_ROLLSPEED_DEFAULT (0.0f)
#define MH_LIFE_DEFAULT (100)
#define MH_DEFAULT_SIZE (1.0f) 

#define MH_ATK (0)
#define MH_DEF (0) 
#define MH_GOLD (50)
#define MH_EXP (50) 

#define MH_ATKCOOLTIME (600) 

#define MH_PASS "asset\\model\\monsterhouse.obj" 
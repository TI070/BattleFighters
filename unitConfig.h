#pragma once

enum UNIT_MOVEMODE {

	MOVE_NONE,//移動しない
	MOVE_TO_ROAD,//道なりに移動
	MOVE_TO_ENEMY,//敵の方向へ移動
	ESCAPE//敵から逃亡

};


enum UNIT_TYPE {
	UTYPE_NULL,
	UTYPE_UNIT,
	UTYPE_BUILDING,
	UTYPE_BUILDING_EX//外見用
};

enum UNIT_TYPE_DETAIL {
	UTD_NULL,
	UTD_UNIT,
	UTD_SOLDIER,
	UTD_WIZARD,
	UTD_PRIEST,
	UTD_BUILDING,
	UTD_INN,
	UTD_GATE,
	UTD_EDGETREE,
	UTD_WP_SHOP,
	UTD_CHURCH,
	UTD_STATUE
};


//行動範囲(マス)
#define ACTION_RANGE (20)

#define SELECTUNIT_RANGE (1.5f)

#define UNIT_DEFAULT_SIZE (1.0f) 
#define UNIT_TEST_PASS "asset\\model\\unit.obj" 

#define BUILDING_TEST_PASS "asset\\model\\building.obj" 
#define BUILDING2_TEST_PASS "asset\\model\\building2.obj" 
#define BUILDING3_TEST_PASS "asset\\model\\building3.obj" 
#define BUILDING4_TEST_PASS "asset\\model\\building4.obj" 


//障害物
#define BUILDING_EDGETREE_PASS "asset\\model\\edge_tree.obj" 

//像
#define BUILDING_STATUE_PASS "asset\\model\\statue.obj" 


//宿
#define BUILDING_INN_PASS "asset\\model\\inn.obj" 

//武器屋
#define BUILDING_WPSHOP_PASS "asset\\model\\weaponshop.obj" 

//教会
#define BUILDING_CHURCH_PASS "asset\\model\\church.obj" 

#define BUILDING_ATK (0)
#define BUILDING_DEF (0) 

#define B_INN_GOLD (0)
#define B_INN_EXP (5)

#define B_CHURCH_GOLD (0)
#define B_CHURCH_EXP (5)

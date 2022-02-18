#pragma once

enum UNIT_MOVEMODE {

	MOVE_NONE,//à⁄ìÆÇµÇ»Ç¢
	MOVE_TO_ROAD,//ìπÇ»ÇËÇ…à⁄ìÆ
	MOVE_TO_ENEMY,//ìGÇÃï˚å¸Ç÷à⁄ìÆ
	ESCAPE//ìGÇ©ÇÁì¶ñS

};


enum UNIT_TYPE {
	UTYPE_NULL,
	UTYPE_UNIT,
	UTYPE_BUILDING,
	UTYPE_BUILDING_EX//äOå©óp
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


//çsìÆîÕàÕ(É}ÉX)
#define ACTION_RANGE (20)

#define SELECTUNIT_RANGE (1.5f)

#define UNIT_DEFAULT_SIZE (1.0f) 
#define UNIT_TEST_PASS "asset\\model\\unit.obj" 

#define BUILDING_TEST_PASS "asset\\model\\building.obj" 
#define BUILDING2_TEST_PASS "asset\\model\\building2.obj" 
#define BUILDING3_TEST_PASS "asset\\model\\building3.obj" 
#define BUILDING4_TEST_PASS "asset\\model\\building4.obj" 


//è·äQï®
#define BUILDING_EDGETREE_PASS "asset\\model\\edge_tree.obj" 

//ëú
#define BUILDING_STATUE_PASS "asset\\model\\statue.obj" 


//èh
#define BUILDING_INN_PASS "asset\\model\\inn.obj" 

//ïêäÌâÆ
#define BUILDING_WPSHOP_PASS "asset\\model\\weaponshop.obj" 

//ã≥âÔ
#define BUILDING_CHURCH_PASS "asset\\model\\church.obj" 

#define BUILDING_ATK (0)
#define BUILDING_DEF (0) 

#define B_INN_GOLD (0)
#define B_INN_EXP (5)

#define B_CHURCH_GOLD (0)
#define B_CHURCH_EXP (5)

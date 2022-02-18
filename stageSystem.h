#pragma once
#include "main.h"


struct Area {

	float rightMax;
	float leftMax;
	float upMax;
	float downMax;

	D3DXVECTOR2 GetCenter(){

		D3DXVECTOR2 center;

		center.x = (rightMax + leftMax) / 2;
		center.y = (upMax + downMax) / 2;

		return center;

	}

	D3DXVECTOR2 GetRange() {

		D3DXVECTOR2 range;

		range.x = fabs(rightMax - leftMax);
		range.y = fabs(upMax - downMax);

		return range;

	}

	bool CheckInArea(D3DXVECTOR2 s_pos) {

		if (s_pos.x > rightMax) return false;
		else if (s_pos.x < leftMax) return false;
		else if (s_pos.y > upMax) return false;
		else if (s_pos.y < downMax) return false;

		return true;
	}
	bool CheckInArea2D(D3DXVECTOR2 s_pos) {

		if (s_pos.x > rightMax) return false;
		else if (s_pos.x < leftMax) return false;
		else if (s_pos.y < upMax) return false;
		else if (s_pos.y > downMax) return false;

		return true;
	}

};

enum STAGEKIND {

	SK_BATTLEFIELD,
	SK_LASTSTOP


};

class BackGround;

class StageSystem {

private:
	//中央エリアのデータを保存
	static Area centerArea;

	//生存エリアのデータを保存
	static Area inStageArea;

	//足場のデータを保存
	static Area groundArea;

	//カメラ最狭データを保存
	static Area cameraZoomLimitArea;


	static BackGround* skyDome;
	static BackGround* skyDome_bottom;

public:

	static void SetStage(STAGEKIND s_sk);

	static Area GetCenterArea() { return centerArea; }
	static Area GetInStageArea() { return inStageArea; }
	static Area GetGroundArea() { return groundArea; }

	static void SetCenterArea(Area s_area) { centerArea = s_area; }
	static void SetInStageArea(Area s_area) { inStageArea = s_area; }
	static void SetGroundArea(Area s_area) { groundArea = s_area; }
	static void SetCameraZoomLimitArea(Area s_area) { cameraZoomLimitArea = s_area; }

	static bool CheckInArea(Area s_area,D3DXVECTOR3 s_pos);

	static void Draw();

	static void ReadyBattle();

};
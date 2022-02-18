
#include "meshFieldBoxManager.h"

std::list<MeshFieldBox*> MeshFieldBoxManager::meshBoxList;

void MeshFieldBoxManager::Draw() {

	for (MeshFieldBox* object : meshBoxList) {

		object->Draw();

	}

}

void MeshFieldBoxManager::Update() {

	for (MeshFieldBox* object : meshBoxList) {

		object->Update();

	}

}

MeshFieldBox* MeshFieldBoxManager::SetMeshFieldBox(D3DXVECTOR3 s_pivot, float s_pieceSize, float s_uvSize, float s_height, float s_zWidth) {

	MeshFieldBox* mb = new MeshFieldBox;
	mb->Init(s_pivot,s_pieceSize,s_uvSize,s_height,s_zWidth);
	meshBoxList.push_back(mb);
	return mb;
}


float MeshFieldBoxManager::GetHeightUp(D3DXVECTOR3 s_pos) {

	//引数の位置のxzから判定対象となるmeshboxを探し、
	//その中から着地判定の対象となる高さを引数のyから判定する。

	//std::list<float> heights;
	float max = LAND_DOWNLIMIT;
	for (MeshFieldBox* object : meshBoxList) {
		float dummy = object->GetHeightUp(s_pos);
		//heights.push_back(object->GetHeightUp(s_pos));
		if (max < dummy) max = dummy;

	}

	//char buf[64];
	//sprintf(buf, "max = %f\n", max);
	//OutputDebugString(buf);
	//sprintf(buf, "x = %f y = %f z = %f\n", s_pos.x, s_pos.y, s_pos.z);
	//OutputDebugString(buf);

	return max;


}

bool MeshFieldBoxManager::CheckBlockThrough(D3DXVECTOR3 s_pos) {

	//引数から判定対象となるブロックを探す
	for (MeshFieldBox* object : meshBoxList) {

		//s_posx,zがmeshfieldboxの範囲に入っているか確認
		if (object->GetRightMax() > s_pos.x && object->GetLeftMax() < s_pos.x && 
			object->GetFrontMax() < s_pos.z && object->GetBackMax() > s_pos.z)
		{
			float objY = object->GetPivotY();

			//高さのチェック
			if (!(objY - 0.3f < s_pos.y && objY + 0.3f > s_pos.y)) continue;

			//対象が見つかったので判定する
			return object->GetThroughFlag();

		}

	}

	//見つからなかったのでfalseを返す
	return false;

}

bool MeshFieldBoxManager::CheckBlockCliff(D3DXVECTOR3 s_pos, AnimationPlayer* s_p) {

	//引数から判定対象となるブロックを探す
	for (MeshFieldBox* object : meshBoxList) {

		if (object->GetCliff(s_pos,s_p)) {



			return true;
		}
	}

	//見つからなかったのでfalseを返す
	return false;

}

void MeshFieldBoxManager::Uninit() {

	for (MeshFieldBox* object : meshBoxList) {

		object->Uninit();
		delete object;
	}
	meshBoxList.clear();//リストの削除

}
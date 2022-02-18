
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

	//�����̈ʒu��xz���画��ΏۂƂȂ�meshbox��T���A
	//���̒����璅�n����̑ΏۂƂȂ鍂����������y���画�肷��B

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

	//�������画��ΏۂƂȂ�u���b�N��T��
	for (MeshFieldBox* object : meshBoxList) {

		//s_posx,z��meshfieldbox�͈̔͂ɓ����Ă��邩�m�F
		if (object->GetRightMax() > s_pos.x && object->GetLeftMax() < s_pos.x && 
			object->GetFrontMax() < s_pos.z && object->GetBackMax() > s_pos.z)
		{
			float objY = object->GetPivotY();

			//�����̃`�F�b�N
			if (!(objY - 0.3f < s_pos.y && objY + 0.3f > s_pos.y)) continue;

			//�Ώۂ����������̂Ŕ��肷��
			return object->GetThroughFlag();

		}

	}

	//������Ȃ������̂�false��Ԃ�
	return false;

}

bool MeshFieldBoxManager::CheckBlockCliff(D3DXVECTOR3 s_pos, AnimationPlayer* s_p) {

	//�������画��ΏۂƂȂ�u���b�N��T��
	for (MeshFieldBox* object : meshBoxList) {

		if (object->GetCliff(s_pos,s_p)) {



			return true;
		}
	}

	//������Ȃ������̂�false��Ԃ�
	return false;

}

void MeshFieldBoxManager::Uninit() {

	for (MeshFieldBox* object : meshBoxList) {

		object->Uninit();
		delete object;
	}
	meshBoxList.clear();//���X�g�̍폜

}
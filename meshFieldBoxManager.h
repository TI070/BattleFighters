#pragma once

#include <list>
#include "MeshFieldBox.h"

class AnimationPlayer;

class MeshFieldBoxManager {

private:
	static std::list<MeshFieldBox*> meshBoxList;


public:
	static void Draw();
	static void Update();
	static void Uninit();

	static MeshFieldBox* SetMeshFieldBox(D3DXVECTOR3 s_pivot, float s_pieceSize, float s_uvSize, float s_height, float s_zWidth);

	static float GetHeightUp(D3DXVECTOR3 s_pos);
	static bool CheckBlockThrough(D3DXVECTOR3 s_pos);
	static bool CheckBlockCliff(D3DXVECTOR3 s_pos,AnimationPlayer* s_p);
	static std::list<MeshFieldBox*> GetMeshBoxList() { return meshBoxList; }

};
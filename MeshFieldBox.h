#pragma once
#include "main.h"
#include "gameObject.h"
#include "fieldConfig.h"
#include "imageManager.h"
#include "renderer.h"
#include "MeshField.h"

#define MESHFIELDBOX_X (21)
#define MESHFIELDBOX_Z (21)



class Player;

class MeshFieldBox : public MeshField {

private:

	float FieldBoxHeight[MESHFIELDBOX_X][MESHFIELDBOX_Z] =
	{
		//{0.0f,1.0f,2.0f,1.0f,0.0f},
		//{0.0f,1.0f,2.0f,1.0f,0.0f},
		//{0.0f,1.0f,2.0f,1.0f,0.0f},
		//{0.0f,1.0f,2.0f,1.0f,0.0f},
		//{0.0f,1.0f,2.0f,1.0f,0.0f},
/*
	{0.0f,5.0f,1.0f,5.0f,1.5f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,5.0f,1.0f,5.0f,1.5f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,6.0f,1.0f,6.0f,2.5f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,1.0f,2.0f,2.0f,2.0f,1.0f,0.0f,0.0f,0.0f},
	{0.0f,6.0f,1.0f,6.0f,2.5f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,1.0f,2.0f,3.0f,3.0f,3.0f,2.0f,1.0f,0.0f,0.0f},
	{0.0f,6.0f,1.0f,6.0f,2.5f,1.0f,1.0f,0.0f,0.0f,0.0f,1.0f,1.0f,1.0f,2.0f,3.0f,3.0f,3.0f,2.0f,1.0f,0.0f,0.0f},
	{0.0f,6.0f,1.0f,6.0f,2.5f,1.0f,1.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,1.0f,2.0f,2.0f,2.0f,1.0f,0.0f,0.0f,0.0f},
	{0.0f,7.0f,1.0f,7.0f,2.5f,2.0f,1.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,1.5f,1.5f,1.5f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,7.0f,1.0f,7.0f,3.5f,2.0f,1.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,8.0f,1.0f,8.0f,3.5f,2.0f,1.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,8.0f,1.0f,8.0f,3.5f,2.0f,1.0f,0.0f,0.0f,0.0f,1.0f,1.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,8.0f,1.0f,8.0f,3.5f,2.0f,1.0f,0.0f,0.0f,0.0f,1.0f,2.0f,1.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,8.0f,1.0f,8.0f,3.5f,2.0f,1.0f,0.0f,0.0f,0.0f,1.0f,2.0f,2.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,8.0f,1.0f,8.0f,3.5f,2.0f,1.0f,0.0f,0.0f,0.0f,3.0f,3.0f,2.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,7.0f,1.0f,7.0f,3.5f,2.0f,1.0f,0.0f,0.0f,0.0f,3.0f,3.0f,3.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,7.0f,1.0f,7.0f,3.5f,2.0f,1.0f,0.0f,0.0f,0.0f,3.0f,4.0f,3.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,7.0f,1.0f,7.0f,2.5f,2.0f,1.0f,0.0f,0.0f,0.0f,3.0f,3.0f,3.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,6.0f,1.0f,6.0f,2.5f,1.0f,1.0f,0.0f,0.0f,0.0f,2.0f,3.0f,2.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,6.0f,1.0f,6.0f,2.5f,1.0f,0.0f,0.0f,0.0f,0.0f,2.0f,2.0f,2.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,6.0f,1.0f,6.0f,2.5f,1.0f,0.0f,0.0f,0.0f,0.0f,2.0f,2.0f,2.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,5.0f,1.0f,5.0f,2.5f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,1.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,5.0f,1.0f,5.0f,1.5f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f}
*/	

	{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	
	};

	D3DXVECTOR3 pivot;//íÜêS
	float pieceSize;
	float height;//çÇÇ≥
	float uvSize;
	float zWidth;//zé≤ïù


	ID3D11Buffer* m_VertexBufferFront = NULL;
	ID3D11Buffer* m_VertexBufferRight = NULL;
	ID3D11Buffer* m_VertexBufferLeft = NULL;
	ID3D11Buffer* m_VertexBufferBottom = NULL;


	bool throughFlag;
	bool cliffFlag;//äRÇ¬Ç©Ç›â¬î\Ç©

public:

	void Init(D3DXVECTOR3 s_pivot,float s_pieceSize,float s_uvSize,float s_height,float s_zWidth);
	void Uninit();
	void Update();
	void Draw();
	float GetHeightUp(D3DXVECTOR3 Position);
	float GetHeightDown(D3DXVECTOR3 Position);
	bool GetCliff(D3DXVECTOR3 Position, AnimationPlayer* s_p);
	void SetThroughFlag(bool sFlag) { throughFlag = sFlag; }
	void SetCliffFlag(bool sFlag) { cliffFlag = sFlag; }

	bool BlockOverCharaCheck(D3DXVECTOR3 s_pos, float py);
	bool GetThroughFlag() {return throughFlag; }
	float GetRightMax() { return pivot.x + 10 * pieceSize; }
	float GetLeftMax() { return pivot.x - 10 * pieceSize; }
	float GetFrontMax() { return pivot.z - 10 * pieceSize; }
	float GetBackMax() { return pivot.z + 10 * pieceSize; }
	float GetPivotY() { return pivot.y; }
	float GetPivotX() { return pivot.x; }
	void SetTexture(TEX_KIND s_tk);

};
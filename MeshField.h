#pragma once
#pragma once
#include "main.h"
#include "gameObject.h"
#include "fieldConfig.h"
#include "imageManager.h"
#include "renderer.h"

#define MESHFIELD_X (21)
#define MESHFIELD_Z (21)

#define LAND_DOWNLIMIT (-10)

class Player;

class MeshField : public GameObject {

protected:

	VERTEX_3D m_Vertex[MESHFIELD_X][MESHFIELD_Z];
	VERTEX_3D m_VertexFront[MESHFIELD_X][MESHFIELD_Z];
	VERTEX_3D m_VertexRight[MESHFIELD_X][MESHFIELD_Z];
	VERTEX_3D m_VertexLeft[MESHFIELD_X][MESHFIELD_Z];
	VERTEX_3D m_VertexBottom[MESHFIELD_X][MESHFIELD_Z];


	ID3D11Buffer* m_IndexBuffer = NULL;
	ID3D11Buffer* m_IndexBufferFront = NULL;
	ID3D11Buffer* m_IndexBufferRight = NULL;
	ID3D11Buffer* m_IndexBufferLeft = NULL;
	ID3D11Buffer* m_IndexBufferBottom = NULL;

public:


	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();
	float GetHeight(D3DXVECTOR3 Position);


};



#pragma once
#include "main.h"
#include "gameObject.h"

class Effect : public GameObject {



protected:

	ID3D11VertexShader* m_InstancingVertexShader = NULL;
	ID3D11PixelShader* m_InstancingPixelShader = NULL;
	ID3D11InputLayout* m_InstancingVertxeLayout = NULL;

	float size;
	D3DXVECTOR2 v2Size = {0,0};

	int life;//生存時間(フレーム)
	int tex1_scale;//フレームごとの時間

	float tcx;
	float tcy;

	float tcw;
	float tch;

	D3DXVECTOR4 color = { 1.0f,1.0f, 1.0f, 1.0f };
	D3DXVECTOR4 lastColor = { 1.0f,1.0f, 1.0f, 1.0f };

	D3DXVECTOR2 lastSize;
	bool sizeChange = false;

	GameObject* followObj = nullptr;
	D3DXVECTOR3 followOffset = {0,0,0};
	bool followFlag = false;

	int delayTime = 0;

public:

	virtual void Init(D3DXVECTOR3 s_pos, float s_size, unsigned short texId) = 0;
	virtual void Init(D3DXVECTOR3 s_pos, D3DXVECTOR2 s_size, unsigned short texId) = 0;
	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	bool CheckLife() { 
		if (life > 0) return true;
		return false;
	}

	void SetColor(D3DXVECTOR4 s_color) { color = s_color; };
	void SetLastColor(D3DXVECTOR4 s_color) { lastColor = s_color; };
	void SetLastSize(D3DXVECTOR2 s_scale) {
		lastSize = s_scale;
		sizeChange = true;
	}
	void SetDelayTime(int s_time) { delayTime = s_time; }

	D3DXVECTOR4 GetColor() { return color; }
	D3DXVECTOR3 GetSize() { return { size,size,size }; }

	void FollowObect(GameObject* s_obj) { 
		followObj = s_obj;
		followFlag = true;
	}
	void SetFollowOffset(D3DXVECTOR3 s_pos) { followOffset = s_pos; }

};
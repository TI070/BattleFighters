#pragma once
#include "main.h"
#include "gameObject.h"

enum POLYGON2D_DEPTH {

	DEPTH_ZERO,
	DEPTH_ONE,
	DEPTH_TWO,
	DEPTH_THREE,
	DEPTH_MAX

};



class Polygon2D : public GameObject {

protected:
	bool isUse;

	int depth;
	D3DXVECTOR2 size;
	D3DXVECTOR4 Color = {1.0f,1.0f, 1.0f, 1.0f};

public:
	Polygon2D() {};
	Polygon2D(D3DXVECTOR2 s_pos, float s_size, int texId,int s_depth) { 
		Init(s_pos,s_size,texId); 
		depth = s_depth;
	}
	Polygon2D(D3DXVECTOR2 s_pos, D3DXVECTOR2 s_size, int texId, int s_depth) {
		Init(s_pos, s_size, texId);
		depth = s_depth;
	}

	Polygon2D(D3DXVECTOR2 s_pos, D3DXVECTOR2 s_size, int texId, int s_depth,bool dynamic) {
		if(dynamic) Init_D(s_pos, s_size, texId);
		else Init(s_pos, s_size, texId);
		depth = s_depth;
	}

	virtual void Init(D3DXVECTOR2 s_pos, float s_size, int texId);
	virtual void Init(D3DXVECTOR2 s_pos, D3DXVECTOR2 s_size, int texId);
	virtual void Init_D(D3DXVECTOR2 s_pos, D3DXVECTOR2 s_size, int texId);
	virtual void Init() {};
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	int GetDepth() { return depth; }
	D3DXVECTOR4 GetColor() { return Color; }
	void SetColor(D3DXVECTOR4 s_color);
	void UpdateVertex(D3DXVECTOR2 s_pos,D3DXVECTOR2 s_size);//頂点データの更新
	void UpdateVertex();//頂点データの更新

	void D_Draw(D3DXVECTOR2 s_pos, D3DXVECTOR2 s_size);

	void SetIsUse(bool s_isUse) { isUse = s_isUse; }

	void SetTexture(unsigned short texId);

};

class TexStringJapanese : public Polygon2D{

protected:
	char string[64];
	

public:

	TexStringJapanese(D3DXVECTOR2 s_pos, float s_size, int texId, int s_depth,const char s_string[64], D3DXVECTOR4 s_color) {
		Init(s_pos, s_size, texId);
		//string = s_string;
		strcpy(string, s_string);
		depth = s_depth;
		Color = s_color;
	}

	virtual void Draw();

};

class TexStringAlphabet : public Polygon2D {

protected:
	char string[64];

public:

	TexStringAlphabet(D3DXVECTOR2 s_pos, float s_size, int texId, int s_depth, const char s_string[64], D3DXVECTOR4 s_color) {
		Init(s_pos, s_size, texId);
		//string = s_string;
		strcpy(string,s_string);
		depth = s_depth;
		Color = s_color;
	}

	virtual void Draw();

};


class TexString : public Polygon2D {

protected:
	char string[64];

public:

	TexString(D3DXVECTOR2 s_pos, float s_size, int s_depth, const char s_string[64], D3DXVECTOR4 s_color) {
		Init(s_pos, s_size, 0);
		//string = s_string;
		strcpy(string, s_string);
		depth = s_depth;
		Color = s_color;
	}

	virtual void Draw();
	void TextUpdate(const char s_string[64]) { strcpy(string, s_string); }
	void SizeUpdate(D3DXVECTOR2 s_size) {
		size.x = s_size.x;
		size.y = s_size.y;
	};

};
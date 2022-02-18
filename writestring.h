#pragma once
#ifndef WRITESTRING_H
#define WRITESTRING_H

#include "main.h"

class writestring {//staticƒNƒ‰ƒX

private:
	writestring();

	static D3DXVECTOR4 fontColor;

	static ID3D11Buffer* m_VertexBuffer;
	static ID3D11ShaderResourceView* m_Texture;
	static ID3D11VertexShader* m_VertexShader;
	static ID3D11PixelShader* m_PixelShader;
	static ID3D11InputLayout* m_VertxeLayout;

public:

	static void Initialize();
	static void JapaneseFont_Draw(float dx, float dy, const char* pString);
	static void JapaneseFont_Draw(float dx, float dy, const char* pString, float sizex, float sizey);
	//static void JapaneseFont_ScrollDraw(float dx, float dy, const char* pString, float sizex, float sizey);
	static void AlphabetFont_Draw(float dx, float dy, const char* pString);
	static void AlphabetFont_Draw(float dx, float dy, const char* pString,float sizex, float sizey);
	//static void AlphabetFont_ScrollDraw(float dx, float dy, const char* pString, float sizex, float sizey);

	static void FontDraw(int textureId, float dx, float dy, float dw, float dh, int tcx, int tcy, int tcw, int tch);

	static void SetColor(D3DXVECTOR4 s_color);
	static void SetColor(int r,int g,int b,int a);
	static void SetColorDefault();

	static void JPNandALP_FontDraw(float dx, float dy, const char* pString);
	static void JPNandALP_FontDraw(float dx, float dy, const char* pString, float sizex, float sizey);

};


#endif
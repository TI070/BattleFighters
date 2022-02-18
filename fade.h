#pragma once
#ifndef FADE_H
#define FADE_H

#include "main.h"

typedef enum FadeMode_tag {
	FADE_NONE,
	FADE_IN,
	FADE_OUT

}FadeMode;

class Fade{
	
private:
	static ID3D11Buffer* m_VertexBuffer;
	static ID3D11ShaderResourceView* m_Texture;
	static ID3D11VertexShader* m_VertexShader;
	static ID3D11PixelShader* m_PixelShader;
	static ID3D11InputLayout* m_VertxeLayout;

public:
	//‰Šú‰»
	static void Fade_Initialize();

	//XV
	static void Fade_Update();

	//•`‰æ
	static void Fade_Draw();

	//I—¹ˆ—
	static void Fade_Finalize();

	static void Fade_Start(FadeMode mode, float r, float g, float b, int frame);
	static bool Fade_IsFade();

};

#endif
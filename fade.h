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
	//初期化
	static void Fade_Initialize();

	//更新
	static void Fade_Update();

	//描画
	static void Fade_Draw();

	//終了処理
	static void Fade_Finalize();

	static void Fade_Start(FadeMode mode, float r, float g, float b, int frame);
	static bool Fade_IsFade();

};

#endif
#include "writestring.h"
#include "texture.h"
#include "main.h"
#include "camera.h"
#include "billboard.h"
#include "renderer.h"
#include "imageManager.h"
#include "rendererManager.h"
#include <mbstring.h>

#define FONT_WIDTH (64)
#define FONT_HEIGHT (64)

#define FONT_H_NUM (16) //横の文字数

D3DXVECTOR4 writestring::fontColor;

ID3D11Buffer* writestring::m_VertexBuffer = nullptr;
ID3D11ShaderResourceView* writestring::m_Texture = nullptr;
ID3D11VertexShader* writestring::m_VertexShader = nullptr;
ID3D11PixelShader* writestring::m_PixelShader = nullptr;
ID3D11InputLayout* writestring::m_VertxeLayout = nullptr;

void writestring::Initialize() {




}

void writestring::JPNandALP_FontDraw(float dx, float dy, const char* pString) {

	int num = 0;
	int byte2Cnt = 0;

	for (int i = 0; i < strlen(pString);) 
	{
		const char *p = &pString[i];

		if (_mbclen((BYTE*)p) == 1)
		{
			//1バイトなのでアルファベット
			const char s_string[2] = { pString[i],'\0' };

			AlphabetFont_Draw(dx + (i - byte2Cnt) * FONT_WIDTH, dy, s_string);

			i++;
		}
		else
		{
			//2バイトなのでひらがな、カタカナ
			const char s_string[3] = {pString[i],pString[i + 1] ,'\0' };

			JapaneseFont_Draw(dx + (i - byte2Cnt) * FONT_WIDTH, dy, s_string);
			i += 2;
			byte2Cnt++;
		}
	}

}

void writestring::JPNandALP_FontDraw(float dx, float dy, const char* pString, float sizex, float sizey) {

	int num = 0;
	int byte2Cnt = 0;

	for (int i = 0; i < strlen(pString);)
	{
		const char *p = &pString[i];

		if (_mbclen((BYTE*)p) == 1)
		{
			//1バイトなのでアルファベット
			const char s_string[2] = { pString[i],'\0' };

			AlphabetFont_Draw(dx + (i - byte2Cnt) * sizex, dy, s_string,sizex,sizey);

			i++;
		}
		else
		{
			//2バイトなのでひらがな、カタカナ
			const char s_string[3] = { pString[i],pString[i + 1] ,'\0' };

			JapaneseFont_Draw(dx + (i - byte2Cnt) * sizex, dy, s_string, sizex, sizey);
			i += 2;
			byte2Cnt++;
		}
	}

}


void writestring::JapaneseFont_Draw(float dx, float dy, const char* pString) {


	for (int i = 0; i < strlen(pString); i = i + 2) {

		unsigned int dummy_int;
		char dummy_string[3] = "ぁ";
		char dummy_string2[3] = "　";
		char dummy_string3[3] = "ー";
		char dummy_string4[3] = "ェ";

		//例外処理
		if (pString[i] * 10 + pString[i + 1] == dummy_string4[0] * 10 + dummy_string4[1]) {
			FontDraw(ImageManager::GetTexId(TEX_JAPANESE_FONT), dx + i / 2 * FONT_WIDTH, dy, 64, 64, 64 * 8, 64 * 6, FONT_WIDTH, FONT_HEIGHT);

			continue;
		}
		if (pString[i] == ' ') {
			i--;
			continue;
		}
		if (pString[i] * 10 + pString[i + 1] == dummy_string2[0] * 10 + dummy_string2[1]) {

			continue;
		}
		if (pString[i] * 10 + pString[i + 1] == dummy_string3[0] * 10 + dummy_string3[1]) {

			FontDraw(ImageManager::GetTexId(TEX_JAPANESE_FONT), dx + i / 2 * FONT_WIDTH, dy, 64, 64, 64, 0, FONT_WIDTH, FONT_HEIGHT);

			continue;
		}


		//いったん数値に直す
		dummy_int = pString[i] * 0 + pString[i + 1];
		dummy_int -= (dummy_string[0] * 0 + dummy_string[1]);
		dummy_int += 1;

		if(dummy_int > 160) dummy_int -= 65;
		if (dummy_int > 224) dummy_int -= 1;

		int tcx = (dummy_int % FONT_H_NUM) * FONT_WIDTH;
		int tcy = (dummy_int / FONT_H_NUM) * FONT_HEIGHT;

		FontDraw(ImageManager::GetTexId(TEX_JAPANESE_FONT), dx + i/2 * FONT_WIDTH, dy, 64, 64, tcx, tcy, FONT_WIDTH, FONT_HEIGHT);

	}

}

void writestring::JapaneseFont_Draw(float dx, float dy, const char* pString, float sizex, float sizey) {

	for (int i = 0; i < strlen(pString); i = i + 2) {

		unsigned int dummy_int;
		char dummy_string[3] = "ぁ";
		char dummy_string2[3] = "　";
		char dummy_string3[3] = "ー";
		char dummy_string4[3] = "ェ";
		char dummy_string5[3] = "エ";

		//例外処理
		//if (pString[i] * 10 + pString[i + 1] == dummy_string4[0] * 10 + dummy_string4[1]) {
		//	FontDraw(ImageManager::GetTexId(TEX_JAPANESE_FONT), dx + i / 2 * sizex, dy, sizex, sizey, 64 * 8, 64 * 6, FONT_WIDTH, FONT_HEIGHT);
		//
		//	continue;
		//}
		if (pString[i] == ' ') {
			i--;
			continue;
		}

		if (pString[i] * 10 + pString[i + 1] == dummy_string2[0] * 10 + dummy_string2[1]) {
			
			continue;
		}
		//if (pString[i] * 10 + pString[i + 1] == dummy_string3[0] * 10 + dummy_string3[1]) {
		//
		//	FontDraw(ImageManager::GetTexId(TEX_JAPANESE_FONT), dx + i / 2 * sizex, dy, sizex, sizey, 64, 0, FONT_WIDTH, FONT_HEIGHT);
		//
		//	continue;
		//}


		//いったん数値に直す
		dummy_int = pString[i] * 0 + pString[i + 1];
		dummy_int -= (dummy_string[0] * 0 + dummy_string[1]);
		dummy_int += 1;

		if (dummy_int > 160) dummy_int -= 65;
		if (dummy_int > 224) dummy_int -= 1;
		//if (dummy_int > 200) dummy_int -= 321;

		int tcx = (dummy_int % FONT_H_NUM) * FONT_WIDTH;
		int tcy = (dummy_int / FONT_H_NUM) * FONT_HEIGHT;

		FontDraw(ImageManager::GetTexId(TEX_JAPANESE_FONT), dx + i / 2 * sizex, dy, sizex, sizey, tcx, tcy, FONT_WIDTH, FONT_HEIGHT);

	}

}

//void writestring::JapaneseFont_ScrollDraw(float dx, float dy, const char* pString, float sizex, float sizey) {
//
//	dx += camera::GetCameraPos().x;
//	dy += camera::GetCameraPos().y;
//
//	JapaneseFont_Draw(dx, dy, pString, sizex, sizey);
//
//}
//
void writestring::AlphabetFont_Draw(float dx, float dy, const char* pString) {


	for (int i = 0; i < strlen(pString); i++) {


		int tcx = ((pString[i] % 16) % FONT_H_NUM) * FONT_WIDTH;
		int tcy = ((pString[i] - ' ') / FONT_H_NUM) * FONT_HEIGHT;

		FontDraw(ImageManager::GetTexId(TEX_ALPHABET_FONT), dx + i * FONT_WIDTH, dy, 64, 64, tcx, tcy, FONT_WIDTH, FONT_HEIGHT);

	}

}

void writestring::AlphabetFont_Draw(float dx, float dy, const char* pString, float sizex, float sizey) {


	for (int i = 0; i < strlen(pString); i++) {


		if (pString[i] == ' ') continue;

		int tcx = ((pString[i] % 16) % FONT_H_NUM) * FONT_WIDTH;
		int tcy = ((pString[i] - ' ') / FONT_H_NUM) * FONT_HEIGHT;

		FontDraw(ImageManager::GetTexId(TEX_ALPHABET_FONT), dx + i * sizex, dy, sizex, sizey, tcx, tcy, FONT_WIDTH, FONT_HEIGHT);

	}

}

//void writestring::AlphabetFont_ScrollDraw(float dx, float dy, const char* pString, float sizex, float sizey) {
//
//	dx += camera::GetCameraPos().x;
//	dy += camera::GetCameraPos().y;
//
//	AlphabetFont_Draw(dx, dy, pString, sizex, sizey);
//
//}

void writestring::FontDraw(int textureId, float dx, float dy, float dw, float dh, int tcx, int tcy, int tcw, int tch) {

	//テクスチャのサイズ取得
	int w = Texture_GetTextureWidth(textureId);
	int h = Texture_GetTextureHeight(textureId);

	//テクスチャ切り取りUV座標
	float u0 = (float)tcx / w;
	float v0 = (float)tcy / h;
	float u1 = (float)(tcx + tcw) / w;
	float v1 = (float)(tcy + tch) / h;

	//dx *= 2;
	//dy *= 2;

//	VERTEX_3D vertex[] = {
//		{D3DXVECTOR3(dx - 0.5f,dy - 0.5f,0.0f)	,{0,0,0},			fontColor,D3DXVECTOR2(u0,v0)},
//		{D3DXVECTOR3(dx - 0.5f + dw,dy - 0.5f,0.0f)	,{0,0,0}	,fontColor,D3DXVECTOR2(u1,v0)},
//		{D3DXVECTOR3(dx - 0.5f,dy - 0.5f + dh,0.0f)	,{0,0,0}	,fontColor,D3DXVECTOR2(u0,v1)},
//		{D3DXVECTOR3(dx - 0.5f + dw,dy - 0.5f + dh,0.0f),{0,0,0}	,fontColor,D3DXVECTOR2(u1,v1)},
//	};

	////VERTEX_3D vertex[] = {
	////	{D3DXVECTOR3(100,100,0.0f)	,{0,0,0},			fontColor,D3DXVECTOR2(u0,v0)},
	////	{D3DXVECTOR3(100 + 100,100,0.0f)	,{0,0,0}	,fontColor,D3DXVECTOR2(u1,v0)},
	////	{D3DXVECTOR3(100,100 + 100,0.0f)	,{0,0,0}	,fontColor,D3DXVECTOR2(u0,v1)},
	////	{D3DXVECTOR3(100 + 100,100 + 100,0.0f),{0,0,0}	,fontColor,D3DXVECTOR2(u1,v1)},
	////};

	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(dx, dy, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = fontColor;//色
	vertex[0].TexCoord = D3DXVECTOR2(u0, v0);

	vertex[1].Position = D3DXVECTOR3(dx + dw, dy, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = fontColor;
	vertex[1].TexCoord = D3DXVECTOR2(u1, v0);

	vertex[2].Position = D3DXVECTOR3(dx, dy + dh, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = fontColor;
	vertex[2].TexCoord = D3DXVECTOR2(u0, v1);

	vertex[3].Position = D3DXVECTOR3(dx + dw, dy + dh, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = fontColor;
	vertex[3].TexCoord = D3DXVECTOR2(u1, v1);

	//頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);



	m_Texture = Texture_GetTexture(textureId);

	//エラーチェック
	assert(m_Texture);//中身がNULLだとエラーを出すらしい


	//Renderer::CreateVertexShader(&m_VertexShader, &m_VertxeLayout, "unlitTextureVS.cso");
	//Renderer::CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");

	//RendererManager::UnlitShaderSet(m_VertexShader, m_VertxeLayout, m_PixelShader);
	m_VertexShader = RendererManager::GetUnlitVS();
	m_VertxeLayout = RendererManager::GetUnlitIL();
	m_PixelShader = RendererManager::GetUnlitPS();


	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertxeLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	Renderer::SetWorldViewProjection2D();

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;

	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);


	m_VertexBuffer->Release();
	//m_Texture->Release();

	//m_VertxeLayout->Release();
	//m_VertexShader->Release();
	//m_PixelShader->Release();

}

//カラー設定
void writestring::SetColor(D3DXVECTOR4 s_color) {

	fontColor = s_color;
}

void writestring::SetColor(int r, int g, int b, int a) {

	fontColor = { r / 255.0f,g / 255.0f ,b / 255.0f ,a / 255.0f };
}

void writestring::SetColorDefault() {

	fontColor = { 1.0f,1.0f ,1.0f ,1.0f };
}
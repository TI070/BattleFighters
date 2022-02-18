
#include "texture.h"
#include "renderer.h"
#include <d3dx9.h>
#include <string.h>
#include <stdio.h>

////構造体宣言
//テクスチャ管理テーブル構造体
typedef struct Texture_tag
{
	ID3D11ShaderResourceView* pTexture;
	unsigned long width;
	unsigned long height;
	char filename[TEXTURE_FILENAME_MAX];

}Texture;

////グローバル変数宣言
//テクスチャ管理テーブル
static Texture g_Textures[TEXTURE_MAX];


//テクスチャ管理モジュールの初期化
void Texture_Initialize() {

	for (int i = 0; i < TEXTURE_MAX; i++) {
		g_Textures[i].pTexture = NULL;
		g_Textures[i].width = 0;
		g_Textures[i].height = 0;
		g_Textures[i].filename[0] = 0;//'\0'と同義

	}

}

//テクスチャ管理モジュールの終了処理
void Texture_Finalize() {


}
////テクスチャファイルの読み込み予約
//引数:ファイル名
//戻り値:テクスチャ管理番号 
//       最大管理数を超えていた場合は-1(INVALID_TEXTURE_ID)
int Texture_SetTextureLoadFile(const char* pFileName) {


	//ファイル名検索
	for (int i = 0; i < TEXTURE_MAX; i++) {
		////テーブルの空き状況確認
		//空いてたらスキップ
		if (g_Textures[i].filename[0] == 0) continue;

		//既に指定のファイルが予約されているか
		if (strcmp(pFileName, g_Textures[i].filename) == 0) {
			//予約されている
			return i;

		}
	}


	//新規予約
	for (int i = 0; i < TEXTURE_MAX; i++) {
		////テーブルの空き状況確認
		//使用中ならスキップ
		if (g_Textures[i].filename[0] != 0) continue;

		strcpy(g_Textures[i].filename,pFileName);

		return i;
	}

	return TEXTURE_INVALID_ID;

}
//テクスチャの読み込み
//予約されているファイルを読み込む
//戻り値:読み込めなかったファイル数
int Texture_Load() {

	int err_count = 0;


	for (int i = 0; i < TEXTURE_MAX; i++) {


		if (g_Textures[i].filename[0] == 0) {
			//空のテーブル
			continue;
		}
		if (g_Textures[i].pTexture != NULL) {
			//すでに読み込まれている
			continue;
		}


		//テクスチャの読み込み
		HRESULT hr = D3DX11CreateShaderResourceViewFromFile(
				Renderer::GetDevice(),
				g_Textures[i].filename,
				NULL,
				NULL,
				&g_Textures[i].pTexture,
				NULL
			); 
		
		
		if (FAILED(hr)) {
			//テクスチャの読み込みに失敗
			err_count++;
		}
		else {
			//テクスチャの解像度(幅・高さ)の調査
			D3DXIMAGE_INFO info;
			D3DXGetImageInfoFromFile(g_Textures[i].filename,&info);
			g_Textures[i].width = info.Width;
			g_Textures[i].height = info.Height;

		}

	}

	return err_count;

}
//テクスチャの部分開放
//引数:textureIds[]...解放したいtextureIdが入ったint型配列の先頭アドレス
//     count       ...解放するテクスチャの数

void Texture_Release(int textureIds[], int count) {

	for (int i = 0; i < count; i++) {
		if (g_Textures[textureIds[i]].pTexture) {
			g_Textures[textureIds[i]].pTexture->Release();
			g_Textures[textureIds[i]].pTexture = NULL;
		}

		g_Textures[textureIds[i]].filename[0] = 0;
		g_Textures[textureIds[i]].width = 0;
		g_Textures[textureIds[i]].height = 0;

	}


}
//テクスチャの全開放
void Texture_AllRelease() {



}
//テクスチャの幅の取得
//引数:テクスチャ管理番号
//戻り値:テクスチャの幅・高さ
unsigned long Texture_GetTextureWidth(int textureId) {
	
	return  g_Textures[textureId].width;
}

unsigned long Texture_GetTextureHeight(int textureId) {

	return  g_Textures[textureId].height;
}

//テクスチャインターフェースポインタの取得
//引数:テクスチャ管理番号
//戻り値:テクスチャインターフェースポインタ
//       読み込めていないまたは不正な管理番号だった場合NULL
ID3D11ShaderResourceView* Texture_GetTexture(int textureId) {

	if (textureId <= TEXTURE_INVALID_ID) return NULL;
	else if (textureId >= 1024) return NULL;
	return g_Textures[textureId].pTexture;

}
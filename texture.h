#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <d3d9.h>
#include "imageManager.h"

//マクロ定義
#define TEXTURE_INVALID_ID (-1) //無効なテクスチャ管理番号
#define TEXTURE_FILENAME_MAX (64)//テクスチャファイル名最大文字数
#define TEXTURE_MAX (1024)//テクスチャ管理最大数


//テクスチャ管理モジュールの初期化
void Texture_Initialize();

//テクスチャ管理モジュールの終了処理
void Texture_Finalize();

////テクスチャファイルの読み込み予約
//引数:ファイル名
//戻り値:テクスチャ管理番号 
//       最大管理数を超えていた場合は-1(INVALID_TEXTURE_ID)
int Texture_SetTextureLoadFile(const char* pFileName);

//テクスチャの読み込み
//予約されているファイルを読み込む
//戻り値:読み込めなかったファイル数
int Texture_Load();

//テクスチャの部分開放
//引数:textureIds[]...解放したいtextureIdが入ったint型配列の先頭アドレス
//     count       ...解放するテクスチャの数

void Texture_Release(int textureIds[],int count);

//テクスチャの全開放
void Texture_AllRelease();

//テクスチャの幅の取得
//引数:テクスチャ管理番号
//戻り値:テクスチャの幅・高さ
unsigned long Texture_GetTextureWidth(int textureId);
unsigned long Texture_GetTextureHeight(int textureId);


//テクスチャインターフェースポインタの取得
//引数:テクスチャ管理番号
//戻り値:テクスチャインターフェースポインタ
//       読み込めていないまたは不正な管理番号だった場合NULL
ID3D11ShaderResourceView* Texture_GetTexture(int textureId);

#endif
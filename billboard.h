#pragma once
#ifndef BILLBOARD_H
#define BILLBOARD_H

#include <d3dx9.h>
#include "gameObject.h"


class Billboard{

private:

	RendererSet renSet;

	static ID3D11Buffer* m_VertexBuffer;

	static ID3D11VertexShader* m_VertexShader;
	static ID3D11PixelShader* m_PixelShader;
	static ID3D11InputLayout* m_VertxeLayout;

	//インスタンシング用
	static ID3D11VertexShader* m_InstancingVertexShader;
	static ID3D11PixelShader* m_InstancingPixelShader;
	static ID3D11InputLayout* m_InstancingVertxeLayout;


public:

	 static void Init();
	 static void Uninit();
	 static void BillBoard_SetViewMatrix(const D3DXMATRIX& mtxView);
	 //void BillBoard_SetOffsetY(float offset_y);
	 static void BillBorad_Draw(int textureId,D3DXVECTOR3 s_pos, float width, float height,float d);
	 static void BillBorad_Draw(int textureId, D3DXVECTOR3 s_pos, float width, float height, float d, float tcx, float tcy, float tcw, float tch);
	 static void BillBorad_Draw(RendererSet s_renSet, D3DXVECTOR3 s_pos);
	 static void BillBorad_Draw(RendererSet s_renSet, D3DXVECTOR3 s_pos,D3DXVECTOR3 s_rot);
	 
	 static void BillBoard_SetColor(D3DXVECTOR4 s_color);
	 static void BillBoard_SetColor(int r,int g,int b,int a);
	 static void BillBoard_SetColorDefault();

	 //頂点位置オフセットデータを読み込み,インスタンシング描画する
	 static void BillBorad_InstanceDraw(RendererSet s_renSet, D3DXVECTOR3 s_pos, D3DXVECTOR3 s_offsetPos[INSTANCING_NUM], D3DXVECTOR3 s_rot[INSTANCING_NUM], D3DXVECTOR4 s_color[INSTANCING_NUM], int s_instanceNum);

	 
};
#endif
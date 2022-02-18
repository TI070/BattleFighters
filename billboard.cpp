#include <d3dx9.h>
#include <stdio.h>
#include "camera.h"
#include "renderer.h"
#include "texture.h"
#include "billboard.h"

#define FVF_BILLBOARD_VERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)

#define TEXTURE_INVALID_ID (-1)

///////

///////

ID3D11Buffer* Billboard::m_VertexBuffer;

ID3D11VertexShader* Billboard::m_VertexShader;
ID3D11PixelShader* Billboard::m_PixelShader;
ID3D11InputLayout* Billboard::m_VertxeLayout;

ID3D11VertexShader*  Billboard::m_InstancingVertexShader;
ID3D11PixelShader*  Billboard::m_InstancingPixelShader;
ID3D11InputLayout*  Billboard::m_InstancingVertxeLayout;

static float g_angle = 0.0f;
static float g_value = 0.0f;

static int g_TexId = TEXTURE_INVALID_ID;
static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
static LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;

static D3DXMATRIX g_mtxView;

static D3DXVECTOR4 Color = {1.0f,1.0f, 1.0f, 1.0f};

typedef struct BillboardVertex_tag {
	D3DXVECTOR3 Position;
	D3DCOLOR Color;
	D3DXVECTOR2 TexCoord;

}BillboardVertex;

//頂点データ配列

void Billboard::Init() {

	D3DXMatrixIdentity(&g_mtxView);

}

void Billboard::Uninit() {


}

//カラー設定
void Billboard::BillBoard_SetColor(D3DXVECTOR4 s_color) {

	Color = s_color;
}

void Billboard::BillBoard_SetColor(int r, int g, int b, int a) {

	Color = {r/255.0f,g / 255.0f ,b / 255.0f ,a / 255.0f};
}

//カラー初期化
void Billboard::BillBoard_SetColorDefault() {

	Color = {1.0f,1.0f, 1.0f, 1.0f};
}

//void BillBoard_SetOffsetY(float offset_y);

void Billboard::BillBorad_Draw(int textureId, D3DXVECTOR3 s_pos, float width,float height, float d) {

	//BillboardVertex v[] = {
	//	{{-width / 2, height / 2,0.0f},Color,{0.0f,0.0f} },
	//	{{ width / 2, height / 2,0.0f},Color,{1.0f,0.0f} },
	//	{{-width / 2,-height / 2,0.0f},Color,{0.0f,1.0f} },
	//	{{ width / 2,-height / 2,0.0f},Color,{1.0f,1.0f} },
	//
	//};

	//テクスチャ読込
	ID3D11ShaderResourceView* m_Tex = Texture_GetTexture(textureId);

	D3DXVECTOR2 s_size = {width/2,height/2};

	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(- s_size.x,s_size.y, d);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = Color;//色
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(s_size.x,s_size.y, d);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = Color;
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(- s_size.x,- s_size.y, d);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = Color;
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(s_size.x, - s_size.y, d);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = Color;
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	//LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();

	D3DXMATRIX mtxWorld,mtxT;
	D3DXMATRIX mtxInvView = g_mtxView;
	//view行列の逆行列なんだけど平行移動成分xyzが0が欲しいらしい
	mtxInvView._41 = 0.0f;
	mtxInvView._42 = 0.0f;
	mtxInvView._43 = 0.0f;
	
	//D3DXMatrixInverse(&mtxInvView,NULL, &mtxInvView);
	D3DXMatrixTranspose(&mtxInvView, &mtxInvView);

	D3DXMatrixIdentity(&mtxWorld);

	D3DXMATRIX mtxRZ, mtxS;
	D3DXMatrixTranslation(&mtxT, s_pos.x, s_pos.y, s_pos.z);

	mtxWorld = mtxInvView * mtxT;
	//mtxWorld = mtxInvView * mtxS * mtxRZ * mtxT;
	
	
	//αテストを有効に
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE,TRUE);
	//pDevice->SetRenderState(D3DRS_ALPHAREF,0.8f);//閾値
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC,D3DCMP_GREATER);//判定方法

	//Zテスト,Zバッファの有効、無効の切り替え
	//Zバッファへの書き込み,Zバッファの参照をしない
	//pDevice->SetRenderState(D3DRS_ZENABLE,FALSE);

	//Zバッファへの書き込みをしない
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);

	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);


	//pDevice->SetTransform(D3DTS_WORLD,&mtxWorld);
	//pDevice->SetFVF(FVF_BILLBOARD_VERTEX);
	//pDevice->SetTexture(0,Texture_GetTexture(textureId));
	//pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);
	//pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,v,sizeof(BillboardVertex));
	//pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	//エラーチェック
	assert(m_Tex);//中身がNULLだとエラーを出すらしい

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertxeLayout, "unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");


	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertxeLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
		//マトリクス設定
	//Renderer::SetWorldViewProjection2D();
	D3DXVECTOR3 m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);
	Renderer::SetWorldMatrix(&mtxWorld);

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;

	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Tex);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);


	m_VertexBuffer->Release();
	//m_Texture->Release();

	m_VertxeLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}
/*
void BillBorad_Draw(int textureId, float x, float y, float z, float width, float height,int tcx, int tcy, int tcw, int tch) {
	
	LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();

	//pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, Texture_GetTexture(textureId));

	//テクスチャのサイズ取得
	int w = Texture_GetTextureWidth(textureId);
	int h = Texture_GetTextureHeight(textureId);


	float u0 = (float)tcx / w;
	float v0 = (float)tcy / h;
	float u1 = (float)(tcx + tcw) / w;
	float v1 = (float)(tcy + tch) / h;

	
	BillboardVertex v[] = {
		{{-width / 2, height / 2,0.0f},Color,{u0, v0} },
		{{ width / 2, height / 2,0.0f},Color,{u1, v0} },
		{{-width / 2,-height / 2,0.0f},Color,{u0, v1} },
		{{ width / 2,-height / 2,0.0f},Color,{u1, v1} },
		//{{-width / 2, height / 2,0.0f},Color,{0.25f, 0.50f} },
		//{{ width / 2, height / 2,0.0f},Color,{0.50f, 0.50f} },
		//{{-width / 2,-height / 2,0.0f},Color,{0.25f, 1.00f} },
		//{{ width / 2,-height / 2,0.0f},Color,{0.50f, 1.00f} },

	};


	D3DXMATRIX mtxWorld, mtxT;
	D3DXMATRIX mtxInvView = g_mtxView;
	//view行列の逆行列なんだけど平行移動成分xyzが0が欲しいらしい
	mtxInvView._41 = 0.0f;
	mtxInvView._42 = 0.0f;
	mtxInvView._43 = 0.0f;

	//D3DXMatrixInverse(&mtxInvView,NULL, &mtxInvView);
	D3DXMatrixTranspose(&mtxInvView, &mtxInvView);

	D3DXMatrixIdentity(&mtxWorld);

	D3DXMATRIX mtxRZ, mtxS;
	D3DXMatrixTranslation(&mtxT, x, y, z);

	mtxWorld = mtxInvView * mtxT;
	//mtxWorld = mtxInvView * mtxS * mtxRZ * mtxT;


	//αテストを有効に
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//pDevice->SetRenderState(D3DRS_ALPHAREF, 0.8f);//閾値
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);//判定方法

	//Zテスト,Zバッファの有効、無効の切り替え
	//Zバッファへの書き込み,Zバッファの参照をしない
	//pDevice->SetRenderState(D3DRS_ZENABLE,FALSE);

	//Zバッファへの書き込みをしない
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);

	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);


	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
	pDevice->SetFVF(FVF_BILLBOARD_VERTEX);
	//pDevice->SetTexture(0, Texture_GetTexture(textureId));
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(BillboardVertex));
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

*/

void Billboard::BillBorad_Draw(RendererSet s_renSet,D3DXVECTOR3 s_pos) {


	//テクスチャ読込
	ID3D11ShaderResourceView* m_Tex = s_renSet.m_Texture;

	//LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();

	D3DXMATRIX mtxWorld, mtxT;
	D3DXMATRIX mtxInvView = g_mtxView;
	//view行列の逆行列なんだけど平行移動成分xyzが0が欲しいらしい
	mtxInvView._41 = 0.0f;
	mtxInvView._42 = 0.0f;
	mtxInvView._43 = 0.0f;

	//D3DXMatrixInverse(&mtxInvView,NULL, &mtxInvView);
	D3DXMatrixTranspose(&mtxInvView, &mtxInvView);

	D3DXMatrixIdentity(&mtxWorld);

	D3DXMATRIX mtxRZ, mtxS;
	D3DXMatrixTranslation(&mtxT, s_pos.x, s_pos.y, s_pos.z);

	mtxWorld = mtxInvView * mtxT;

	//追加
	D3DXMATRIX world, scale, rot, trans;


	//エラーチェック
	assert(m_Tex);//中身がNULLだとエラーを出すらしい

	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(s_renSet.m_VertxeLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(s_renSet.m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(s_renSet.m_PixelShader, NULL, 0);

	//マトリクス設定
		//マトリクス設定
	//Renderer::SetWorldViewProjection2D();
	D3DXVECTOR3 m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	D3DXVECTOR3 m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);
	Renderer::SetWorldMatrix(&mtxWorld);

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;

	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &s_renSet.m_VertexBuffer, &stride, &offset);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Tex);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);

}

void Billboard::BillBorad_Draw(RendererSet s_renSet, D3DXVECTOR3 s_pos, D3DXVECTOR3 s_rot) {


	//テクスチャ読込
	ID3D11ShaderResourceView* m_Tex = s_renSet.m_Texture;

	//LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();

	D3DXMATRIX mtxWorld, mtxT,mtxRT;
	D3DXMATRIX mtxInvView = g_mtxView;
	//view行列の逆行列なんだけど平行移動成分xyzが0が欲しいらしい
	mtxInvView._41 = 0.0f;
	mtxInvView._42 = 0.0f;
	mtxInvView._43 = 0.0f;

	//D3DXMatrixInverse(&mtxInvView,NULL, &mtxInvView);
	D3DXMatrixTranspose(&mtxInvView, &mtxInvView);

	D3DXMatrixIdentity(&mtxWorld);

	D3DXVECTOR3 m_Rotation = s_rot;
	D3DXMATRIX world, scale, rot, trans;


	D3DXMATRIX mtxRZ, mtxS;

	D3DXMatrixTranslation(&mtxT, s_pos.x, s_pos.y, s_pos.z);
	D3DXMatrixTranslation(&mtxRT, -s_pos.x, -s_pos.y, -s_pos.z);

	//mtxWorld *= mtxRT * rot * mtxT;


	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	//mtxWorld *= rot;

	mtxWorld = mtxInvView * rot * mtxT;
	//mtxWorld *= mtxInvView * mtxT;



	//エラーチェック
	assert(m_Tex);//中身がNULLだとエラーを出すらしい

	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(s_renSet.m_VertxeLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(s_renSet.m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(s_renSet.m_PixelShader, NULL, 0);

	//マトリクス設定
		//マトリクス設定
	//Renderer::SetWorldViewProjection2D();
	//D3DXVECTOR3 m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	////D3DXVECTOR3 m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//D3DXVECTOR3 m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	//
	//D3DXMATRIX world, scale, rot, trans;
	//D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	//D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	//D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	//world = scale * rot * trans;
	//Renderer::SetWorldMatrix(&world);
	Renderer::SetWorldMatrix(&mtxWorld);

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;

	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &s_renSet.m_VertexBuffer, &stride, &offset);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Tex);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);

}


void Billboard::BillBorad_Draw(int textureId, D3DXVECTOR3 s_pos, float width, float height, float d, float tcx, float tcy, float tcw, float tch) {

	//BillboardVertex v[] = {
	//	{{-width / 2, height / 2,0.0f},Color,{0.0f,0.0f} },
	//	{{ width / 2, height / 2,0.0f},Color,{1.0f,0.0f} },
	//	{{-width / 2,-height / 2,0.0f},Color,{0.0f,1.0f} },
	//	{{ width / 2,-height / 2,0.0f},Color,{1.0f,1.0f} },
	//
	//};

	//テクスチャ読込
	ID3D11ShaderResourceView* m_Tex = Texture_GetTexture(textureId);


	D3DXVECTOR2 s_size = { width / 2,height / 2 };

	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(-s_size.x, s_size.y, d);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = Color;//色
	vertex[0].TexCoord = D3DXVECTOR2(tcx, tcy);

	vertex[1].Position = D3DXVECTOR3(s_size.x, s_size.y, d);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = Color;
	vertex[1].TexCoord = D3DXVECTOR2(tcx + tcw, tcy);

	vertex[2].Position = D3DXVECTOR3(-s_size.x, -s_size.y, d);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = Color;
	vertex[2].TexCoord = D3DXVECTOR2(tcx, tcy + tch);

	vertex[3].Position = D3DXVECTOR3(s_size.x, -s_size.y, d);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = Color;
	vertex[3].TexCoord = D3DXVECTOR2(tcx + tcw, tcy + tch);

	//LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();

	D3DXMATRIX mtxWorld, mtxT;
	D3DXMATRIX mtxInvView = g_mtxView;
	//view行列の逆行列なんだけど平行移動成分xyzが0が欲しいらしい
	mtxInvView._41 = 0.0f;
	mtxInvView._42 = 0.0f;
	mtxInvView._43 = 0.0f;

	//D3DXMatrixInverse(&mtxInvView,NULL, &mtxInvView);
	D3DXMatrixTranspose(&mtxInvView, &mtxInvView);

	D3DXMatrixIdentity(&mtxWorld);

	D3DXMATRIX mtxRZ, mtxS;
	D3DXMatrixTranslation(&mtxT, s_pos.x, s_pos.y, s_pos.z);

	mtxWorld = mtxInvView * mtxT;

	//頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	//エラーチェック
	assert(m_Tex);//中身がNULLだとエラーを出すらしい

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertxeLayout, "unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");


	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertxeLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
		//マトリクス設定
	//Renderer::SetWorldViewProjection2D();
	D3DXVECTOR3 m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);
	Renderer::SetWorldMatrix(&mtxWorld);

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;

	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Tex);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);

	m_VertexBuffer->Release();
	//m_Texture->Release();

	m_VertxeLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}


void Billboard::BillBoard_SetViewMatrix(const D3DXMATRIX& mtxView) {

	g_mtxView = mtxView;
}

void Billboard::BillBorad_InstanceDraw(RendererSet s_renSet, D3DXVECTOR3 s_pos, D3DXVECTOR3 s_offsetPos[INSTANCING_NUM], D3DXVECTOR3 s_rot[INSTANCING_NUM], D3DXVECTOR4 s_color[INSTANCING_NUM],int s_instanceNum) {

	//繰り返し回数　後で修正
	int instanceNum = s_instanceNum;

	if (instanceNum <= 0) return;

	//インスタンシングバッファのセット
	InstancingData s_ID;
	for (int i = 0; i < instanceNum; i++) {
		s_ID.Position[i] = { s_offsetPos[i].x,s_offsetPos[i].y,s_offsetPos[i].z,0 };

		s_ID.Rotation[i] = { s_rot[i].x,s_rot[i].y,s_rot[i].z,0 };
		s_ID.Scale[i] = { 0,0,0 ,0};
		s_ID.Diffuse[i] = s_color[i];
	}

	Renderer::SetInstancingData(s_ID);


	//テクスチャ読込
	ID3D11ShaderResourceView* m_Tex = s_renSet.m_Texture;

	//LPDIRECT3DDEVICE9 pDevice = MYDirect3D_GetDevice();

	D3DXMATRIX mtxWorld, mtxT;
	D3DXMATRIX mtxInvView = g_mtxView;
	//view行列の逆行列なんだけど平行移動成分xyzが0が欲しいらしい
	mtxInvView._41 = 0.0f;
	mtxInvView._42 = 0.0f;
	mtxInvView._43 = 0.0f;

	//D3DXMatrixInverse(&mtxInvView,NULL, &mtxInvView);
	D3DXMatrixTranspose(&mtxInvView, &mtxInvView);

	D3DXMatrixIdentity(&mtxWorld);

	D3DXMATRIX mtxRZ, mtxS;
	D3DXMatrixTranslation(&mtxT, s_pos.x, s_pos.y, s_pos.z);

	mtxWorld = mtxInvView * mtxT;

	//追加
	D3DXMATRIX world, scale, rot, trans;


	//エラーチェック
	assert(m_Tex);//中身がNULLだとエラーを出すらしい

	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(s_renSet.m_VertxeLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(s_renSet.m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(s_renSet.m_PixelShader, NULL, 0);

	//マトリクス設定
	//Renderer::SetWorldViewProjection2D();
	/*
	D3DXVECTOR3 m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	D3DXVECTOR3 m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);*/
	Renderer::SetWorldMatrix(&mtxWorld);

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;

	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &s_renSet.m_VertexBuffer, &stride, &offset);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Tex);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン描画
	Renderer::GetDeviceContext()->DrawInstanced(4, instanceNum,0,0);



}

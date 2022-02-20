
#include "renderer.h"
#include "polygon2d.h"
#include "texture.h"
#include "writestring.h"


void Polygon2D::Init(D3DXVECTOR2 s_pos, float s_size, int texId) {

	s_size /= 2;

	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(s_pos.x - s_size, s_pos.y - s_size, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f,1.0f);//色
	vertex[0].TexCoord = D3DXVECTOR2(0.0f,0.0f);

	vertex[1].Position = D3DXVECTOR3(s_pos.x + s_size, s_pos.y - s_size, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(s_pos.x - s_size, s_pos.y + s_size, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(s_pos.x + s_size, s_pos.y + s_size, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);


	//頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd,&sd,&m_VertexBuffer);

	//テクスチャ読込
	m_Texture = Texture_GetTexture(texId);




	//エラーチェック
	assert(m_Texture);//中身がNULLだとエラーを出すらしい

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertxeLayout, "unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader,"unlitTexturePS.cso");

	isUse = true;

	m_Position = { s_pos.x,s_pos.y,0 };
	size.x = s_size;
	size.y = s_size;

}


void Polygon2D::Init(D3DXVECTOR2 s_pos, D3DXVECTOR2 s_size, int texId) {

	s_size /= 2;

	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(s_pos.x - s_size.x, s_pos.y - s_size.y, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);//色
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(s_pos.x + s_size.x, s_pos.y - s_size.y, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(s_pos.x - s_size.x, s_pos.y + s_size.y, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(s_pos.x + s_size.x, s_pos.y + s_size.y, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);


	//頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	//テクスチャ読込
	m_Texture = Texture_GetTexture(texId);




	//エラーチェック
	assert(m_Texture);//中身がNULLだとエラーを出すらしい

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertxeLayout, "unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");

	isUse = true;

	m_Position = { s_pos.x,s_pos.y,0 };
	size = s_size;

}

void Polygon2D::Init_D(D3DXVECTOR2 s_pos, D3DXVECTOR2 s_size, int texId) {

	s_size /= 2;

	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(s_pos.x - s_size.x, s_pos.y - s_size.y, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);//色
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(s_pos.x + s_size.x, s_pos.y - s_size.y, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(s_pos.x - s_size.x, s_pos.y + s_size.y, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(s_pos.x + s_size.x, s_pos.y + s_size.y, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);


	//頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	//テクスチャ読込
	m_Texture = Texture_GetTexture(texId);


	//エラーチェック
	assert(m_Texture);//中身がNULLだとエラーを出すらしい
	
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertxeLayout, "unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");

	isUse = true;

	m_Position = { s_pos.x,s_pos.y,0 };
	size = s_size;

}


void Polygon2D::Uninit() {

	//if (m_VertexBuffer != nullptr) m_VertexBuffer->Release();

	//if (m_VertxeLayout != nullptr)m_VertxeLayout->Release();
	//if (m_VertexShader != nullptr)m_VertexShader->Release();
	//if (m_PixelShader != nullptr) m_PixelShader->Release();

}

void Polygon2D::Update() {

}

void Polygon2D::Draw() {

	if (!isUse) return;

	

	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertxeLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader,NULL,0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);
	Renderer::SetParameter(shaderParameter);

	//マトリクス設定
	Renderer::SetWorldViewProjection2D();

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;

	Renderer::GetDeviceContext()->IASetVertexBuffers(0,1,&m_VertexBuffer,&stride,&offset);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0,1,&m_Texture);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4,0);
	

	Renderer::SetParameter({0,0,0,0});

}

void Polygon2D::UpdateVertex(D3DXVECTOR2 s_pos, D3DXVECTOR2 s_size) {

	s_size /= 2;

	//頂点データ書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	//
	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;
	
	vertex[0].Position = D3DXVECTOR3(s_pos.x - s_size.x, s_pos.y - s_size.y, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = Color;
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
	
	vertex[1].Position = D3DXVECTOR3(s_pos.x + s_size.x, s_pos.y - s_size.y, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = Color;
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(s_pos.x - s_size.x, s_pos.y + s_size.y, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = Color;//色
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);
	
	vertex[3].Position = D3DXVECTOR3(s_pos.x  + s_size.x, s_pos.y + s_size.y, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = Color;
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);
	//
	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

}

void Polygon2D::UpdateVertex() {

	D3DXVECTOR3 s_pos = m_Position;
	D3DXVECTOR2 s_size = size;

	//頂点データ書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	//
	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex[0].Position = D3DXVECTOR3(s_pos.x - s_size.x, s_pos.y - s_size.y, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = Color;
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(s_pos.x + s_size.x, s_pos.y - s_size.y, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = Color;
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(s_pos.x - s_size.x, s_pos.y + s_size.y, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = Color;//色
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(s_pos.x + s_size.x, s_pos.y + s_size.y, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = Color;
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);
	//
	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

}

void Polygon2D::D_Draw(D3DXVECTOR2 s_pos, D3DXVECTOR2 s_size) {

	//頂点データ書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	
	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;
	
	vertex[0].Position = D3DXVECTOR3(s_pos.x - s_size.x, s_pos.y - s_size.y, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = Color;
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
	
	vertex[1].Position = D3DXVECTOR3(s_pos.x - s_size.x, s_pos.y + s_size.y, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = Color;//色
	vertex[1].TexCoord = D3DXVECTOR2(0.0f, 1.0f);
	
	vertex[2].Position = D3DXVECTOR3(s_pos.x + s_size.x, s_pos.y - s_size.y, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = Color;
	vertex[2].TexCoord = D3DXVECTOR2(1.0f, 0.0f);
	
	vertex[3].Position = D3DXVECTOR3(s_pos.x  + s_size.x, s_pos.y + s_size.y, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = Color;
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);
	
	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);


	if (!isUse) return;


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

}

void Polygon2D::SetColor(D3DXVECTOR4 s_color) {

	Color = s_color;

}

void Polygon2D::SetTexture(unsigned short texId) {

	m_Texture = Texture_GetTexture(texId);
}

//////////texString//////////////////////
void TexStringJapanese::Draw() {

	writestring::SetColor(Color);
	writestring::JapaneseFont_Draw(m_Position.x, m_Position.y,string,size.x,size.y);
	writestring::SetColorDefault();

}

void TexStringAlphabet::Draw() {

	writestring::SetColor(Color);
	writestring::AlphabetFont_Draw(m_Position.x, m_Position.y, string, size.x, size.y);
	writestring::SetColorDefault();

}

void TexString::Draw() {

	writestring::SetColor(Color);
	writestring::JPNandALP_FontDraw(m_Position.x, m_Position.y, string, size.x, size.y);
	writestring::SetColorDefault();

}
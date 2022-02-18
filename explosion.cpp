#include "renderer.h"
#include "explosion.h"
#include "key_manager.h"
#include "billboard.h"
#include "imageManager.h"
#include "texture.h"

void Explosion::Init(D3DXVECTOR3 s_pos, float s_size, unsigned short texId) {

	s_size /= 2;


	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(- s_size, - s_size, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);//色
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(+ s_size, - s_size, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(- s_size, + s_size, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(+ s_size, + s_size, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);


	//頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd,sizeof(bd));
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

	Renderer::CreateVertexShader(&m_InstancingVertexShader, &m_InstancingVertxeLayout, "instancingShaderVS.cso");
	Renderer::CreatePixelShader(&m_InstancingPixelShader, "instancingShaderPS.cso");


	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	m_Position = s_pos;
	size = s_size;

	life = 0;


	tcx = 0;
	tcy = 0;

	SetrenSet();

	color = {1,1,1,1};

}

void Explosion::Init(D3DXVECTOR3 s_pos, D3DXVECTOR2 s_size, unsigned short texId) {

	s_size /= 2;


	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(-s_size.x, -s_size.y, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);//色
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(+s_size.x, -s_size.y, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(-s_size.x, +s_size.y, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(+s_size.x, +s_size.y, 0.0f);
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

	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	m_Position = s_pos;
	size = s_size.x;
	v2Size = s_size;

	life = 0;


	tcx = 0;
	tcy = 0;

	SetrenSet();

	color = { 1,1,1,1 };

}

void Explosion::Uninit() {

	//m_VertexBuffer->Release();

	if (m_VertexBuffer != nullptr) m_VertexBuffer->Release();

	if (m_VertxeLayout != nullptr) m_VertxeLayout->Release();
	if (m_VertexShader != nullptr)m_VertexShader->Release();
	if (m_PixelShader != nullptr) m_PixelShader->Release();

}

void Explosion::Update() {


	if(life > eLife) SetDestroy();
	else life++;

	//横部分の総フレームを計算
	int x_time = tex1_scale * eNumX;


	tcx = ((life / tex1_scale) % eNumX) * tcw;
	tcy = (life / x_time) * tch;




}

void Explosion::Draw() {

	//ブロック
	if (tcy >= 1 || tcx >= 1) return;

	float s_sizeX, s_sizeY;
	if (v2Size.x != 0) {
		s_sizeX = v2Size.x;
		s_sizeY = v2Size.y;
	}
	else {
		s_sizeX = size;
		s_sizeY = size;
	}
	D3DXVECTOR2 s_pos = { m_Position.x,m_Position.z };

	//Billboard::BillBorad_Draw(TEX_EXPLOSION, { 12,0,12 },size ,size , 0, tcx, tcy, tcw, tch);


	//頂点データ書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	
	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;


	vertex[0].Position = D3DXVECTOR3(- s_sizeX,  s_sizeY, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = color;//色
	vertex[0].TexCoord = D3DXVECTOR2(tcx, tcy + tch);
	
	vertex[1].Position = D3DXVECTOR3(+ s_sizeX, + s_sizeY, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = color;
	vertex[1].TexCoord = D3DXVECTOR2(tcx + tcw, tcy + tch);
	
	vertex[2].Position = D3DXVECTOR3(- s_sizeX,-s_sizeY, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = color;
	vertex[2].TexCoord = D3DXVECTOR2(tcx, tcy);
	
	vertex[3].Position = D3DXVECTOR3(+ s_sizeX, -s_sizeY, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = color;
	vertex[3].TexCoord = D3DXVECTOR2(tcx + tcw, tcy);

	
	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer,0);
	
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertxeLayout);
	
	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);
	
	//マトリクス設定
	//Renderer::SetWorldViewProjection2D();
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);
	
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
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);
	
	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	SetrenSet();
	
	Billboard::BillBorad_Draw(GetRendererSet(), m_Position);
	/*
	//Billboard::BillBorad_Draw(GetRendererSet(), m_Position,m_Rotation);

	//下テスト中//

	D3DXVECTOR3 s_offsetPos[1024], s_rot[1024];

	for (int i = 0; i < 32; i++) {
		for (int j = 0; j < 32; j++) {
			s_offsetPos[i* 32 + j] = D3DXVECTOR3((j - 16) * 0.1f,(i - 16) * 0.1f, 0);
			//s_rot[i * 32 + j] = D3DXVECTOR3(0, 0, 0.1f * i * j);
			s_rot[i * 32 + j] = D3DXVECTOR3(0, 0,0);
	
		}
	}



	SetrenSet_Shader(m_InstancingPixelShader, m_InstancingVertexShader, m_InstancingVertxeLayout);

	Billboard::BillBorad_InstanceDraw(GetRendererSet(), m_Position, s_offsetPos, s_rot,1024);

	*/
}




void Explosion::InstancingDraw() {

	//ブロック
	if (tcy >= 1 || tcx >= 1) return;

	float s_sizeX, s_sizeY;
	if (v2Size.x != 0) {
		s_sizeX = v2Size.x;
		s_sizeY = v2Size.y;
	}
	else {
		s_sizeX = size;
		s_sizeY = size;
	}
	D3DXVECTOR2 s_pos = { m_Position.x,m_Position.z };

	//Billboard::BillBorad_Draw(TEX_EXPLOSION, { 12,0,12 },size ,size , 0, tcx, tcy, tcw, tch);


	//頂点データ書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;


	vertex[0].Position = D3DXVECTOR3(-s_sizeX, s_sizeY, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);//色
	vertex[0].TexCoord = D3DXVECTOR2(tcx, tcy + tch);

	vertex[1].Position = D3DXVECTOR3(+s_sizeX, +s_sizeY, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(tcx + tcw, tcy + tch);

	vertex[2].Position = D3DXVECTOR3(-s_sizeX, -s_sizeY, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(tcx, tcy);

	vertex[3].Position = D3DXVECTOR3(+s_sizeX, -s_sizeY, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(tcx + tcw, tcy);


	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertxeLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	//Renderer::SetWorldViewProjection2D();
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

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
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	SetrenSet();

	//Billboard::BillBorad_Draw(GetRendererSet(), m_Position);
	//Billboard::BillBorad_Draw(GetRendererSet(), m_Position, m_Rotation);

	D3DXVECTOR3 s_offsetPos[1024],s_rot[1024];

	for (int i = 0; i < 1024; i++) {
		s_offsetPos[i] = D3DXVECTOR3(i * 0.1f,0,0);
		s_rot[i] = D3DXVECTOR3(0, 0, i * 0.1f);
	}

	SetrenSet_Shader(m_InstancingPixelShader, m_InstancingVertexShader, m_InstancingVertxeLayout);

	D3DXVECTOR4 s_color[INSTANCING_NUM];
	for (int i = 0; i < INSTANCING_NUM; i++) s_color[i] = {1,1,1,1};
	Billboard::BillBorad_InstanceDraw(GetRendererSet(), m_Position, s_offsetPos, s_rot,s_color,1024);


}
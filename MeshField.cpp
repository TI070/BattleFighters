
#include "MeshField.h"
#include "texture.h"


float g_FieldHeight[MESHFIELD_X][MESHFIELD_Z] =
{
	{0.0f,5.0f,1.0f,5.0f,1.5f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,5.0f,1.0f,5.0f,1.5f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,6.0f,1.0f,6.0f,2.5f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,1.0f,2.0f,2.0f,2.0f,1.0f,0.0f,0.0f,0.0f},
	{0.0f,6.0f,1.0f,6.0f,2.5f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,1.0f,2.0f,3.0f,3.0f,3.0f,2.0f,1.0f,0.0f,0.0f},
	{0.0f,6.0f,1.0f,6.0f,2.5f,1.0f,1.0f,0.0f,0.0f,0.0f,1.0f,1.0f,1.0f,2.0f,3.0f,3.0f,3.0f,2.0f,1.0f,0.0f,0.0f},
	{0.0f,6.0f,1.0f,6.0f,2.5f,1.0f,1.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,1.0f,2.0f,2.0f,2.0f,1.0f,0.0f,0.0f,0.0f},
	{0.0f,7.0f,1.0f,7.0f,2.5f,2.0f,1.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,1.5f,1.5f,1.5f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,7.0f,1.0f,7.0f,3.5f,2.0f,1.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,8.0f,1.0f,8.0f,3.5f,2.0f,1.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,8.0f,1.0f,8.0f,3.5f,2.0f,1.0f,0.0f,0.0f,0.0f,1.0f,1.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,8.0f,1.0f,8.0f,3.5f,2.0f,1.0f,0.0f,0.0f,0.0f,1.0f,2.0f,1.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,8.0f,1.0f,8.0f,3.5f,2.0f,1.0f,0.0f,0.0f,0.0f,1.0f,2.0f,2.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,8.0f,1.0f,8.0f,3.5f,2.0f,1.0f,0.0f,0.0f,0.0f,3.0f,3.0f,2.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,7.0f,1.0f,7.0f,3.5f,2.0f,1.0f,0.0f,0.0f,0.0f,3.0f,3.0f,3.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,7.0f,1.0f,7.0f,3.5f,2.0f,1.0f,0.0f,0.0f,0.0f,3.0f,4.0f,3.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,7.0f,1.0f,7.0f,2.5f,2.0f,1.0f,0.0f,0.0f,0.0f,3.0f,3.0f,3.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,6.0f,1.0f,6.0f,2.5f,1.0f,1.0f,0.0f,0.0f,0.0f,2.0f,3.0f,2.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,6.0f,1.0f,6.0f,2.5f,1.0f,0.0f,0.0f,0.0f,0.0f,2.0f,2.0f,2.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,6.0f,1.0f,6.0f,2.5f,1.0f,0.0f,0.0f,0.0f,0.0f,2.0f,2.0f,2.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,5.0f,1.0f,5.0f,2.5f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,1.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,5.0f,1.0f,5.0f,1.5f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f}


};



void MeshField::Init() {

	for (int x = 0; x <= 20; x++) {
		for (int z = 0; z <= 20; z++) {
			m_Vertex[x][z].Position = D3DXVECTOR3((x - 10) * 5.0f, g_FieldHeight[z][x], (z - 10) * -5.0f);
			m_Vertex[x][z].Normal   = D3DXVECTOR3(0.0f,1.0f,0.0f);
			m_Vertex[x][z].Diffuse  = D3DXVECTOR4(1.0f,1.0f,1.0f,1.0f);
			m_Vertex[x][z].TexCoord = D3DXVECTOR2(x * 0.5f,z * 0.5f);

		}
	}

	//法線ベクトル算出
	for (int x = 1; x <= 19; x++) {

		for (int z = 1; z <= 19; z++) {

			D3DXVECTOR3 vx, vn, vz;

			vx = m_Vertex[x + 1][z].Position - m_Vertex[x - 1][z].Position;
			vz = m_Vertex[x][z - 1].Position - m_Vertex[x][z + 1].Position;

			D3DXVec3Cross(&vn,&vz,&vx);//外積
			D3DXVec3Normalize(&vn,&vn);//正規化
			m_Vertex[x][z].Normal = vn;

		}


	}

	//頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 21 * 21;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	ZeroMemory(&sd,sizeof(sd));
	sd.pSysMem = m_Vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);


	//インデックスバッファ生成

	unsigned int index[(22 * 2) * 20 - 2];

	int i = 0;
	for (int x = 0; x < 20; x++) {

		for (int z = 0; z < 21; z++) {

			index[i] = x * 21 + z;
			i++;

			index[i] = (x + 1) * 21 + z;
			i++;
		}

		if (x == 19) break;

		//縮退ポリゴン用インデックス
		index[i] = (x + 1) * 21 + 20;
		i++;

		index[i] = (x + 1) * 21;
		i++;

	}


	//インデックスバッファ生成
	//D3D11_BUFFER_DESC bd{};
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(unsigned int) * (22 * 2) * 20 - 2;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	//D3D11_SUBRESOURCE_DATA sd{};
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = index;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);




	//テクスチャ読込
	//D3DX11CreateShaderResourceViewFromFile(
	//	Renderer::GetDevice(),
	//	"asset/texture/paintblock.png",
	//	NULL,
	//	NULL,
	//	&m_Texture,
	//	NULL
	//);

	m_Texture = Texture_GetTexture(ImageManager::GetTexId(TEX_PAINTBLOCK));

	//エラーチェック
	assert(m_Texture);//中身がNULLだとエラーを出すらしい

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertxeLayout, "vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");

	//m_VertexShader = RendererManager::GetlitVS();
	//m_VertxeLayout = RendererManager::GetlitIL();
	//m_PixelShader = RendererManager::GetlitPS();

	//m_Position = s_pos;




	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);


}

void MeshField::Uninit() {

	if (m_VertexBuffer != nullptr) m_VertexBuffer->Release();
	//m_Texture->Release();

	m_VertxeLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	m_IndexBuffer->Release();
}

void MeshField::Update() {


}

void MeshField::Draw() {

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

	//インデックスバッファ設定
	Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer,DXGI_FORMAT_R32_UINT,0);


	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン描画
	//Renderer::GetDeviceContext()->Draw(4, 0);
	Renderer::GetDeviceContext()->DrawIndexed((22 * 2) * 20 - 2,0,0);

}

/*
float g_FieldHeight[5][21] =
{
	{0.0f,5.0f,1.0f,5.0f,1.5f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,5.0f,1.0f,5.0f,1.5f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,6.0f,1.0f,6.0f,2.5f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,1.0f,2.0f,2.0f,2.0f,1.0f,0.0f,0.0f,0.0f},
	{0.0f,6.0f,1.0f,6.0f,2.5f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,1.0f,2.0f,3.0f,3.0f,3.0f,2.0f,1.0f,0.0f,0.0f},
	{0.0f,6.0f,1.0f,6.0f,2.5f,1.0f,1.0f,0.0f,0.0f,0.0f,1.0f,1.0f,1.0f,2.0f,3.0f,3.0f,3.0f,2.0f,1.0f,0.0f,0.0f},

};



void MeshField::Init() {

	for (int x = 0; x <= MESHFIELD_X - 1; x++) {
		for (int z = 0; z <= MESHFIELD_Z - 1; z++) {
			m_Vertex[x][z].Position = D3DXVECTOR3((x - 10) * 5.0f, g_FieldHeight[z][x], (z - 10) * -5.0f);
			m_Vertex[x][z].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			m_Vertex[x][z].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			m_Vertex[x][z].TexCoord = D3DXVECTOR2(x * 0.5f, z * 0.5f);

		}
	}

	//法線ベクトル算出
	for (int x = 1; x <= MESHFIELD_X - 2; x++) {

		for (int z = 1; z <= MESHFIELD_X - 2; z++) {

			D3DXVECTOR3 vx, vn, vz;

			vx = m_Vertex[x + 1][z].Position - m_Vertex[x - 1][z].Position;
			vz = m_Vertex[x][z - 1].Position - m_Vertex[x][z + 1].Position;

			D3DXVec3Cross(&vn, &vz, &vx);//外積
			D3DXVec3Normalize(&vn, &vn);//正規化
			m_Vertex[x][z].Normal = vn;

		}


	}

	//頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * MESHFIELD_X * MESHFIELD_Z;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = m_Vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);


	//インデックスバッファ生成

	unsigned int index[((MESHFIELD_X + 1) * 2) * (MESHFIELD_Z - 1) - 2];

	int i = 0;
	for (int x = 0; x < MESHFIELD_X - 1; x++) {

		for (int z = 0; z < MESHFIELD_Z; z++) {

			index[i] = x * MESHFIELD_X + z;
			i++;

			index[i] = (x + 1) * MESHFIELD_X + z;
			i++;
		}

		if (x == MESHFIELD_X - 2) break;

		//縮退ポリゴン用インデックス
		index[i] = (x + 1) * MESHFIELD_X + (MESHFIELD_Z - 1);
		i++;

		index[i] = (x + 1) * MESHFIELD_X;
		i++;

	}


	//インデックスバッファ生成
	//D3D11_BUFFER_DESC bd{};
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(unsigned int) * (MESHFIELD_X * 2) * MESHFIELD_Z - 2;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	//D3D11_SUBRESOURCE_DATA sd{};
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = index;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);




	//テクスチャ読込
	//D3DX11CreateShaderResourceViewFromFile(
	//	Renderer::GetDevice(),
	//	"asset/texture/paintblock.png",
	//	NULL,
	//	NULL,
	//	&m_Texture,
	//	NULL
	//);

	m_Texture = Texture_GetTexture(ImageManager::GetTexId(TEX_PAINTBLOCK));

	//エラーチェック
	assert(m_Texture);//中身がNULLだとエラーを出すらしい

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertxeLayout, "vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");

	//m_VertexShader = RendererManager::GetlitVS();
	//m_VertxeLayout = RendererManager::GetlitIL();
	//m_PixelShader = RendererManager::GetlitPS();

	//m_Position = s_pos;




	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);


}

void MeshField::Uninit() {

	if (m_VertexBuffer != nullptr) m_VertexBuffer->Release();
	//m_Texture->Release();

	m_VertxeLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	m_IndexBuffer->Release();
}

void MeshField::Update() {


}

void MeshField::Draw() {

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

	//インデックスバッファ設定
	Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);


	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン描画
	//Renderer::GetDeviceContext()->Draw(4, 0);
	Renderer::GetDeviceContext()->DrawIndexed((MESHFIELD_X * 2) * MESHFIELD_Z - 2, 0, 0);

}


*/

float MeshField::GetHeight(D3DXVECTOR3 Position) {

	int x, z;

	//ブロック番号算出
	x = Position.x / 5.0f + 10.0f;
	z = Position.z / -5.0f + 10.0f;


	D3DXVECTOR3 pos0, pos1, pos2, pos3;

	pos0 = m_Vertex[x][z].Position;
	pos1 = m_Vertex[x + 1][z].Position;
	pos2 = m_Vertex[x][z + 1].Position;
	pos3 = m_Vertex[x + 1][z + 1].Position;

	D3DXVECTOR3 v12, v1p, c;

	v12 = pos2 - pos1;
	v1p = Position - pos1;

	D3DXVec3Cross(&c, &v12, &v1p);//外積から左上ポリゴンか右下ポリゴン化を判別

	float py;
	D3DXVECTOR3 n;


	if (c.y > 0.0f) {

		//左上ポリゴン
		D3DXVECTOR3 v10;
		v10 = pos0 - pos1;
		D3DXVec3Cross(&n, &v10, &v12);

	}
	else {
		//右下ポリゴン
		D3DXVECTOR3 v13;
		v13 = pos3 - pos1;
		D3DXVec3Cross(&n, &v12, &v13);

	}

	//高さ取得
	py = -((Position.x - pos1.x) * n.x + (Position.z - pos1.z) * n.z) / n.y + pos1.y;

	return py;
}
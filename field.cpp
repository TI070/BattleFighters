
#include "field.h"
#include "renderer.h"
#include "rendererManager.h"
#include "texture.h"

///////////////////////////////Field�N���X//////////////////////////////

void Field::Init(D3DXVECTOR3 s_pos, float s_size, const char* textureName) {

	s_size /= 2;

	VERTEX_3D vertex[25];

	/*
	vertex[0].Position = D3DXVECTOR3(s_pos.x - s_size, 0.0f, s_pos.y + s_size);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);//�F
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(s_pos.x + s_size, 0.0f, s_pos.y + s_size);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(s_pos.x - s_size, 0.0f, s_pos.y - s_size);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(s_pos.x + s_size, 0.0f, s_pos.y - s_size);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);
	*/
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			vertex[i * 5 + j].Position = D3DXVECTOR3(-s_size + 2 * s_size *j / 4.0f, -s_size + 2 * s_size * i / 4.0f, s_size);
			vertex[i * 5 + j].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			vertex[i * 5 + j].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);//�F
			vertex[i * 5 + j].TexCoord = D3DXVECTOR2(s_size * j / 4.0f, s_size * i / 4.0f);
		}
	}

	//vertex[0].Position = D3DXVECTOR3(- s_size, 0.0f, s_size);
	//vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);//�F
	//vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
	//
	//vertex[1].Position = D3DXVECTOR3(s_size, 0.0f, s_size);
	//vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	//vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);
	//
	//vertex[2].Position = D3DXVECTOR3(- s_size, 0.0f, - s_size);
	//vertex[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	//vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);
	//
	//vertex[3].Position = D3DXVECTOR3( s_size, 0.0f,- s_size);
	//vertex[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	//vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@����
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 25;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	//�e�N�X�`���Ǎ�
	D3DX11CreateShaderResourceViewFromFile(
		Renderer::GetDevice(),
		textureName,
		NULL,
		NULL,
		&m_Texture,
		NULL
	);

	//m_Texture = Texture_GetTexture(ImageManager::GetTexId(TEX_FIELD_GRASS));

	//�G���[�`�F�b�N
	assert(m_Texture);//���g��NULL���ƃG���[���o���炵��

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertxeLayout, "vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");

	//m_VertexShader = RendererManager::GetlitVS();
	//m_VertxeLayout = RendererManager::GetlitIL();
	//m_PixelShader = RendererManager::GetlitPS();

	m_Position = s_pos;
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);


}

void Field::Init(D3DXVECTOR3 s_pos, float s_size, unsigned short texId) {

	s_size /= 2;

	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(-s_size, 0.0f, s_size);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);//�F
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(s_size, 0.0f, s_size);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(-s_size, 0.0f, -s_size);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(s_size, 0.0f, -s_size);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@����
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);


	m_Texture = Texture_GetTexture(texId);

	//�G���[�`�F�b�N
	assert(m_Texture);//���g��NULL���ƃG���[���o���炵��

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertxeLayout, "vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");

	//m_VertexShader = RendererManager::GetlitVS();
	//m_VertxeLayout = RendererManager::GetlitIL();
	//m_PixelShader = RendererManager::GetlitPS();

	m_Position = s_pos;
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);


}

void Field::Uninit() {

	if(m_VertexBuffer != nullptr) m_VertexBuffer->Release();
	//m_Texture->Release();

	//m_VertxeLayout->Release();
	//m_VertexShader->Release();
	//m_PixelShader->Release();

}

void Field::Update() {


}

void Field::Draw() {

	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertxeLayout);

	//�V�F�[�_�[�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//�}�g���N�X�ݒ�
	//Renderer::SetWorldViewProjection2D();
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale,m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot,m_Rotation.y,m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans,m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;

	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//�}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material,sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	//�e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//�v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);


}

bool Field::CheckPos(D3DXVECTOR3 s_pos) {
	
	if (m_Position == s_pos) return true;
	return false;

}


///////////////////////////////SelecrtTile�N���X//////////////////////////////
/*
void SelecrtTile::Init(Player* pPlayer, float s_size, const char* textureName) {

	D3DXVECTOR3 s_pos;

	pP = pPlayer;

	s_pos = pPlayer->GetPosition();

	s_size /= 2;

	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(-s_size, 0.0f, s_size);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 0.5f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(s_size, 0.0f, s_size);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 0.5f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(-s_size, 0.0f, -s_size);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 0.5f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(s_size, 0.0f, -s_size);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 0.5f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);


	//���_�o�b�t�@����
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	//�e�N�X�`���Ǎ�
	D3DX11CreateShaderResourceViewFromFile(
		Renderer::GetDevice(),
		textureName,
		NULL,
		NULL,
		&m_Texture,
		NULL
	);



	//�G���[�`�F�b�N
	assert(m_Texture);//���g��NULL���ƃG���[���o���炵��

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertxeLayout, "vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");

	m_Position = D3DXVECTOR3(s_pos.x, 0.0f, s_pos.z);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	size = s_size;

}

void SelecrtTile::Update() {

	D3DXVECTOR3 s_pos;

	s_pos = pP->GetPosition();

	//���W���^�C������̑傫�����Œ�P�ʂƂ��Ĉ���
	s_pos.x = (int)s_pos.x;
	s_pos.y = (int)s_pos.y;
	s_pos.z = (int)s_pos.z;


	float s_size = size;

	m_Position = D3DXVECTOR3(s_pos.x, 0.01f, s_pos.z);



}

///////////////////////////////Grass�N���X//////////////////////////////
void Grass::Init(D3DXVECTOR3 s_pos, float s_size, unsigned short texId) {

	Field::Init(s_pos,s_size,texId);
	FieldManager::SetFieldData(m_Position, GRASS);

}


///////////////////////////////Road�N���X//////////////////////////////

void Road::Init(D3DXVECTOR3 s_pos, float s_size, const char* textureName) {

	Field::Init(s_pos, s_size, textureName);
	FieldManager::SetFieldData(m_Position, ROAD);

}

void Road::Update() {

	//����̓��ڑ��󋵂ŕ\����ς��������

}

void Road::ChangeTex() {

	
	///const char* textureName = FIELDTILE_ROAD_0_PASS;
	///
	///if (roadType == NOT_CONNECTION) textureName = FIELDTILE_ROAD_0_PASS;
	///else if (roadType == HORIZON)textureName = FIELDTILE_ROAD_1_PASS;
	///else if (roadType == VERTICAL)textureName = FIELDTILE_ROAD_2_PASS;
	///else if (roadType == HORIZON_FRONT)textureName = FIELDTILE_ROAD_3_PASS;
	///else if (roadType == VERTICAL_RIGHT)textureName = FIELDTILE_ROAD_4_PASS;
	///else if (roadType == HORIZON_BACK)textureName = FIELDTILE_ROAD_5_PASS;
	///else if (roadType == VERTICAL_LEFT)textureName = FIELDTILE_ROAD_6_PASS;
	///else if (roadType == CROSS)textureName = FIELDTILE_ROAD_7_PASS;
	///
	///else if (roadType == RT_BACK)textureName = FIELDTILE_ROAD_8_PASS;
	///else if (roadType == RT_LEFT)textureName = FIELDTILE_ROAD_9_PASS;
	///else if (roadType == RT_FRONT)textureName = FIELDTILE_ROAD_10_PASS;
	///else if (roadType == RT_RIGHT)textureName = FIELDTILE_ROAD_11_PASS;
	///else if (roadType == BACK_RIGHT)textureName = FIELDTILE_ROAD_12_PASS;
	///else if (roadType == BACK_LEFT)textureName = FIELDTILE_ROAD_13_PASS;
	///else if (roadType == FRONT_LEFT)textureName = FIELDTILE_ROAD_14_PASS;
	///else if (roadType == FRONT_RIGHT)textureName = FIELDTILE_ROAD_15_PASS;
	

	unsigned short texid = TEX_FIELD_ROAD_0 + roadType;

	m_Texture = Texture_GetTexture(texid);

	//�e�N�X�`���Ǎ�
	//D3DX11CreateShaderResourceViewFromFile(
	//	Renderer::GetDevice(),
	//	textureName,
	//	NULL,
	//	NULL,
	//	&m_Texture,
	//	NULL
	//);

	//char buf[64];
	//sprintf(buf, "rT = %d\n", roadType);
	//OutputDebugString(buf);


}

void Road::CheckAroundRoad() {

	roadType = NOT_CONNECTION;

	bool CheckF[DIRECTION_MAX] = {0,0,0,0};

	D3DXVECTOR3 s_pos = m_Position;

	////�㉺���E�ɓ������邩�m�F
	//��
	s_pos.z += DEFAULT_FIELDTILE_SIZE;
	if (FieldManager::CheckFieldClass<Road>(s_pos)) CheckF[FRONT] = true;
	//��
	s_pos = m_Position;
	s_pos.z -= DEFAULT_FIELDTILE_SIZE;
	if (FieldManager::CheckFieldClass<Road>(s_pos)) CheckF[BACK] = true;
	//�E
	s_pos = m_Position;
	s_pos.x += DEFAULT_FIELDTILE_SIZE;
	if (FieldManager::CheckFieldClass<Road>(s_pos)) CheckF[RIGHT] = true;
	//��
	s_pos = m_Position;
	s_pos.x -= DEFAULT_FIELDTILE_SIZE;
	if (FieldManager::CheckFieldClass<Road>(s_pos)) CheckF[LEFT] = true;


	//for (int i = 0; i < 4;i++) {
	//	char buf[64];
	//	sprintf(buf, "CF = %d\n", CheckF[i]);
	//	OutputDebugString(buf);
	//}

	////�t���O�ɂ���ă^�C�v��ς���

	//0
	if (!CheckF[FRONT] && !CheckF[BACK] && !CheckF[RIGHT] && !CheckF[LEFT]) roadType = NOT_CONNECTION;

	//1
	else if ( CheckF[FRONT] && !CheckF[BACK] && !CheckF[RIGHT] && !CheckF[LEFT]) roadType = RT_FRONT;
	else if (!CheckF[FRONT] &&  CheckF[BACK] && !CheckF[RIGHT] && !CheckF[LEFT]) roadType = RT_BACK;
	else if (!CheckF[FRONT] && !CheckF[BACK] &&  CheckF[RIGHT] && !CheckF[LEFT]) roadType = RT_RIGHT;
	else if (!CheckF[FRONT] && !CheckF[BACK] && !CheckF[RIGHT] &&  CheckF[LEFT]) roadType = RT_LEFT;
	
	//2
	else if ( CheckF[FRONT] &&  CheckF[BACK] && !CheckF[RIGHT] && !CheckF[LEFT]) roadType = VERTICAL;
	else if ( CheckF[FRONT] && !CheckF[BACK] &&  CheckF[RIGHT] && !CheckF[LEFT]) roadType = FRONT_RIGHT;
	else if ( CheckF[FRONT] && !CheckF[BACK] && !CheckF[RIGHT] &&  CheckF[LEFT]) roadType = FRONT_LEFT;

	else if (!CheckF[FRONT] &&  CheckF[BACK] &&  CheckF[RIGHT] && !CheckF[LEFT]) roadType = BACK_RIGHT;
	else if (!CheckF[FRONT] &&  CheckF[BACK] && !CheckF[RIGHT] &&  CheckF[LEFT]) roadType = BACK_LEFT;
	
	else if (!CheckF[FRONT] && !CheckF[BACK] &&  CheckF[RIGHT] && CheckF[LEFT]) roadType = HORIZON;
	
	//3
	else if (!CheckF[FRONT] &&  CheckF[BACK] && CheckF[RIGHT] &&  CheckF[LEFT]) roadType = HORIZON_BACK;
	else if ( CheckF[FRONT] && !CheckF[BACK] && CheckF[RIGHT] &&  CheckF[LEFT]) roadType = HORIZON_FRONT;
	else if ( CheckF[FRONT] &&  CheckF[BACK] &&!CheckF[RIGHT] &&  CheckF[LEFT]) roadType = VERTICAL_LEFT;
	else if ( CheckF[FRONT] &&  CheckF[BACK] && CheckF[RIGHT] && !CheckF[LEFT]) roadType = VERTICAL_RIGHT;
    
	//4
	else if ( CheckF[FRONT] &&  CheckF[BACK] &&  CheckF[RIGHT] && CheckF[LEFT]) roadType = CROSS;

	//�e�N�X�`���ݒ�
	ChangeTex();


	//���͂S�����̃e�N�X�`���A�b�v�f�[�g
	//��
	s_pos = m_Position;
	s_pos.z += DEFAULT_FIELDTILE_SIZE;
	FieldManager::UpdateFieldTex(s_pos);
	//��
	s_pos = m_Position;
	s_pos.z -= DEFAULT_FIELDTILE_SIZE;
	FieldManager::UpdateFieldTex(s_pos);
	//�E
	s_pos = m_Position;
	s_pos.x += DEFAULT_FIELDTILE_SIZE;
	FieldManager::UpdateFieldTex(s_pos);
	//��
	s_pos = m_Position;
	s_pos.x -= DEFAULT_FIELDTILE_SIZE;
	FieldManager::UpdateFieldTex(s_pos);



}

void Road::CheckFieldType() {

	bool CheckF[DIRECTION_MAX] = { 0,0,0,0 };

	D3DXVECTOR3 s_pos = m_Position;

	////�㉺���E�ɓ������邩�m�F
	//��
	s_pos.z += DEFAULT_FIELDTILE_SIZE;
	if (FieldManager::CheckFieldClass<Road>(s_pos)) CheckF[FRONT] = true;
	//��
	s_pos = m_Position;
	s_pos.z -= DEFAULT_FIELDTILE_SIZE;
	if (FieldManager::CheckFieldClass<Road>(s_pos)) CheckF[BACK] = true;
	//�E
	s_pos = m_Position;
	s_pos.x += DEFAULT_FIELDTILE_SIZE;
	if (FieldManager::CheckFieldClass<Road>(s_pos)) CheckF[RIGHT] = true;
	//��
	s_pos = m_Position;
	s_pos.x -= DEFAULT_FIELDTILE_SIZE;
	if (FieldManager::CheckFieldClass<Road>(s_pos)) CheckF[LEFT] = true;


	//for (int i = 0; i < 4; i++) {
	//	char buf[64];
	//	sprintf(buf, "CF = %d\n", CheckF[i]);
	//	OutputDebugString(buf);
	//}

	//////�t���O�ɂ���ă^�C�v��ς���
	////��
	//if (CheckF[FRONT] && !CheckF[BACK] && !CheckF[RIGHT] && !CheckF[LEFT]) roadType = VERTICAL;
	//else if (CheckF[FRONT] && CheckF[BACK] && !CheckF[RIGHT] && !CheckF[LEFT]) roadType = VERTICAL;
	//else if (CheckF[FRONT] && CheckF[BACK] && CheckF[RIGHT] && !CheckF[LEFT]) roadType = VERTICAL_RIGHT;
	//else if (CheckF[FRONT] && CheckF[BACK] && !CheckF[RIGHT] && CheckF[LEFT]) roadType = VERTICAL_LEFT;
	////��
	//else if (!CheckF[FRONT] && CheckF[BACK] && !CheckF[RIGHT] && !CheckF[LEFT]) roadType = VERTICAL;
	//else if (CheckF[FRONT] && CheckF[BACK] && !CheckF[RIGHT] && !CheckF[LEFT]) roadType = VERTICAL;
	//else if (!CheckF[FRONT] && CheckF[BACK] && CheckF[RIGHT] && !CheckF[LEFT]) roadType = VERTICAL_RIGHT;
	//else if (!CheckF[FRONT] && CheckF[BACK] && !CheckF[RIGHT] && CheckF[LEFT]) roadType = VERTICAL_LEFT;
	////�E
	//else if (!CheckF[FRONT] && !CheckF[BACK] && CheckF[RIGHT] && !CheckF[LEFT]) roadType = HORIZON;
	//else if (!CheckF[FRONT] && !CheckF[BACK] && CheckF[RIGHT] && CheckF[LEFT]) roadType = HORIZON;
	//else if (CheckF[FRONT] && !CheckF[BACK] && CheckF[RIGHT] && !CheckF[LEFT]) roadType = HORIZON_FRONT;
	//else if (!CheckF[FRONT] && !CheckF[BACK] && CheckF[RIGHT] && !CheckF[LEFT]) roadType = HORIZON_BACK;
	////��
	//else if (!CheckF[FRONT] && !CheckF[BACK] && !CheckF[RIGHT] && CheckF[LEFT]) roadType = HORIZON;
	//else if (!CheckF[FRONT] && !CheckF[BACK] && CheckF[RIGHT] && CheckF[LEFT]) roadType = HORIZON;
	//else if (CheckF[FRONT] && !CheckF[BACK] && !CheckF[RIGHT] && CheckF[LEFT]) roadType = HORIZON_FRONT;
	//else if (!CheckF[FRONT] && CheckF[BACK] && !CheckF[RIGHT] && CheckF[LEFT]) roadType = HORIZON_BACK;
	////�S
	//else if (CheckF[FRONT] && CheckF[BACK] && CheckF[RIGHT] && CheckF[LEFT]) roadType = CROSS;

		//0
	if (!CheckF[FRONT] && !CheckF[BACK] && !CheckF[RIGHT] && !CheckF[LEFT]) roadType = NOT_CONNECTION;

	//1
	else if (CheckF[FRONT] && !CheckF[BACK] && !CheckF[RIGHT] && !CheckF[LEFT]) roadType = RT_FRONT;
	else if (!CheckF[FRONT] && CheckF[BACK] && !CheckF[RIGHT] && !CheckF[LEFT]) roadType = RT_BACK;
	else if (!CheckF[FRONT] && !CheckF[BACK] && CheckF[RIGHT] && !CheckF[LEFT]) roadType = RT_RIGHT;
	else if (!CheckF[FRONT] && !CheckF[BACK] && !CheckF[RIGHT] && CheckF[LEFT]) roadType = RT_LEFT;

	//2
	else if (CheckF[FRONT] && CheckF[BACK] && !CheckF[RIGHT] && !CheckF[LEFT]) roadType = VERTICAL;
	else if (CheckF[FRONT] && !CheckF[BACK] && CheckF[RIGHT] && !CheckF[LEFT]) roadType = FRONT_RIGHT;
	else if (CheckF[FRONT] && !CheckF[BACK] && !CheckF[RIGHT] && CheckF[LEFT]) roadType = FRONT_LEFT;

	else if (!CheckF[FRONT] && CheckF[BACK] && CheckF[RIGHT] && !CheckF[LEFT]) roadType = BACK_RIGHT;
	else if (!CheckF[FRONT] && CheckF[BACK] && !CheckF[RIGHT] && CheckF[LEFT]) roadType = BACK_LEFT;

	else if (!CheckF[FRONT] && !CheckF[BACK] && CheckF[RIGHT] && CheckF[LEFT]) roadType = HORIZON;

	//3
	else if (!CheckF[FRONT] && CheckF[BACK] && CheckF[RIGHT] && CheckF[LEFT]) roadType = HORIZON_BACK;
	else if (CheckF[FRONT] && !CheckF[BACK] && CheckF[RIGHT] && CheckF[LEFT]) roadType = HORIZON_FRONT;
	else if (CheckF[FRONT] && CheckF[BACK] && !CheckF[RIGHT] && CheckF[LEFT]) roadType = VERTICAL_LEFT;
	else if (CheckF[FRONT] && CheckF[BACK] && CheckF[RIGHT] && !CheckF[LEFT]) roadType = VERTICAL_RIGHT;

	//4
	else if (CheckF[FRONT] && CheckF[BACK] && CheckF[RIGHT] && CheckF[LEFT]) roadType = CROSS;





}
*/
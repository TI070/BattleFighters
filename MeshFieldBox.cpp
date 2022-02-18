
#include "MeshFieldBox.h"
#include "texture.h"
#include "animationPlayer.h"
#include "stageSystem.h"



void MeshFieldBox::Init(D3DXVECTOR3 s_pivot, float s_pieceSize,float s_uvSize, float s_height, float s_zWidth) {

	m_Position = {0,0,0};
	pivot = s_pivot;
	pieceSize = s_pieceSize;
	uvSize = s_uvSize;
	height = s_height;
	zWidth = s_zWidth;


	char buf[64];


	for (int x = 0; x <= MESHFIELDBOX_X - 1; x++) {
		for (int z = 0; z <= MESHFIELDBOX_Z - 1; z++) {
			m_Vertex[x][z].Position = D3DXVECTOR3((x - 10) * pieceSize + pivot.x, FieldBoxHeight[z][x] + pivot.y, (z - 10) * -pieceSize * s_zWidth + pivot.z);
			m_Vertex[x][z].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			m_Vertex[x][z].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			m_Vertex[x][z].TexCoord = D3DXVECTOR2(x * uvSize, z * uvSize * s_zWidth);


			//�O��
			m_VertexFront[x][z].Position = D3DXVECTOR3((x - 10) * pieceSize + pivot.x, FieldBoxHeight[z][x] - z * pieceSize * height / MESHFIELDBOX_Z + pivot.y,pivot.z - pieceSize * 10 * s_zWidth);
			m_VertexFront[x][z].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			m_VertexFront[x][z].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			m_VertexFront[x][z].TexCoord = D3DXVECTOR2(x * uvSize, z * uvSize * height / MESHFIELDBOX_Z * s_zWidth);

			//�E��
			m_VertexRight[x][z].Position = D3DXVECTOR3(10 * pieceSize + pivot.x, FieldBoxHeight[z][x] - x * pieceSize * height / MESHFIELDBOX_Z + pivot.y, (z - 10) * -pieceSize * s_zWidth + pivot.z);
			m_VertexRight[x][z].Normal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
			m_VertexRight[x][z].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			m_VertexRight[x][z].TexCoord = D3DXVECTOR2(x * uvSize, z * uvSize * height / MESHFIELDBOX_Z * s_zWidth);

			//����
			m_VertexLeft[x][z].Position = D3DXVECTOR3(-10 * pieceSize + pivot.x, FieldBoxHeight[z][x] - z * pieceSize * height / MESHFIELDBOX_Z + pivot.y, (x - 10) * -pieceSize * s_zWidth + pivot.z);
			m_VertexLeft[x][z].Normal = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
			m_VertexLeft[x][z].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			m_VertexLeft[x][z].TexCoord = D3DXVECTOR2(x * uvSize, z * uvSize * height / MESHFIELDBOX_Z * s_zWidth);

			//���
			m_VertexBottom[x][z].Position = D3DXVECTOR3((10 - x) * pieceSize + pivot.x, FieldBoxHeight[z][x] + pivot.y - height * pieceSize, (z - 10) * -pieceSize * s_zWidth + pivot.z);
			m_VertexBottom[x][z].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			m_VertexBottom[x][z].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			m_VertexBottom[x][z].TexCoord = D3DXVECTOR2(x * uvSize, z * uvSize * s_zWidth);

		}
	}

	//�@���x�N�g���Z�o
	for (int x = 1; x <= MESHFIELDBOX_X - 2; x++) {

		for (int z = 1; z <= MESHFIELDBOX_Z - 2; z++) {

			D3DXVECTOR3 vx, vn, vz;

			vx = m_Vertex[x + 1][z].Position - m_Vertex[x - 1][z].Position;
			vz = m_Vertex[x][z - 1].Position - m_Vertex[x][z + 1].Position;

			D3DXVec3Cross(&vn, &vz, &vx);//�O��
			D3DXVec3Normalize(&vn, &vn);//���K��
			m_Vertex[x][z].Normal = vn;

		}

	}

	//���_�o�b�t�@����
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * MESHFIELDBOX_X * MESHFIELDBOX_Z;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = m_Vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);


	//�C���f�b�N�X�o�b�t�@����

	unsigned int index[((MESHFIELDBOX_X + 1) * 2) * (MESHFIELDBOX_Z - 1) - 2];

	int i = 0;
	for (int x = 0; x < MESHFIELDBOX_X - 1; x++) {

		for (int z = 0; z < MESHFIELDBOX_Z; z++) {

			index[i] = x * MESHFIELDBOX_X + z;
			i++;

			index[i] = (x + 1) * MESHFIELDBOX_X + z;
			i++;
		}

		if (x == MESHFIELDBOX_X - 2) break;

		//�k�ރ|���S���p�C���f�b�N�X
		index[i] = (x + 1) * MESHFIELDBOX_X + (MESHFIELDBOX_Z - 1);
		i++;

		index[i] = (x + 1) * MESHFIELDBOX_X;
		i++;

	}

	//�C���f�b�N�X�o�b�t�@����
	//D3D11_BUFFER_DESC bd{};
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(unsigned int) * (MESHFIELDBOX_X * 2) * MESHFIELDBOX_Z - 2;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	//D3D11_SUBRESOURCE_DATA sd{};
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = index;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);


	//�O��
	{
		//���_�o�b�t�@����
		D3D11_BUFFER_DESC bd{};
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * MESHFIELDBOX_X * MESHFIELDBOX_Z;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd{};
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_VertexFront;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBufferFront);


		//�C���f�b�N�X�o�b�t�@����

		unsigned int index[((MESHFIELDBOX_X + 1) * 2) * (MESHFIELDBOX_Z - 1) - 2];

		int i = 0;
		for (int x = 0; x < MESHFIELDBOX_X - 1; x++) {

			for (int z = 0; z < MESHFIELDBOX_Z; z++) {

				index[i] = x * MESHFIELDBOX_X + z;
				i++;

				index[i] = (x + 1) * MESHFIELDBOX_X + z;
				i++;
			}

			if (x == MESHFIELDBOX_X - 2) break;

			//�k�ރ|���S���p�C���f�b�N�X
			index[i] = (x + 1) * MESHFIELDBOX_X + (MESHFIELDBOX_Z - 1);
			i++;

			index[i] = (x + 1) * MESHFIELDBOX_X;
			i++;

		}


		//�C���f�b�N�X�o�b�t�@����
		//D3D11_BUFFER_DESC bd{};
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * (MESHFIELDBOX_X * 2) * MESHFIELDBOX_Z - 2;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		//D3D11_SUBRESOURCE_DATA sd{};
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = index;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBufferFront);


	}



	//�����܂őO��


	//�E��
	{
		//���_�o�b�t�@����
		D3D11_BUFFER_DESC bd{};
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * MESHFIELDBOX_X * MESHFIELDBOX_Z;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd{};
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_VertexRight;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBufferRight);


		//�C���f�b�N�X�o�b�t�@����

		unsigned int index[((MESHFIELDBOX_X + 1) * 2) * (MESHFIELDBOX_Z - 1) - 2];

		int i = 0;
		for (int x = 0; x < MESHFIELDBOX_X - 1; x++) {

			for (int z = 0; z < MESHFIELDBOX_Z; z++) {

				index[i] = x * MESHFIELDBOX_X + z;
				i++;

				index[i] = (x + 1) * MESHFIELDBOX_X + z;
				i++;
			}

			if (x == MESHFIELDBOX_X - 2) break;

			//�k�ރ|���S���p�C���f�b�N�X
			index[i] = (x + 1) * MESHFIELDBOX_X + (MESHFIELDBOX_Z - 1);
			i++;

			index[i] = (x + 1) * MESHFIELDBOX_X;
			i++;

		}


		//�C���f�b�N�X�o�b�t�@����
		//D3D11_BUFFER_DESC bd{};
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * (MESHFIELDBOX_X * 2) * MESHFIELDBOX_Z - 2;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		//D3D11_SUBRESOURCE_DATA sd{};
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = index;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBufferRight);


	}



	//�����܂ŉE��



	//����
	{
		//���_�o�b�t�@����
		D3D11_BUFFER_DESC bd{};
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * MESHFIELDBOX_X * MESHFIELDBOX_Z;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd{};
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_VertexLeft;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBufferLeft);


		//�C���f�b�N�X�o�b�t�@����

		unsigned int index[((MESHFIELDBOX_X + 1) * 2) * (MESHFIELDBOX_Z - 1) - 2];

		int i = 0;
		for (int x = 0; x < MESHFIELDBOX_X - 1; x++) {

			for (int z = 0; z < MESHFIELDBOX_Z; z++) {

				index[i] = x * MESHFIELDBOX_X + z;
				i++;

				index[i] = (x + 1) * MESHFIELDBOX_X + z;
				i++;
			}

			if (x == MESHFIELDBOX_X - 2) break;

			//�k�ރ|���S���p�C���f�b�N�X
			index[i] = (x + 1) * MESHFIELDBOX_X + (MESHFIELDBOX_Z - 1);
			i++;

			index[i] = (x + 1) * MESHFIELDBOX_X;
			i++;

		}


		//�C���f�b�N�X�o�b�t�@����
		//D3D11_BUFFER_DESC bd{};
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * (MESHFIELDBOX_X * 2) * MESHFIELDBOX_Z - 2;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		//D3D11_SUBRESOURCE_DATA sd{};
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = index;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBufferLeft);


	}
	//�����܂ō���

	//���
	{
		//���_�o�b�t�@����
		D3D11_BUFFER_DESC bd{};
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * MESHFIELDBOX_X * MESHFIELDBOX_Z;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd{};
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_VertexBottom;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBufferBottom);


		//�C���f�b�N�X�o�b�t�@����

		unsigned int index[((MESHFIELDBOX_X + 1) * 2) * (MESHFIELDBOX_Z - 1) - 2];

		int i = 0;
		for (int x = 0; x < MESHFIELDBOX_X - 1; x++) {

			for (int z = 0; z < MESHFIELDBOX_Z; z++) {

				index[i] = x * MESHFIELDBOX_X + z;
				i++;

				index[i] = (x + 1) * MESHFIELDBOX_X + z;
				i++;
			}

			if (x == MESHFIELDBOX_X - 2) break;

			//�k�ރ|���S���p�C���f�b�N�X
			index[i] = (x + 1) * MESHFIELDBOX_X + (MESHFIELDBOX_Z - 1);
			i++;

			index[i] = (x + 1) * MESHFIELDBOX_X;
			i++;

		}


		//�C���f�b�N�X�o�b�t�@����
		//D3D11_BUFFER_DESC bd{};
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * (MESHFIELDBOX_X * 2) * MESHFIELDBOX_Z - 2;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		//D3D11_SUBRESOURCE_DATA sd{};
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = index;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBufferBottom);


	}
	//�����܂Œ��


	//m_Texture = Texture_GetTexture(ImageManager::GetTexId(TEX_PAINTBLOCK));
	m_Texture = Texture_GetTexture(ImageManager::GetTexId(TEX_FIELD_FIELD1));

	//�G���[�`�F�b�N
	assert(m_Texture);//���g��NULL���ƃG���[���o���炵��

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertxeLayout, "vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");


	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	//obb�p��scale���Z�b�g���Ă���
	m_Scale = { 21 * pieceSize,height,21 * pieceSize * s_zWidth };
	m_Position = pivot;

}

void MeshFieldBox::Uninit() {

	if (m_VertexBuffer != nullptr) m_VertexBuffer->Release();
	//m_Texture->Release();

	m_VertxeLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	m_IndexBuffer->Release();
}

void MeshFieldBox::Update() {


}

void MeshFieldBox::Draw() {

	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertxeLayout);

	//�V�F�[�_�[�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);



	////�X�P�[���ƃ|�W�V�����v����
	//�}�g���N�X�ݒ�
	//Renderer::SetWorldViewProjection2D();
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixScaling(&scale, 1, 1, 1);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	D3DXMatrixTranslation(&trans, 0, 0, 0);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;

	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//�C���f�b�N�X�o�b�t�@�ݒ�
	Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);


	//�}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	//�e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//�v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�|���S���`��
	//Renderer::GetDeviceContext()->Draw(4, 0);
	Renderer::GetDeviceContext()->DrawIndexed((MESHFIELDBOX_X * 2) * MESHFIELDBOX_Z - 2 - 2, 0, 0);



	////�ǉ��`��
	//�O��
	//�C���f�b�N�X�o�b�t�@�ݒ�
	Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBufferFront, DXGI_FORMAT_R32_UINT, 0);
	//���_�o�b�t�@�ݒ�
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBufferFront, &stride, &offset);
	//�|���S���`��
	Renderer::GetDeviceContext()->DrawIndexed((MESHFIELDBOX_X * 2) * MESHFIELDBOX_Z - 2 - 2, 0, 0);

	//�E��
	//�C���f�b�N�X�o�b�t�@�ݒ�
	Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBufferRight, DXGI_FORMAT_R32_UINT, 0);
	//���_�o�b�t�@�ݒ�
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBufferRight, &stride, &offset);
	//�|���S���`��
	Renderer::GetDeviceContext()->DrawIndexed((MESHFIELDBOX_X * 2) * MESHFIELDBOX_Z - 2 - 2, 0, 0);

	//����
	//�C���f�b�N�X�o�b�t�@�ݒ�
	Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBufferLeft, DXGI_FORMAT_R32_UINT, 0);
	//���_�o�b�t�@�ݒ�
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBufferLeft, &stride, &offset);
	//�|���S���`��
	Renderer::GetDeviceContext()->DrawIndexed((MESHFIELDBOX_X * 2) * MESHFIELDBOX_Z - 2 - 2, 0, 0);

	//���
	//�C���f�b�N�X�o�b�t�@�ݒ�
	Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBufferBottom, DXGI_FORMAT_R32_UINT, 0);
	//���_�o�b�t�@�ݒ�
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBufferBottom, &stride, &offset);
	//�|���S���`��
	Renderer::GetDeviceContext()->DrawIndexed((MESHFIELDBOX_X * 2) * MESHFIELDBOX_Z - 2 - 2, 0, 0);


}

float MeshFieldBox::GetHeightUp(D3DXVECTOR3 Position) {

	int x, z;




	//�u���b�N�ԍ��Z�o

	//x = Position.x / pieceSize + 10.0f * pieceSize - pivot.x / pieceSize;
	//z = Position.z / -pieceSize + 10.0f * pieceSize - pivot.y / pieceSize;

	x = (Position.x - pivot.x) / pieceSize + 10.0f;
	z = (Position.z - pivot.z) / -pieceSize + 10.0f;

	//�u���b�N�̏�łȂ������ꍇ�A������Ԃ�
	if (x >= MESHFIELDBOX_X || x < 0 || z >= MESHFIELDBOX_Z || z < 0) return LAND_DOWNLIMIT;

	//char buf[64];
	//sprintf(buf, "x = %d z = %d\n", x, z);
	//OutputDebugString(buf);


	D3DXVECTOR3 pos0, pos1, pos2, pos3;

	pos0 = m_Vertex[x][z].Position;
	if((x + 1) < MESHFIELDBOX_X) pos1 = m_Vertex[x + 1][z].Position;
	else {
		pos1 = { (x + 1 - 10) * pieceSize + pivot.x, LAND_DOWNLIMIT, (z - 10) * -pieceSize + pivot.z };
		
		//�ꎞ�Ώ�
		if (BlockOverCharaCheck(Position, m_Vertex[x][z].Position.y)) {
			return LAND_DOWNLIMIT;
		}
		return m_Vertex[x][z].Position.y;
	
	}
		//pos2 = m_Vertex[x][z + 1].Position;
	if ((z + 1) < MESHFIELDBOX_Z) pos2 = m_Vertex[x][z + 1].Position;
	else {
		pos2 = { (x - 10) * pieceSize + pivot.x, LAND_DOWNLIMIT, (z + 1 - 10) * -pieceSize + pivot.z };
	
		//�ꎞ�Ώ�
		if (BlockOverCharaCheck(Position, m_Vertex[x][z].Position.y)) {
			return LAND_DOWNLIMIT;
		}
		return m_Vertex[x][z].Position.y;
	}
	//pos3 = m_Vertex[x + 1][z + 1].Position;
	if ((z + 1) < MESHFIELDBOX_Z && (x + 1) < MESHFIELDBOX_X) pos3 = m_Vertex[x + 1][z + 1].Position;
	else {
		pos3 = { (x + 1 - 10) * pieceSize + pivot.x, LAND_DOWNLIMIT, (z + 1 - 10) * -pieceSize + pivot.z };
	
		//�ꎞ�Ώ�
		if (BlockOverCharaCheck(Position, m_Vertex[x][z].Position.y)) {
			return LAND_DOWNLIMIT;
		}
		return m_Vertex[x][z].Position.y;
	}
	char buf[64];
	//sprintf(buf, "pos0.x = %f pos0.y = %f pos0.z = %f\n", pos0.x, pos0.y, pos0.z);
	//OutputDebugString(buf);
	//sprintf(buf, "pos1.x = %f pos1.y = %f pos1.z = %f\n", pos1.x, pos1.y, pos1.z);
	//OutputDebugString(buf);
	//sprintf(buf, "pos2.x = %f pos2.y = %f pos2.z = %f\n", pos2.x, pos2.y, pos2.z);
	//OutputDebugString(buf);
	//sprintf(buf, "pos3.x = %f pos3.y = %f pos3.z = %f\n", pos3.x, pos3.y, pos3.z);
	//OutputDebugString(buf);


	D3DXVECTOR3 v12, v1p, c;

	v12 = pos2 - pos1;
	v1p = Position - pos1;

	D3DXVec3Cross(&c, &v12, &v1p);//�O�ς��獶��|���S�����E���|���S�����𔻕�

	float py;
	D3DXVECTOR3 n;

	if (c.y > 0.0f) {

		//����|���S��
		D3DXVECTOR3 v10;
		v10 = pos0 - pos1;
		D3DXVec3Cross(&n, &v10, &v12);

	}
	else {
		//�E���|���S��
		D3DXVECTOR3 v13;
		v13 = pos3 - pos1;
		D3DXVec3Cross(&n, &v12, &v13);

	}

	//sprintf(buf, "n.x = %f n.y = %f n.z = %f\n", n.x, n.y, n.z);
	//OutputDebugString(buf);

	//�����擾
	py = -((Position.x - pos1.x) * n.x + (Position.z - pos1.z) * n.z) / n.y + pos1.y;

	//�u���b�N�̍������L�����̍�����荂���ꍇ�A������Ԃ�
	if (BlockOverCharaCheck(Position,py)) {
		return LAND_DOWNLIMIT;
	}

	return py;
}


//�u���b�N�̍������L�����̍�����荂���ꍇ�Atrue��Ԃ�
bool MeshFieldBox::BlockOverCharaCheck(D3DXVECTOR3 s_pos,float py) {

	if (s_pos.y < py - CHARA_CLIMEABLE_HEIGHT) return true;

	return false;
}

bool MeshFieldBox::GetCliff(D3DXVECTOR3 Position, AnimationPlayer* s_p) {

	//���ݕs�Ȃ珈�����Ȃ�
	if (!cliffFlag) return false;

	float cliffAreaWidth = 1.0f;
	float cliffAreaHeight = 2.0f;

	//�u���b�N�̂ɔ���𐶐����A����ɐڂ��Ă��邩�ǂ�����Ԃ�
	Area rightCliffArea, leftCliffArea;
	rightCliffArea.rightMax = pivot.x + pieceSize * 10.5f + cliffAreaWidth;
	rightCliffArea.leftMax = pivot.x + pieceSize * 10.5f;
	rightCliffArea.upMax = pivot.y;
	rightCliffArea.downMax = pivot.y - cliffAreaHeight;

	leftCliffArea.rightMax = pivot.x - pieceSize * 10.5f;
	leftCliffArea.leftMax = pivot.x - pieceSize * 10.5f - cliffAreaWidth;
	leftCliffArea.upMax = pivot.y;
	leftCliffArea.downMax = pivot.y - cliffAreaHeight;


	////����ɓ������Ă�����true
	//�E
	if (StageSystem::CheckInArea(rightCliffArea, Position)) {
		
		float charaScaleY = s_p->GetScale().y + 0.5f;

		//�R���܂�ʒu�Ɉړ�
		s_p->SetPosition({ rightCliffArea.leftMax ,rightCliffArea.downMax + charaScaleY ,0});

		//�R�����擾
		s_p->SetCliffData(this,RIGHT);

		return true;

	}
	//��
	else if (StageSystem::CheckInArea(leftCliffArea, Position)) {

		float charaScaleY = s_p->GetScale().y + 0.5f;

		//�R���܂�ʒu�Ɉړ�
		s_p->SetPosition({ leftCliffArea.rightMax ,leftCliffArea.downMax + charaScaleY ,0 });

		//�R�����擾
		s_p->SetCliffData(this, LEFT);

		return true;

	}
	return false;
}

void MeshFieldBox::SetTexture(TEX_KIND s_tk) {

	m_Texture = Texture_GetTexture(ImageManager::GetTexId(s_tk));

}
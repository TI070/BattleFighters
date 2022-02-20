#define NOMINMAX

#include "particle.h"
#include "managerBase.h"
#include "billboard.h"
#include "renderer.h"
#include "rendererManager.h"
#include "usefulFunction.h"
#include <algorithm>
#include  <list>

void ParticleSystem::Init() {

	VERTEX_3D vertex[4];


	vertex[0].Position = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);//�F
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(+0.5f, -0.5f,0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(-0.5f, +0.5f, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(+0.5f, +0.5f, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@����
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


	//Renderer::CreateVertexShader(&m_InstancingVertexShader, &m_InstancingVertxeLayout, "instancingShaderVS.cso");
	//Renderer::CreatePixelShader(&m_InstancingPixelShader, "instancingShaderPS.cso");

	m_InstancingPixelShader = RendererManager::GetInstancePS();
	m_InstancingVertexShader = RendererManager::GetInstanceVS();
	m_InstancingVertxeLayout = RendererManager::GetInstanceIL();

	SetrenSet();

	//�`��
	SetrenSet_Shader(m_InstancingPixelShader, m_InstancingVertexShader, m_InstancingVertxeLayout);

	//2D
	//Renderer::CreateVertexShader(&m_VertexShader, &m_VertxeLayout, "unlitTextureVS.cso");
	//Renderer::CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");

	m_PixelShader = RendererManager::GetUnlitPS();
	m_VertexShader = RendererManager::GetUnlitVS();
	m_VertxeLayout = RendererManager::GetUnlitIL();


}

void ParticleSystem::Uninit() {

	//���X�g���J�����Ď���
	for (Particle* object : particleList) {

		object->Uninit();
		delete object;
	}
	particleList.clear();//���X�g�̍폜

}

void ParticleSystem::Update() {

	//�Ǐ]���肪����ΒǏ]
	if (followObj != nullptr) {
		//�Ǐ]����͂��Ȃ̂ɑ��肪���Ȃ���Δj��
		if (followFlag && followObj->GetDestroy()) {
			life = 0;
		}
		else m_Position = followObj->GetPosition() + followOffset;
	}



	//�p�[�e�B�N���j�󏈗�
	//RemoveMember<Particle*>(&particleList);
	ParticleListRemove();

	//�p�[�e�B�N����������
	SpawnParticle();

	////�p�[�e�B�N�����X�g�̍X�V
	for (Particle* object : particleList) {

		object->Update();

	}

	//��������
	life--;
	if (life <= 0) SetDestroy();

}

void ParticleSystem::Draw() {

	if (delayTime > 0) {
		delayTime--;
		return;
	}

	//�C���X�^���V���O�ňꊇ�`��
	if (isInstancing) {
		InstancingDraw();
		return;
	}

	//���X�g�Ńp�[�e�B�N���̏��������r���{�[�h�ŕ`��
	for (Particle* object : particleList) {

		object->Draw();

	}

}

void ParticleSystem::InstancingDraw() {

	
	//���_�f�[�^��������
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	
	
	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;


	vertex[0].Position = D3DXVECTOR3(-size, size, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);//�F
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(+size, +size, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(-size, -size, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(+size, -size, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);


	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);



	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertxeLayout);

	//�V�F�[�_�[�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//�}�g���N�X�ݒ�
	//Renderer::SetWorldViewProjection2D();
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;

	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//�}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	//�e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//�v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	



	//�o�b�t�@�ɃZ�b�g
	D3DXVECTOR3 s_offsetPos[1024] = {}, s_rot[1024] = {};
	D3DXVECTOR4 s_color[1024] = {};
	for (int i = 0; i < INSTANCING_NUM; i++) {
		s_offsetPos[i] = {0,0,0};
		s_rot[i] = { 0,0,0 };
		s_color[i] = { 0,0,0,0 };
	}
	int setMax = particleList.size();
	
	int cnt = 0;
	for (Particle* obj : particleList) {
		s_offsetPos[cnt] = obj->GetPosition();
		s_rot[cnt] = obj->GetRotation();
		s_color[cnt] = obj->GetColor();
		//s_rot[cnt] = {0,0,0};
		cnt++;
	}


	Billboard::BillBorad_InstanceDraw(GetRendererSet(), {0,0,0}, s_offsetPos, s_rot, s_color,setMax);


}

void ParticleSystem::SpawnParticle() {

	//���X�g�����E�l�𒴂��Ă����琶�����Ȃ�
	if (particleList.size() > INSTANCING_NUM) return;

	if (life % spawnInterval == 0) {

		for (int i = 0; i < spawnNum; i++) {

			D3DXVECTOR3 s_vel;

			s_vel.x = UsefulFunction::GetRandomRange(velocityMin.x, velocityMax.x) * PARTICLE_SPEEDCORRECT;
			s_vel.y = UsefulFunction::GetRandomRange(velocityMin.y, velocityMax.y) * PARTICLE_SPEEDCORRECT;
			s_vel.z = UsefulFunction::GetRandomRange(velocityMin.z, velocityMax.z) * PARTICLE_SPEEDCORRECT;

			//�p�����[�^�ɏ]������
			Particle* pP = new Particle(m_Position, size, particleLife, s_vel,m_Texture,color,lastColor);
			if (sizeChange) {
				pP->SetLastSize(lastSize);
				pP->SetC_SizeValue();
			}

			//���X�g�ɒǉ�
			particleList.push_back(pP);
		}
	}
}

void ParticleSystem::ParticleListRemove() {

	//�߂�l��true�Ȃ烊�X�g�������
	particleList.remove_if([](Particle* object) {
		bool r = object->Destroy();
		if (r) {
			object = nullptr;
		}
		return r;
	});

}


void Particle::Init() {

	float s_size = size / 2;


	VERTEX_3D vertex[4];
	
	vertex[0].Position = D3DXVECTOR3(-s_size, -s_size, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = color;//�F
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(+s_size, -s_size, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = color;
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(-s_size, +s_size, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = color;
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(+s_size, +s_size, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = color;
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);


	//���_�o�b�t�@����
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

	//�G���[�`�F�b�N
	assert(m_Texture);//���g��NULL���ƃG���[���o���炵��

	//Renderer::CreateVertexShader(&m_VertexShader, &m_VertxeLayout, "unlitTextureVS.cso");
	//Renderer::CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");

	m_VertexShader = RendererManager::GetUnlitVS();
	m_VertxeLayout = RendererManager::GetUnlitIL();
	m_PixelShader = RendererManager::GetUnlitPS();

	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	size = s_size;

	//�J���[�ݒ�
	C_ColorValue = color;
	C_ColorValue = color - lastColor;


	C_ColorValue /= life;

	//�X�P�[���ݒ�
	if (sizeChange) {
		C_SizeValue = size - lastSize.x;

		C_SizeValue /= life;
	}
}

//void Particle::Init(D3DXVECTOR3 s_pos, float s_size, unsigned short texId) {
//
//
//}

void Particle::Uninit() {

	if(m_VertexBuffer != nullptr) m_VertexBuffer->Release();

}

void Particle::Update() {

	//���x�ɂ��ړ�
	m_Position += velocity;

	//�J���[�ύX
	ChangeColorLinear();
	if (sizeChange) ChangeSizeLinear();

	//��������
	life--;
	if (life <= 0) SetDestroy();
}

void Particle::Draw() {

	float s_size = size;
	D3DXVECTOR2 s_pos = { m_Position.x,m_Position.z };

	//���_�f�[�^��������
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;


	vertex[0].Position = D3DXVECTOR3(-s_size, s_size, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = color;//�F
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[1].Position = D3DXVECTOR3(+s_size, +s_size, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = color;
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	vertex[2].Position = D3DXVECTOR3(-s_size, -s_size, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = color;
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[3].Position = D3DXVECTOR3(+s_size, -s_size, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = color;
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertxeLayout);

	//�V�F�[�_�[�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//�}�g���N�X�ݒ�
	//Renderer::SetWorldViewProjection2D();
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;

	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

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
	SetrenSet();
	Billboard::BillBorad_Draw(GetRendererSet(), m_Position);


}

void Particle::ChangeColorLinear() {

	color -= C_ColorValue;
}

void Particle::ChangeSizeLinear() {

	size -= C_SizeValue;
}

void ParticleSystem2D::InstancingDraw() {

	//���_�f�[�^��������
	/*
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);


	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;


	vertex[0].Position = D3DXVECTOR3(-size, size, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);//�F
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(+size, +size, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(-size, -size, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(+size, -size, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);


	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
	*/

	//�o�b�t�@�ɃZ�b�g
	D3DXVECTOR3 s_offsetPos[1024] = {}, s_rot[1024] = {}, s_size[1024] = {};
	D3DXVECTOR4 s_color[1024] = {};

	for (int i = 0; i < INSTANCING_NUM; i++) {
		s_offsetPos[i] = { 0,0,0 };
		s_rot[i] = { 0,0,0 };
		s_color[i] = { 0,0,0,0 };
		s_size[i] = { 0,0,0};
	}
	int instanceNum = particleList.size();

	int cnt = 0;
	for (Particle* obj : particleList) {
		s_offsetPos[cnt] = obj->GetPosition();
		s_rot[cnt] = obj->GetRotation();
		s_color[cnt] = obj->GetColor();
		s_size[cnt] = obj->GetSize();
		cnt++;
	}

	if (instanceNum <= 0) return;

	//�C���X�^���V���O�o�b�t�@�̃Z�b�g
	InstancingData s_ID;
	for (int i = 0; i < instanceNum; i++) {
		s_ID.Position[i] = { s_offsetPos[i].x,s_offsetPos[i].y,s_offsetPos[i].z,0 };

		s_ID.Rotation[i] = { s_rot[i].x,s_rot[i].y,s_rot[i].z,0 };
		s_ID.Scale[i] = { s_size[i].x ,s_size[i].y ,s_size[i].z ,0 };
		s_ID.Diffuse[i] = s_color[i];
	}

	Renderer::SetInstancingData(s_ID);

	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_InstancingVertxeLayout);

	//�V�F�[�_�[�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_InstancingVertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_InstancingPixelShader, NULL, 0);

	//�}�g���N�X�ݒ�
	Renderer::SetWorldViewProjection2D();

	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;

	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//�e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//�v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�|���S���`��
	Renderer::GetDeviceContext()->DrawInstanced(4, instanceNum, 0, 0);

}
#include "fade.h"
#include "texture.h"
#include "polygon2d.h"
#include "config.h"
#include "renderer.h"
#include "imageManager.h"
#include "main.h"

#include <d3dx9.h>

static FadeMode g_FadeMode = FADE_NONE;
static D3DXVECTOR4 g_Color;
static float g_Prop = 0.0f;
static int g_FadeFrame = 0;
static int g_FrameCount = 0;
static int g_FadeStartFrame = 0;
static int g_TextureId = TEXTURE_INVALID_ID;

ID3D11Buffer* Fade::m_VertexBuffer;
ID3D11ShaderResourceView* Fade::m_Texture;
ID3D11VertexShader* Fade::m_VertexShader;
ID3D11PixelShader* Fade::m_PixelShader;
ID3D11InputLayout* Fade::m_VertxeLayout;

//������
void Fade::Fade_Initialize() {

	g_FadeMode = FADE_NONE;
	g_Color = D3DXVECTOR4(0.0f,0.0f,0.0f,0.0f);
	g_Prop = 0.0f;
	g_FadeFrame = 0;
	g_FrameCount = 0;
	g_FadeStartFrame = 0;
	g_TextureId = TEXTURE_INVALID_ID;

	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(0, 0, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = g_Color;//�F
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = g_Color;
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = g_Color;
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = g_Color;
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

	//�e�N�X�`���Ǎ�
	m_Texture = Texture_GetTexture(ImageManager::GetTexId(TEX_WHITE));





	//�G���[�`�F�b�N
	assert(m_Texture);//���g��NULL���ƃG���[���o���炵��

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertxeLayout, "unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");





}

//�X�V
void Fade::Fade_Update() {

	g_FrameCount++;

	//�t�F�[�h���ĂȂ�������I���
	if (g_FadeMode == FADE_NONE) return;

	int frame = g_FrameCount - g_FadeStartFrame;

	//�i�s�����̎Z�o
	g_Prop = (float)frame / g_FadeFrame;

	if (g_Prop > 1.0f) {

		g_FadeMode = FADE_NONE;
		g_Prop = 0;
	}

}

//�`��
void Fade::Fade_Draw() {

	g_Color.w = g_FadeMode == FADE_IN ? 1 - g_Prop : g_Prop;

	//�قړ����Ȃ̂ŕ`�悵�Ȃ�
	if (g_Color.w <= 0.0001f) return;

	//���_�f�[�^��������
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex[0].Position = D3DXVECTOR3(0, 0, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = g_Color;//�F
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = g_Color;
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = g_Color;
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = g_Color;
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	//vertex[0].Diffuse = g_Color;//�F
	//vertex[1].Diffuse = g_Color;//�F
	//vertex[2].Diffuse = g_Color;//�F
	//vertex[3].Diffuse = g_Color;//�F


	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

	//Sprite_SetColor(g_Color);

	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertxeLayout);

	//�V�F�[�_�[�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

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
	Renderer::GetDeviceContext()->Draw(4, 0);

	//Sprite_SetColorDefault();
}

//�I������
void Fade::Fade_Finalize() {

	Texture_Release(&g_TextureId,1);

}

void Fade::Fade_Start(FadeMode mode, float r, float g, float b, int frame) {

	g_FadeMode = mode;
	g_Color = D3DXVECTOR4(r, g, b, 0.0f);
	g_Prop = 0.0f;
	g_FadeFrame = frame;
	g_FadeStartFrame = g_FrameCount;

}

bool Fade::Fade_IsFade() {

	return g_FadeMode != FADE_NONE;
}
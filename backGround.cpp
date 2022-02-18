
#include "backGround.h"
#include "modelsystem.h"
#include "model.h"
#include "renderer.h"
#include "fieldConfig.h"
#include "rendererManager.h"

void BackGround::Init() {

	skyDome = ModelSystem::LoadandGetModel("asset\\model\\skybox.obj");

	m_VertexShader = RendererManager::GetUnlitVS();
	m_VertxeLayout = RendererManager::GetUnlitIL();
	m_PixelShader = RendererManager::GetUnlitPS();

	m_Position = { 0 ,-10,0 };

	m_Scale = { 30,30,30 };

	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

void BackGround::Init2() {

	skyDome = ModelSystem::LoadandGetModel("asset\\model\\skybox2.obj");

	m_VertexShader = RendererManager::GetUnlitVS();
	m_VertxeLayout = RendererManager::GetUnlitIL();
	m_PixelShader = RendererManager::GetUnlitPS();

	m_Position = { 0 ,-10,0 };

	m_Scale = { 30,30,30 };

	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

void BackGround::Init3() {

	skyDome = ModelSystem::LoadandGetModel("asset\\model\\skybox3.obj");

	m_VertexShader = RendererManager::GetUnlitVS();
	m_VertxeLayout = RendererManager::GetUnlitIL();
	m_PixelShader = RendererManager::GetUnlitPS();

	m_Position = { 0 ,-10,0 };

	m_Scale = { 30,30,30 };

	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

void BackGround::Uninit() {

	

}

void BackGround::Draw() {

	if (skyDome == nullptr) return;

	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertxeLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);

	world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);


	skyDome->Draw();

}
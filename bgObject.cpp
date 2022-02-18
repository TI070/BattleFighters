
#include "bgObject.h"
#include "model.h"
#include "modelsystem.h"

void BgObject::Init() {

	pModel = ModelSystem::LoadandCreateModel("asset\\model\\bgObj.obj");


	Renderer::CreateVertexShader(&m_VertexShader, &m_VertxeLayout, "ParameterShaderVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "ParameterShaderPS.cso");

}

void BgObject::Draw() {

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


	pModel.Draw();

}

void BgObject::Uninit() {

	pModel.Uninit();

}
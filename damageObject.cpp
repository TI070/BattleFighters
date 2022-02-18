
#include "damageObject.h"
#include "modelsystem.h"
#include "animationPlayer.h"
#include "camera.h"
#include "particle.h"
#include "effectManager.h"

#define GRAVITY_POWER (0.01f)

void DamageObject::Init() {

	//pModel = ModelSystem::LoadandCreateModel("asset\\model\\hitbox2.obj");
	pModel = ModelSystem::LoadandCreateModel("asset\\model\\hitbox.obj");


	Renderer::CreateVertexShader(&m_VertexShader, &m_VertxeLayout, "ParameterShaderVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "ParameterShaderPS.cso");

	//Renderer::CreateVertexShader(&m_VertexShader, &m_VertxeLayout, "unlitTextureVS.cso");
	//Renderer::CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");

	rollFlag = false;
	isDraw = false;
	delayTime = 0;
}


void DamageObject::Uninit() {

	if (m_VertxeLayout != nullptr) m_VertxeLayout->Release();
	if (m_VertexShader != nullptr) m_VertexShader->Release();
	if (m_PixelShader != nullptr)  m_PixelShader->Release();


	

}


void DamageObject::Update() {

	//有効判定
	if (delayTime > 0) {
		delayTime--;
		return;
	}

	//移動
	Move();

	//寿命判定
	CheckLifeTime();

	//回転
	if(rollFlag) m_Rotation += rollSpeed;

}


void DamageObject::Draw() {

	//有効判定
	if (delayTime > 0) {
		return;
	}

	if (!isDraw) return;

	//視錐台カリング
	if (!pCamera->CheckView(m_Position, frustumCullingScale)) {


		return;
	}


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

bool DamageObject::CheckLifeTime() {

	if (lifeTime > 0) {
		lifeTime--;
		return true;
	}

	SetDestroy();
	return false;

}

void DamageObject::Move() {

	//追従するかしないか
	if (followOwner) FollowOwner();
	else m_Position += velocity;

}

void DamageObject::FollowOwnerOn() {

	followOwner = true; 
	followOffset = m_Position - owner->GetPosition();

}

void DamageObject::FollowOwner() {

	m_Position = owner->GetPosition() + followOffset;
}

void ShieldObject::Init() {

	pModel = ModelSystem::LoadandCreateModel("asset\\model\\shield.obj");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertxeLayout, "ParameterShaderVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "ParameterShaderPS.cso");

	shaderParameter = {0,0,0,0};
}


void ShieldObject::Draw() {

	//視錐台カリング
	if (!pCamera->CheckView(m_Position, frustumCullingScale)) {


		return;
	}




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


	//パラメータ設定

	//シールドが削れた時にパラメータを変更するようにする
	Renderer::SetParameter(shaderParameter);


	Renderer::SetWorldMatrix(&world);



	pModel.Draw();

	Renderer::SetParameter({ 0,0,0,0 });

}


void ArrowObject::Init() {

	pModel = ModelSystem::LoadandCreateModel("asset\\model\\obj\\arrow.obj");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertxeLayout, "ParameterShaderVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "ParameterShaderPS.cso");

	isDraw = true;


	//パーティクル
	D3DXVECTOR3 maxVec, minVec;
	maxVec = { 0.5f,0.5f ,0.5f };
	minVec = { -0.5f,-0.5f,-0.5f };
	D3DXVECTOR4 s_startColor, s_lastColor;
	s_startColor = { 159 / 255.0f,255 / 255.0f,255 / 255.0f,196 / 255.0f };
	s_lastColor = { 179 / 255.0f,255 / 255.0f,217 / 255.0f,128 / 255.0f };


	//パーティクル生成
	ParticleSystem* pPS = EffectManager::SetParticle();
	pPS->SetPosition({ m_Position.x,m_Position.y,m_Position.z });
	pPS->SetColor(s_startColor);
	pPS->SetLastColor(s_lastColor);
	pPS->SetLife(60);
	pPS->SetParticleLife(6);
	pPS->SetVelocityMax(maxVec);
	pPS->SetVelocityMin(minVec);
	pPS->SetInterval(2);
	pPS->SetSpawnNum(24);
	pPS->SetSize(0.4f);
	pPS->SetLastSize({ 0.2f,0.2f });
	pPS->SetTexture(ImageManager::GetTexId(TEX_PARTICLE_FLASH));
	pPS->SetisInstancing(true);
	pPS->FollowObect(this);
	pPS->Init();


}


void FireballObject::Init() {

	pModel = ModelSystem::LoadandCreateModel("asset\\model\\magicBullet.obj");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertxeLayout, "ParameterShaderVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "ParameterShaderPS.cso");


	isDraw = false;

	D3DXVECTOR3 maxVec, minVec;
	maxVec = { 1.1f,0.6f ,0.1f };
	minVec = { -0.1f,0.1f,-0.1f };
	D3DXVECTOR4 s_startColor, s_lastColor;
	s_startColor = { 255 / 255.0f,246 / 255.0f,102 / 255.0f,196 / 255.0f };
	s_lastColor = { 255 / 255.0f,102 / 255.0f,102 / 255.0f,128 / 255.0f };


	//パーティクル生成
	ParticleSystem* pPS = EffectManager::SetParticle();
	pPS->SetPosition({ m_Position.x,m_Position.y,m_Position.z});
	pPS->SetColor(s_startColor);
	pPS->SetLastColor(s_lastColor);
	pPS->SetLife(120);
	pPS->SetParticleLife(6);
	pPS->SetVelocityMax(maxVec);
	pPS->SetVelocityMin(minVec);
	pPS->SetInterval(2);
	pPS->SetSpawnNum(24);
	pPS->SetSize(0.4f);
	pPS->SetTexture(ImageManager::GetTexId(TEX_EFF_MOYA));
	pPS->SetisInstancing(true);
	pPS->FollowObect(this);
	pPS->Init();

}

void FireballObject::Update() {

	//有効判定
	if (delayTime > 0) {
		delayTime--;
		return;
	}

	//移動
	Move();

	//寿命判定
	CheckLifeTime();

	//回転
	if (rollFlag) m_Rotation += rollSpeed;




}

void FireballObject::Move() {

	if (target == nullptr) return;

	float spd = velocity.x;


	//ベクトルの算出
	D3DXVECTOR3 targetPos = target->GetPosition();
	targetPos.y += 1.0f;
	D3DXVECTOR3 vec = targetPos - m_Position;
	D3DXVec3Normalize(&vec,&vec);

	m_Position += vec * spd;

}

void ThunderObject::Init() {

	pModel = ModelSystem::LoadandCreateModel("asset\\model\\hitbox.obj");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertxeLayout, "ParameterShaderVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "ParameterShaderPS.cso");


	isDraw = false;

	D3DXVECTOR3 maxVec, minVec;
	maxVec = { 8.1f,0.4f ,0.1f };
	minVec = { -0.5f,-0.4f,-0.1f };
	D3DXVECTOR4 s_startColor, s_lastColor;
	s_startColor = { 255 / 255.0f,252 / 255.0f,204 / 255.0f,196 / 255.0f };
	s_lastColor = { 255 / 255.0f,240 / 255.0f,26 / 255.0f,0 / 255.0f };
	
	float effoffPosX = 1.5f;

	if (owner->GetCharaDir() == CD_LEFT) {
		float dummy;
		dummy = -maxVec.x;
		maxVec.x = -minVec.x;
		minVec.x = dummy;
		effoffPosX *= -1;
	}



	//パーティクル生成
	ParticleSystem* pPS = EffectManager::SetParticle();
	pPS->SetPosition({ m_Position.x + effoffPosX,m_Position.y + 1.0f,m_Position.z });
	pPS->SetColor(s_startColor);
	pPS->SetLastColor(s_lastColor);
	pPS->SetLife(120);
	pPS->SetParticleLife(32);
	pPS->SetVelocityMax(maxVec);
	pPS->SetVelocityMin(minVec);
	pPS->SetInterval(4);
	pPS->SetSpawnNum(48);
	pPS->SetSize(0.3f);
	pPS->SetTexture(ImageManager::GetTexId(TEX_EFF_MOYA));
	pPS->SetisInstancing(true);
	pPS->SetDelayTime(45);
	pPS->Init();


}

void IceObject::Init() {

	pModel = ModelSystem::LoadandCreateModel("asset\\model\\ice.obj");
	//pModel = ModelSystem::LoadandCreateModel("asset\\model\\hitbox.obj");

	isDraw = true;

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertxeLayout, "ParameterShaderVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "ParameterShaderPS.cso");

	shaderParameter = { 0,0,0,0 };

}

void IceObject::Move() {

	//速度に重力を加算する
	if(velocity.y > GRAVITY_POWER) velocity.y -= GRAVITY_POWER;
	else velocity.y = -GRAVITY_POWER;

	m_Position += velocity;

	D3DXVECTOR3 maxVec, minVec;
	maxVec = { 0.5f,0.5f ,0.1f };
	minVec = { -0.5f,-0.5f,-0.1f };
	D3DXVECTOR4 s_startColor, s_lastColor;
	s_startColor = { 175 / 255.0f,250 / 255.0f,250 / 255.0f,196 / 255.0f };
	s_lastColor = { 225 / 255.0f,250 / 255.0f,250 / 255.0f,64 / 255.0f };


	ParticleSystem* pPS = EffectManager::SetParticle();
	pPS->SetPosition({ m_Position.x,m_Position.y + 1.0f,m_Position.z });
	pPS->SetColor(s_startColor);
	pPS->SetLastColor(s_lastColor);
	pPS->SetLife(90);
	pPS->SetParticleLife(16);
	pPS->SetVelocityMax(maxVec);
	pPS->SetVelocityMin(minVec);
	pPS->SetInterval(16);
	pPS->SetSpawnNum(48);
	pPS->SetSize(0.2f);
	pPS->SetTexture(ImageManager::GetTexId(TEX_EFF_MOYA));
	pPS->SetisInstancing(true);
	pPS->FollowObect(this);
	pPS->SetFollowOffset({0,0,-1.0f});
	pPS->Init();

}

void IceObject::Draw() {

	//有効判定
	if (delayTime > 0) {
		return;
	}

	if (!isDraw) return;

	//視錐台カリング
	if (!pCamera->CheckView(m_Position, frustumCullingScale)) {


		return;
	}


	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertxeLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x / 5, m_Scale.y / 5, m_Scale.z / 5);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);

	world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);

	pModel.Draw();

}


void WaterObject::Init() {

	pModel = ModelSystem::LoadandCreateModel("asset\\model\\water.obj");
	//pModel = ModelSystem::LoadandCreateModel("asset\\model\\hitbox.obj");

	isDraw = true;

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertxeLayout, "ParameterShaderVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "ParameterShaderPS.cso");

	shaderParameter = { 0,0,0,0 };

}
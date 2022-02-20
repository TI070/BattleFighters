
#include "animationPlayer.h"
#include "key_manager.h"
#include "renderer.h"
#include "camera.h"
#include "meshFieldBoxManager.h"
#include "damageObjectManager.h"
#include "effectManager.h"
#include "damageObject.h"
#include "modelsystem.h"
#include "effect.h"
#include "UIwindow.h"
#include "gameSystem.h"
#include "stageSystem.h"
#include "battleAI.h"
#include "particle.h"
#include "playerManager.h"
#include "billboard.h"
#include "usefulFunction.h"
#include "controller.h"

#define LAND_BASE (0)
#define GRAVITY_POWER (0.01f)
  
#define GROUNDSHOCKEFFECT_LIFE (30)
#define GROUNDSHOCKEFFECT_ENUM {6,1}


#define JUMPANIME_FRAMEMAX (60)

int tes = 0;

void AnimationPlayer::Init() {

	//モデルロード
	a_Model = new AnimationModel;
	//a_Model->Load("asset/model/Akai_Idle.fbx")
	ModelDataLoad();

	pHitBoxModel = ModelSystem::LoadandCreateModel("asset\\model\\hitbox.obj");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertxeLayout, "charaShaderVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "charaShaderPS.cso");

	//ステータスセット
	m_Scale = { 0.01f,0.01f, 0.01f };
	m_Rotation = { 0.0f,0.0f, 0.0f };
	m_Position = { 0.0f,0.1f, 0.0f };

	isJump = false;
	throughFlag = false;
	modelFrameStop = false;

	velocity = {0,0,0};
	blownVelocity = { 0,0,0 };
	shaderPlusColor = { 0,0,0,0 };
	pastPos = m_Position;

	lifeStock = GameSystem::GetStartStock(playerNumber - 1);
	
	addJumpCnt = 0;
	accumulationDamage = 0;
	invincibleTime = 0;
	grapCooltime = 0;
	ShieldBreakTime = 0;

	//SetCharacterParameter();
	SetStates();
	charaCon = CC_NORMAL;
	SetFrustumCullingScale(0.5f);
	modelOffset = { 0,0,0 };
	modelOffsetVelocity = { 0,0,0 };
	modelRotate = { 0,0,0 };

	SetStartStates();

	actionCharge = 0;

	//当たり判定設定
	//hitPosition = m_Position;
	//hitScale = {0.3f,0.3f, 0.3f};	
	HitBoxInit();
	HitBoxUpdate();

	//hitScale = { 3.3f,0.3f, 0.3f };

	//UI設定
	StatesWindow = ImageManager::SetStatusWindow({ SCREEN_WIDTH / 4.0f * playerNumber - 100,SCREEN_HEIGHT - 100 }, {100,100},0,GetCharaKind(),lifeStock);
	StatesWindow->SetWindowTex(NULL);

	char buf[64];
	sprintf(buf, "%d%%", accumulationDamage);
	DamageText = StatesWindow->SetTextTex(buf, { 10,50 }, { 64,64 }, { 0,0,0,1 });

	PlayerSignInit();


	//ゲームシステムに設定
	GameSystem::SetCurrentStock(playerNumber - 1,&lifeStock);


	
}

void AnimationPlayer::Uninit() {

	a_Model->Unload();
	delete a_Model;

	m_VertxeLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void AnimationPlayer::Update() {

	//ストックがなければ動作しない
	if (lifeStock <= 0) return;

	//ステージプレイしていなければ動作しない
	if (!GameSystem::GetStagePlaing()) return;

	pastPos = m_Position;

	//メッシュフィールド高さ取得
	//landHeight = mfbm->GetHeightUp(m_Position);

	CheckJumping();

	if (charaMode == CM_CPU) CPUThink();

	//硬直中でなければ操作可能
	if (!ActionCooltimeUpdate() && charaCon == CC_NORMAL) {

		//キャラモードごとの固有動作
		switch (charaMode)
		{
		case CM_SANDBAG:
			SandbagUpdate();
			break;
		case CM_PLAYER:
			PlayerUpdate();
			break;
		case CM_CPU:
			CPUUpdate();
			break;
		default:
			break;
		}
	}
	else {//特殊状態
		switch (charaCon)
		{
		case CC_NORMAL:
			break;
		case CC_SHIELDBREAK:
			ShieldBreakUpdate();
			break;
		case CC_CLLIFGRAP:
			CliffGrap();
			//if (charaMode == CM_CPU) battleAI->MoveAIThink();
			break;

		case CC_AVOID:
			AvoidUpdate();
		case CC_GRAB:
			GrabingAction();
		default:
			break;
		}

	}

	AnimationStateUpdate();

	//if (KeyManager_Trigger(KM_DECIDE))Jump();

	//メッシュフィールド高さ取得
	//すり抜け状態なら下限を設定
	if (throughFlag) {
		landHeight = LAND_DOWNLIMIT;
	}
	else landHeight = MeshFieldBoxManager::GetHeightUp(m_Position);



	//CheckJumping();

	//if (KeyManager_Trigger(KM_CANCEL)) Attack();

	//崖つかみ状態なら落ちない
	if(charaCon != CC_CLLIFGRAP) AddGravity();

	m_Position += velocity + blownVelocity;

	if (playerNumber == 2) {

		//char buf[64];
		//sprintf(buf, "m_posY = %f\n", m_Position.y);
		//OutputDebugString(buf);

	}


	HitBoxUpdate();

	//崖つかみ
	CheckGrap();

	//吹っ飛ばし速度減速
	if (D3DXVec3Length(&blownVelocity) > 0.01f) {
		blownVelocity *= BLOWN_DEC_COEFFICIENT;


	}
	else blownVelocity = {0,0,0};

	//無敵時間処理
	InvincibleUpdate();

	//死亡関連処理
	DeathFunction();


	//デバッグ
	if (KeyManager_Trigger(KM_L)) {

	}

	//イージング関数
	if (damageTextEasingTime < EASING_DAMAGETEXT_TIME) {
		damageTextEasingTime++;

		if (damageTextEasingTime >= EASING_DAMAGETEXT_TIME) {

			DamageText->SizeUpdate({ 32,32});

			char buf[64];
			sprintf(buf, "%d%%", accumulationDamage);
			DamageText->TextUpdate(buf);

		}
		else {
			float c4 = (2 * 3.14) / 3;
			float s_x = damageTextEasingTime / EASING_DAMAGETEXT_TIME;

			float cl = -pow(2, 10 * s_x - 10) * sin((s_x * 10 - 10.75) * c4);

			if (cl < 0.5f) cl = 0.5f;

			DamageText->SizeUpdate({ 64 * cl,64 * cl });

			char buf[64];
			sprintf(buf, "%d%%", accumulationDamage);
			DamageText->TextUpdate(buf);
		}


	}





}

void AnimationPlayer::Draw() {

	//ストックがなければ動作しない
	if (lifeStock <= 0) return;

	//視錐台カリング

	if (!pCamera->CheckView(m_Position, frustumCullingScale)) return;

	//if (!pCamera->CheckView(m_Position,frustumCullingScale)) {
	//	if(charaMode == CM_PLAYER) OutputDebugString("notDraw\n");
	//	return;
	//}
	//else if (charaMode == CM_PLAYER) OutputDebugString("Draw\n");

	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertxeLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	D3DXVECTOR3 modelPos = m_Position + modelOffset;
	D3DXVECTOR3 modelRot = m_Rotation + modelRotate;

	//マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, modelRot.y, modelRot.x, modelRot.z);
	D3DXMatrixTranslation(&trans, modelPos.x, modelPos.y, modelPos.z);

	world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);

	//パラメータ設定
	Renderer::SetParameter(shaderPlusColor);

	a_Model->Draw();

	//パラメータ初期化しておく
	Renderer::SetParameter({0,0,0,0});

	//UI描画
	PlayerSignDraw();


	//追加描画 デバッグ
	//後でOnOffできるように改良
	//DebugDraw();

}

bool AnimationPlayer::AnimationCharacterMove() {

	bool successMove = false;
	D3DXVECTOR3 dirm;

	dirm.x = 0;
	dirm.y = 0;
	dirm.z = 0;

	throughFlag = false;

	float dashC = 1;
	if (dashFlag) dashC = 1.5f;
	bool dashF = false;

	//ダッシュ判定
	if (dashReceptionTime > 0) {
		if (CheckIputKey(CK_RIGHT, KEY_TRIGGER)) {
			dashFlag = true;
		}
		else if (CheckIputKey(CK_LEFT, KEY_TRIGGER)) {
			dashFlag = true;
		}
	}

	if (CheckIputKey(CK_RIGHT,KEY_PRESS)) {
		m_Position += pCamera->GetRightV() * moveSpeed * dashC;
		dirm += pCamera->GetRightV();// *moveSpeed;
		successMove = true;

		charaDir = CD_RIGHT;
		//受付時間をセット
		if (dashReceptionTime <= 0) {
			dashReceptionTime = DASH_RECEPTIONTIME;
		}


		dashF = true;

	}
	if (CheckIputKey(CK_LEFT, KEY_PRESS)) {
		m_Position -= pCamera->GetRightV() * moveSpeed * dashC;
		dirm -= pCamera->GetRightV();// * moveSpeed;
		successMove = true;

		charaDir = CD_LEFT;
		//受付時間をセット
		if (dashReceptionTime <= 0) {
			dashReceptionTime = DASH_RECEPTIONTIME;
		}


		dashF = true;

	}
	if (CheckIputKey(CK_DOWN, KEY_PRESS)) {

		//すり抜け
		throughFlag = MeshFieldBoxManager::CheckBlockThrough(m_Position);


		//将来的にしゃがみ？

	}


	if(dashFlag) dashFlag = dashF;
	

	DirectionRoll({ dirm.x,dirm.z }, 270);
	
	if(dashReceptionTime > 0 )dashReceptionTime--;


	return successMove;

}

void AnimationPlayer::CharacterRoll() {



}

void AnimationPlayer::Jump() {
	
	//クールタイム中ならreturn 
	if (jumpCooltime > 0) return;

	//ジャンプ残り回数がなければreturn
	if (isJump && addJumpCnt >= addJumpMax) return;

	//既にジャンプ状態なら追加ジャンプを１回消費する
	if (isJump) addJumpCnt++;

	velocity.y = jumpPower;
	isJump = true;
	aModel_frame = 10;

	jumpCooltime = jumpPower / GRAVITY_POWER;

	SetAnimationState(AS_JUMP);

}

//ジャンプ可能ならtrueを返す
bool AnimationPlayer::CheckJumpAble() {

	if(isJump && addJumpCnt >= addJumpMax) return false;
	return true;
}

//ジャンプ中ならtrue
bool AnimationPlayer::CheckJumping() {

	//ジャンプクールタイムの更新
	if(jumpCooltime > 0) jumpCooltime--;

	if (m_Position.y < landHeight) {

		//着地
		m_Position.y = landHeight;
		isJump = false;
		addJumpCnt = 0;
		upAttackAble = true;

		return false;
	}

	isJump = true;

	return true;
}


void AnimationPlayer::AddGravity() {


	//吹っ飛ばし速度のY方向ベクトルが0より上なら重力を無効にする
	if (blownVelocity.y > 0) {
		velocity.y = 0;
		return;
	}

	if(velocity.y > -GRAVITY_POWER * 5) velocity.y -= GRAVITY_POWER;
}


bool AnimationPlayer::ClimeAbleCheck() {

	return false;

}


void AnimationPlayer::Attack() {

	if (!upAttackAble) return;

	SetAnimationState(AS_NATTACK,10);

	//エフェクト
	D3DXVECTOR3 maxVec, minVec;
	maxVec = { 0.1f,0.1f ,0.1f};
	minVec = {-0.1f,-0.1f,-0.1f };

	ParticleSystem* pPS = EffectManager::SetParticle();
	pPS->SetPosition({ m_Position.x,m_Position.y + 1.0f,m_Position.z - 1.0f });
	pPS->SetColor({ 63/255.0f,191 / 255.0f,255 / 255.0f,196/255.0f });
	pPS->SetLife(4);
	pPS->SetParticleLife(4);
	pPS->SetVelocityMax(maxVec);
	pPS->SetVelocityMin(minVec);
	pPS->SetInterval(4);
	pPS->SetSpawnNum(16);
	pPS->SetSize(0.0f);
	pPS->SetTexture(ImageManager::GetTexId(TEX_PARTICLE_FLASH));
	pPS->SetisInstancing(true);
	pPS->Init();

	//チャージ 量でエフェクトを変える
	ActionCharge();
	if (CheckActionCharge(ARCHER_CHARGE_BASE * 2)) {
		pPS->SetSize(1.2f);
	}
	else if (CheckActionCharge(ARCHER_CHARGE_BASE)) {
		pPS->SetSize(0.6f);
	}


}

void AnimationPlayer::SubAttack() {

	if (!upAttackAble) return;

	DamageObject* dObj = DamageObjectManager::SetDObjT<ArrowObject>(m_Position, this);

	//パラメータ設定
	float atkSpeed = 0.1f;
	if (charaDir == CD_LEFT) atkSpeed *= -1;

	dObj->SetDamage(atkData[AK_NEUTRAL].damage);
	dObj->SetBlowPower(atkData[AK_NEUTRAL].blowPower);
	dObj->SetBlowVec(atkData[AK_NEUTRAL].blowVec);
	dObj->SetLifeTime(atkData[AK_NEUTRAL].lifetime);
	dObj->SetScale(atkData[AK_NEUTRAL].scale);
	dObj->SetVelocity(atkData[AK_NEUTRAL].velocity);
	dObj->SetPosition(m_Position + atkData[AK_NEUTRAL].offsetPos);

	if (charaDir == CD_LEFT) dObj->SetRotation({ 0,3.14f / 2,0 });
	else dObj->SetRotation({ 0,-3.14f / 2,0 });

	//チャージによる追加
	if (CheckActionCharge(ARCHER_CHARGE_BASE * 2)) {
		dObj->SetDamage(12);
		dObj->SetBlowPower(0.4f);
		dObj->SetVelocity({ atkSpeed * 4,0.0f, 0.0f });
		dObj->SetScale({ atkData[AK_NEUTRAL].scale.x * 5.0f,atkData[AK_NEUTRAL].scale.y,atkData[AK_NEUTRAL].scale.z });
	}
	else if (CheckActionCharge(ARCHER_CHARGE_BASE)) {
		dObj->SetDamage(6);
		dObj->SetBlowPower(0.2f);
		dObj->SetVelocity({ atkSpeed * 2,0.0f, 0.0f });
		dObj->SetScale({ atkData[AK_NEUTRAL].scale.x * 2.0f,atkData[AK_NEUTRAL].scale.y,atkData[AK_NEUTRAL].scale.z });

	}
	else {
		dObj->SetVelocity({ atkSpeed,0.0f, 0.0f });
	}

	dObj->SetSoundKind(AK_SE_BLOWSMALL);

	//チャージリセット
	ChargeReset();

	//硬直時間
	SetActionCooltime(6);

	//アニメーションセット
	SetAnimationState(AS_NATTACK);
	
	//攻撃中情報をセット
	attackObj = dObj;


}

void AnimationPlayer::UpAttack() {

	//使用可能でなければreturn
	if (!upAttackAble) return;

	DamageObject* dObj = DamageObjectManager::SetDObjT<ArrowObject>(m_Position, this);

	//パラメータ設定
	float atkSpeed = 0.25f;

	dObj->SetDamage(atkData[AK_UP].damage);
	dObj->SetBlowPower(atkData[AK_UP].blowPower);
	dObj->SetBlowVec(atkData[AK_UP].blowVec);
	dObj->SetLifeTime(atkData[AK_UP].lifetime);
	dObj->SetScale(atkData[AK_UP].scale);
	dObj->SetVelocity(atkData[AK_UP].velocity);
	dObj->SetPosition(m_Position + atkData[AK_UP].offsetPos);
	//dObj->FollowOwnerOn();
	dObj->SetRotation({ -3.14f / 2,-3.14f / 2,0 });
	dObj->SetDelayTime(atkData[AK_UP].delay);

	dObj->SetSoundKind(AK_SE_BLOWSMALL);


	//硬直時間
	SetActionCooltime(30);

	SetAnimationState(AS_UPATTACK, 0);

	//上昇
	velocity.y = atkSpeed;

	upAttackAble = false;


	modelRotate.z = -3.14 / 4;

	//攻撃中情報をセット
	attackObj = dObj;

}

void AnimationPlayer::DownAttack() {

	if (!upAttackAble) return;

	DamageObject* dObj = DamageObjectManager::SetDamageObject(m_Position, this);

	//パラメータ設定
	float atkSpeed = 0.0f;

	dObj->SetDamage(atkData[AK_DOWN].damage);
	dObj->SetBlowPower(atkData[AK_DOWN].blowPower);
	dObj->SetBlowVec(atkData[AK_DOWN].blowVec);
	dObj->SetLifeTime(atkData[AK_DOWN].lifetime);
	dObj->SetScale(atkData[AK_DOWN].scale);
	dObj->SetVelocity(atkData[AK_DOWN].velocity);
	dObj->SetPosition(m_Position + atkData[AK_DOWN].offsetPos);
	dObj->SetDelayTime(atkData[AK_DOWN].delay);

	dObj->SetSoundKind(AK_SE_BLOWMIDDLE);


	//硬直時間
	SetActionCooltime(35);

	//アニメーション
	SetAnimationState(AS_DOWNATTACK,0);
	modelOffset.x = -1.5f;
	if (charaDir == CD_LEFT) modelOffset.x *= -1;

	//攻撃中情報をセット
	attackObj = dObj;
}

void AnimationPlayer::CliffAttack() {

	DamageObject* dObj = DamageObjectManager::SetDamageObject(m_Position, this);

	//パラメータ設定
	float atkSpeed = 0.0f;

	dObj->SetDamage(5);
	dObj->SetBlowPower(0.2f);
	dObj->SetBlowVec({ 0, 0.0f });
	dObj->SetLifeTime(15);
	dObj->SetScale({ 2.0f,1.0f, 0.3f });
	dObj->SetVelocity({ atkSpeed,0.0f, 0.0f });
	dObj->SetPosition({ m_Position.x,m_Position.y + 0.5f,m_Position.z });

	dObj->SetSoundKind(AK_SE_BLOWSMALL);


	//硬直時間
	SetActionCooltime(30);

	//攻撃中情報をセット
	attackObj = dObj;
}

void AnimationPlayer::DirAttack() {

	if (!upAttackAble) return;

	DamageObject* dObj = DamageObjectManager::SetDamageObject(m_Position, this);

	//パラメータ設定
	float atkSpeed = 1.0f;
	if (charaDir == CD_LEFT) atkSpeed *= -1;

	dObj->SetDamage(atkData[AK_DIR].damage);
	dObj->SetBlowPower(atkData[AK_DIR].blowPower);
	dObj->SetBlowVec(atkData[AK_DIR].blowVec);
	dObj->SetLifeTime(atkData[AK_DIR].lifetime);
	dObj->SetScale(atkData[AK_DIR].scale);
	dObj->SetVelocity(atkData[AK_DIR].velocity);
	dObj->SetPosition({ m_Position.x + 1.0f * atkSpeed,m_Position.y + 1.0f ,m_Position.z });

	//dObj->SetRollSpeed({0,0.1f,0});
	dObj->SetRotation({0,0,0});

	dObj->FollowOwnerOn();

	dObj->SetSoundKind(AK_SE_BLOWSMALL);


	D3DXVECTOR3 maxVec, minVec;
	maxVec = { 1.1f,0.6f ,0.1f };
	minVec = { -0.1f,0.1f,-0.1f };
	D3DXVECTOR4 s_startColor, s_lastColor;
	s_startColor = { 232 / 255.0f,197 / 255.0f,162 / 255.0f,196 / 255.0f };
	s_lastColor = { 232 / 255.0f,197 / 255.0f,162 / 255.0f,128 / 255.0f };

	float offPosX = 1.0f;
	if (charaDir == CD_LEFT) offPosX *= -1;

	//パーティクル生成
	ParticleSystem* pPS = EffectManager::SetParticle();
	pPS->SetPosition({ m_Position.x,m_Position.y - 0.5f,m_Position.z });
	pPS->SetColor(s_startColor);
	pPS->SetLastColor(s_lastColor);
	pPS->SetLife(30);
	pPS->SetParticleLife(6);
	pPS->SetVelocityMax(maxVec);
	pPS->SetVelocityMin(minVec);
	pPS->SetInterval(2);
	pPS->SetSpawnNum(24);
	pPS->SetSize(0.4f);
	pPS->SetTexture(ImageManager::GetTexId(TEX_EFF_MOYA));
	pPS->SetisInstancing(true);
	pPS->FollowObect(this);
	pPS->SetFollowOffset({offPosX,-0.2f,-0.5f});
	pPS->Init();



	//硬直時間
	SetActionCooltime(35);

	//速度付加
	velocity.x = 2.5/35.0f * atkSpeed;

	SetAnimationState(AS_DIRATTACK,0);
	modelOffsetVelocity.x -= velocity.x * 2;

	//攻撃中情報をセット
	attackObj = dObj;

}

void AnimationPlayer::AttackObjDeath() {

	if (attackObj != nullptr) {
		attackObj->SetDestroy();
		attackObj = nullptr;
	}

}

void AnimationPlayer::SetStates() {

	//重さ
	weight = 0.5f;

	//ジャンプ力
	jumpPower = 0.2f;

	//ジャンプ回数
	addJumpMax = 1;

	//移動速度
	moveSpeed = CHARA_MOVESPEED_DEFAULT;

	//当たり判定大きさ
	hitScale = { 0.3f,0.3f, 0.3f };

	//シールド持続時間
	shieldLife = shieldLifeMax = 180;

	//技データ入れる
	SetAttackData();

}

void AnimationPlayer::PlayerUpdate() {

	//x方向速度0
	velocity.x = 0;


	//チャージ中なら限定された行動しかとれない
	if (CheckActionCharge(1)) {
		if (CheckIputKey(CK_ATTACK, KEY_PRESS)) {
			Attack();
		}
		else if (CheckIputKey(CK_ATTACK, KEY_RELEASE)) {
			SubAttack();
		}


		return;
	}

	//移動
	if (isJump) {
		//AS = AS_JUMP;
		//SetAnimationState(AS_JUMP);
		AnimationCharacterMove();
	}
	else {
		if (AnimationCharacterMove()) {
			//AS = AS_RUN;
			SetAnimationState(AS_RUN);
		}
		else {
			//AS = AS_WAIT;
			SetAnimationState(AS_WAIT);
		}
	}

	//ジャンプ
	if (CheckIputKey(CK_JUMP, KEY_TRIGGER))Jump();
	//攻撃
	if (CheckIputKey(CK_ATTACK, KEY_TRIGGER)) {

		//シールドボタンを押していたら攻撃しない
		if (CheckIputKey(CK_SHIELD, KEY_PRESS)) {

		}
		//下攻撃
		else if (CheckIputKey(CK_DOWN, KEY_PRESS)) {
			DownAttack();

		}
		//上攻撃
		else if (CheckIputKey(CK_UP, KEY_PRESS)) {
			UpAttack();

		}
		//横攻撃
		else if (CheckIputKey(CK_RIGHT, KEY_PRESS) || CheckIputKey(CK_LEFT, KEY_PRESS)) {
			DirAttack();

		}

		//ニュートラル
		//else Attack();
	}
	else if (CheckIputKey(CK_ATTACK, KEY_PRESS)) {
		Attack();
	}
	else if (CheckIputKey(CK_ATTACK, KEY_RELEASE)) {
		SubAttack();
	}


	//シールド
	ShieldFunction();


	//デバッグ
	/*
	if (KeyManager_Trigger(KM_K)) {
		ParticleSystem* pEf = EffectManager::SetParticle({ m_Position.x,m_Position.y + 3,1 }, 1.0f, 64, 16, { 0.3f,0.3f, 0.3f }, 16, 320, ImageManager::GetTexId(TEX_PARTICLE_FLASH));
		pEf->SetisInstancing(true);
		//pEf->SetisInstancing(false);
		pEf->SetVelocityMax({ 0.3f,0.5f, 0.0f });
		pEf->SetVelocityMin({ -0.3f,0.1f, 0.0f });
	}
	*/

}

void AnimationPlayer::CPUUpdate() {


	battleAI->Update();

}

void AnimationPlayer::CPUThink() {

	battleAI->Think();

}

void AnimationPlayer::SandbagUpdate() {


}

void AnimationPlayer::DebugDraw() {

	//当たり判定描画
	HitBoxDraw();

}

void AnimationPlayer::HitBoxDraw() {

	D3DXVECTOR3 pHBoxScale = pHitBoxModel.GetScale();
	D3DXVECTOR3 pHBoxPos = pHitBoxModel.GetPosition();

	//マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, pHBoxScale.x, pHBoxScale.y, pHBoxScale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, pHBoxPos.x, pHBoxPos.y, pHBoxPos.z);

	world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);

	pHitBoxModel.Draw();
	


}

void AnimationPlayer::HitBoxInit() {

	pHitBoxModel.SetScale({ 0.3f,0.3f, 0.3f });
	D3DXVECTOR3 setPos = m_Position;
	setPos.y += 1.0f;
	pHitBoxModel.SetPosition(setPos);


}

void AnimationPlayer::HitBoxUpdate() {

	//hitPosition = m_Position;
	//hitPosition.y += 1.0f;

	D3DXVECTOR3 setPos = m_Position;
	setPos.y += 1.0f;
	pHitBoxModel.SetPosition(setPos);

}


void AnimationPlayer::BlownCharacter(D3DXVECTOR2 s_vec) {

	blownVelocity.x = s_vec.x;
	blownVelocity.y = s_vec.y;

}

float AnimationPlayer::GetBlownCorrect() {

	return accumulationDamage / BLOWN_CORRECT_BASE;

}

void AnimationPlayer::DamageFunction(int s_add) {

	//ダメージ蓄積
	AddDamage(s_add);

	//無敵時間設定
	invincibleTime = DAMAGE_INVINCIBLETIME;

	//UI更新
	damageTextEasingTime = 50;
	DamageUIUpdate();

	//コンディション解除
	if (charaCon == CC_SHIELDBREAK) charaCon = CC_NORMAL;
	else if (charaCon == CC_CLLIFGRAP) charaCon = CC_NORMAL;

	//攻撃解除
	AttackObjDeath();


}

void AnimationPlayer::BlownEffect() {

	//エフェクト
	D3DXVECTOR4 s_startColor, s_lastColor;
	s_startColor = { 255 / 255.0f,255 / 255.0f,255 / 255.0f,168 / 255.0f };
	s_lastColor = { 255 / 255.0f,255 / 255.0f,255 / 255.0f,64 / 255.0f };
	D3DXVECTOR3 maxVec, minVec;
	maxVec = { 0.1f,0.1f ,0.5f };
	minVec = { -0.1f,-0.1f,-0.5f };
	//吹っ飛ばし速度によって持続時間を変える
	int life = 0;
	if (D3DXVec3Length(&blownVelocity) > 0.4f) {
		life = 64;
	}
	else if (D3DXVec3Length(&blownVelocity) > 0.2f) {
		life = 32;
	}
	else if (D3DXVec3Length(&blownVelocity) > 0.05f) {
		life = 16;
	}



	if (life > 0) {
		ParticleSystem* pPS = EffectManager::SetParticle();
		pPS->SetPosition(m_Position);
		pPS->SetColor(s_startColor);
		pPS->SetLastColor(s_lastColor);
		pPS->SetLife(life);
		pPS->SetParticleLife(32);
		pPS->SetVelocityMax(maxVec);
		pPS->SetVelocityMin(minVec);
		pPS->SetInterval(8);
		pPS->SetSpawnNum(4);
		pPS->SetSize(0.4f);
		pPS->SetLastSize({ 5,5 });
		pPS->SetTexture(ImageManager::GetTexId(TEX_EFF_MOYA));
		pPS->SetisInstancing(true);
		pPS->FollowObect(this);
		pPS->SetFollowOffset({ 0,0.5f,0 });
		pPS->Init();
	}

}

void AnimationPlayer::InvincibleUpdate() {

	if (invincibleTime > 0) {

		invincibleTime--;

		//無敵表現
		SetInvincibleColor();

		if (invincibleTime == 0) shaderPlusColor = { 0,0,0,0 };

	}
}

bool AnimationPlayer::CheckInvincible() {

	if (invincibleTime > 0) return true;
	return false;
}

void AnimationPlayer::DamageUIUpdate() {
	
	/*
	//イージング関数
	if (damageTextEasingTime < EASING_DAMAGETEXT_TIME) {
		damageTextEasingTime++;

		float c4 = (2 * 3.14) / 3;
		float s_x = damageTextEasingTime / EASING_DAMAGETEXT_TIME;

		float cl = -pow(2, 10 * s_x - 10) * sin((s_x * 10 - 10.75) * c4);

		DamageText->SizeUpdate({64 * cl,64 * cl });

		
		return x == = 0
			? 0
			: x == = 1
			? 1
			: -pow(2, 10 * x - 10) * sin((x * 10 - 10.75) * c4);
		
	}*/

	char buf[64];
	sprintf(buf, "%d%%", accumulationDamage);
	DamageText->TextUpdate(buf);
}

void AnimationPlayer::DeathFunction() {

	//死亡範囲
	Area InStage = StageSystem::GetInStageArea();
	float rightMax =  InStage.rightMax;
	float leftMax  =  InStage.leftMax;
	float upMax    =  InStage.upMax;
	float downMax  =  InStage.downMax;

	//死亡範囲なら死亡
	if (m_Position.x > rightMax || leftMax > m_Position.x || upMax < m_Position.y || downMax > m_Position.y) {

		//残機を減らす
		lifeStock--;

		//死亡エフェクト
		//Effect* pE = EffectManager::SetBurstEffect(m_Position, 0);
		//死亡位置によって向きを変える
		D3DXVECTOR3 minVec = { -10.0f,-2.0f, 0.0f }, maxVec = { -1.0f,2.0f, 0.0f };

		if (leftMax > m_Position.x) {
			maxVec = { 10.0f,2.0f, 0.0f };
			minVec = { 1.0f,-2.0f, 0.0f };
		}
		else if (upMax < m_Position.y) {
			minVec = {-2.0f,-10.0f, 0.0f };
			maxVec = { 2.0f, -1.0f, 0.0f };
		}
		else if (downMax > m_Position.y) {
			minVec = { -2.0f,1.0f, 0.0f };
			maxVec = { 2.0f, 10.0f, 0.0f };
		}
		//死亡エフェクト
		//カラーランダム
		int cd = rand() % 4;

		ParticleSystem* pPS = EffectManager::SetParticle();
		pPS->SetPosition(m_Position);
		if(cd == 0) pPS->SetColor({1,0,0,1});
		else if (cd == 1) pPS->SetColor({ 0,1,0,1 });
		else if (cd == 2) pPS->SetColor({ 0,1,1,1 });
		else if (cd == 3) pPS->SetColor({ 0,0,1,1 });

		pPS->SetLife(64);
		pPS->SetParticleLife(32);
		pPS->SetVelocityMax(maxVec);
		pPS->SetVelocityMin(minVec);
		pPS->SetInterval(4);
		pPS->SetSpawnNum(64);
		pPS->SetSize(0.2f);
		pPS->SetTexture(ImageManager::GetTexId(TEX_PARTICLE_FLASH));
		pPS->SetisInstancing(true);
		pPS->Init();

		//UI更新
		StatesWindow->SetStockImageisUse(lifeStock, false);

		//撃破音
		AudioManager::AudioPlay(AK_SE_BLOWMIDDLE,false);

		//残機が残っていたらリスポーン
		if (lifeStock > 0) Respawn();
		else {
			//残っていなかったら死亡 後で追加

			//画面から消すような処理をする


			//生存時間を保存
			GameSystem::SetSurviveTime(playerNumber - 1);

		}

	}

}


void AnimationPlayer::Respawn() {

	//リスポーン位置に移動
	m_Position = RESPAWN_POINT;

	//無敵時間設定
	invincibleTime = RESPAWN_INVINCIBLETIME;

	//速度を初期化
	velocity = { 0,0,0 };
	blownVelocity = {0,0,0};

	//ダメージリセット
	DamageFunction(-999);

	//アニメーションセット
	SetAnimationState(AS_WAIT);

}

void AnimationPlayer::SetCharacterParameter() {

	//キャラクターごとにパラメータ設定

	switch (charaKind)
	{
	case CK_TEST:

		//重さ
		weight = 0.5f;

		//ジャンプ力
		jumpPower = 0.2f;

		//ジャンプ回数
		addJumpMax = 1;

		//移動速度
		moveSpeed = CHARA_MOVESPEED_DEFAULT;

		//当たり判定大きさ
		hitScale = { 0.3f,0.3f, 0.3f };

		//シールド持続時間
		shieldLife = shieldLifeMax = 180;

		//技データ入れる
		SetAttackData();

		break;
	default:
		break;
	}


}

void AnimationPlayer::SetInvincibleColor() {

	if (invincibleColorSw) shaderPlusColor = { 1,1,1,1 };
	else shaderPlusColor = { 0,0,0,0 };

	if (invincibleTime % 3 == 0) invincibleColorSw = !invincibleColorSw;

}

void AnimationPlayer::ShieldFunction() {

	useShield = false;

	if (shieldWParameter < -0.5f) shieldWParameter += 0.01f;

	//シールド・緊急回避・つかみ
	if (CheckIputKey(CK_SHIELD, KEY_PRESS)) {
		
		if (CheckIputKey(CK_LEFT, KEY_TRIGGER)){
			
			//向き設定
			charaDir = CD_LEFT;

			//緊急回避
			AvoidEmergency();
		}
		else if (CheckIputKey(CK_RIGHT, KEY_TRIGGER)) {

			//向き設定
			charaDir = CD_RIGHT;

			//緊急回避
			AvoidEmergency();
		}
		else if (CheckIputKey(CK_ATTACK, KEY_TRIGGER)) {

			//つかみ
			GrabChara();

		}
		else {//シールド貼る
			ShieldUp();
		}

	}
	else {//貼らない間自動回復
		shieldLife++;
		if (shieldLife > shieldLifeMax) shieldLife = shieldLifeMax;

	}
}

void AnimationPlayer::ShieldBreak() {

	//上に吹っ飛ぶ
	BlownCharacter({ 0,0.1f });

	charaCon = CC_SHIELDBREAK;
	AS = AS_BREAK;

}

void AnimationPlayer::ShieldUp() {


	//シールドを消費
	ShieldDamage(1);

	//シールドがなくなったらブレイク
	if (shieldLife <= 0) {
		ShieldBreak();
		return;
	}
	//あるならシールド状態有効


	//シールド生成
	ShieldObject* psObj = DamageObjectManager::SetShieldObject(m_Position,this);
	D3DXVECTOR3 sScale = D3DXVECTOR3(2.5f,2.5f, 2.5f) * (shieldLife / (float)shieldLifeMax);
	psObj->SetScale(sScale);

	psObj->SetDamage(0);
	psObj->SetBlowPower(0);
	psObj->SetBlowVec({ 0, 0});
	psObj->SetLifeTime(1);
	psObj->SetVelocity({ 0.0f,0.0f, 0.0f });
	psObj->SetPosition({ m_Position.x,m_Position.y + 1.0f,m_Position.z });

	psObj->SetShaderParameter({0,0,0,shieldWParameter});

	useShield = true;

}

bool AnimationPlayer::CheckShield() {

	return useShield;
}

void AnimationPlayer::ShieldDamage(int s_damage) {

	shieldLife -= s_damage;
	if (shieldLife <= 0) {
		shieldLife = 0;
		charaCon = CC_SHIELDBREAK;
	}



}

void AnimationPlayer::ShieldBreakUpdate() {

	if (ShieldBreakTime < SHIELDBREAK_TIME) {
		ShieldBreakTime++;

		//色を調整
		float s_r = sinf(ShieldBreakTime * 0.05f) * 0.5f;//(ShieldBreakTime % 60) /59.0f;
		shaderPlusColor = { s_r + 0.5f,0.05f,0.05f,1 };

	}
	else {
		ShieldBreakTime = 0;
		charaCon = CC_NORMAL;
		shaderPlusColor = {0,0,0,0};
	}
}

bool AnimationPlayer::CheckIputKey(CHARA_KEY cK, KEYINPUT_TYPE KT) {

	switch (cK)
	{
	case CK_UP:

		if (KT == KEY_PRESS) {
			if (playerNumber == 1) {
				if (ControllerManager::GetCDevice(playerNumber - 1) != nullptr) {
					if (ControllerManager::GetButtonState(playerNumber - 1).LstickY == -1000) return true;
					else return false;
				}
				return KeyManager_Press(KM_W);
			}
			else if (playerNumber >= 2) {
				if (ControllerManager::GetCDevice(playerNumber - 1) != nullptr) {
					if (ControllerManager::GetButtonState(playerNumber - 1).LstickY == -1000) return true;
					else return false;
				}
				//return KeyManager_Press(KM_U);
			}
		}
		else if (KT == KEY_TRIGGER) {
			if (playerNumber == 1) {
				if (ControllerManager::GetCDevice(playerNumber - 1) != nullptr) {
					if (ControllerManager::GetButtonState(playerNumber - 1).LstickY == -1000) return true;
					else return false;
				}
				return KeyManager_Trigger(KM_W);
			}

			else if (playerNumber >= 2) {
				if (ControllerManager::GetCDevice(playerNumber - 1) != nullptr) {
					if (ControllerManager::GetButtonState(playerNumber - 1).LstickY == -1000) return true;
					else return false;
				}
				//return KeyManager_Trigger(KM_U);
			}
		}
		else if (KT == KEY_RELEASE) {
			if (playerNumber == 1) return KeyManager_Release(KM_W);
			else if (playerNumber == 2) return KeyManager_Release(KM_U);
		}

		break;
	case CK_DOWN:

		if (KT == KEY_PRESS) {
			if (playerNumber == 1) {
				if (ControllerManager::GetCDevice(playerNumber - 1) != nullptr) {
					if (ControllerManager::GetButtonState(playerNumber - 1).LstickY == 1000) return true;
					else return false;
				}
				return KeyManager_Press(KM_S);
			}
			else if (playerNumber >= 2) {
				if (ControllerManager::GetCDevice(playerNumber - 1) != nullptr) {
					if (ControllerManager::GetButtonState(playerNumber - 1).LstickY == 1000) return true;
					else return false;
				}
				//return KeyManager_Press(KM_J);
			}
		}
		else if (KT == KEY_TRIGGER) {
			if (playerNumber == 1) {
				if (ControllerManager::GetCDevice(playerNumber - 1) != nullptr) {
					if (ControllerManager::GetButtonState(playerNumber - 1).LstickY == 1000) return true;
					else return false;
				}
				return KeyManager_Trigger(KM_S);
			}
			else if (playerNumber >= 2) {
				if (ControllerManager::GetCDevice(playerNumber - 1) != nullptr) {
					if (ControllerManager::GetButtonState(playerNumber - 1).LstickY == 1000) return true;
					else return false;
				}
				//return KeyManager_Trigger(KM_J);
			}
		}
		else if (KT == KEY_RELEASE) {
			if (playerNumber == 1) return KeyManager_Release(KM_S);
			else if (playerNumber == 2) return KeyManager_Release(KM_J);
		}

		break;
	case CK_RIGHT:

		if (KT == KEY_PRESS) {
			if (playerNumber == 1) {
				if (ControllerManager::GetCDevice(playerNumber - 1) != nullptr) {
					if (ControllerManager::GetButtonState(playerNumber - 1).LstickX == 1000) return true;
					else return false;
				}
				return KeyManager_Press(KM_D);

			}
			else if (playerNumber >= 2) {
				if (ControllerManager::GetCDevice(playerNumber - 1) != nullptr) {
					if (ControllerManager::GetButtonState(playerNumber - 1).LstickX == 1000) return true;
					else return false;
				}
				//return KeyManager_Press(KM_K);
			}
		}
		else if (KT == KEY_TRIGGER) {
			if (playerNumber == 1) {
				if (ControllerManager::GetCDevice(playerNumber - 1) != nullptr) {
					if (ControllerManager::GetButtonState(playerNumber - 1).LstickX == 1000) return true;
					else return false;
				}
				return KeyManager_Trigger(KM_D);
			}
			else if (playerNumber >= 2) {
				if (ControllerManager::GetCDevice(playerNumber - 1) != nullptr) {
					if (ControllerManager::GetButtonState(playerNumber - 1).LstickX == 1000) return true;
					else return false;
				}
				//return KeyManager_Trigger(KM_K);
			}
		}
		else if (KT == KEY_RELEASE) {
			if (playerNumber == 1) return KeyManager_Release(KM_D);
			else if (playerNumber == 2) return KeyManager_Release(KM_K);
		}

		break;
	case CK_LEFT:

		if (KT == KEY_PRESS) {
			if (playerNumber == 1) {
				if (ControllerManager::GetCDevice(playerNumber - 1) != nullptr) {
					if (ControllerManager::GetButtonState(playerNumber - 1).LstickX == -1000) return true;
					else return false;
				}
				return KeyManager_Press(KM_A);
			}
			else if (playerNumber >= 2) {
				if (ControllerManager::GetCDevice(playerNumber - 1) != nullptr) {
					if (ControllerManager::GetButtonState(playerNumber - 1).LstickX == -1000) return true;
					else return false;
				}
				//return KeyManager_Press(KM_H);
			}
		}
		else if (KT == KEY_TRIGGER) {
			if (playerNumber == 1) {
				if (ControllerManager::GetCDevice(playerNumber - 1) != nullptr) {
					if (ControllerManager::GetButtonState(playerNumber - 1).LstickX == -1000) return true;
					else return false;
				}
				return KeyManager_Trigger(KM_A);
			}
			else if (playerNumber >= 2) {
				if (ControllerManager::GetCDevice(playerNumber - 1) != nullptr) {
					if (ControllerManager::GetButtonState(playerNumber - 1).LstickX == -1000) return true;
					else return false;
				}
				//return KeyManager_Trigger(KM_H);
			}
		}
		else if (KT == KEY_RELEASE) {
			if (playerNumber == 1) return KeyManager_Release(KM_A);
			else if (playerNumber == 2) return KeyManager_Release(KM_H);
		}

		break;
	case CK_JUMP:

		if (KT == KEY_PRESS) {
			if (playerNumber == 1) {
				if (ControllerManager::GetCDevice(playerNumber - 1) != nullptr) {
					if (ControllerManager::GetButtonState(playerNumber - 1).aButton == BS_PRESS) return true;
					else return false;
				}
				return KeyManager_Press(KM_DECIDE);
			}

			else if (playerNumber >= 2) {
				if (ControllerManager::GetCDevice(playerNumber - 1) != nullptr) {
					if (ControllerManager::GetButtonState(playerNumber - 1).aButton == BS_PRESS) return true;
					else return false;
				}
				//return KeyManager_Press(KM_N);
			}
		}
		else if (KT == KEY_TRIGGER) {
			if (playerNumber == 1) {
				if (ControllerManager::GetCDevice(playerNumber - 1) != nullptr) {
					if (ControllerManager::GetButtonState(playerNumber - 1).aButton == BS_TRIGGER) return true;
					else return false;
				}
				return KeyManager_Trigger(KM_DECIDE);
			}
			else if (playerNumber >= 2) {
				if (ControllerManager::GetCDevice(playerNumber - 1) != nullptr) {
					if (ControllerManager::GetButtonState(playerNumber - 1).aButton == BS_TRIGGER) return true;
					else return false;
				}
				//return KeyManager_Trigger(KM_N);
			}
		}
		else if (KT == KEY_RELEASE) {
			if (playerNumber == 1) {
				if (ControllerManager::GetCDevice(playerNumber - 1) != nullptr) {
					if (ControllerManager::GetButtonState(playerNumber - 1).aButton == BS_RELEASE) return true;
					else return false;
				}
				return KeyManager_Release(KM_DECIDE);
			}
			else if (playerNumber >= 2) {
				if (ControllerManager::GetCDevice(playerNumber - 1) != nullptr) {
					if (ControllerManager::GetButtonState(playerNumber - 1).aButton == BS_RELEASE) return true;
					else return false;
				}
				//return KeyManager_Release(KM_N);
			}
		}

		break;
	case CK_ATTACK:

		if (KT == KEY_PRESS) {
			if (playerNumber == 1) {
				if (ControllerManager::GetCDevice(playerNumber - 1) != nullptr) {
					if (ControllerManager::GetButtonState(playerNumber - 1).bButton == BS_PRESS) return true;
					else return false;
				}
				return KeyManager_Press(KM_CANCEL);
			}
			else if (playerNumber >= 2) {
				if (ControllerManager::GetCDevice(playerNumber - 1) != nullptr) {
					if (ControllerManager::GetButtonState(playerNumber - 1).bButton == BS_PRESS) return true;
					else return false;
				}
				//return KeyManager_Press(KM_M);
			}
		}
		else if (KT == KEY_TRIGGER) {
			if (playerNumber == 1) {
				if (ControllerManager::GetCDevice(playerNumber - 1) != nullptr) {
					if (ControllerManager::GetButtonState(playerNumber - 1).bButton == BS_TRIGGER) return true;
					else return false;
				}
				return KeyManager_Trigger(KM_CANCEL);
			}
			else if (playerNumber >= 2) {
				if (ControllerManager::GetCDevice(playerNumber - 1) != nullptr) {
					if (ControllerManager::GetButtonState(playerNumber - 1).bButton == BS_TRIGGER) return true;
					else return false;
				}
				//return KeyManager_Trigger(KM_M);
			}
		}
		else if (KT == KEY_RELEASE) {
			if (playerNumber == 1) {
				if (ControllerManager::GetCDevice(playerNumber - 1) != nullptr) {
					if (ControllerManager::GetButtonState(playerNumber - 1).bButton == BS_RELEASE) return true;
					else return false;
				}
				return KeyManager_Release(KM_CANCEL);
			}
			else if (playerNumber >= 2) {
				if (ControllerManager::GetCDevice(playerNumber - 1) != nullptr) {
					if (ControllerManager::GetButtonState(playerNumber - 1).bButton == BS_RELEASE) return true;
					else return false;
				}
				//return KeyManager_Release(KM_M);
			}
		}

		break;
	case CK_SHIELD:
		if (KT == KEY_PRESS) {
			if (playerNumber == 1) {
				if (ControllerManager::GetCDevice(playerNumber - 1) != nullptr) {
					if (ControllerManager::GetButtonState(playerNumber - 1).rButton == BS_PRESS) return true;
					else return false;
				}
				return KeyManager_Press(KM_CAMERA);
			}
			else if (playerNumber >= 2) {
				if (ControllerManager::GetCDevice(playerNumber - 1) != nullptr) {
					if (ControllerManager::GetButtonState(playerNumber - 1).rButton == BS_PRESS) return true;
					else return false;
				}
				//return KeyManager_Press(KM_SPACE);
			}
		}
		else if (KT == KEY_TRIGGER) {
			if (playerNumber == 1) {
				if (ControllerManager::GetCDevice(playerNumber - 1) != nullptr) {
					if (ControllerManager::GetButtonState(playerNumber - 1).rButton == BS_TRIGGER) return true;
					else return false;
				}
				return KeyManager_Trigger(KM_CAMERA);
			}
			else if (playerNumber >= 2) {
				if (ControllerManager::GetCDevice(playerNumber - 1) != nullptr) {
					if (ControllerManager::GetButtonState(playerNumber - 1).rButton == BS_TRIGGER) return true;
					else return false;
				}
				//return KeyManager_Trigger(KM_SPACE);
			}
		}
		else if (KT == KEY_RELEASE) {
			if (playerNumber == 1) {
				if (ControllerManager::GetCDevice(playerNumber - 1) != nullptr) {
					if (ControllerManager::GetButtonState(playerNumber - 1).rButton == BS_RELEASE) return true;
					else return false;
				}
				return KeyManager_Release(KM_CAMERA);
			}
			else if (playerNumber >= 2) {
				if (ControllerManager::GetCDevice(playerNumber - 1) != nullptr) {
					if (ControllerManager::GetButtonState(playerNumber - 1).rButton == BS_RELEASE) return true;
					else return false;
				}
				//return KeyManager_Release(KM_SPACE);
			}
		}

		break;
	default:
		break;
	}


	return false;


}

void AnimationPlayer::SetCharacterMode(CHARA_MODE s_cm) {
	charaMode = s_cm;
	//CPUならAI設定
	if (charaMode == CM_CPU) {
		battleAI = new BattleAI(this);
		battleAI->Init();
	}

}

void AnimationPlayer::ThroughFloor() {

	//すり抜け
	throughFlag = MeshFieldBoxManager::CheckBlockThrough(m_Position);

}

//クールタイムが１以上ならtrueを返す
bool AnimationPlayer::ActionCooltimeUpdate() {

	if (actionCooltime > 0) {
		actionCooltime--;
		return true;
	}
	else return false;


}

void AnimationPlayer::CheckGrap() {


	//クールタイム中なら処理しない
	if (grapCooltime > 0) {
		grapCooltime--;
		return;
	}

	////一定の条件を満たしたら崖つかみ状態にする
	if (MeshFieldBoxManager::CheckBlockCliff(m_Position,this)) {

		charaCon = CC_CLLIFGRAP;

		//ジャンプ回数復活
		isJump = false;

	}

}

void AnimationPlayer::CliffGrap() {

	//速度をなくす
	velocity = { 0,0,0 };

	//追加ジャンプ回復
	addJumpCnt = addJumpMax;
	//設地判定
	isJump = false;

	//モーションを崖つかまりに 未実装
	if(cliffGrapData.dir == RIGHT) m_Rotation = {0,1.57f,0};//向き
	else m_Rotation = { 0,-1.57f,0 };//向き
	//AS = AS_WAIT;
	SetAnimationState(AS_CLIFF, 0);

	bool mF = false;

	//ふっとばされていたら解除
	if (D3DXVec3Length(&blownVelocity) > 0.05f) {
		mF = true;
	}


	//入力があるまで移動しない
	if (CheckIputKey(CK_RIGHT, KEY_TRIGGER)) mF = true;
	else if (CheckIputKey(CK_LEFT, KEY_TRIGGER)) mF = true;
	else if (CheckIputKey(CK_DOWN, KEY_TRIGGER)) {//崖つかまりを解除して落下
		mF = true;
	}
	else if (CheckIputKey(CK_UP, KEY_TRIGGER)) {
		
		//崖つかまりを解除してあがる
		mF = true;
		
		CliffAction(CA_UP);
	}
	else if (CheckIputKey(CK_ATTACK, KEY_TRIGGER)) {
	
		//崖つかまりを解除して攻撃
		
		mF = true;

		CliffAction(CA_ATTACK);

	}
	else if (CheckIputKey(CK_JUMP, KEY_TRIGGER)) {
		//崖つかまりを解除してジャンプ
		mF = true;

		Jump();
	}
	else if (CheckIputKey(CK_SHIELD, KEY_TRIGGER)) {
		
		//崖つかまりを解除して回避
		mF = true;

		//回避
		CliffAction(CA_AVOID);
	}
	if (mF) {
		charaCon = CC_NORMAL;
		grapCooltime = CRIFFGRAP_COOLTIME;
	}
}


void AnimationPlayer::CliffAction(CLIFFACTION s_ca) {

	//念のためエラー処理
	if (cliffGrapData.pMBox == nullptr) return;

	MeshFieldBox* pMBox = cliffGrapData.pMBox;
	D3DXVECTOR3 setPos;

	switch (s_ca)
	{
	case AnimationPlayer::CA_UP:

		//つかんでいる崖のデータを参照し、適切な位置に上がる

		setPos.y = pMBox->GetPosition().y;
		if(cliffGrapData.dir == RIGHT) setPos.x = pMBox->GetRightMax() - 1.0f;
		else if (cliffGrapData.dir == LEFT) setPos.x = pMBox->GetLeftMax() + 1.0f;

		m_Position = setPos;

		break;
	case AnimationPlayer::CA_ATTACK:

		//上がる
		setPos.y = pMBox->GetPosition().y;
		if (cliffGrapData.dir == RIGHT) setPos.x = pMBox->GetRightMax() - 1.0f;
		else if (cliffGrapData.dir == LEFT) setPos.x = pMBox->GetLeftMax() + 1.0f;

		m_Position = setPos;

		//攻撃を出す
		CliffAttack();

		break;
	case AnimationPlayer::CA_AVOID:

		//上がる
		setPos.y = pMBox->GetPosition().y;
		if (cliffGrapData.dir == RIGHT) setPos.x = pMBox->GetRightMax() - 1.0f;
		else if (cliffGrapData.dir == LEFT) setPos.x = pMBox->GetLeftMax() + 1.0f;

		m_Position = setPos;

		//無敵時間追加
		invincibleTime = CRIFFAVOID_INVINCIBLETIME;

		//硬直を入れる
		actionCooltime = CRIFFAVOID_INVINCIBLETIME;

		break;
	default:
		break;
	}


}


//技データ
void AnimationPlayer::SetAttackData() {


	//ニュートラル
	atkData[AK_NEUTRAL].range = 12;
	atkData[AK_NEUTRAL].damage = 3;
	atkData[AK_NEUTRAL].delay = 5;
	atkData[AK_NEUTRAL].lifetime = 30;
	atkData[AK_NEUTRAL].blowVec = {0,0.5f};
	atkData[AK_NEUTRAL].blowPower = 0.1f;
	atkData[AK_NEUTRAL].velocity = { 0.1f,0.0,0 };
	atkData[AK_NEUTRAL].rollSpeed = { 0,0,0 };
	atkData[AK_NEUTRAL].scale = { 0.3f,0.3f,0.3f };
	atkData[AK_NEUTRAL].follow = false;
	atkData[AK_NEUTRAL].offsetPos = {0.0f,1.0f,0.0f};

	//上
	atkData[AK_UP].range = 5;
	atkData[AK_UP].damage = 7;
	atkData[AK_UP].delay = 5;
	atkData[AK_UP].lifetime = 20;
	atkData[AK_UP].blowPower = 0.6f;
	atkData[AK_UP].blowVec = {0.0f,0.0f};
	atkData[AK_UP].velocity = { 0.0f,-0.25,0.0f };
	atkData[AK_UP].rollSpeed = { 0,0,0 };
	atkData[AK_UP].scale = { 1.0f,0.6f,0.6f };
	atkData[AK_UP].follow = false;
	atkData[AK_UP].offsetPos = { 0.0f,0.0f,0.0f };

	//下
	atkData[AK_DOWN].range = 2;
	atkData[AK_DOWN].damage = 10;
	atkData[AK_DOWN].delay = 5;
	atkData[AK_DOWN].lifetime = 15;
	atkData[AK_DOWN].blowPower = 0.5f;
	atkData[AK_DOWN].blowVec = { 0,0.5f };
	atkData[AK_DOWN].velocity = { 0.0f,-0.25,0.0f };
	atkData[AK_DOWN].rollSpeed = { 0,0,0 };
	atkData[AK_DOWN].scale = { 2.0f,1.5f,0.3f };
	atkData[AK_DOWN].follow = false;
	atkData[AK_DOWN].offsetPos = { 0.0f,1.0f,0.0f };

	//横
	atkData[AK_DIR].range = 2.5f;
	atkData[AK_DIR].damage = 8;
	atkData[AK_DIR].delay = 5;
	atkData[AK_DIR].lifetime = 30;
	atkData[AK_DIR].blowPower = 0.3f;
	atkData[AK_DIR].blowVec = { 0,0.4f };
	atkData[AK_DIR].velocity = { 0.0f,0.0f,0.0f };
	atkData[AK_DIR].rollSpeed = { 0,0,0 };
	atkData[AK_DIR].scale = { 2.0f,1.0f,0.3f };
	atkData[AK_DIR].follow = false;
	atkData[AK_DIR].offsetPos = { 1.0f,1.0f,0.0f };


}

void AnimationPlayer::CharacterBound(D3DXVECTOR3 s_pos) {

	//現在の吹っ飛ばし速度を参照
	float vel = D3DXVec3Length(&blownVelocity);

	//一定以上の速度なら反射する
	if (vel < BOUND_LIMIT) return;

	//当たり方によって反射角を変更
	blownVelocity.y *= -1;

	//バウンドエフェクト
	D3DXVECTOR3 effPos = m_Position;

	float effSize = 2.0f;

	//前に表示するために動かす
	effPos.z -= 1.0f;
	effPos.y += 0.5f;

	Effect* pE = EffectManager::SetAnimationEffect(effPos, effSize, ImageManager::GetTexId(TEX_EFF_GROUNDSHOCK), GROUNDSHOCKEFFECT_LIFE, GROUNDSHOCKEFFECT_ENUM);
	pE->SetColor({0,1,0,1});

	//右に飛んでいる場合
	/*
	if (blownVelocity.x > 0) {

	}
	*/
}


void AnimationPlayer::AvoidEmergency() {

	//通常状態でなければ処理しない
	if (charaCon != CC_NORMAL) return;

	//地上でなければ処理しない
	if (isJump) return;

	//アニメーション設定
	SetAnimationState(AS_AVOID, 0);

	//今の向きを参照して速度を設定
	if (charaDir == CD_RIGHT) velocity.x = 1.0f / AVOID_INVINCIBLETIME;
	else velocity.x = -1.0f / AVOID_INVINCIBLETIME;
	modelOffsetVelocity.x = -(velocity.x) * 2;
	//無敵時間設定
	invincibleTime = AVOID_INVINCIBLETIME;


	//もしつかみ状態だったらつかみ相手を解放する
	if (charaCon == CC_GRAB) {
		if (GrapingPlayer != nullptr) {

			GrapingPlayer->SetCharaCon(CC_NORMAL);
		}
	}


	//キャラ状態を緊急回避状態にする
	charaCon = CC_AVOID;

	//クールタイム設定
	actionCooltime = AVOID_COOLTIME;


	

}

void AnimationPlayer::AvoidUpdate() {

	//クールタイムが０以下になったら通常状態に戻す
	if (actionCooltime <= 0) {
		charaCon = CC_NORMAL;

		//速度も戻す
		velocity.x = 0;

		//modelOffsetVelocity = { 0,0,0 };
		//modelOffset = {0,0,0};

		SetAnimationState(AS_WAIT,0);

	}

}

void AnimationPlayer::GrabChara() {

	//今誰かつかんでいたら離す
	if (GrapingPlayer != nullptr) {
		GrapingPlayer->GrabRelease();
		return;
	}

	//つかみ判定を生成し、その中に対象がいるかチェック

	Area grabArea;
	if (charaDir == CD_RIGHT) {
		grabArea.leftMax = m_Position.x;
		grabArea.rightMax = m_Position.x + 1.0f;
	}
	else {
		grabArea.leftMax = m_Position.x - 1.0f;
		grabArea.rightMax = m_Position.x;
	}
	grabArea.upMax = m_Position.y + 0.5f;
	grabArea.downMax = m_Position.y - 0.5f;

	GrapingPlayer = PlayerManager::GetPlayerinArea(grabArea,this);

	//つかみに成功
	if (GrapingPlayer != nullptr) {

		//ダメージを与える
		GrapingPlayer->AddDamage(3);
		//UI更新
		GrapingPlayer->DamageUIUpdate();

		//相手をつかまれ状態にする
		GrapingPlayer->SetGrab();

		//自分をつかみ状態にする
		charaCon = CC_GRAB;

		//アニメーション
		SetAnimationState(AS_THROW,29);
		modelFrameStop = true;

		grabHoldTime = 0;
	}

}

void AnimationPlayer::GrabRelease() {

	if (charaCon == CC_GRABBED) charaCon = CC_NORMAL;

}

void AnimationPlayer::SetGrab() {

	charaCon = CC_GRABBED;

}

void AnimationPlayer::GrabingAction() {


	//対象がいないのに投げようとした場合状態を解除する
	if (GrapingPlayer == nullptr) {
		charaCon == CC_NONE;
		return;
	}

	//時間経過で投げ解除
	if (grabHoldTime >= GRABHOLD_TIMEMAX) {

		charaCon = CC_NORMAL;

		GrapingPlayer->SetCharaCon(CC_NORMAL);
		GrapingPlayer = nullptr;

		grabHoldTime = 0;

		return;
	}
	else grabHoldTime++;



	//投げ
	if (CheckIputKey(CK_RIGHT, KEY_TRIGGER)) {
		//右投げ

		//対象にダメージ
		GrapingPlayer->AddDamage(8);

		//UI更新
		GrapingPlayer->DamageUIUpdate();

		//相手を吹っ飛ばす
		float bPower = 0.5f;
		D3DXVECTOR2 b_vec;
		float blowPower = bPower * GrapingPlayer->GetBlownCorrect();//ふっとばし力  技の吹っ飛ばし力 * 累積ダメージによる吹っ飛ばし係数
		D3DXVECTOR2 dObjtochara_Vec;
		dObjtochara_Vec.x = 0.3f;
		dObjtochara_Vec.y = 0.3f;
		D3DXVec2Normalize(&dObjtochara_Vec, &dObjtochara_Vec);

		b_vec = (dObjtochara_Vec) * blowPower;//(攻撃位置から敵位置へのベクトル + 技ごとの方向補正ベクトル) * ふっとばし力
		GrapingPlayer->BlownCharacter(b_vec);

		//対象の状態を開放
		GrapingPlayer->GrabRelease();
		GrapingPlayer = nullptr;

		//自分の状態を開放
		charaCon = CC_NORMAL;

		//硬直
		actionCooltime = 20;

		//アニメーション
		SetAnimationState(AS_THROW,90);

	}
	else if (CheckIputKey(CK_LEFT, KEY_TRIGGER)) {
		//左投げ

		//対象にダメージ
		GrapingPlayer->AddDamage(8);

		//相手を吹っ飛ばす
		float bPower = 0.5f;
		D3DXVECTOR2 b_vec;
		float blowPower = bPower * GrapingPlayer->GetBlownCorrect();//ふっとばし力  技の吹っ飛ばし力 * 累積ダメージによる吹っ飛ばし係数
		D3DXVECTOR2 dObjtochara_Vec;
		dObjtochara_Vec.x = -0.3f;
		dObjtochara_Vec.y =  0.3f;
		D3DXVec2Normalize(&dObjtochara_Vec, &dObjtochara_Vec);

		b_vec = (dObjtochara_Vec)* blowPower;//(攻撃位置から敵位置へのベクトル + 技ごとの方向補正ベクトル) * ふっとばし力
		GrapingPlayer->BlownCharacter(b_vec);


		//UI更新
		GrapingPlayer->DamageUIUpdate();

		//対象の状態を開放
		GrapingPlayer->GrabRelease();
		GrapingPlayer = nullptr;

		//自分の状態を開放
		charaCon = CC_NORMAL;

		//硬直
		actionCooltime = 20;

		//アニメーション
		SetAnimationState(AS_THROW,90);

	}



}


void AnimationPlayer::SetAnimationState(ANIMATION_STATE s_as) {

	AS = s_as;

	//モデル速度を初期化する

	modelOffsetVelocity = { 0,0,0 };
	modelOffset = { 0,0,0 };
	modelRotate = { 0,0,0 };

	modelFrameStop = false;
}

void AnimationPlayer::SetAnimationState(ANIMATION_STATE s_as,int s_startFrame) {

	SetAnimationState(s_as);

	aModel_frame = s_startFrame;
}

void AnimationPlayer::AnimationStateUpdate() {

	//アニメーション関連
	//ふっとばされアニメーション
	if (D3DXVec3Length(&blownVelocity) > 0.01f) {
		SetAnimationState(AS_BLOWN, aModel_frame);
	}

	switch (AS)
	{
	case AS_WAIT:
		pastAs_name = as_name;
		as_name = "Idle";
		break;
	case AS_RUN:
		pastAs_name = as_name;
		as_name = "Run";

		break;
	case AS_JUMP:
		pastAs_name = as_name;
		as_name = "Jump";
		break;
	case AS_AVOID:
		pastAs_name = as_name;
		as_name = "Avoid";
		break;
	case AS_THROW:
		pastAs_name = as_name;
		as_name = "Throw";
		break;
	case AS_BLOWN:
		pastAs_name = as_name;
		as_name = "Blown";
		break;
	case AS_CLIFF:
		pastAs_name = as_name;
		as_name = "Cliff";
		break;
	case AS_NATTACK:
		pastAs_name = as_name;
		as_name = "Arrow";
		break;
	case AS_DOWNATTACK:
		pastAs_name = as_name;
		as_name = "Flipkick";
		break;
	case AS_BREAK:
		pastAs_name = as_name;
		as_name = "Break";
		break;
	case AS_DIRATTACK:
		pastAs_name = as_name;
		as_name = "Sliding";
		break;
	case AS_UPATTACK:
		pastAs_name = as_name;
		as_name = "UpArrow";
		break;
	default:
		break;
	}

	//ジャンプを繰り返さないようにする処理
	if (as_name == "Jump" && aModel_frame >= JUMPANIME_FRAMEMAX) {
		SetAnimationState(AS_BLOWN, 0);
	}


	a_Model->Update(as_name, pastAs_name, 0.3f, aModel_frame);

	if(!modelFrameStop) aModel_frame++;

	//オフセット速度
	modelOffset += modelOffsetVelocity;

}

void AnimationPlayer::SetStartStates() {

	switch (playerNumber)
	{

	case 1:

		charaDir = CD_RIGHT;
		m_Position.x += -2;
		m_Position.y = 0.1f;
		m_Position.z = 0.0f;
		DirectionRoll({ 1,0 }, 270);

		break;

	case 2:

		charaDir = CD_LEFT;
		m_Position.x += 2;
		m_Position.y = 0.1f;
		m_Position.z = 0.0f;
		DirectionRoll({ -1,0 }, 270);

	case 3:

		charaDir = CD_RIGHT;
		m_Position.x -= 3;
		m_Position.y = 0.1f;
		m_Position.z = 0.0f;
		DirectionRoll({ 1,0 }, 270);

	case 4:

		charaDir = CD_LEFT;
		m_Position.x += 3;
		m_Position.y = 0.1f;
		m_Position.z = 0.0f;
		DirectionRoll({ -1,0 }, 270);

		break;

	default:
		break;
	}

}

bool AnimationPlayer::CheckActionCharge(int chargeCheckNum) {

	if (actionCharge >= chargeCheckNum) return true;
	return false;
}

void AnimationPlayer::ActionCharge() {

	actionCharge++;
}

void AnimationPlayer::ChargeReset() {
	actionCharge = 0;
}

void AnimationPlayer::ModelDataLoad() {

	a_Model->Load("asset/model/Player1.fbx");

	a_Model->LoadAnimation("asset/model/animation/player1_Wait.fbx", "Idle");
	a_Model->LoadAnimation("asset/model/Akai_Run.fbx", "Run");
	a_Model->LoadAnimation("asset/model/Akai_Jump.fbx", "Jump");

	a_Model->LoadAnimation("asset/model/animation/avoid.fbx", "Avoid");
	a_Model->LoadAnimation("asset/model/animation/Falling.fbx", "Blown");
	a_Model->LoadAnimation("asset/model/animation/Throw.fbx", "Throw");
	a_Model->LoadAnimation("asset/model/animation/cliff.fbx", "Cliff");
	a_Model->LoadAnimation("asset/model/animation/breakReaction.fbx", "Break");

	//弓
	a_Model->LoadAnimation("asset/model/animation/flipKick.fbx", "Flipkick");
	a_Model->LoadAnimation("asset/model/animation/arrow.fbx", "Arrow");
	a_Model->LoadAnimation("asset/model/animation/sliding.fbx", "Sliding");
	a_Model->LoadAnimation("asset/model/animation/upArrow.fbx", "UpArrow");

}


void AnimationPlayer::PlayerSignInit() {

	float s_size = 0.5f;

	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(-s_size, -s_size, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);//色
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(+s_size, -s_size, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(-s_size, +s_size, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(+s_size, +s_size, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &UIRenset.m_VertexBuffer);


	Renderer::CreateVertexShader(&UIRenset.m_VertexShader, &UIRenset.m_VertxeLayout ,"charaShaderVS.cso");
	Renderer::CreatePixelShader(&UIRenset.m_PixelShader, "charaShaderPS.cso");

	if (playerNumber == 1) UIRenset.m_Texture = ImageManager::GetTex(TEX_UI_PLAYERSIGN1);
	else if(playerNumber == 2) UIRenset.m_Texture = ImageManager::GetTex(TEX_UI_PLAYERSIGN2);
	else if (playerNumber == 3) UIRenset.m_Texture = ImageManager::GetTex(TEX_UI_PLAYERSIGN3);
	else if (playerNumber == 4) UIRenset.m_Texture = ImageManager::GetTex(TEX_UI_PLAYERSIGN4);


}


void AnimationPlayer::PlayerSignDraw() {

	
	D3DXVECTOR3 sPos = m_Position;
	sPos.y += 2.0f;

	//Billboard::BillBorad_Draw(UIRenset,sPos);
	
	if (playerNumber == 1) Billboard::BillBorad_Draw(ImageManager::GetTexId(TEX_UI_PLAYERSIGN1),sPos,0.5f,0.5f,0);
	else if (playerNumber == 2) Billboard::BillBorad_Draw(ImageManager::GetTexId(TEX_UI_PLAYERSIGN2), sPos, 0.5f, 0.5f, 0);
	else if (playerNumber == 3) Billboard::BillBorad_Draw(ImageManager::GetTexId(TEX_UI_PLAYERSIGN3), sPos, 0.5f, 0.5f, 0);
	else if (playerNumber == 4) Billboard::BillBorad_Draw(ImageManager::GetTexId(TEX_UI_PLAYERSIGN4), sPos, 0.5f, 0.5f, 0);


}




////////騎士/////////

void AP_Paradin::ModelDataLoad() {

	a_Model->Load("asset/model/paladin.fbx");

	a_Model->LoadAnimation("asset/model/animation/player1_Wait.fbx", "Idle");
	a_Model->LoadAnimation("asset/model/Akai_Run.fbx", "Run");
	a_Model->LoadAnimation("asset/model/Akai_Jump.fbx", "Jump");

	a_Model->LoadAnimation("asset/model/animation/avoid.fbx", "Avoid");
	a_Model->LoadAnimation("asset/model/animation/Falling.fbx", "Blown");
	a_Model->LoadAnimation("asset/model/animation/Throw.fbx", "Throw");
	a_Model->LoadAnimation("asset/model/animation/cliff.fbx", "Cliff");
	a_Model->LoadAnimation("asset/model/animation/breakReaction.fbx", "Break");

	a_Model->LoadAnimation("asset/model/animation/paladin/paladinSlash.fbx", "Slash");
	a_Model->LoadAnimation("asset/model/animation/paladin/paladinJumpSlash.fbx", "Jumpslash");
	a_Model->LoadAnimation("asset/model/animation/paladin/paladinShield.fbx", "Pushshield");
	a_Model->LoadAnimation("asset/model/animation/paladin/paladinThreeAttack6.fbx", "Threeattack");

}



void  AP_Paradin::PlayerUpdate() {

	//x方向速度0
	velocity.x = 0;

	//移動
	if (isJump) {
		//SetAnimationState(AS_JUMP);
		AnimationCharacterMove();
	}
	else {
		if (AnimationCharacterMove()) {
			SetAnimationState(AS_RUN);
		}
		else {
			SetAnimationState(AS_WAIT);
		}
	}

	//ジャンプ
	if (CheckIputKey(CK_JUMP, KEY_TRIGGER))Jump();
	//攻撃
	if (CheckIputKey(CK_ATTACK, KEY_TRIGGER)) {

		//シールドボタンを押していたら攻撃しない
		if (CheckIputKey(CK_SHIELD, KEY_PRESS)) {

		}
		//下攻撃
		else if (CheckIputKey(CK_DOWN, KEY_PRESS)) {
			DownAttack();

		}
		//上攻撃
		else if (CheckIputKey(CK_UP, KEY_PRESS)) {
			UpAttack();

		}
		//横攻撃
		else if (CheckIputKey(CK_RIGHT, KEY_PRESS) || CheckIputKey(CK_LEFT, KEY_PRESS)) {
			DirAttack();

		}

		//ニュートラル
		else Attack();
	}


	//シールド
	ShieldFunction();

}

void AP_Paradin::Attack() {
	
	if (!upAttackAble) return;

	DamageObject* dObj = DamageObjectManager::SetDObjT<DamageObject>(m_Position, this);

	D3DXVECTOR3 oPos = atkData[AK_NEUTRAL].offsetPos + m_Position;
	if (charaDir == CD_LEFT) oPos.x = (atkData[AK_NEUTRAL].offsetPos.x * -1) + m_Position.x;
	
	//パラメータ設定

	dObj->SetDamage(atkData[AK_NEUTRAL].damage);
	dObj->SetBlowPower(atkData[AK_NEUTRAL].blowPower);
	dObj->SetBlowVec(atkData[AK_NEUTRAL].blowVec);
	dObj->SetLifeTime(atkData[AK_NEUTRAL].lifetime);
	dObj->SetScale(atkData[AK_NEUTRAL].scale);
	dObj->SetVelocity(atkData[AK_NEUTRAL].velocity);
	dObj->SetDelayTime(atkData[AK_NEUTRAL].delay);
	dObj->SetPosition(oPos);
	dObj->FollowOwnerOn();

	//dObj->SetisDraw(true);

	dObj->SetSoundKind(AK_SE_ZANGEKI);


	//硬直時間
	SetActionCooltime(25);

	//アニメーションセット
	SetAnimationState(AS_NATTACK,0);

	//攻撃中情報をセット
	attackObj = dObj;


}

void AP_Paradin::UpAttack() {
	
	if (!upAttackAble) return;

	DamageObject* dObj = DamageObjectManager::SetDObjT<DamageObject>(m_Position, this);

	D3DXVECTOR3 oPos = atkData[AK_UP].offsetPos + m_Position;
	if (charaDir == CD_LEFT) oPos.x = (atkData[AK_UP].offsetPos.x * -1) + m_Position.x;
	float atkSpeed = 1.0f;
	if (charaDir == CD_LEFT) atkSpeed *= -1;
	//パラメータ設定

	dObj->SetDamage(atkData[AK_UP].damage);
	dObj->SetBlowPower(atkData[AK_UP].blowPower);
	dObj->SetBlowVec(atkData[AK_UP].blowVec);
	dObj->SetLifeTime(atkData[AK_UP].lifetime);
	dObj->SetScale(atkData[AK_UP].scale);
	dObj->SetVelocity(atkData[AK_UP].velocity);
	dObj->SetDelayTime(atkData[AK_UP].delay);
	dObj->SetPosition(oPos);
	dObj->FollowOwnerOn();

	dObj->SetSoundKind(AK_SE_ZANGEKI);


	//dObj->SetisDraw(true);

	//速度生成
	velocity.x = atkData[AK_UP].range/45.0f * atkSpeed;
	velocity.y = 2.0f/45;

	upAttackAble = false;

	//硬直時間
	SetActionCooltime(45);

	//アニメーションセット
	SetAnimationState(AS_UPATTACK, 0);
	modelOffsetVelocity = { -3.0f/45.0f * atkSpeed ,0,0 };

	//攻撃中情報をセット
	attackObj = dObj;

}


void AP_Paradin::DownAttack() {

	if (!upAttackAble) return;

	DamageObject* dObj = DamageObjectManager::SetDObjT<DamageObject>(m_Position, this);

	D3DXVECTOR3 oPos = atkData[AK_NEUTRAL].offsetPos + m_Position;
	if (charaDir == CD_LEFT) oPos.x = (atkData[AK_NEUTRAL].offsetPos.x * -1) + m_Position.x;

	//パラメータ設定

	dObj->SetDamage(atkData[AK_DOWN].damage);
	dObj->SetBlowPower(atkData[AK_DOWN].blowPower);
	dObj->SetBlowVec(atkData[AK_DOWN].blowVec);
	dObj->SetLifeTime(atkData[AK_DOWN].lifetime);
	dObj->SetScale(atkData[AK_DOWN].scale);
	dObj->SetVelocity(atkData[AK_DOWN].velocity);
	dObj->SetDelayTime(atkData[AK_DOWN].delay);
	dObj->SetPosition(oPos);
	dObj->FollowOwnerOn();

	dObj->SetSoundKind(AK_SE_BLOWMIDDLE);


	//dObj->SetisDraw(true);

	//硬直時間
	SetActionCooltime(25);

	//アニメーションセット
	SetAnimationState(AS_DOWNATTACK, 0);

	//攻撃中情報をセット
	attackObj = dObj;

}



void AP_Paradin::DirAttack() {

	if (!upAttackAble) return;

	DamageObject* dObj = DamageObjectManager::SetDObjT<DamageObject>(m_Position, this);

	D3DXVECTOR3 oPos = atkData[AK_DIR].offsetPos + m_Position;
	if (charaDir == CD_LEFT) oPos.x = (atkData[AK_DIR].offsetPos.x * -1) + m_Position.x;

	//パラメータ設定

	dObj->SetDamage(atkData[AK_DIR].damage);
	dObj->SetBlowPower(atkData[AK_DIR].blowPower);
	dObj->SetBlowVec(atkData[AK_DIR].blowVec);
	dObj->SetLifeTime(atkData[AK_DIR].lifetime);
	dObj->SetScale(atkData[AK_DIR].scale);
	dObj->SetVelocity(atkData[AK_DIR].velocity);
	dObj->SetDelayTime(atkData[AK_DIR].delay);
	dObj->SetPosition(oPos);
	dObj->FollowOwnerOn();

	//dObj->SetisDraw(true);

	dObj->SetSoundKind(AK_SE_ZANGEKI);


	//エフェクト
	D3DXVECTOR3 maxVec, minVec;
	maxVec = { 1.1f,2.6f ,0.1f };
	minVec = { -0.1f,0.6f,-0.1f };

	ParticleSystem* pPS = EffectManager::SetParticle();
	pPS->SetPosition({ m_Position.x,m_Position.y + 1.5f,m_Position.z - 1.0f });
	pPS->SetColor({ 255 / 255.0f,102 / 255.0f,102 / 255.0f,196 / 255.0f });
	pPS->SetLastColor({ 255 / 255.0f,182 / 255.0f,102 / 255.0f,0 / 255.0f });
	pPS->SetLife(60);
	pPS->SetParticleLife(30);
	pPS->SetVelocityMax(maxVec);
	pPS->SetVelocityMin(minVec);
	pPS->SetInterval(4);
	pPS->SetSpawnNum(24);
	pPS->SetSize(0.2f);
	pPS->SetTexture(ImageManager::GetTexId(TEX_PARTICLE_FLASH));
	pPS->SetisInstancing(true);
	pPS->Init();


	velocity.x = 0;

	//硬直時間
	SetActionCooltime(120);

	//アニメーションセット
	SetAnimationState(AS_DIRATTACK, 0);
	//pastAs_name = "Threeattack";

	//攻撃中情報をセット
	attackObj = dObj;

}




void AP_Paradin::SetStates() {

	//重さ
	weight = 1.0f;

	//ジャンプ力
	jumpPower = 0.18f;

	//ジャンプ回数
	addJumpMax = 1;

	//移動速度
	moveSpeed = CHARA_MOVESPEED_DEFAULT * 0.5f;

	//当たり判定大きさ
	hitScale = { 0.4f,0.4f, 0.4f };

	//シールド持続時間
	shieldLife = shieldLifeMax = 180;

	//技データ入れる
	SetAttackData();
}

void AP_Paradin::AnimationStateUpdate() {

	//アニメーション関連
	//ふっとばされアニメーション
	if (D3DXVec3Length(&blownVelocity) > 0.05f) {
		SetAnimationState(AS_BLOWN, aModel_frame);
	}

	switch (AS)
	{
	case AS_WAIT:
		pastAs_name = as_name;
		as_name = "Idle";
		break;
	case AS_RUN:
		pastAs_name = as_name;
		as_name = "Run";

		break;
	case AS_JUMP:
		pastAs_name = as_name;
		as_name = "Jump";
		break;
	case AS_AVOID:
		pastAs_name = as_name;
		as_name = "Avoid";
		break;
	case AS_THROW:
		pastAs_name = as_name;
		as_name = "Throw";
		break;
	case AS_BLOWN:
		pastAs_name = as_name;
		as_name = "Blown";
		break;
	case AS_CLIFF:
		pastAs_name = as_name;
		as_name = "Cliff";
		break;
	case AS_NATTACK:
		pastAs_name = as_name;
		as_name = "Slash";
		break;
	case AS_DOWNATTACK:
		pastAs_name = as_name;
		as_name = "Pushshield";
		break;
	case AS_BREAK:
		pastAs_name = as_name;
		as_name = "Break";
		break;
	case AS_DIRATTACK:
		pastAs_name = as_name;
		as_name = "Threeattack";
		break;
	case AS_UPATTACK:
		pastAs_name = as_name;
		as_name = "Jumpslash";
		break;
	default:
		break;
	}

	//ジャンプを繰り返さないようにする処理
	if (as_name == "Jump" && aModel_frame >= JUMPANIME_FRAMEMAX) {
		SetAnimationState(AS_BLOWN, 0);
	}

	a_Model->Update(as_name, pastAs_name, 0.3f, aModel_frame);

	if (!modelFrameStop) aModel_frame++;

	//オフセット速度
	modelOffset += modelOffsetVelocity;

}

void AP_Paradin::SetAttackData() {


	//ニュートラル
	atkData[AK_NEUTRAL].range = 2;
	atkData[AK_NEUTRAL].damage = 12;
	atkData[AK_NEUTRAL].delay = 10;
	atkData[AK_NEUTRAL].lifetime = 15;
	atkData[AK_NEUTRAL].blowVec = { 0,0.0f };
	atkData[AK_NEUTRAL].blowPower = 0.5f;
	atkData[AK_NEUTRAL].velocity = { 0.0f,0.0,0 };
	atkData[AK_NEUTRAL].rollSpeed = { 0,0,0 };
	atkData[AK_NEUTRAL].scale = { 2.0f,1.0f,2.0f };
	atkData[AK_NEUTRAL].follow = true;
	atkData[AK_NEUTRAL].offsetPos = { 0.5f,1.0f,0.0f };

	//上
	atkData[AK_UP].range = 2.5f;
	atkData[AK_UP].damage = 18;
	atkData[AK_UP].delay = 15;
	atkData[AK_UP].lifetime = 30;
	atkData[AK_UP].blowPower = 0.7f;
	atkData[AK_UP].blowVec = { 0.0f,0.0f };
	atkData[AK_UP].velocity = { 0.0f,0.0,0.0f };
	atkData[AK_UP].rollSpeed = { 0,0,0 };
	atkData[AK_UP].scale = { 2.0f,1.0f,2.0f };
	atkData[AK_UP].follow = true;
	atkData[AK_UP].offsetPos = { 0.0f,1.0f,0.0f };

	//下
	atkData[AK_DOWN].range = 1.5f;
	atkData[AK_DOWN].damage = 3;
	atkData[AK_DOWN].delay = 5;
	atkData[AK_DOWN].lifetime = 15;
	atkData[AK_DOWN].blowPower = 0.4f;
	atkData[AK_DOWN].blowVec = { 0,0.5f };
	atkData[AK_DOWN].velocity = { 0.0f,0.0f,0.0f };
	atkData[AK_DOWN].rollSpeed = { 0,0,0 };
	atkData[AK_DOWN].scale = { 1.5f,1.5f,0.3f };
	atkData[AK_DOWN].follow = true;
	atkData[AK_DOWN].offsetPos = { 0.0f,1.0f,0.0f };

	//横
	atkData[AK_DIR].range = 2.5f;
	atkData[AK_DIR].damage = 30;
	atkData[AK_DIR].delay = 75;
	atkData[AK_DIR].lifetime = 30;
	atkData[AK_DIR].blowPower = 0.8f;
	atkData[AK_DIR].blowVec = { 0,0.0f };
	atkData[AK_DIR].velocity = { 0.0f,0.0f,0.0f };
	atkData[AK_DIR].rollSpeed = { 0,0,0 };
	atkData[AK_DIR].scale = { 2.0f,1.0f,0.3f };
	atkData[AK_DIR].follow = true;
	atkData[AK_DIR].offsetPos = { 0.5f,1.0f,0.0f };


}


//戦士


void AP_Fighter::ModelDataLoad() {

	a_Model->Load("asset/model/animation/fighter/Mma Kickbase.fbx");

	a_Model->LoadAnimation("asset/model/animation/player1_Wait.fbx", "Idle");
	a_Model->LoadAnimation("asset/model/Akai_Run.fbx", "Run");
	a_Model->LoadAnimation("asset/model/Akai_Jump.fbx", "Jump");

	a_Model->LoadAnimation("asset/model/animation/avoid.fbx", "Avoid");
	a_Model->LoadAnimation("asset/model/animation/Falling.fbx", "Blown");
	a_Model->LoadAnimation("asset/model/animation/Throw.fbx", "Throw");
	a_Model->LoadAnimation("asset/model/animation/cliff.fbx", "Cliff");
	a_Model->LoadAnimation("asset/model/animation/breakReaction.fbx", "Break");

	a_Model->LoadAnimation("asset/model/animation/fighter/kachujin_Kick.fbx", "F_Kick");
	a_Model->LoadAnimation("asset/model/animation/fighter/kachujinFlip.fbx", "F_Flip");
	a_Model->LoadAnimation("asset/model/animation/fighter/kachujin_DropKick.fbx", "F_DropKick");
	a_Model->LoadAnimation("asset/model/animation/fighter/kachujin_GreatSwordKick.fbx", "F_RollKick");

}



void  AP_Fighter::PlayerUpdate() {

	//x方向速度0
	velocity.x = 0;

	//移動
	if (isJump) {
		//SetAnimationState(AS_JUMP);
		AnimationCharacterMove();
	}
	else {
		if (AnimationCharacterMove()) {
			SetAnimationState(AS_RUN);
		}
		else {
			SetAnimationState(AS_WAIT);
		}
	}

	//ジャンプ
	if (CheckIputKey(CK_JUMP, KEY_TRIGGER))Jump();
	//攻撃
	if (CheckIputKey(CK_ATTACK, KEY_TRIGGER)) {

		//シールドボタンを押していたら攻撃しない
		if (CheckIputKey(CK_SHIELD, KEY_PRESS)) {

		}
		//下攻撃
		else if (CheckIputKey(CK_DOWN, KEY_PRESS)) {
			DownAttack();

		}
		//上攻撃
		else if (CheckIputKey(CK_UP, KEY_PRESS)) {
			UpAttack();

		}
		//横攻撃
		else if (CheckIputKey(CK_RIGHT, KEY_PRESS) || CheckIputKey(CK_LEFT, KEY_PRESS)) {
			DirAttack();

		}

		//ニュートラル
		else Attack();
	}


	//シールド
	ShieldFunction();

}

void AP_Fighter::Attack() {

	
	if (!upAttackAble) return;

	DamageObject* dObj = DamageObjectManager::SetDObjT<DamageObject>(m_Position, this);

	D3DXVECTOR3 oPos = atkData[AK_NEUTRAL].offsetPos + m_Position;
	if (charaDir == CD_LEFT) oPos.x = (atkData[AK_NEUTRAL].offsetPos.x * -1) + m_Position.x;

	//パラメータ設定

	dObj->SetDamage(atkData[AK_NEUTRAL].damage);
	dObj->SetBlowPower(atkData[AK_NEUTRAL].blowPower);
	dObj->SetBlowVec(atkData[AK_NEUTRAL].blowVec);
	dObj->SetLifeTime(atkData[AK_NEUTRAL].lifetime);
	dObj->SetScale(atkData[AK_NEUTRAL].scale);
	dObj->SetVelocity(atkData[AK_NEUTRAL].velocity);
	dObj->SetDelayTime(atkData[AK_NEUTRAL].delay);
	dObj->SetPosition(oPos);
	dObj->FollowOwnerOn();

	dObj->SetSoundKind(AK_SE_BLOWSMALL);


	//dObj->SetisDraw(true);

	//硬直時間
	SetActionCooltime(5);

	//アニメーションセット
	SetAnimationState(AS_NATTACK, 15);

	//攻撃中情報をセット
	attackObj = dObj;
	

}

void AP_Fighter::UpAttack() {

	
	if (!upAttackAble) return;

	DamageObject* dObj = DamageObjectManager::SetDObjT<DamageObject>(m_Position, this);

	D3DXVECTOR3 oPos = atkData[AK_UP].offsetPos + m_Position;
	if (charaDir == CD_LEFT) oPos.x = (atkData[AK_UP].offsetPos.x * -1) + m_Position.x;
	float atkSpeed = 1.0f;
	if (charaDir == CD_LEFT) atkSpeed *= -1;
	//パラメータ設定

	dObj->SetDamage(atkData[AK_UP].damage);
	dObj->SetBlowPower(atkData[AK_UP].blowPower);
	dObj->SetBlowVec(atkData[AK_UP].blowVec);
	dObj->SetLifeTime(atkData[AK_UP].lifetime);
	dObj->SetScale(atkData[AK_UP].scale);
	dObj->SetVelocity(atkData[AK_UP].velocity);
	dObj->SetDelayTime(atkData[AK_UP].delay);
	dObj->SetPosition(oPos);
	dObj->FollowOwnerOn();

	dObj->SetSoundKind(AK_SE_BLOWMIDDLE);


	//dObj->SetisDraw(true);

	//速度生成
	velocity.x = atkData[AK_UP].range / 45.0f * atkSpeed;
	//velocity.y = 2.0f / 45;

	//位置チェンジ
	m_Position.y += 2.5f;

	upAttackAble = false;

	//硬直時間
	SetActionCooltime(20);

	//アニメーションセット
	SetAnimationState(AS_UPATTACK, 0);
	//modelOffsetVelocity = { -3.0f / 45.0f * atkSpeed ,0,0 };
	modelOffset.y = -2.5f;
	modelOffsetVelocity = { -3.0f / 45.0f * atkSpeed ,0,0 };

	//攻撃中情報をセット
	attackObj = dObj;
	
}


void AP_Fighter::DownAttack() {

	
	if (!upAttackAble) return;

	DamageObject* dObj = DamageObjectManager::SetDObjT<DamageObject>(m_Position, this);

	D3DXVECTOR3 oPos = atkData[AK_NEUTRAL].offsetPos + m_Position;
	if (charaDir == CD_LEFT) oPos.x = (atkData[AK_NEUTRAL].offsetPos.x * -1) + m_Position.x;

	//パラメータ設定

	dObj->SetDamage(atkData[AK_DOWN].damage);
	dObj->SetBlowPower(atkData[AK_DOWN].blowPower);
	dObj->SetBlowVec(atkData[AK_DOWN].blowVec);
	dObj->SetLifeTime(atkData[AK_DOWN].lifetime);
	dObj->SetScale(atkData[AK_DOWN].scale);
	dObj->SetVelocity(atkData[AK_DOWN].velocity);
	dObj->SetDelayTime(atkData[AK_DOWN].delay);
	dObj->SetPosition(oPos);
	dObj->FollowOwnerOn();

	dObj->SetSoundKind(AK_SE_BLOWMIDDLE);


	//dObj->SetisDraw(true);

	//硬直時間
	SetActionCooltime(30);

	//アニメーションセット
	SetAnimationState(AS_DOWNATTACK, 5);

	//攻撃中情報をセット
	attackObj = dObj;
	
}



void AP_Fighter::DirAttack() {

	
	if (!upAttackAble) return;

	DamageObject* dObj = DamageObjectManager::SetDObjT<DamageObject>(m_Position, this);

	D3DXVECTOR3 oPos = atkData[AK_DIR].offsetPos + m_Position;
	if (charaDir == CD_LEFT) oPos.x = (atkData[AK_DIR].offsetPos.x * -1) + m_Position.x;

	//パラメータ設定

	dObj->SetDamage(atkData[AK_DIR].damage);
	dObj->SetBlowPower(atkData[AK_DIR].blowPower);
	dObj->SetBlowVec(atkData[AK_DIR].blowVec);
	dObj->SetLifeTime(atkData[AK_DIR].lifetime);
	dObj->SetScale(atkData[AK_DIR].scale);
	dObj->SetVelocity(atkData[AK_DIR].velocity);
	dObj->SetDelayTime(atkData[AK_DIR].delay);
	dObj->SetPosition(oPos);
	dObj->FollowOwnerOn();

	dObj->SetSoundKind(AK_SE_BLOWSMALL);


	//dObj->SetisDraw(true);

	/*
	//エフェクト
	D3DXVECTOR3 maxVec, minVec;
	maxVec = { 1.1f,0.6f ,0.1f };
	minVec = { -0.1f,0.1f,-0.1f };

	ParticleSystem* pPS = EffectManager::SetParticle();
	pPS->SetPosition({ m_Position.x,m_Position.y + 1.5f,m_Position.z - 1.0f });
	pPS->SetColor({ 255 / 255.0f,102 / 255.0f,102 / 255.0f,196 / 255.0f });
	pPS->SetLastColor({ 255 / 255.0f,182 / 255.0f,102 / 255.0f,0 / 255.0f });
	pPS->SetLife(60);
	pPS->SetParticleLife(30);
	pPS->SetVelocityMax(maxVec);
	pPS->SetVelocityMin(minVec);
	pPS->SetInterval(8);
	pPS->SetSpawnNum(24);
	pPS->SetSize(0.2f);
	pPS->SetTexture(ImageManager::GetTexId(TEX_PARTICLE_FLASH));
	pPS->SetisInstancing(true);
	pPS->Init();
	*/

	velocity.x = 0;

	//硬直時間
	SetActionCooltime(20);

	//アニメーションセット
	SetAnimationState(AS_DIRATTACK, 0);
	//pastAs_name = "Threeattack";

	//攻撃中情報をセット
	attackObj = dObj;
	
}




void AP_Fighter::SetStates() {

	//重さ
	weight = 0.4f;

	//ジャンプ力
	jumpPower = 0.25f;

	//ジャンプ回数
	addJumpMax = 1;

	//移動速度
	moveSpeed = CHARA_MOVESPEED_DEFAULT * 1.25f;

	//当たり判定大きさ
	hitScale = { 0.4f,0.4f, 0.4f };

	//シールド持続時間
	shieldLife = shieldLifeMax = 180;

	//技データ入れる
	SetAttackData();
}

void AP_Fighter::AnimationStateUpdate() {

	//アニメーション関連
	//ふっとばされアニメーション
	if (D3DXVec3Length(&blownVelocity) > 0.05f) {
		SetAnimationState(AS_BLOWN, aModel_frame);
	}

	switch (AS)
	{
	case AS_WAIT:
		pastAs_name = as_name;
		as_name = "Idle";
		break;
	case AS_RUN:
		pastAs_name = as_name;
		as_name = "Run";

		break;
	case AS_JUMP:
		pastAs_name = as_name;
		as_name = "Jump";
		break;
	case AS_AVOID:
		pastAs_name = as_name;
		as_name = "Avoid";
		break;
	case AS_THROW:
		pastAs_name = as_name;
		as_name = "Throw";
		break;
	case AS_BLOWN:
		pastAs_name = as_name;
		as_name = "Blown";
		break;
	case AS_CLIFF:
		pastAs_name = as_name;
		as_name = "Cliff";
		break;
	case AS_NATTACK:
		pastAs_name = as_name;
		as_name = "F_Kick";
		break;
	case AS_DOWNATTACK:
		pastAs_name = as_name;
		as_name = "F_DropKick";
		break;
	case AS_BREAK:
		pastAs_name = as_name;
		as_name = "Break";
		break;
	case AS_DIRATTACK:
		pastAs_name = as_name;
		as_name = "F_RollKick";
		break;
	case AS_UPATTACK:
		pastAs_name = as_name;
		as_name = "F_Flip";
		break;
	default:
		break;
	}

	//ジャンプを繰り返さないようにする処理
	if (as_name == "Jump" && aModel_frame >= JUMPANIME_FRAMEMAX) {
		SetAnimationState(AS_BLOWN, 0);
	}

	a_Model->Update(as_name, pastAs_name, 0.3f, aModel_frame);

	if (!modelFrameStop) aModel_frame++;

	//オフセット速度
	modelOffset += modelOffsetVelocity;

}

void AP_Fighter::SetAttackData() {


	//ニュートラル
	atkData[AK_NEUTRAL].range = 2;
	atkData[AK_NEUTRAL].damage = 7;
	atkData[AK_NEUTRAL].delay = 4;
	atkData[AK_NEUTRAL].lifetime = 9;
	atkData[AK_NEUTRAL].blowVec = { 0,0.0f };
	atkData[AK_NEUTRAL].blowPower = 0.2f;
	atkData[AK_NEUTRAL].velocity = { 0.0f,0.0,0 };
	atkData[AK_NEUTRAL].rollSpeed = { 0,0,0 };
	atkData[AK_NEUTRAL].scale = { 2.0f,1.0f,2.0f };
	atkData[AK_NEUTRAL].follow = true;
	atkData[AK_NEUTRAL].offsetPos = { 0.5f,1.0f,0.0f };

	//上
	atkData[AK_UP].range = 4.0f;
	atkData[AK_UP].damage = 9;
	atkData[AK_UP].delay = 15;
	atkData[AK_UP].lifetime = 30;
	atkData[AK_UP].blowPower = 0.7f;
	atkData[AK_UP].blowVec = { 0.0f,0.0f };
	atkData[AK_UP].velocity = { 0.0f,0.0,0.0f };
	atkData[AK_UP].rollSpeed = { 0,0,0 };
	atkData[AK_UP].scale = { 2.0f,1.0f,2.0f };
	atkData[AK_UP].follow = true;
	atkData[AK_UP].offsetPos = { 0.0f,1.0f,0.0f };

	//下
	atkData[AK_DOWN].range = 2.5f;
	atkData[AK_DOWN].damage = 13;
	atkData[AK_DOWN].delay = 5;
	atkData[AK_DOWN].lifetime = 20;
	atkData[AK_DOWN].blowPower = 0.4f;
	atkData[AK_DOWN].blowVec = { 0,0.5f };
	atkData[AK_DOWN].velocity = { 0.0f,0.0f,0.0f };
	atkData[AK_DOWN].rollSpeed = { 0,0,0 };
	atkData[AK_DOWN].scale = { 2.5f,1.5f,2.5f };
	atkData[AK_DOWN].follow = true;
	atkData[AK_DOWN].offsetPos = { 0.0f,1.0f,0.0f };

	//横
	atkData[AK_DIR].range = 2.5f;
	atkData[AK_DIR].damage = 10;
	atkData[AK_DIR].delay = 15;
	atkData[AK_DIR].lifetime = 10;
	atkData[AK_DIR].blowPower = 0.5f;
	atkData[AK_DIR].blowVec = { 0,0.0f };
	atkData[AK_DIR].velocity = { 0.0f,0.0f,0.0f };
	atkData[AK_DIR].rollSpeed = { 0,0,0 };
	atkData[AK_DIR].scale = { 2.0f,1.0f,0.3f };
	atkData[AK_DIR].follow = true;
	atkData[AK_DIR].offsetPos = { 0.5f,1.0f,0.0f };


}



//魔法使い

void AP_Wizard::ModelDataLoad() {

	a_Model->Load("asset/model/animation/wizard/magic_Idle 02.fbx");

	a_Model->LoadAnimation("asset/model/animation/player1_Wait.fbx", "Idle");
	a_Model->LoadAnimation("asset/model/Akai_Run.fbx", "Run");
	a_Model->LoadAnimation("asset/model/Akai_Jump.fbx", "Jump");

	a_Model->LoadAnimation("asset/model/animation/avoid.fbx", "Avoid");
	a_Model->LoadAnimation("asset/model/animation/Falling.fbx", "Blown");
	a_Model->LoadAnimation("asset/model/animation/Throw.fbx", "Throw");
	a_Model->LoadAnimation("asset/model/animation/cliff.fbx", "Cliff");
	a_Model->LoadAnimation("asset/model/animation/breakReaction.fbx", "Break");

	a_Model->LoadAnimation("asset/model/animation/wizard/magicNAttack.fbx", "M_Attack");
	a_Model->LoadAnimation("asset/model/animation/wizard/MagicupAttack.fbx", "M_UpAttack");
	a_Model->LoadAnimation("asset/model/animation/wizard/Magic_underAttack.fbx", "M_UnderAttack");
	a_Model->LoadAnimation("asset/model/animation/wizard/MagicdirAttack.fbx", "M_DirAttack");

}



void  AP_Wizard::PlayerUpdate() {

	//x方向速度0
	velocity.x = 0;

	//移動
	if (isJump) {
		//SetAnimationState(AS_JUMP);
		AnimationCharacterMove();
	}
	else {
		if (AnimationCharacterMove()) {
			SetAnimationState(AS_RUN);
		}
		else {
			SetAnimationState(AS_WAIT);
		}
	}

	//ジャンプ
	if (CheckIputKey(CK_JUMP, KEY_TRIGGER))Jump();
	//攻撃
	if (CheckIputKey(CK_ATTACK, KEY_TRIGGER)) {

		//シールドボタンを押していたら攻撃しない
		if (CheckIputKey(CK_SHIELD, KEY_PRESS)) {

		}
		//下攻撃
		else if (CheckIputKey(CK_DOWN, KEY_PRESS)) {
			DownAttack();

		}
		//上攻撃
		else if (CheckIputKey(CK_UP, KEY_PRESS)) {
			UpAttack();

		}
		//横攻撃
		else if (CheckIputKey(CK_RIGHT, KEY_PRESS) || CheckIputKey(CK_LEFT, KEY_PRESS)) {
			DirAttack();

		}

		//ニュートラル
		else Attack();
	}


	//シールド
	ShieldFunction();

}

void AP_Wizard::Attack() {

	
	if (!upAttackAble) return;

	DamageObject* dObj = DamageObjectManager::SetDObjT<FireballObject>(m_Position, this);
	dObj->SetTarget(PlayerManager::GetNearPlayer(this));

	D3DXVECTOR3 oPos = atkData[AK_NEUTRAL].offsetPos + m_Position;
	if (charaDir == CD_LEFT) oPos.x = (atkData[AK_NEUTRAL].offsetPos.x * -1) + m_Position.x;

	//パラメータ設定

	dObj->SetDamage(atkData[AK_NEUTRAL].damage);
	dObj->SetBlowPower(atkData[AK_NEUTRAL].blowPower);
	dObj->SetBlowVec(atkData[AK_NEUTRAL].blowVec);
	dObj->SetLifeTime(atkData[AK_NEUTRAL].lifetime);
	dObj->SetScale(atkData[AK_NEUTRAL].scale);
	dObj->SetVelocity(atkData[AK_NEUTRAL].velocity);
	dObj->SetDelayTime(atkData[AK_NEUTRAL].delay);
	dObj->SetPosition(oPos);
	dObj->FollowOwnerOn();

	dObj->SetSoundKind(AK_SE_BLOWSMALL);


	//dObj->SetisDraw(true);

	//硬直時間
	SetActionCooltime(25);

	//アニメーションセット
	SetAnimationState(AS_NATTACK, 0);

	//攻撃中情報をセット
	attackObj = dObj;
	

}

void AP_Wizard::UpAttack() {

	
	if (!upAttackAble) return;

	//下方向にランダムに攻撃オブジェクトを８個出す

	DamageObject* dObj;

	for (int i = 0; i < 8;i++) {

		dObj = DamageObjectManager::SetDObjT<WaterObject>(m_Position, this);

		D3DXVECTOR3 oPos = atkData[AK_UP].offsetPos + m_Position;
		if (charaDir == CD_LEFT) oPos.x = (atkData[AK_UP].offsetPos.x * -1) + m_Position.x;
		float atkSpeed = 1.0f;
		if (charaDir == CD_LEFT) atkSpeed *= -1;
		
		//ランダム速度生成
		D3DXVECTOR3 vec;
		vec.x = UsefulFunction::GetRandomRange(-0.5f,0.5f);
		vec.y = UsefulFunction::GetRandomRange(-1.5f, -0.8f);
		vec.z = 0;

		vec *= 1 / 30.0f;

		//パラメータ設定

		dObj->SetDamage(atkData[AK_UP].damage);
		dObj->SetBlowPower(atkData[AK_UP].blowPower);
		dObj->SetBlowVec(atkData[AK_UP].blowVec);
		dObj->SetLifeTime(atkData[AK_UP].lifetime);
		dObj->SetScale(atkData[AK_UP].scale);
		dObj->SetVelocity(vec);
		dObj->SetDelayTime(atkData[AK_UP].delay);
		dObj->SetPosition(oPos);
		dObj->SetisDraw(true);

		dObj->SetSoundKind(AK_SE_BLOWSMALL);

	}

	//dObj->SetisDraw(true);

	//速度生成
	velocity.y = 12.0f / 45;

	upAttackAble = false;

	//硬直時間
	SetActionCooltime(45);

	//アニメーションセット
	SetAnimationState(AS_UPATTACK, 0);
	//modelOffsetVelocity = { -3.0f / 45.0f * atkSpeed ,0,0 };

	//攻撃中情報をセット
	attackObj = dObj;
	
}


void AP_Wizard::DownAttack() {

	
	if (!upAttackAble) return;

	//ランダムに氷オブジェクトを3～5つ投げる
	int objNum = rand() % 3 + 3;
	DamageObject* dObj;

	for (int i = 0; i < objNum; i++) {
		dObj = DamageObjectManager::SetDObjT<IceObject>(m_Position, this);

		D3DXVECTOR3 oPos = atkData[AK_NEUTRAL].offsetPos + m_Position;
		if (charaDir == CD_LEFT) oPos.x = (atkData[AK_NEUTRAL].offsetPos.x * -1) + m_Position.x;

		//ランダム速度生成
		D3DXVECTOR3 vec;
		vec.x = UsefulFunction::GetRandomRange(0.8f, 1.5f);
		vec.y = UsefulFunction::GetRandomRange(2.0f, 5.0f);
		vec.z = 0;

		vec *= 1 / 30.0f;

		if (charaDir == CD_LEFT) vec.x *= -1;

		//パラメータ設定

		dObj->SetDamage(atkData[AK_DOWN].damage);
		dObj->SetBlowPower(atkData[AK_DOWN].blowPower);
		dObj->SetBlowVec(atkData[AK_DOWN].blowVec);
		dObj->SetLifeTime(atkData[AK_DOWN].lifetime);
		dObj->SetScale(atkData[AK_DOWN].scale);
		dObj->SetVelocity(vec);
		dObj->SetDelayTime(atkData[AK_DOWN].delay);
		dObj->SetPosition(oPos);

		dObj->SetSoundKind(AK_SE_BLOWMIDDLE);

	}

	//dObj->SetisDraw(true);

	//硬直時間
	SetActionCooltime(25);

	//アニメーションセット
	SetAnimationState(AS_DOWNATTACK, 0);

	//攻撃中情報をセット
	attackObj = dObj;
	
}



void AP_Wizard::DirAttack() {

	
	if (!upAttackAble) return;

	DamageObject* dObj = DamageObjectManager::SetDObjT<ThunderObject>(m_Position, this);

	D3DXVECTOR3 oPos = atkData[AK_DIR].offsetPos + m_Position;
	if (charaDir == CD_LEFT) oPos.x = (atkData[AK_DIR].offsetPos.x * -1) + m_Position.x;

	//パラメータ設定

	dObj->SetDamage(atkData[AK_DIR].damage);
	dObj->SetBlowPower(atkData[AK_DIR].blowPower);
	dObj->SetBlowVec(atkData[AK_DIR].blowVec);
	dObj->SetLifeTime(atkData[AK_DIR].lifetime);
	dObj->SetScale(atkData[AK_DIR].scale);
	dObj->SetVelocity(atkData[AK_DIR].velocity);
	dObj->SetDelayTime(atkData[AK_DIR].delay);
	dObj->SetPosition(oPos);
	dObj->FollowOwnerOn();
	//dObj->SetisDraw(true);


	//エフェクト
	D3DXVECTOR3 maxVec, minVec;
	maxVec = { 1.1f,0.6f ,0.1f };
	minVec = { -0.1f,0.1f,-0.1f };
	/*
	float effOffPosX = -oPos.x;
	if (charaDir == CD_LEFT) {
		effOffPosX *= -1;
		float dummy;
		dummy = maxVec.x;
		maxVec.x = minVec.x;
		minVec.x = dummy;
	}*/

	ParticleSystem* pPS = EffectManager::SetParticle();
	pPS->SetPosition({ m_Position.x,m_Position.y + 1.5f,m_Position.z - 1.0f });
	pPS->SetColor({ 255 / 255.0f,102 / 255.0f,102 / 255.0f,196 / 255.0f });
	pPS->SetLastColor({ 255 / 255.0f,182 / 255.0f,102 / 255.0f,0 / 255.0f });
	pPS->SetLife(60);
	pPS->SetParticleLife(30);
	pPS->SetVelocityMax(maxVec);
	pPS->SetVelocityMin(minVec);
	pPS->SetInterval(8);
	pPS->SetSpawnNum(24);
	pPS->SetSize(0.2f);
	pPS->SetTexture(ImageManager::GetTexId(TEX_PARTICLE_FLASH));
	pPS->SetisInstancing(true);
	pPS->Init();

	dObj->SetSoundKind(AK_SE_BLOWMIDDLE);


	velocity.x = 0;

	//硬直時間
	SetActionCooltime(120);

	//アニメーションセット
	SetAnimationState(AS_DIRATTACK, 0);
	//pastAs_name = "Threeattack";

	//攻撃中情報をセット
	attackObj = dObj;
	
}




void AP_Wizard::SetStates() {

	//重さ
	weight = 0.4f;

	//ジャンプ力
	jumpPower = 0.18f;

	//ジャンプ回数
	addJumpMax = 1;

	//移動速度
	moveSpeed = CHARA_MOVESPEED_DEFAULT * 0.75f;

	//当たり判定大きさ
	hitScale = { 0.4f,0.4f, 0.4f };

	//シールド持続時間
	shieldLife = shieldLifeMax = 180;

	//技データ入れる
	SetAttackData();
}

void AP_Wizard::AnimationStateUpdate() {

	//アニメーション関連
	//ふっとばされアニメーション
	if (D3DXVec3Length(&blownVelocity) > 0.05f) {
		SetAnimationState(AS_BLOWN, aModel_frame);
	}

	switch (AS)
	{
	case AS_WAIT:
		pastAs_name = as_name;
		as_name = "Idle";
		break;
	case AS_RUN:
		pastAs_name = as_name;
		as_name = "Run";

		break;
	case AS_JUMP:
		pastAs_name = as_name;
		as_name = "Jump";
		break;
	case AS_AVOID:
		pastAs_name = as_name;
		as_name = "Avoid";
		break;
	case AS_THROW:
		pastAs_name = as_name;
		as_name = "Throw";
		break;
	case AS_BLOWN:
		pastAs_name = as_name;
		as_name = "Blown";
		break;
	case AS_CLIFF:
		pastAs_name = as_name;
		as_name = "Cliff";
		break;
	case AS_NATTACK:
		pastAs_name = as_name;
		as_name = "M_Attack";
		break;
	case AS_DOWNATTACK:
		pastAs_name = as_name;
		as_name = "M_UnderAttack";
		break;
	case AS_BREAK:
		pastAs_name = as_name;
		as_name = "Break";
		break;
	case AS_DIRATTACK:
		pastAs_name = as_name;
		as_name = "M_DirAttack";
		break;
	case AS_UPATTACK:
		pastAs_name = as_name;
		as_name = "M_UpAttack";
		break;
	default:
		break;
	}

	//ジャンプを繰り返さないようにする処理
	if (as_name == "Jump" && aModel_frame >= JUMPANIME_FRAMEMAX) {
		SetAnimationState(AS_BLOWN, 0);
	}

	a_Model->Update(as_name, pastAs_name, 0.3f, aModel_frame);

	if (!modelFrameStop) aModel_frame++;

	//オフセット速度
	modelOffset += modelOffsetVelocity;

}

void AP_Wizard::SetAttackData() {


	//ニュートラル
	atkData[AK_NEUTRAL].range = 8;
	atkData[AK_NEUTRAL].damage = 6;
	atkData[AK_NEUTRAL].delay = 10;
	atkData[AK_NEUTRAL].lifetime = 120;
	atkData[AK_NEUTRAL].blowVec = { 0,0.0f };
	atkData[AK_NEUTRAL].blowPower = 0.2f;
	atkData[AK_NEUTRAL].velocity = { 4/60.0f,0.0,0 };
	atkData[AK_NEUTRAL].rollSpeed = { 0,0,0 };
	atkData[AK_NEUTRAL].scale = { 2.0f,1.0f,2.0f };
	atkData[AK_NEUTRAL].follow = true;
	atkData[AK_NEUTRAL].offsetPos = { 0.5f,1.0f,0.0f };

	//上
	atkData[AK_UP].range = 2.5f;
	atkData[AK_UP].damage = 2;
	atkData[AK_UP].delay = 3;
	atkData[AK_UP].lifetime = 60;
	atkData[AK_UP].blowPower = 0.1f;
	atkData[AK_UP].blowVec = { 0.0f,0.0f };
	atkData[AK_UP].velocity = { 0.0f,0.0,0.0f };
	atkData[AK_UP].rollSpeed = { 0,0,0 };
	atkData[AK_UP].scale = { 0.5f,0.5f,0.5f };
	atkData[AK_UP].follow = true;
	atkData[AK_UP].offsetPos = { 0.0f,1.0f,0.0f };

	//下
	atkData[AK_DOWN].range = 1.5f;
	atkData[AK_DOWN].damage = 5;
	atkData[AK_DOWN].delay = 5;
	atkData[AK_DOWN].lifetime = 90;
	atkData[AK_DOWN].blowPower = 0.4f;
	atkData[AK_DOWN].blowVec = { 0,0.3f };
	atkData[AK_DOWN].velocity = { 0.0f,0.0f,0.0f };
	atkData[AK_DOWN].rollSpeed = { 0,0,0 };
	atkData[AK_DOWN].scale = { 0.5f,0.5f,0.3f };
	atkData[AK_DOWN].follow = true;
	atkData[AK_DOWN].offsetPos = { 0.0f,1.0f,0.0f };

	//横
	atkData[AK_DIR].range = 5.0f;
	atkData[AK_DIR].damage = 15;
	atkData[AK_DIR].delay = 50;
	atkData[AK_DIR].lifetime = 45;
	atkData[AK_DIR].blowPower = 0.5f;
	atkData[AK_DIR].blowVec = { 0,0.3f };
	atkData[AK_DIR].velocity = { 0.0f,0.0f,0.0f };
	atkData[AK_DIR].rollSpeed = { 0,0,0 };
	atkData[AK_DIR].scale = { 8.0f,1.0f,0.3f };
	atkData[AK_DIR].follow = true;
	atkData[AK_DIR].offsetPos = { 4.5f,1.0f,0.0f };


}


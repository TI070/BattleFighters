#pragma once

#include "main.h"

struct RendererSet {

	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11ShaderResourceView* m_Texture = NULL;

	ID3D11VertexShader* m_VertexShader = NULL;
	ID3D11PixelShader* m_PixelShader = NULL;
	ID3D11InputLayout* m_VertxeLayout = NULL;

};


class GameObject {

protected:

	D3DXVECTOR3 m_Position = { 0,0,0 };
	D3DXVECTOR3 m_Rotation = { 0,0,0 };
	D3DXVECTOR3 m_Scale = { 0,0,0 };

	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11ShaderResourceView* m_Texture = NULL;

	ID3D11VertexShader* m_VertexShader = NULL;
	ID3D11PixelShader* m_PixelShader = NULL;
	ID3D11InputLayout* m_VertxeLayout = NULL;

	RendererSet renSet;

	bool isInstancing = false;

	float frustumCullingScale = 0;//éãêçë‰ÉJÉäÉìÉOópëÂÇ´Ç≥

	bool m_Destroy = false;

	void SetrenSet() {

		renSet.m_PixelShader = m_PixelShader;
		renSet.m_VertexBuffer = m_VertexBuffer;
		renSet.m_VertexShader = m_VertexShader;
		renSet.m_Texture = m_Texture;
		renSet.m_VertxeLayout = m_VertxeLayout;

	}
	
	void SetrenSet_Shader(ID3D11PixelShader* s_PS, ID3D11VertexShader* s_VS, ID3D11InputLayout* s_IL) {

		renSet.m_PixelShader = s_PS;
		renSet.m_VertexShader = s_VS;
		renSet.m_VertxeLayout = s_IL;

	}

	bool shaderParameterOn = false;
	D3DXVECTOR4 shaderParameter;

public :

	GameObject() {};
	virtual ~GameObject() {};

	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	void SetPosition(D3DXVECTOR3 s_pos);
	void SetRotation(D3DXVECTOR3 s_rot);
	void SetScale(D3DXVECTOR3 s_scl);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();
	D3DXVECTOR3 GetScale();

	void SetDestroy() { m_Destroy = true; }
	bool GetDestroy() { return m_Destroy; }

	bool Destroy() {
		if (m_Destroy) {
			Uninit();
			delete this;

			return true;
		}
		else {
			return false;
		}
	}

	RendererSet GetRendererSet() { return renSet; }


	void DirectionRoll(D3DXVECTOR2 s_vec);
	void DirectionRoll(D3DXVECTOR2 s_vec, float correct);
	void PositionAdjust();

	void SetFrustumCullingScale(float s_scale) { frustumCullingScale = s_scale; };

	void SetisInstancing(bool s_bool) { isInstancing = s_bool; }

	D3DXVECTOR3 GetObbX() {

		D3DXMATRIX rot, scale, world;
		D3DXMatrixScaling(&scale,m_Scale.x, m_Scale.y, m_Scale.z);
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.x, m_Rotation.y,m_Rotation.z);
		world = scale * rot;

		D3DXVECTOR3 vx;
		vx.x = world._11;
		vx.y = world._12;
		vx.z = world._13;

		return vx * 0.5f;
	}

	D3DXVECTOR3 GetObbZ() {

		D3DXMATRIX rot, scale, world;
		D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
		world = scale * rot;

		D3DXVECTOR3 vz;
		vz.x = world._31;
		vz.y = world._32;
		vz.z = world._33;

		return vz * 0.5f;
	}

	D3DXVECTOR3 GetObbY() {

		D3DXMATRIX rot, scale, world;
		D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
		world = scale * rot;

		D3DXVECTOR3 vy;
		vy.x = world._21;
		vy.y = world._22;
		vy.z = world._23;

		return vy * 0.5f;
	}

	void SetVertexShader(ID3D11VertexShader* s_vs) { m_VertexShader = s_vs; }
	void SetPixelShader(ID3D11PixelShader* s_ps) { m_PixelShader = s_ps; }
	void SetVertxeLayout(ID3D11InputLayout* s_il) { m_VertxeLayout= s_il; }


	void SetShaderParameter(D3DXVECTOR4 s_p) { 
		shaderParameter = s_p;
		shaderParameterOn = true;
	}

};
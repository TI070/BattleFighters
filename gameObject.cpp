
#include "gameObject.h"

void GameObject::SetPosition(D3DXVECTOR3 s_pos) {
	m_Position = s_pos;
}

void GameObject::SetRotation(D3DXVECTOR3 s_rot) {

	m_Rotation = s_rot;
}

void GameObject::SetScale(D3DXVECTOR3 s_scl) {
	m_Scale = s_scl;
}



D3DXVECTOR3 GameObject::GetPosition() {
	return m_Position;
}

D3DXVECTOR3 GameObject::GetRotation() {
	return m_Rotation;
}

D3DXVECTOR3 GameObject::GetScale() {
	return m_Scale;
}



void GameObject::DirectionRoll(D3DXVECTOR2 s_vec) {

	if (s_vec.x == 0 && s_vec.y == 0) return;

	D3DXVECTOR2 base = { 1,0.0001f };

	D3DXVec2Normalize(&s_vec, &s_vec);

	//‰ñ“]•ûŒü‚ª+‚©-‚©”»•Ê‚·‚é
	int turnDirPlus;
	if (s_vec.y > 0) turnDirPlus = 1;
	else turnDirPlus = -1;

	float atan;
	atan = atan2(s_vec.x * base.y - s_vec.y * base.x, s_vec.x * base.x + base.y * s_vec.y);

	float st = atan;

	m_Rotation.y = st;

}

void GameObject::DirectionRoll(D3DXVECTOR2 s_vec, float correct) {

	if (s_vec.x == 0 && s_vec.y == 0) return;

	D3DXVECTOR2 base = { 1,0.0001f };

	D3DXVec2Normalize(&s_vec, &s_vec);

	//‰ñ“]•ûŒü‚ª+‚©-‚©”»•Ê‚·‚é
	int turnDirPlus;
	if (s_vec.y > 0) turnDirPlus = 1;
	else turnDirPlus = -1;

	float atan;
	atan = atan2(s_vec.x * base.y - s_vec.y * base.x, s_vec.x * base.x + base.y * s_vec.y);

	float st = atan + ((correct * 3.14f) / 180);

	m_Rotation.y = st;

}

void GameObject::PositionAdjust() {

	m_Position.x = (int)m_Position.x;
	m_Position.y = (int)m_Position.y;
	m_Position.z = (int)m_Position.z;

}
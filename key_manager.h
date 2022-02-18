#pragma once
#ifndef KEY_MANAGER_H
#define KEY_MANAGER_H

#include <d3d9.h>
#include <d3dx9.h> 

//アプリケーションで使う操作を列挙
typedef enum KeyKind_tag {
	
	KM_UP,
	KM_DOWN,
	KM_LEFT,
	KM_RIGHT,
	KM_DECIDE,
	KM_CAMERA,
	KM_CANCEL,
	KM_Q,
	KM_W,
	KM_E,
	KM_A,
	KM_S,
	KM_D,
	KM_K,
	KM_L,
	KM_U,
	KM_O,
	KM_J,
	KM_I,
	KM_H,
	KM_N,
	KM_M,
	KM_CONMA,
	KM_SPACE,
	KM_1,
	KM_2,
	KM_3,
	KM_4,
	KM_MAX

}KeyKind;


void KeyManager_Initilalize();

void KeyManager_Finalize();

void KeyManager_Update();


bool KeyManager_Press(KeyKind KM);//押してたら
bool KeyManager_Trigger(KeyKind KM);//押した瞬間
bool KeyManager_Release(KeyKind KM);//離した瞬間

void KeyManager_RecordStart(int frame_max);
void KeyManager_RecordEnd();
void KeyManager_RecordPlay();
bool KeyManager_IsRecordPlay();
void KeyManager_RecordLoad();

#endif



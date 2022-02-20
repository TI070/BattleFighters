#pragma once
#define INITGUID
#include <Windows.h>
#include "main.h"
#define DIRECTINPUT_VERSION     0x0800          // DirectInputのバージョン指定
#include <dinput.h>
#pragma comment(lib,"dinput8.lib")

#define CONTROLLER_MAX (4)


enum BUTTON_STATE
{
	BS_NONE,
	BS_TRIGGER,
	BS_PRESS,
	BS_RELEASE

};

enum BUTTON_KIND {

	BK_BUTTON_A,
	BK_BUTTON_B,
	BK_BUTTON_X,
	BK_BUTTON_Y,
	BK_BUTTON_L,
	BK_BUTTON_R,
	BK_BUTTON_NONE
};


struct ControllerButton {

	BUTTON_STATE aButton;
	BUTTON_STATE bButton;
	BUTTON_STATE xButton;
	BUTTON_STATE yButton;

	BUTTON_STATE rButton;
	BUTTON_STATE lButton;

	int LstickX;
	int LstickY;

};

struct ConfigState {

	BUTTON_KIND aButtonSet;
	BUTTON_KIND bButtonSet;
	BUTTON_KIND xButtonSet;
	BUTTON_KIND yButtonSet;
	BUTTON_KIND lButtonSet;
	BUTTON_KIND rButtonSet;

};


class ControllerManager {
private:

	static ControllerButton buttonStates[CONTROLLER_MAX];

	static ConfigState configStates[CONTROLLER_MAX];


public:

	static void Init();
	static void DebugDraw();
	static void Update();
	static void Uninit();

	static void ButtonStateUpdate(BUTTON_KIND buttonKind, int index);


	static ControllerButton GetButtonState(int controllerNum) { return buttonStates[controllerNum]; }

	static LPDIRECTINPUTDEVICE8A GetCDevice(int index);

	static void KeyConfig(BUTTON_KIND s_baseButton, BUTTON_KIND s_bk,int keyNum,int index);

	static void KeyConInit(int index);

	static BUTTON_KIND GetTriggerButton(int index);

	static BUTTON_KIND GetConfigStates(int index,BUTTON_KIND s_bk) { 
		
		/*
		if(s_bk == BK_BUTTON_A) return configStates[index].aButtonSet;
		else if (s_bk == BK_BUTTON_B) return configStates[index].bButtonSet;
		else if (s_bk == BK_BUTTON_X) return configStates[index].xButtonSet;
		else if (s_bk == BK_BUTTON_Y) return configStates[index].yButtonSet;
		else if (s_bk == BK_BUTTON_R) return configStates[index].rButtonSet;
		else if (s_bk == BK_BUTTON_L) return configStates[index].lButtonSet;
		*/


		
		if (s_bk == configStates[index].aButtonSet) return BK_BUTTON_A;
		else if (s_bk == configStates[index].bButtonSet) return BK_BUTTON_B;
		else if (s_bk == configStates[index].xButtonSet) return BK_BUTTON_X;
		else if (s_bk == configStates[index].yButtonSet) return BK_BUTTON_Y;
		else if (s_bk == configStates[index].rButtonSet) return BK_BUTTON_R;
		else if (s_bk == configStates[index].lButtonSet) return BK_BUTTON_L;
		

		return BK_BUTTON_NONE;
	}

	static ConfigState GetCS(int index) { return configStates[index]; }


};


#include "controller.h"
#include "imgui.h"

ControllerButton ControllerManager::buttonStates[CONTROLLER_MAX];
LPDIRECTINPUT8 pDinput;
LPCDIDEVICEINSTANCE lpddiData;

int testman = 99;
LPDIRECTINPUTDEVICE8A cDevice[4];
DIJOYSTATE key;

LPDIRECTINPUTDEVICE8A ControllerManager::GetCDevice(int index) { return cDevice[index]; }

ConfigState ControllerManager::configStates[CONTROLLER_MAX];

int connectNum = 0;

BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	HRESULT hr;

	hr = pDinput->CreateDevice(
		lpddi->guidInstance,
		&cDevice[connectNum],
		NULL
	);

	if (FAILED(hr))
	{
		// 失敗
		MessageBox(GetWindow(), "デバイス生成失敗", "エラー", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK;
		return DIENUM_STOP;
	}

	connectNum++;

	return DIENUM_CONTINUE;
}

void ControllerManager::Init() {

	HINSTANCE iHND;

	HRESULT hr = DirectInput8Create(
		GetInstance(),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&pDinput,
		NULL);

	if (FAILED(hr))
	{
		// 失敗
		MessageBox(GetWindow(), "生成失敗", "エラー", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK;
		return;
	}


	if (FAILED(pDinput->EnumDevices(
		//DI8DEVTYPE_GAMEPAD,
		DI8DEVCLASS_GAMECTRL,
		DeviceFindCallBack,
		0,//&parameter,
		DIEDFL_ATTACHEDONLY
	)))
	{
		MessageBox(GetWindow(), "列挙失敗", "エラー", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK;
		return;
	}


	if (cDevice == nullptr) return;


	for (int i = 0; i < connectNum; i++) {

		// デバイスのフォーマットの設定
		hr = cDevice[i]->SetDataFormat(&c_dfDIJoystick);
		//HRESULT hr = parameter->GamePadDevice->SetDataFormat(&c_dfDIJoystick);

		if (FAILED(hr))
		{
			MessageBox(GetWindow(), "フォーマット失敗", "エラー", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK;
			return;
		}

		// 軸モードを絶対値モードとして設定
		DIPROPDWORD diprop;
		ZeroMemory(&diprop, sizeof(diprop));
		diprop.diph.dwSize = sizeof(diprop);
		diprop.diph.dwHeaderSize = sizeof(diprop.diph);
		diprop.diph.dwHow = DIPH_DEVICE;
		diprop.diph.dwObj = 0;
		diprop.dwData = DIPROPAXISMODE_ABS;

		cDevice[i]->SetProperty(DIPROP_AXISMODE, &diprop.diph);

		// X軸の値の範囲設定
		DIPROPRANGE diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.diph.dwObj = DIJOFS_X;
		diprg.lMin = -1000;
		diprg.lMax = 1000;
		if (FAILED(cDevice[i]->SetProperty(DIPROP_RANGE, &diprg.diph)))
		{
			MessageBox(GetWindow(), "X軸失敗", "エラー", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK;

			return;
		}

		// Y軸の値の範囲設定
		diprg.diph.dwObj = DIJOFS_Y;
		if (FAILED(cDevice[i]->SetProperty(DIPROP_RANGE, &diprg.diph)))
		{
			MessageBox(GetWindow(), "Y軸失敗", "エラー", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK;

			return;
		}

		// 協調モードの設定
		if (FAILED(cDevice[i]->SetCooperativeLevel(
			GetWindow(),
			DISCL_EXCLUSIVE | DISCL_FOREGROUND)
		))
		{
			// 失敗
			MessageBox(GetWindow(), "協調モード失敗", "エラー", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK;
			return;
		}

		// 制御開始
		hr = cDevice[i]->Acquire();

		while (hr == DIERR_INPUTLOST) {
			hr = cDevice[i]->Acquire();
		}
		/*
		if (FAILED(hr))
		{
			// 制御開始失敗
			MessageBox(GetWindow(), "制御開始失敗", "エラー", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK;
			return;
		}

		// ポーリング開始
		hr = cDevice->Poll();
		if (FAILED(hr))
		{
			// 失敗
			MessageBox(GetWindow(), "ポーリング失敗", "エラー", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK;
			return;
		}
		*/


		KeyConInit(i);
	}
}


void ControllerManager::Update() {

	for (int i = 0; i < connectNum; i++) {

		if (cDevice[i] == nullptr) return;

		HRESULT hr;
		bool noInput = true;
		//ボタン状態の更新
		//ButtonStateUpdate();

		//cDevice->GetDeviceState(sizeof(DIJOYSTATE), &key);
		hr = cDevice[i]->Poll();
		if (FAILED(hr)) {
			// 失敗なら再開させてもう一度取得
			cDevice[i]->Acquire();
			cDevice[i]->GetDeviceState(sizeof(DIJOYSTATE), &key);
		}


		if (key.rgbButtons[0]) {//Y?

			//Yに割り当てられているボタンを調べる
			BUTTON_KIND yButton = configStates[i].yButtonSet;

			//割り当てられたボタンに対応する処理をする
			ButtonStateUpdate(yButton, i);

			noInput = false;
		}


		if (key.rgbButtons[1]) {//X?

			//Xに割り当てられているボタンを調べる

			//割り当てられたボタンに対応する処理をする
			ButtonStateUpdate(configStates[i].xButtonSet, i);

			noInput = false;
		}


		if (key.rgbButtons[2]) {//B?

			//割り当てられたボタンに対応する処理をする
			ButtonStateUpdate(configStates[i].bButtonSet, i);

			noInput = false;
		}

		if (key.rgbButtons[3]) {//A

			//割り当てられたボタンに対応する処理をする
			ButtonStateUpdate(configStates[i].aButtonSet, i);

			noInput = false;
		}

		if (key.rgbButtons[4]) {//L

			//割り当てられたボタンに対応する処理をする
			ButtonStateUpdate(configStates[i].lButtonSet, i);

			noInput = false;
		}

		if (key.rgbButtons[5]) {//R

			//割り当てられたボタンに対応する処理をする
			ButtonStateUpdate(configStates[i].rButtonSet, i);

			noInput = false;
		}


		if (key.lX > 700) key.lX = 1000;
		else if (key.lX < -700) key.lX = -1000;

		if (key.lY > 700) key.lY = 1000;
		else if (key.lY < -700) key.lY = -1000;

		buttonStates[i].LstickX = key.lX;
		buttonStates[i].LstickY = key.lY;


		if (noInput) ButtonStateUpdate(BK_BUTTON_NONE, i);


		cDevice[i]->GetDeviceState(sizeof(DIJOYSTATE), &key);

	}

}

void ControllerManager::Uninit() {


}


void ControllerManager::ButtonStateUpdate(BUTTON_KIND buttonKind,int index) {


	if (buttonKind == BK_BUTTON_Y) {//Y?

		//Yに割り当てられているボタンを調べる
		BUTTON_KIND yButton = configStates->yButtonSet;

		//割り当てられたボタンに対応する処理をする

		testman = 1;
		if (buttonStates[index].yButton == BS_TRIGGER || buttonStates[index].yButton == BS_PRESS) buttonStates[index].yButton = BS_PRESS;
		else buttonStates[index].yButton = BS_TRIGGER;

	}
	else {
		if (buttonStates[index].yButton == BS_TRIGGER || buttonStates[index].yButton == BS_PRESS) buttonStates[index].yButton = BS_RELEASE;
		else buttonStates[index].yButton = BS_NONE;
	}

	if (buttonKind == BK_BUTTON_X) {//X?
		testman = 2;
		if (buttonStates[index].xButton == BS_TRIGGER || buttonStates[index].xButton == BS_PRESS) buttonStates[index].xButton = BS_PRESS;
		else buttonStates[index].xButton = BS_TRIGGER;
	}
	else {
		if (buttonStates[index].xButton == BS_TRIGGER || buttonStates[index].xButton == BS_PRESS) buttonStates[index].xButton = BS_RELEASE;
		else buttonStates[index].xButton = BS_NONE;
	}

	if (buttonKind == BK_BUTTON_B) {//B
		testman = 4;
		if (buttonStates[index].bButton == BS_TRIGGER || buttonStates[index].bButton == BS_PRESS) buttonStates[index].bButton = BS_PRESS;
		else buttonStates[index].bButton = BS_TRIGGER;
	}
	else {
		if (buttonStates[index].bButton == BS_TRIGGER || buttonStates[index].bButton == BS_PRESS) buttonStates[index].bButton = BS_RELEASE;
		else buttonStates[index].bButton = BS_NONE;
	}

	if (buttonKind == BK_BUTTON_A) {//A
		testman = 3;
		if (buttonStates[index].aButton == BS_TRIGGER || buttonStates[index].aButton == BS_PRESS) buttonStates[index].aButton = BS_PRESS;
		else buttonStates[index].aButton = BS_TRIGGER;
	}
	else {
		if (buttonStates[index].aButton == BS_TRIGGER || buttonStates[index].aButton == BS_PRESS) buttonStates[index].aButton = BS_RELEASE;
		else buttonStates[index].aButton = BS_NONE;
	}

	if (buttonKind == BK_BUTTON_L) {//L
		testman = 5;
		if (buttonStates[index].lButton == BS_TRIGGER || buttonStates[index].lButton == BS_PRESS) buttonStates[index].lButton = BS_PRESS;
		buttonStates[index].lButton = BS_TRIGGER;
	}
	else {
		if (buttonStates[index].lButton == BS_TRIGGER || buttonStates[index].lButton == BS_PRESS) buttonStates[index].lButton = BS_RELEASE;
		else buttonStates[index].lButton = BS_NONE;
	}

	if (buttonKind == BK_BUTTON_R) {//R
		testman = 6;
		if (buttonStates[index].rButton == BS_TRIGGER || buttonStates[index].rButton == BS_PRESS) buttonStates[index].rButton = BS_PRESS;
		else buttonStates[index].rButton = BS_TRIGGER;
	}
	else {
		if (buttonStates[index].rButton == BS_TRIGGER || buttonStates[index].rButton == BS_PRESS) buttonStates[index].rButton = BS_RELEASE;
		else buttonStates[index].rButton = BS_NONE;
	}


}

void ControllerManager::DebugDraw() {

	return;

	int aButton = buttonStates[0].aButton;
	float lx = key.lX;
	float ly = key.lY;
	int aB, bB, xB, yB, rB, lB;
	aB = configStates[0].aButtonSet;
	bB = configStates[0].bButtonSet;
	xB = configStates[0].xButtonSet;
	yB = configStates[0].yButtonSet;
	rB = configStates[0].rButtonSet;
	lB = configStates[0].lButtonSet;

	ImGui::Begin("cState");

	//ImGui::InputInt("connnectNum", &connectNum);
	ImGui::InputInt("a", &aB);
	ImGui::InputInt("b", &bB);
	ImGui::InputInt("x", &xB);
	ImGui::InputInt("y", &yB);
	ImGui::InputInt("r", &rB);
	ImGui::InputInt("l", &lB);

	ImGui::End();

}

void ControllerManager::KeyConfig(BUTTON_KIND s_baseButton,BUTTON_KIND s_bk, int index) {

	BUTTON_KIND changeBK;

	if (s_baseButton == configStates[index].aButtonSet) {
		changeBK = BK_BUTTON_A;
	}
	else if (s_baseButton == configStates[index].bButtonSet) {
		changeBK = BK_BUTTON_B;
	}
	else if (s_baseButton == configStates[index].xButtonSet) {
		changeBK = BK_BUTTON_X;
	}
	else if (s_baseButton == configStates[index].yButtonSet) {
		changeBK = BK_BUTTON_Y;
	}
	else if (s_baseButton == configStates[index].rButtonSet) {
		changeBK = BK_BUTTON_R;
	}
	else if (s_baseButton == configStates[index].lButtonSet) {
		changeBK = BK_BUTTON_L;
	}


	if (s_baseButton == BK_BUTTON_A) {
		//changeBK = configStates[index].aButtonSet;
		configStates[index].aButtonSet = s_bk;
	}
	else if (s_baseButton == BK_BUTTON_B) {
		//changeBK = configStates[index].bButtonSet;
		configStates[index].bButtonSet = s_bk;
	}
	else if (s_baseButton == BK_BUTTON_X) {
		//changeBK = configStates[index].xButtonSet;
		configStates[index].xButtonSet = s_bk;
	}
	else if (s_baseButton == BK_BUTTON_Y) {
		//changeBK = configStates[index].yButtonSet;
		configStates[index].yButtonSet = s_bk;
	}
	else if (s_baseButton == BK_BUTTON_R) {
		//changeBK = configStates[index].rButtonSet;
		configStates[index].rButtonSet = s_bk;
	}
	else if (s_baseButton == BK_BUTTON_L) {
		//changeBK = configStates[index].lButtonSet;
		configStates[index].lButtonSet = s_bk;
	}

	//該当ボタンがすべて別のキーにアサインされていたら入れ替える

	if (s_bk == BK_BUTTON_A) {
		configStates[index].aButtonSet = changeBK;
	}
	else if (s_bk == BK_BUTTON_B) {
		configStates[index].bButtonSet = changeBK;
	}
	else if (s_bk == BK_BUTTON_X) {
		configStates[index].xButtonSet = changeBK;
	}
	else if (s_bk == BK_BUTTON_Y) {
		configStates[index].yButtonSet = changeBK;
	}
	else if (s_bk == BK_BUTTON_R) {
		configStates[index].rButtonSet = changeBK;
	}
	else if (s_bk == BK_BUTTON_L) {
		configStates[index].lButtonSet = changeBK;
	}

}

void ControllerManager::KeyConInit(int index) {

	
	configStates[index].aButtonSet = BK_BUTTON_A;
	configStates[index].bButtonSet = BK_BUTTON_B;
	configStates[index].xButtonSet = BK_BUTTON_X;
	configStates[index].yButtonSet = BK_BUTTON_Y;
	configStates[index].lButtonSet = BK_BUTTON_L;
	configStates[index].rButtonSet = BK_BUTTON_R;
	
	/*
	configStates[index].aButtonSet = BK_BUTTON_R;
	configStates[index].bButtonSet = BK_BUTTON_L;
	configStates[index].xButtonSet = BK_BUTTON_Y;
	configStates[index].yButtonSet = BK_BUTTON_X;
	configStates[index].lButtonSet = BK_BUTTON_A;
	configStates[index].rButtonSet = BK_BUTTON_B;
	*/

}

BUTTON_KIND ControllerManager::GetTriggerButton(int index) {

	HRESULT hr;

	hr = cDevice[index]->Poll();

	if (cDevice[index] != nullptr) {
		cDevice[index]->GetDeviceState(sizeof(DIJOYSTATE), &key);
	}

	if (FAILED(hr)) {
		// 失敗なら再開させてもう一度取得
		cDevice[index]->Acquire();
		cDevice[index]->GetDeviceState(sizeof(DIJOYSTATE), &key);
	}

	if (key.rgbButtons[0]) return BK_BUTTON_Y;
	else if (key.rgbButtons[1]) return BK_BUTTON_X;
	else if (key.rgbButtons[2]) return BK_BUTTON_B;
	else if (key.rgbButtons[3]) return BK_BUTTON_A;
	else if (key.rgbButtons[4]) return BK_BUTTON_L;
	else if (key.rgbButtons[5]) return BK_BUTTON_R;

	return BK_BUTTON_NONE;

}
#pragma once

#include <list>
#include "managerBase.h"


#define MESSAGEWINDOW_SIZE_X (600)
#define MESSAGEWINDOW_SIZE_Y (150)

#define MESSAGEWINDOW_POS_X (500)

#define MESSAGEWINDOW_SIZE {MESSAGEWINDOW_SIZE_X,MESSAGEWINDOW_SIZE_Y}



class UIWindow;


class SystemMessage : public ManagerBase{

private:
	static UIWindow* pMessageWindow;

	static bool windowAble;
	static std::list<UIWindow*> pList;

public:
	static void SetMessageWindow();

	static void Init();
	static void Uninit();

	//�E�C���h�E���\���ɂ���
	static void WindowReset();
	static void SetJMessage(int row, int col, const char* mes);
	static void SetAMessage(int row, int col, const char* mes);

	//���ݐݒ蒆�̃��b�Z�[�W�E�C���h�E�����X�g�ɒǉ�
	static void SetList();

	//���X�g�̐擪���폜���l�߂�
	static void TurnPage();

	static void Update();
	static void Draw();


	//�g�p���郁�b�Z�[�W�E�C���h�E�̃��X�g
	static void SetTutorialMessage_1();
	static void TestMessage();


};
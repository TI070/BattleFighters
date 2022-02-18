
#include "systemMessage.h"
#include "imageManager.h"
#include "polygon2d.h"
#include "UIwindow.h"
#include "key_manager.h"



bool SystemMessage::windowAble = false;
UIWindow* SystemMessage::pMessageWindow = nullptr;
std::list<UIWindow*> SystemMessage::pList;

void SystemMessage::Init() {

	windowAble = false;
}

void SystemMessage::SetMessageWindow() {


	//�c���Ă�����̂�����Ώ���
	//if (pMessageWindow != nullptr) {
	//	WindowReset();
	//}

	//pMessageWindow = ImageManager::SetUIWindow({SCREEN_WIDTH,SCREEN_HEIGHT - MESSAGEWINDOW_SIZE_Y/2 }, MESSAGEWINDOW_SIZE, ImageManager::GetTexId(TEX_UI_MESSAGEWINDOW), DEPTH_ZERO);
	//pMessageWindow = new UIWindow({ MESSAGEWINDOW_POS_X,SCREEN_HEIGHT - MESSAGEWINDOW_SIZE_Y / 2 }, MESSAGEWINDOW_SIZE, ImageManager::GetTexId(TEX_UI_MESSAGEWINDOW), DEPTH_ZERO);
	//pMessageWindow->SetWindowTex(new Polygon2D({ MESSAGEWINDOW_POS_X,SCREEN_HEIGHT - MESSAGEWINDOW_SIZE_Y / 2 }, MESSAGEWINDOW_SIZE, ImageManager::GetTexId(TEX_UI_MESSAGEWINDOW), DEPTH_ZERO));

}

void SystemMessage::WindowReset() {

	pMessageWindow->Uninit();
	pMessageWindow = nullptr;
	windowAble = false;

}

void SystemMessage::SetAMessage(int row, int col, const char* mes) {

	if (pMessageWindow == nullptr) return;

	//pMessageWindow->SetATextTex(new TexStringAlphabet({220.0f + 32 * (col - 1),410.0f + 32 * (row - 1) }, 32, ImageManager::GetTexId(TEX_JAPANESE_FONT), DEPTH_ZERO, mes, { 0.0f,0.0f,0.0f,1.0f }));


}

void SystemMessage::SetJMessage(int row, int col, const char* mes) {

	if (pMessageWindow == nullptr) return;

	//pMessageWindow->SetJTextTex(new TexStringJapanese({ 220.0f + 32 * (col - 1),410.0f + 32 * (row - 1) }, 32, ImageManager::GetTexId(TEX_JAPANESE_FONT), DEPTH_ZERO, mes, { 0.0f,0.0f,0.0f,1.0f }));

}

void SystemMessage::SetList() {


	//�ǉ��ʒu���擪�łȂ����isUse��false�ɂ���
	if(pList.size() != 0) pMessageWindow->SetIsUse(false);

	//���X�g�̌��ɒǉ�
	pList.push_back(pMessageWindow);


}

void SystemMessage::TurnPage() {

	std::list<UIWindow*>::iterator  itr = pList.begin();
	
	UIWindow* pUW = *itr;

	pUW->SetDestroy();
	

	itr++;

	if (pList.size() <= 1) return;

	pUW = *itr;

	pUW->SetIsUse(true);
}

void SystemMessage::Update() {

	if (pList.size() == 0) return;

	//��
	if (KeyManager_Trigger(KM_K)) {
		TurnPage();
	}

	ManagerBase::RemoveMember<UIWindow>(&pList);

}

void SystemMessage::Draw() {

	if (pList.size() == 0) return;

	std::list<UIWindow*>::iterator  itr = pList.begin();

	UIWindow* pUW = *itr;

	pUW->Draw();

}

void SystemMessage::SetTutorialMessage_1() {

	//���b�Z�[�W�P
	SetMessageWindow();
	SetJMessage(1, 1, "���̃Q�����́@�݂����Ђ��Ăނ������");
	SetJMessage(2, 1, "�ЂƂ������Ăā@�����X�^�����������Q�����ł�");
	SetList();

	//���b�Z�[�W�Q
	SetMessageWindow();
	SetJMessage(1, 1, "�v���C�����́@�J���\�������������Ă��܂��܂ȃA�N�V���������܂�");
	SetJMessage(2, 1, "�J���\�����ǂ�");
	SetAMessage(2, 5, "WASD key");
	SetList();

	//���b�Z�[�W3
	SetMessageWindow();
	SetJMessage(1, 1, "�A�N�V����������Ƃ��́@���߂񂤂��̃��j�������炻����������т܂�");
	SetJMessage(2, 1, "���j���[����");
	SetAMessage(2, 4, "MAKEROAD");
	SetJMessage(2, 8, "�������ł݂܂��傤");
	SetJMessage(3, 1, "���j�������ǂ�");
	SetAMessage(3, 5, "QE key");
	SetJMessage(4, 1, "���j���������Ă�");
	SetAMessage(4, 5, "L key");
	SetList();

	//���b�Z�[�W4
	SetMessageWindow();
	SetJMessage(1, 1, "�݂ǂ�̃^�C���̂����ŃX�y���X�L����������");
	SetJMessage(2, 1, "�݂������邱�Ƃ��ł��܂�");
	SetJMessage(3, 1, "���̂Ƃ��@�݂������̂����������񂵂傤���܂�");
	SetJMessage(4, 1, "���̂��������@�v���C�����p�����Ƃ�т܂�");

	SetList();

	//���b�Z�[�W5
	SetMessageWindow();
	SetJMessage(1, 1, "�����̃A�N�V������������");
	SetJMessage(2, 1, "�ЂƂ��݂��т��@�������ɂ���{�X���������܂��傤");

	SetList();




}


void SystemMessage::TestMessage() {
	
	SetMessageWindow();
	SetJMessage(1, 1, "�A�C�E�G�I�J�L�N�P�R�T�V�X�Z�\�^�`�c�e�g");
	SetJMessage(2, 1, "�i�j�k�l�m�n�q�t�w�z�}�~����������������������");
	SetJMessage(3, 1, "���������@�B�D�F�H������");
	SetList();

}

void SystemMessage::Uninit() {

	for (UIWindow* pUI: pList) {
		pUI->Uninit();
		delete pUI;
	}
	pList.clear();

}
#pragma once

#include <list>

class ManagerBase {

protected:

	//Destroy�\�񂳂ꂽ�I�u�W�F�N�g�����X�g�������
	//template <typename T>
	//static void RemoveMember(std::list<T*>* pList) {
	//	
	//	//�߂�l��true�Ȃ烊�X�g�������
	//	pList->remove_if([](T* object) {
	//		return object->Destroy();
	//		});
	//	//�����_��
	//
	//}


	template <typename T>
	static void RemoveMember(std::list<T*>* pList) {
	
		//�߂�l��true�Ȃ烊�X�g�������
		
		//�߂�l��true�Ȃ烊�X�g�������
		pList->remove_if([](T* object) {
			bool r = object->Destroy();
			if (r) {
				object = nullptr;
			}
			return r;
			});
		//�����_��
	
		
	}


};


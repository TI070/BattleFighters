#pragma once

#include <list>

class ManagerBase {

protected:

	//Destroy予約されたオブジェクトをリストから消す
	//template <typename T>
	//static void RemoveMember(std::list<T*>* pList) {
	//	
	//	//戻り値がtrueならリストから消す
	//	pList->remove_if([](T* object) {
	//		return object->Destroy();
	//		});
	//	//ラムダ式
	//
	//}


	template <typename T>
	static void RemoveMember(std::list<T*>* pList) {
	
		//戻り値がtrueならリストから消す
		
		//戻り値がtrueならリストから消す
		pList->remove_if([](T* object) {
			bool r = object->Destroy();
			if (r) {
				object = nullptr;
			}
			return r;
			});
		//ラムダ式
	
		
	}


};


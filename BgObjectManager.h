#pragma once

#include <list>
#include "managerBase.h"
#include "bgObject.h"

class BgObjectManager : public ManagerBase
{

private:

	static std::list<BgObject*> pList;

public:

	static void Init();
	static void Uninit();
	static void Draw();

	static BgObject* SetBgObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale);

};
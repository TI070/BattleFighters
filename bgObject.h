#pragma once

#include "gameObject.h"
#include "model.h"


class BgObject : public GameObject {

protected:

	Model pModel;

public:

	void Init();
	void Draw();
	void Update() {};
	void Uninit();

};
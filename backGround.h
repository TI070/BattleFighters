#pragma once

#include "gameObject.h"

class Model;

class BackGround : public GameObject{

private:

	Model* skyDome = nullptr;

public: 

	BackGround() {};

	void Init();
	void Init2();
	void Init3();

	void Uninit();
	void Draw();
	void Update() {};

};
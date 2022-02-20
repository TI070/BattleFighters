#pragma once

#include <list>
#include "model.h"

struct MODEL_DATA {

	Model* model;
	const char* modelName;

};

class ModelSystem {

private:
	static std::list<MODEL_DATA> pList;


public:
	static bool ModelLoad(const char* mName);
	static Model* GetModel(const char* mName);
	static void UnloadModel(const char* mName);
	static void UnloadModelAll();

	static Model* LoadandGetModel(const char* mName) {

		ModelLoad(mName);
		return GetModel(mName);
	}

	static Model LoadandCreateModel(const char* mName) {

		ModelLoad(mName);
		return *GetModel(mName);
	}


};


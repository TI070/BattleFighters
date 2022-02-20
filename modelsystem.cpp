
#include "modelsystem.h"
#include "model.h"

std::list<MODEL_DATA> ModelSystem::pList;

//新規登録成功したらtrueを返す
bool ModelSystem::ModelLoad(const char* mName) {

	////リストを確認し、同じ名前のモデルが登録されていなければ登録
	
	//すでにあるかチェック
	for (MODEL_DATA modelD:pList) {

		if (modelD.modelName == mName) return false;

	}

	//モデルデータ読み込み
	MODEL_DATA md;
	md.model = new Model;
	md.modelName = mName;
	md.model->Load(mName);

	//リストに追加
	pList.push_back(md);

	return true;
}

Model* ModelSystem::GetModel(const char* mName) {

	for (MODEL_DATA modelD : pList) {

		if (modelD.modelName == mName) return modelD.model;

	}


	return nullptr;
}

void ModelSystem::UnloadModel(const char* mName) {


}

void ModelSystem::UnloadModelAll() {


}
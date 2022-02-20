
#include "modelsystem.h"
#include "model.h"

std::list<MODEL_DATA> ModelSystem::pList;

//�V�K�o�^����������true��Ԃ�
bool ModelSystem::ModelLoad(const char* mName) {

	////���X�g���m�F���A�������O�̃��f�����o�^����Ă��Ȃ���Γo�^
	
	//���łɂ��邩�`�F�b�N
	for (MODEL_DATA modelD:pList) {

		if (modelD.modelName == mName) return false;

	}

	//���f���f�[�^�ǂݍ���
	MODEL_DATA md;
	md.model = new Model;
	md.modelName = mName;
	md.model->Load(mName);

	//���X�g�ɒǉ�
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
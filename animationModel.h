#pragma once

#include <unordered_map>

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"
#pragma comment (lib,"assimp.lib")

#include "main.h"



//変形後頂点構造体
struct DEFORM_VERTEX {

	aiVector3D Position;
	aiVector3D Normal;

	int BoneNum;
	std::string BoneName[4];//本来はボーンデックスで管理すべき
	float BoneWeight[4];

};

struct BONE {

	aiMatrix4x4 Matrix;
	aiMatrix4x4 AnimationMatrix;
	aiMatrix4x4 OffsetMatrix;

};



class AnimationModel {

private:

	std::unordered_map<std::string, const aiScene*>m_Animation;

	const aiScene* m_AiScene = NULL;
	ID3D11Buffer** m_VertexBuffer;
	ID3D11Buffer** m_IndexBuffer;

	std::unordered_map<std::string, ID3D11ShaderResourceView*> m_Texture;

	std::vector<DEFORM_VERTEX>* m_DeformVertex;
	std::unordered_map<std::string,BONE> m_Bone;

	void CreateBone(aiNode* node);
	void UpdateBoneMatrix(aiNode* node,aiMatrix4x4 matrix);

public:
	void Load(const char *FileName);
	void Unload();
	void Update(const char* AnimationName, const char* AnimationName2,float BlendRate, int Frame);
	void Draw();

	void LoadAnimation(const char* FileName,const char* AnimationName);

};
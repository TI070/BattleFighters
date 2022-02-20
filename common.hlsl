


cbuffer WorldBuffer : register(b0)
{
	matrix World;
}
cbuffer ViewBuffer : register(b1)
{
	matrix View;
}
cbuffer ProjectionBuffer : register(b2)
{
	matrix Projection;
}

//=========================
//汎用パラメータ
//=========================
cbuffer ParameterBuffer : register(b6)
{
	float4 Parameter; //C言語側からfloat4データを受け取る入れ物
}


struct MATERIAL
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Emission;
	float Shininess;
	float3 Dummy;
};

cbuffer MaterialBuffer : register(b3)
{
	MATERIAL Material;
}




struct LIGHT
{
	bool Enable;
	bool3 Dummy;
	float4 Direction;
	float4 Diffuse;
	float4 Ambient;
};

cbuffer LightBuffer : register(b4)
{
	LIGHT Light;
}

cbuffer CameraBuffer : register(b5) //バッファの５番とする
{
	float4 CameraPosition; //カメラ座標を受けとる変数
}

struct InstancingData {
	float4 Position[1024];
	float4 Rotation[1024];
	float4 Scale[1024];
	float4 Diffuse[1024];

};

cbuffer InstancingBuffer : register(b9) {
	InstancingData instancingData;
}



struct VS_IN
{
	float4 Position		: POSITION0;
	float4 Normal		: NORMAL0;
	float4 Diffuse		: COLOR0;
	float2 TexCoord		: TEXCOORD0;

};


struct PS_IN
{
	float4 Position		: SV_POSITION;
	float4 WorldPosition : POSITION0;
	float4 Normal : NORMAL0;
	float4 Diffuse		: COLOR0;
	float2 TexCoord		: TEXCOORD0;
};



#include "common.hlsl"


Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	/*
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse *= In.Diffuse;
	*/


	float4 normal = normalize(In.Normal); //ピクセルの法線を正規化
	float light = 0.5 - dot(normal.xyz, Light.Direction.xyz) * 0.5; //光源計算をする

	//テクスチャのピクセル色を取得
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse.rgb *= In.Diffuse.rgb * light; //明るさを乗算
	outDiffuse.a *= In.Diffuse.a; //αに明るさは関係ないので別計算

	//カメラからピクセルへ向かうベクトル
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	eyev = normalize(eyev); //正規化する

	//ハーフベクトルの作成
	float3 halfv = eyev + Light.Direction.xyz; //視線とライベクトルを加算
	halfv = normalize(halfv); //正規化する

	float specular = -dot(halfv, normal.xyz); //ハーフベクトルと法線の内積を計算
	specular = saturate(specular); //サチュレートする
	specular = pow(specular, 30);
	outDiffuse.rgb += specular; //スペキュラを足しこむ


}


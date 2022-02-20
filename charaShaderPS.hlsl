

#include "common.hlsl"


Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{

	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse *= In.Diffuse;


	//汎用パラメータを加算する
	outDiffuse.rgb += Parameter.rgb;

	
	//下は調整中
	float4 normal = normalize(In.Normal);
	/*
	float light = 0.5 - dot(normal.xyz, Light.Direction.xyz) * 0.5; //光源計算をする

	//テクスチャのピクセル色を取得
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse.rgb *= In.Diffuse.rgb * light; //明るさを乗算
	outDiffuse.a *= In.Diffuse.a; //αに明るさは関係ないので別計算

	//カメラからピクセルへ向かうベクトル
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	eyev = normalize(eyev); //正規化する

	float rim = 1.0 + dot(eyev, normal.xyz); //視線と法線の内積を明るさに変換する
	rim = pow(rim, 3) * 2.0f; //スペキュラと同じような処理を適当に行う。
	rim = saturate(rim); //rimをサチュレートする
	outDiffuse.rgb += rim * 0.5f; //通常の色へ加算する。
	outDiffuse.a = In.Diffuse.a;
	
	//カメラからピクセルへ向かうベクトル
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	eyev = normalize(eyev); //正規化する

	//光の反射ベクトルを計算
	float3 refv = reflect(Light.Direction.xyz, normal.xyz);
	refv = normalize(refv); //正規化する

	float specular = -dot(eyev, refv); //鏡面反射の計算
	specular = saturate(specular); //値をサチュレート

	specular = pow(specular, 30); //ここでは３０乗してみる

	outDiffuse.rgb += specular;//スペキュラ値をデフューズとして足しこむ
	
	//エッジの作成
	
	
	*/

	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz; //視線ベクトルを作る
	eyev = normalize(eyev); //視線ベクトルを正規化する
	float d = dot(eyev, normal.xyz); //法線と視線ベクトルの内積

	
	if (d > -0.2) //dの値を判定
	{
		//outDiffuse.rgb *= 0.2; //適当な閾値で明度を落とす
		outDiffuse.r = 0.8;
		outDiffuse.b = 0.8;
		outDiffuse.g = 0.8;

	}
	
}

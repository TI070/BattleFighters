
#include "common.hlsl"


void main(in VS_IN In, out PS_IN Out)
{
	/*
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);

	float4 worldNormal, normal;
	normal = float4(In.Normal.xyz, 0.0);
	worldNormal = mul(normal, World);
	worldNormal = normalize(worldNormal);

	float light = -dot(Light.Direction.xyz, worldNormal.xyz);
	light = saturate(light);

	Out.Diffuse = In.Diffuse * Material.Diffuse * light * Light.Diffuse;
	Out.Diffuse += In.Diffuse * Material.Ambient * Light.Ambient;
	Out.Diffuse += Material.Emission;
	Out.Diffuse.a = In.Diffuse.a * Material.Diffuse.a;

	Out.Position = mul(In.Position, wvp);
	Out.TexCoord = In.TexCoord;
	*/

	//１頂点分のデータを受け取り、処理して、出力する
//頂点変換処理 この処理は必ず必要
	matrix wvp; //行列変数を作成
	wvp = mul(World, View); //wvp = ワールド行列＊カメラ行列
	wvp = mul(wvp, Projection); //wvp = wvp *プロジェクション行列
	Out.Position = mul(In.Position, wvp); //変換結果を出力する

		//光源計算
	float4 worldNormal, normal;
	normal = float4(In.Normal.xyz, 0.0f);//平行移動しないようwを０にする
	worldNormal = mul(normal, World);
	worldNormal = normalize(worldNormal);
	Out.Normal = worldNormal;

	//受け取ったこの頂点のデフューズをそのまま出力
	Out.Diffuse = In.Diffuse;
	Out.TexCoord = In.TexCoord;

	//ワールド変換した頂点座標を出力
	Out.WorldPosition = mul(In.Position, World);

}

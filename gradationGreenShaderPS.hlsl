

#include "common.hlsl"


Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{

	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse *= In.Diffuse;


	//汎用パラメータを参照し、グラデーションに影響させる
	//outDiffuse.rgb += Parameter.rgb;
	//outDiffuse += Parameter;


	outDiffuse.r = sin(Parameter.x + In.Position.x * 5 / 360.0f) * 0.5f;
	outDiffuse.g = sin(Parameter.x + In.Position.x * 5 / 360.0f) * 0.5f + 0.5f;
	outDiffuse.b = sin(Parameter.x + In.Position.x * 5 / 360.0f) * 0.5f;


}

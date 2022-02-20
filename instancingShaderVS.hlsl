
#include "common.hlsl"


void main(in VS_IN In, out PS_IN Out,uint InstanceId : SV_InstanceID ,uint vertexId:SV_VertexID)
{

	//�ʒu�����炷
	float3 offsetPos;
	offsetPos = instancingData.Position[InstanceId];

	In.Position.xyz += offsetPos.xyz;

	//�C���X�^���V���O�@�F
	float4 color;
	color = instancingData.Diffuse[InstanceId];

	//�C���X�^���V���O�@�T�C�Y
	//In.Position.xyz *= instancingData.Scale[InstanceId].xyz;

	float4 size = instancingData.Scale[InstanceId];
	
	//���_���ƂɈړ�����
	if (vertexId == 0) {
		In.Position.x -= size.x;
		In.Position.y -= size.y;
	}
	else if(vertexId == 1) {
		In.Position.x += size.x;
		In.Position.y -= size.y;
	}
	else if (vertexId == 2) {
		In.Position.x -= size.x;
		In.Position.y += size.y;

	}
	else if (vertexId == 3) {
		In.Position.x += size.x;
		In.Position.y += size.y;
	}

	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);

	Out.Position = mul(In.Position, wvp);

	Out.TexCoord = In.TexCoord;

	Out.Diffuse = In.Diffuse;
	Out.Diffuse.xyz *= color.xyz;


}

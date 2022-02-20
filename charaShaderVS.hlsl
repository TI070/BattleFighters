
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

	//�P���_���̃f�[�^���󂯎��A�������āA�o�͂���
//���_�ϊ����� ���̏����͕K���K�v
	matrix wvp; //�s��ϐ����쐬
	wvp = mul(World, View); //wvp = ���[���h�s�񁖃J�����s��
	wvp = mul(wvp, Projection); //wvp = wvp *�v���W�F�N�V�����s��
	Out.Position = mul(In.Position, wvp); //�ϊ����ʂ��o�͂���

		//�����v�Z
	float4 worldNormal, normal;
	normal = float4(In.Normal.xyz, 0.0f);//���s�ړ����Ȃ��悤w���O�ɂ���
	worldNormal = mul(normal, World);
	worldNormal = normalize(worldNormal);
	Out.Normal = worldNormal;

	//�󂯎�������̒��_�̃f�t���[�Y�����̂܂܏o��
	Out.Diffuse = In.Diffuse;
	Out.TexCoord = In.TexCoord;

	//���[���h�ϊ��������_���W���o��
	Out.WorldPosition = mul(In.Position, World);

}

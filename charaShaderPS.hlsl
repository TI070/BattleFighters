

#include "common.hlsl"


Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{

	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse *= In.Diffuse;


	//�ėp�p�����[�^�����Z����
	outDiffuse.rgb += Parameter.rgb;

	
	//���͒�����
	float4 normal = normalize(In.Normal);
	/*
	float light = 0.5 - dot(normal.xyz, Light.Direction.xyz) * 0.5; //�����v�Z������

	//�e�N�X�`���̃s�N�Z���F���擾
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse.rgb *= In.Diffuse.rgb * light; //���邳����Z
	outDiffuse.a *= In.Diffuse.a; //���ɖ��邳�͊֌W�Ȃ��̂ŕʌv�Z

	//�J��������s�N�Z���֌������x�N�g��
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	eyev = normalize(eyev); //���K������

	float rim = 1.0 + dot(eyev, normal.xyz); //�����Ɩ@���̓��ς𖾂邳�ɕϊ�����
	rim = pow(rim, 3) * 2.0f; //�X�y�L�����Ɠ����悤�ȏ�����K���ɍs���B
	rim = saturate(rim); //rim���T�`�����[�g����
	outDiffuse.rgb += rim * 0.5f; //�ʏ�̐F�։��Z����B
	outDiffuse.a = In.Diffuse.a;
	
	//�J��������s�N�Z���֌������x�N�g��
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	eyev = normalize(eyev); //���K������

	//���̔��˃x�N�g�����v�Z
	float3 refv = reflect(Light.Direction.xyz, normal.xyz);
	refv = normalize(refv); //���K������

	float specular = -dot(eyev, refv); //���ʔ��˂̌v�Z
	specular = saturate(specular); //�l���T�`�����[�g

	specular = pow(specular, 30); //�����ł͂R�O�悵�Ă݂�

	outDiffuse.rgb += specular;//�X�y�L�����l���f�t���[�Y�Ƃ��đ�������
	
	//�G�b�W�̍쐬
	
	
	*/

	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz; //�����x�N�g�������
	eyev = normalize(eyev); //�����x�N�g���𐳋K������
	float d = dot(eyev, normal.xyz); //�@���Ǝ����x�N�g���̓���

	
	if (d > -0.2) //d�̒l�𔻒�
	{
		//outDiffuse.rgb *= 0.2; //�K����臒l�Ŗ��x�𗎂Ƃ�
		outDiffuse.r = 0.8;
		outDiffuse.b = 0.8;
		outDiffuse.g = 0.8;

	}
	
}

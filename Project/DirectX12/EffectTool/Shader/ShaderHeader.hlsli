struct Output {
	float4 svPos:SV_POSITION;
	float2 uv : TEXCOORD;
};

struct BasicType {
	float4 svpos:SV_POSITION;//�V�X�e���p���_���W
	float4 normal:NORMAL;//�@���x�N�g��
	float2 uv:TEXCOORD;//UV�l
};


Texture2D<float4> tex:register(t0);
SamplerState smp:register(s0);
cbuffer cbuff0:register(b0)
{
	matrix mat;
}

// �}�e���A��
cbuffer ConstantBufferMaterial : register(b1)
{
	float4	RoughnessMaterial;
	float4	DiffuseMaterial;
	float4	MetalnessMaterial;
}

//// �}�e���A��
//cbuffer ConstantBufferMaterial : register(b1)
//{
//	float4	RoughnessMaterial;
//	float4	DiffuseMaterial;
//	float4	MetalnessMaterial;
//}

struct VS_INPUT
{
	float4 Pos : POSITION;
	float4 Nor:NORMAL;
	float2 Tex : TEXCOORD;
	//float4x4 mat: LocalToWorld;
};

// �\���̒�` //
// VS����o��
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
	float2 Tex : TEXCOORD;
	float4 WPos : TEXCOORD1;
	float4 Normal : TEXCOORD2;
};

// PS�ɓ���
struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
	float2 Tex : TEXCOORD;
	float4 WPos : TEXCOORD1;
	float4 Normal : TEXCOORD2;
	float3 ViewDirection:TEXCOORD3;
};
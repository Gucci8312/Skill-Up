struct Output {
	float4 svPos:SV_POSITION;
	float2 uv : TEXCOORD;
};

struct BasicType {
	float4 svpos:SV_POSITION;//システム用頂点座標
	float4 normal:NORMAL;//法線ベクトル
	float2 uv:TEXCOORD;//UV値
};


Texture2D<float4> tex:register(t0);
SamplerState smp:register(s0);
cbuffer cbuff0:register(b0)
{
	matrix mat;
}

// マテリアル
cbuffer ConstantBufferMaterial : register(b1)
{
	float4	RoughnessMaterial;
	float4	DiffuseMaterial;
	float4	MetalnessMaterial;
}

//// マテリアル
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

// 構造体定義 //
// VSから出力
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
	float2 Tex : TEXCOORD;
	float4 WPos : TEXCOORD1;
	float4 Normal : TEXCOORD2;
};

// PSに入力
struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
	float2 Tex : TEXCOORD;
	float4 WPos : TEXCOORD1;
	float4 Normal : TEXCOORD2;
	float3 ViewDirection:TEXCOORD3;
};
#include "ShaderHeader.hlsli"
VS_OUTPUT main(VS_INPUT input, uint instNo : SV_InstanceID/*, float4x4 mat : LocalToWorld*/)
{
	VS_OUTPUT Output;
	input.Pos.x += instNo * 10;
	Output.Pos = mul(mat, input.Pos);
	Output.Tex = input.Tex;
	return Output;
}


//PS_INPUT main(float4 Pos		: POSITION,
//	float4 Normal : NORMAL,
//	float2 Tex : TEXCOORD,
//	float4x4 mat : LocalToWorld)
//{
//	PS_INPUT output = (PS_INPUT)0;
//
//	output.Pos = mul(Pos, mat);
//	output.WPos = output.Pos;
//	output.Pos = mul(output.Pos, View);
//	output.Pos = mul(output.Pos, Projection);
//	output.Tex = Tex;
//	output.ViewDirection = Pos.xyz / Pos.w - View._41_42_43;
//
//	float4 N = Normal;
//	N.w = 0.0f;
//
//	N = mul(N, World);
//	N = normalize(N);
//
//	output.Normal = N;
//
//	return output;
//}
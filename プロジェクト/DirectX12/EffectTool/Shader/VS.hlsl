#include "ShaderHeader.hlsli"
VS_OUTPUT main(float4 Pos : POSITION, float2 Tex : TEXCOORD)
{
	VS_OUTPUT Output;
	Output.Pos = mul(mat, Pos);
	Output.Tex = Tex;
	return Output;
}
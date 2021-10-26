#include "ShaderHeader.hlsli"

float4 main(VS_OUTPUT Input) : SV_TARGET
{
	float4 col;
col = float4(tex.Sample(smp, Input.Tex));
return col;
//return float4(tex.Sample(smp,Input.Tex));
}
//
//
//// PS í èÌï`âÊ //
//float4 main(VS_OUTPUT input) : SV_Target
//{
//	float4 N = input.Normal;
//	float4 L = LightDirection;
//	N.w = 0.0f;
//	L.w = 0.0f;
//
//	N = normalize(N);
//	L = normalize(L);
//
//	float d = max(0.0, dot(L, N));
//	float4 diffuse = DiffuseMaterial * d;
//
//	float4 H;
//	float4 V = normalize(EyePos - input.WPos);
//	H = normalize(L + V);
//
//	float s = max(0, dot(N, H));
//	s = pow(s, 50);
//	float4 specular = s * MetalnessMaterial;
//
//	float4 col = Roughness + specular + diffuse;
//	col.a = 1.0f;
//
//	return col;
//}
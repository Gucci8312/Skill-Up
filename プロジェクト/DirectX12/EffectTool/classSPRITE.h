#pragma once
#include "DIRECTX12.h"
#include "CAMERA.h"
#include "MathUtil.h"
#include "DirectX12_Wrapper.h"

using namespace DirectX;

class SPRITE
{
private:
	struct Vertex {
		XMFLOAT3 Pos;
		XMFLOAT2 uv;
	};
	XMFLOAT4X4		g_mtxworld;									// ÉèÅ[ÉãÉhïœä∑çsóÒ
	XMFLOAT3 Pos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 Angle = { 0.0f,0.0f,0.0f };

	ComPtr<ID3D12Resource> VertexBuffer;
	ComPtr<ID3D12Resource> IndexBuffer;
	ComPtr<ID3D12Resource> ConstBuffer;
	ComPtr<ID3D12Resource> TexBuffer;
	ComPtr<ID3DBlob> vsBlob = nullptr;
	ComPtr<ID3DBlob> psBlob = nullptr;
	ComPtr<ID3D12PipelineState> PipelineState = nullptr;
	ComPtr<ID3D12RootSignature>	RootSignature = nullptr;
	D3D12_VERTEX_BUFFER_VIEW VerBuffView = {};
	D3D12_INDEX_BUFFER_VIEW IndBuffView = {};
	ComPtr<ID3D12DescriptorHeap> BasicDescHeap = nullptr;

	bool CreatePipeline();

public:
	bool Init(const char* _FileName, XMFLOAT3 _Pos, float _Width, float _Height);
	bool BackInit(const char* _FileName, XMFLOAT3 _Pos, float _Width, float _Height);
	void Update();
	void Draw();
	void PosSet(XMFLOAT3 _Pos) { Pos.x = _Pos.x, Pos.y = _Pos.y, Pos.z = _Pos.z; }
	void AngleSet(XMFLOAT3 _Angle) { Angle.x = _Angle.x, Angle.y = _Angle.y, Angle.z = _Angle.z; }
};


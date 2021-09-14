#pragma once
#include "DIRECTX12.h"
#include "CAMERA.h"
#include "MathUtil.h"
#include "DirectX12_Wrapper.h"

class PARTICLE
{
protected:
	struct Vertex {
		XMFLOAT3 Pos;															// 座標
		XMFLOAT3 Normal;														// 法線ベクトル
		XMFLOAT2 Tex;															// テクスチャ座標
	};
	XMFLOAT4X4		g_mtxworld;									// ワールド変換行列
	XMFLOAT3 Pos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 Angle = { 0.0f,0.0f,0.0f };

	ComPtr<ID3D12Resource> VertexBuffer;
	ComPtr<ID3D12Resource> IndexBuffer;
	ComPtr<ID3D12Resource> InstanceBuffer;
	ComPtr<ID3D12Resource> ConstBuffer;
	ComPtr<ID3D12Resource> TexBuffer;
	ComPtr<ID3DBlob> vsBlob = nullptr;
	ComPtr<ID3DBlob> psBlob = nullptr;
	ComPtr<ID3D12PipelineState> PipelineState = nullptr;
	ComPtr<ID3D12RootSignature>	RootSignature = nullptr;
	D3D12_VERTEX_BUFFER_VIEW VerBuffView[2] = {};
	//D3D12_VERTEX_BUFFER_VIEW VerBuffView = {};
	D3D12_INDEX_BUFFER_VIEW IndBuffView = {};
	ComPtr<ID3D12DescriptorHeap> BasicDescHeap = nullptr;
	void* InitData = nullptr;													// インスタンスバッファの初期化データ

public:
	virtual bool Init(const char* _FileName, XMFLOAT3 _Pos, float _Width, float _Height) = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};


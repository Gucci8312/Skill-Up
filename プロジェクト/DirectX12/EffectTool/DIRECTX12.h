#pragma once
#define DEBUG

// インクルード
#include <d3d12.h>
#include <d3dx12.h>
#include <dxgi1_4.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <DirectXTex.h>
#include <d3dcompiler.h>
#include <vector>
#include <string>
#include <locale.h>
#include <memory>
#include <malloc.h>
#include <array>
#include "MathUtil.h"

// ライブラリリンク
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"DirectXTex.lib")

using namespace Microsoft::WRL;
using namespace DirectX;

class DIRECTX12
{
private:
	DIRECTX12() {}
	ComPtr<ID3D12Device>				Device = nullptr;
	ComPtr<IDXGIFactory4>				DxgiGactory = nullptr;
	ComPtr<IDXGISwapChain3>				SwapChain = nullptr;
	ComPtr<ID3D12CommandAllocator>		CmdAllocator = nullptr;
	ComPtr<ID3D12GraphicsCommandList>	CmdList = nullptr;
	ComPtr<ID3D12CommandQueue>			CmdQueue = nullptr;
	ComPtr<ID3D12DescriptorHeap>		RTVHeaps = nullptr;
	ComPtr<ID3D12Fence>					Fence = nullptr;
	UINT64								FenceVal = 0;
	std::vector<ComPtr<ID3D12Resource>> BackBuffers;
	ComPtr<ID3D12Resource>				DepthBuffer = nullptr;
	ComPtr<ID3D12DescriptorHeap>		dsvHeap = nullptr;
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	UINT BuckBufIdx;
	D3D12_VIEWPORT ViewPort ;
	D3D12_RECT ScissorRect;
	
public:
	DIRECTX12(const DIRECTX12&) = delete;
	DIRECTX12& operator=(const DIRECTX12&) = delete;
	DIRECTX12(DIRECTX12&&) = delete;
	DIRECTX12& operator=(DIRECTX12&&) = delete;

	static DIRECTX12* GetInstance() {
		static DIRECTX12 Instance;
		return &Instance;
	}
	bool Init(HWND _hWnd, int _Width,int _Height);
	void UnInit();
	void BeforeRender();
	void AfterRender();
	void FenceWait();
	void BarrierResouce(ID3D12Resource* _Resouce, D3D12_RESOURCE_STATES _BeforeState, D3D12_RESOURCE_STATES _AfterState);
	//HRESULT CompileShader(const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	//HRESULT CompileShaderFromFile(const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

	ID3D12Device* GetDevice() const { return Device.Get(); }
	ID3D12GraphicsCommandList* GetCommandList() const { return CmdList.Get(); }
	ID3D12CommandQueue* GetCommandQueue() const { return CmdQueue.Get(); }
	ID3D12CommandAllocator* GetCommandAllocator() const { return CmdAllocator.Get(); }
	ID3D12Fence* GetFence() const { return Fence.Get(); }
};


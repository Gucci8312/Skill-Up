#pragma once

// ファイル読み込み
#include <Windows.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <dxgi1_4.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <DirectXTex.h>
#include <d3dcompiler.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"DirectXTex.lib")

// 名前空間
using namespace Microsoft::WRL;
using namespace DirectX;

// DirectX12ラッパー
namespace DirectX12Wrapper
{
	class DirectX12Util
	{
	private:
		static DirectX12Util* Instance;
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
		D3D12_VIEWPORT ViewPort;
		D3D12_RECT ScissorRect;

	public:
		// シングルトン インスタンス取得
		static DirectX12Util* GetInstance()
		{
			return Instance;
		}

		static void Create();
		static void Destroy();
		bool Init(HWND _hWnd, int _Width, int _Height);
		void UnInit();
		void BeforeRender();
		void AfterRender();
		void FenceWait();
		void BarrierResouce(ID3D12Resource* _Resouce, D3D12_RESOURCE_STATES _BeforeState, D3D12_RESOURCE_STATES _AfterState);
		ID3D12Device* GetDevice() { return Device.Get(); }
		ID3D12GraphicsCommandList* GetCmdList() { return CmdList.Get(); }
	};
}

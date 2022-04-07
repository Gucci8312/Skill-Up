#include "DirectX12Wrapper.h"
using namespace DirectX12Wrapper;

DirectX12Util* DirectX12Util::Instance = nullptr;

void EnableDebugLayer() {
	ID3D12Debug* debugLayer = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer)))) {
		debugLayer->EnableDebugLayer();
		debugLayer->Release();
	}
}

// 生成
void DirectX12Util::Create()
{
	if (!Instance)
	{
		Instance = new DirectX12Util;
	}
}

// 削除
void DirectX12Util::Destroy()
{
	delete Instance;
	Instance = nullptr;
}

// 生成
bool DirectX12Util::Init(HWND _hWnd, int _Width, int _Height)
{
#ifdef DEBUG
	EnableDebugLayer();
#endif

	// フューチャーレベル列挙
	D3D_FEATURE_LEVEL Levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

#ifdef DEBUG
	CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(DxgiGactory.ReleaseAndGetAddressOf()));
#else
	if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(DxgiGactory.ReleaseAndGetAddressOf()))))
	{
		MessageBox(nullptr, "ファクトリー作成に失敗しました。", "Error", MB_OK);
	}
#endif
	CoInitializeEx(0, COINIT_MULTITHREADED);

	// デバイス
	{
		// アダプター列拳用
		std::vector<ComPtr<IDXGIAdapter>> Adapters;
		ComPtr<IDXGIAdapter> tmpAdapter = nullptr;
		for (int i = 0; DxgiGactory->EnumAdapters(i, tmpAdapter.ReleaseAndGetAddressOf()) != DXGI_ERROR_NOT_FOUND; ++i)
		{
			Adapters.push_back(tmpAdapter);
		}

		for (auto adpt : Adapters)
		{
			DXGI_ADAPTER_DESC adesc = {};
			adpt->GetDesc(&adesc);

			std::wstring strDesc = adesc.Description;

			if (strDesc.find(L"NVIDIA") != std::string::npos)
			{
				tmpAdapter = adpt;
				break;
			}
		}

		// 作成
		D3D_FEATURE_LEVEL FeatureLevel;
		bool Flg;
		for (auto lv : Levels)
		{
			if (SUCCEEDED(D3D12CreateDevice(tmpAdapter.Get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(Device.ReleaseAndGetAddressOf()))))
			{
				FeatureLevel = lv;
				Flg = true;
				break;
			}
			Flg = false;
		}

		// 失敗した場合
		if (Flg == false)
		{
			MessageBox(nullptr, "デバイスの作成に失敗しました。", "Error", MB_OK);
			return false;
		}
	}

	// コマンドアロケーター
	{
		if (FAILED(Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(CmdAllocator.ReleaseAndGetAddressOf()))))
		{
			MessageBox(nullptr, "コマンドアロケーターの作成に失敗しました。", "Error", MB_OK);
			return false;
		}
	}


	// コマンドリスト
	{
		if (FAILED(Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, CmdAllocator.Get(), nullptr, IID_PPV_ARGS(CmdList.ReleaseAndGetAddressOf()))))
		{
			MessageBox(nullptr, "コマンドアロケーターの作成に失敗しました。", "Error", MB_OK);
			return false;
		}
	}

	// コマンドキュー
	{
		// 設定
		D3D12_COMMAND_QUEUE_DESC CmdQueueDesc = {};
		CmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		CmdQueueDesc.NodeMask = 0;
		CmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		CmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

		// 作成
		if (FAILED(Device->CreateCommandQueue(&CmdQueueDesc, IID_PPV_ARGS(CmdQueue.ReleaseAndGetAddressOf()))))
		{
			MessageBox(nullptr, "コマンドキューの作成に失敗しました。", "Error", MB_OK);
			return false;
		}
	}

	// スワップチェイン
	{
		// 設定
		DXGI_SWAP_CHAIN_DESC1 SwapChainDesc = {};
		SwapChainDesc.Width = _Width;
		SwapChainDesc.Height = _Height;
		SwapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		SwapChainDesc.Stereo = false;
		SwapChainDesc.SampleDesc.Count = 1;
		SwapChainDesc.SampleDesc.Quality = 0;
		SwapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
		SwapChainDesc.BufferCount = 2;
		SwapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		SwapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		// 作成
		if (FAILED(DxgiGactory->CreateSwapChainForHwnd(CmdQueue.Get(), _hWnd, &SwapChainDesc, nullptr, nullptr, (IDXGISwapChain1**)SwapChain.ReleaseAndGetAddressOf())))
		{
			MessageBox(nullptr, "スワップチェインの作成に失敗しました。", "Error", MB_OK);
			return false;
		}
	}

	// ディスクリプタヒープ
	{
		// 設定
		D3D12_DESCRIPTOR_HEAP_DESC HeapDesc = {};
		HeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		HeapDesc.NodeMask = 0;
		HeapDesc.NumDescriptors = 2;
		HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

		// 作成
		if (FAILED(Device->CreateDescriptorHeap(&HeapDesc, IID_PPV_ARGS(RTVHeaps.ReleaseAndGetAddressOf()))))
		{
			MessageBox(nullptr, "ディスクリプタヒープの作成に失敗しました。", "Error", MB_OK);
			return false;
		}
	}

	// レンダーターゲットビュー
	{D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	DXGI_SWAP_CHAIN_DESC swcDesc = {};
	SwapChain->GetDesc(&swcDesc);

	BackBuffers.resize(swcDesc.BufferCount);
	// 先頭のアドレス取得
	D3D12_CPU_DESCRIPTOR_HANDLE Handle = RTVHeaps->GetCPUDescriptorHandleForHeapStart();
	for (auto idx = 0; idx < swcDesc.BufferCount; ++idx)
	{
		SwapChain->GetBuffer(static_cast<UINT>(idx), IID_PPV_ARGS(&BackBuffers[idx]));
		// 作成
		Device->CreateRenderTargetView(BackBuffers[idx].Get(), &rtvDesc, Handle);
		Handle.ptr += Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}
	}

	// フェンス
	{
		// 作成
		if (FAILED(Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(Fence.ReleaseAndGetAddressOf()))))
		{
			MessageBox(nullptr, "フェンスの作成に失敗しました。", "Error", MB_OK);
			return false;
		}
	}

	// ビューポート
	{
		ViewPort.Width = _Width;		// 画面の横幅
		ViewPort.Height = _Height;		// 画面の縦幅
		ViewPort.TopLeftX = 0;			// 左上X座標
		ViewPort.TopLeftY = 0;			// 左上Y座標
		ViewPort.MaxDepth = 1.0f;		// 深度の最大値
		ViewPort.MinDepth = 0.0f;		// 深度の最小値
	}

	// シザー矩形
	{
		ScissorRect.top = 0;								// 上座標
		ScissorRect.left = 0;								// 左座標
		ScissorRect.right = ScissorRect.left + _Width;		// 右座標
		ScissorRect.bottom = ScissorRect.top + _Height;		// 下座標
	}

	// 深度バッファ
	{
		D3D12_RESOURCE_DESC DepthResDesc = {};
		DepthResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		DepthResDesc.Width = _Width;
		DepthResDesc.Height = _Height;
		DepthResDesc.DepthOrArraySize = 1;
		DepthResDesc.Format = DXGI_FORMAT_D32_FLOAT;
		DepthResDesc.SampleDesc.Count = 1;
		DepthResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		// 深度用ヒーププロパティ
		D3D12_HEAP_PROPERTIES DepthHeapProp = {};
		DepthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
		DepthHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		DepthHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

		D3D12_CLEAR_VALUE DepthClearValue = {};
		DepthClearValue.DepthStencil.Depth = 1.0f;
		DepthClearValue.Format = DXGI_FORMAT_D32_FLOAT;

		// 作成
		if (FAILED(Device->CreateCommittedResource(&DepthHeapProp, D3D12_HEAP_FLAG_NONE,
			&DepthResDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &DepthClearValue, IID_PPV_ARGS(DepthBuffer.ReleaseAndGetAddressOf()))))
		{
			MessageBox(nullptr, "深度バッファの作成に失敗しました。", "Error", MB_OK);
			return false;
		}

		D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
		dsvHeapDesc.NumDescriptors = 1;
		dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

		// 作成
		if (FAILED(Device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(dsvHeap.ReleaseAndGetAddressOf()))))
		{
			MessageBox(nullptr, "深度用ディスクリプターヒープの作成に失敗しました。", "Error", MB_OK);
			return false;
		}
		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

		// 作成
		Device->CreateDepthStencilView(DepthBuffer.Get(), &dsvDesc, dsvHeap->GetCPUDescriptorHandleForHeapStart());
	}

	return true;
}

void DirectX12Util::UnInit()
{
}

void DirectX12Util::BeforeRender()
{
	//　バックバッファのインデックス取得
	BuckBufIdx = SwapChain->GetCurrentBackBufferIndex();

	BarrierResouce(BackBuffers[BuckBufIdx].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	//　レンダーターゲットを指定
	auto rtvH = RTVHeaps->GetCPUDescriptorHandleForHeapStart();
	rtvH.ptr += static_cast<ULONG_PTR>(BuckBufIdx * Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));

	// 深度バッファ
	auto dsvH = dsvHeap->GetCPUDescriptorHandleForHeapStart();

	// レンダーターゲット
	CmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	//　画面クリア
	float clearColor[] = { 0.0f,0.0f,0.0f,1.0f };
	CmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
	CmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	CmdList->RSSetViewports(1, &ViewPort);			// ビューポート
	CmdList->RSSetScissorRects(1, &ScissorRect);	// シザー矩形
}

void DirectX12Util::AfterRender()
{
	// バリア
	BarrierResouce(BackBuffers[BuckBufIdx].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	//命令のクローズ
	CmdList->Close();

	//コマンドリストの実行
	ID3D12CommandList* cmdlists[] = { CmdList.Get() };
	CmdQueue->ExecuteCommandLists(1, cmdlists);

	//待ち
	FenceWait();
	CmdAllocator->Reset();							// キュークリア
	CmdList->Reset(CmdAllocator.Get(), nullptr);	// コマンドリストクリア

	//フリップ
	SwapChain->Present(1, 0);
}

void DirectX12Util::FenceWait()
{
	CmdQueue->Signal(Fence.Get(), ++FenceVal);

	// GPUの処理が終わっていない時
	if (Fence->GetCompletedValue() != FenceVal)
	{
		auto event = CreateEvent(nullptr, false, false, nullptr);			// イベントハンドル取得
		Fence->SetEventOnCompletion(FenceVal, event);					// イベントセット
		WaitForSingleObject(event, INFINITE);							// イベントが発生するまで待つ
		CloseHandle(event);												// イベント終了
	}
}

void DirectX12Wrapper::DirectX12Util::SetViewPortParmeter(XMFLOAT2 Size, XMFLOAT2 Pos, XMFLOAT2 Depth)
{
	ViewPort.Width = Size.x;			// 画面の横幅
	ViewPort.Height = Size.y;			// 画面の縦幅
	ViewPort.TopLeftX = Pos.x;			// 左上X座標
	ViewPort.TopLeftY = Pos.y;			// 左上Y座標
	ViewPort.MaxDepth = Depth.x;		// 深度の最大値
	ViewPort.MinDepth = Depth.y;		// 深度の最小値
}

void DirectX12Util::BarrierResouce(ID3D12Resource* _Resouce, D3D12_RESOURCE_STATES _BeforeState, D3D12_RESOURCE_STATES _AfterState)
{
	// 設定
	D3D12_RESOURCE_BARRIER BarrierDesc = {};
	BarrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	BarrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	BarrierDesc.Transition.pResource = _Resouce;
	BarrierDesc.Transition.Subresource = 0;
	BarrierDesc.Transition.StateBefore = _BeforeState;
	BarrierDesc.Transition.StateAfter = _AfterState;

	// バリア実行
	CmdList->ResourceBarrier(1, &BarrierDesc);
}

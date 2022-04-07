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

// ����
void DirectX12Util::Create()
{
	if (!Instance)
	{
		Instance = new DirectX12Util;
	}
}

// �폜
void DirectX12Util::Destroy()
{
	delete Instance;
	Instance = nullptr;
}

// ����
bool DirectX12Util::Init(HWND _hWnd, int _Width, int _Height)
{
#ifdef DEBUG
	EnableDebugLayer();
#endif

	// �t���[�`���[���x����
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
		MessageBox(nullptr, "�t�@�N�g���[�쐬�Ɏ��s���܂����B", "Error", MB_OK);
	}
#endif
	CoInitializeEx(0, COINIT_MULTITHREADED);

	// �f�o�C�X
	{
		// �A�_�v�^�[�񌝗p
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

		// �쐬
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

		// ���s�����ꍇ
		if (Flg == false)
		{
			MessageBox(nullptr, "�f�o�C�X�̍쐬�Ɏ��s���܂����B", "Error", MB_OK);
			return false;
		}
	}

	// �R�}���h�A���P�[�^�[
	{
		if (FAILED(Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(CmdAllocator.ReleaseAndGetAddressOf()))))
		{
			MessageBox(nullptr, "�R�}���h�A���P�[�^�[�̍쐬�Ɏ��s���܂����B", "Error", MB_OK);
			return false;
		}
	}


	// �R�}���h���X�g
	{
		if (FAILED(Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, CmdAllocator.Get(), nullptr, IID_PPV_ARGS(CmdList.ReleaseAndGetAddressOf()))))
		{
			MessageBox(nullptr, "�R�}���h�A���P�[�^�[�̍쐬�Ɏ��s���܂����B", "Error", MB_OK);
			return false;
		}
	}

	// �R�}���h�L���[
	{
		// �ݒ�
		D3D12_COMMAND_QUEUE_DESC CmdQueueDesc = {};
		CmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		CmdQueueDesc.NodeMask = 0;
		CmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		CmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

		// �쐬
		if (FAILED(Device->CreateCommandQueue(&CmdQueueDesc, IID_PPV_ARGS(CmdQueue.ReleaseAndGetAddressOf()))))
		{
			MessageBox(nullptr, "�R�}���h�L���[�̍쐬�Ɏ��s���܂����B", "Error", MB_OK);
			return false;
		}
	}

	// �X���b�v�`�F�C��
	{
		// �ݒ�
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

		// �쐬
		if (FAILED(DxgiGactory->CreateSwapChainForHwnd(CmdQueue.Get(), _hWnd, &SwapChainDesc, nullptr, nullptr, (IDXGISwapChain1**)SwapChain.ReleaseAndGetAddressOf())))
		{
			MessageBox(nullptr, "�X���b�v�`�F�C���̍쐬�Ɏ��s���܂����B", "Error", MB_OK);
			return false;
		}
	}

	// �f�B�X�N���v�^�q�[�v
	{
		// �ݒ�
		D3D12_DESCRIPTOR_HEAP_DESC HeapDesc = {};
		HeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		HeapDesc.NodeMask = 0;
		HeapDesc.NumDescriptors = 2;
		HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

		// �쐬
		if (FAILED(Device->CreateDescriptorHeap(&HeapDesc, IID_PPV_ARGS(RTVHeaps.ReleaseAndGetAddressOf()))))
		{
			MessageBox(nullptr, "�f�B�X�N���v�^�q�[�v�̍쐬�Ɏ��s���܂����B", "Error", MB_OK);
			return false;
		}
	}

	// �����_�[�^�[�Q�b�g�r���[
	{D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	DXGI_SWAP_CHAIN_DESC swcDesc = {};
	SwapChain->GetDesc(&swcDesc);

	BackBuffers.resize(swcDesc.BufferCount);
	// �擪�̃A�h���X�擾
	D3D12_CPU_DESCRIPTOR_HANDLE Handle = RTVHeaps->GetCPUDescriptorHandleForHeapStart();
	for (auto idx = 0; idx < swcDesc.BufferCount; ++idx)
	{
		SwapChain->GetBuffer(static_cast<UINT>(idx), IID_PPV_ARGS(&BackBuffers[idx]));
		// �쐬
		Device->CreateRenderTargetView(BackBuffers[idx].Get(), &rtvDesc, Handle);
		Handle.ptr += Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}
	}

	// �t�F���X
	{
		// �쐬
		if (FAILED(Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(Fence.ReleaseAndGetAddressOf()))))
		{
			MessageBox(nullptr, "�t�F���X�̍쐬�Ɏ��s���܂����B", "Error", MB_OK);
			return false;
		}
	}

	// �r���[�|�[�g
	{
		ViewPort.Width = _Width;		// ��ʂ̉���
		ViewPort.Height = _Height;		// ��ʂ̏c��
		ViewPort.TopLeftX = 0;			// ����X���W
		ViewPort.TopLeftY = 0;			// ����Y���W
		ViewPort.MaxDepth = 1.0f;		// �[�x�̍ő�l
		ViewPort.MinDepth = 0.0f;		// �[�x�̍ŏ��l
	}

	// �V�U�[��`
	{
		ScissorRect.top = 0;								// ����W
		ScissorRect.left = 0;								// �����W
		ScissorRect.right = ScissorRect.left + _Width;		// �E���W
		ScissorRect.bottom = ScissorRect.top + _Height;		// �����W
	}

	// �[�x�o�b�t�@
	{
		D3D12_RESOURCE_DESC DepthResDesc = {};
		DepthResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		DepthResDesc.Width = _Width;
		DepthResDesc.Height = _Height;
		DepthResDesc.DepthOrArraySize = 1;
		DepthResDesc.Format = DXGI_FORMAT_D32_FLOAT;
		DepthResDesc.SampleDesc.Count = 1;
		DepthResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		// �[�x�p�q�[�v�v���p�e�B
		D3D12_HEAP_PROPERTIES DepthHeapProp = {};
		DepthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
		DepthHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		DepthHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

		D3D12_CLEAR_VALUE DepthClearValue = {};
		DepthClearValue.DepthStencil.Depth = 1.0f;
		DepthClearValue.Format = DXGI_FORMAT_D32_FLOAT;

		// �쐬
		if (FAILED(Device->CreateCommittedResource(&DepthHeapProp, D3D12_HEAP_FLAG_NONE,
			&DepthResDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &DepthClearValue, IID_PPV_ARGS(DepthBuffer.ReleaseAndGetAddressOf()))))
		{
			MessageBox(nullptr, "�[�x�o�b�t�@�̍쐬�Ɏ��s���܂����B", "Error", MB_OK);
			return false;
		}

		D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
		dsvHeapDesc.NumDescriptors = 1;
		dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

		// �쐬
		if (FAILED(Device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(dsvHeap.ReleaseAndGetAddressOf()))))
		{
			MessageBox(nullptr, "�[�x�p�f�B�X�N���v�^�[�q�[�v�̍쐬�Ɏ��s���܂����B", "Error", MB_OK);
			return false;
		}
		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

		// �쐬
		Device->CreateDepthStencilView(DepthBuffer.Get(), &dsvDesc, dsvHeap->GetCPUDescriptorHandleForHeapStart());
	}

	return true;
}

void DirectX12Util::UnInit()
{
}

void DirectX12Util::BeforeRender()
{
	//�@�o�b�N�o�b�t�@�̃C���f�b�N�X�擾
	BuckBufIdx = SwapChain->GetCurrentBackBufferIndex();

	BarrierResouce(BackBuffers[BuckBufIdx].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	//�@�����_�[�^�[�Q�b�g���w��
	auto rtvH = RTVHeaps->GetCPUDescriptorHandleForHeapStart();
	rtvH.ptr += static_cast<ULONG_PTR>(BuckBufIdx * Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));

	// �[�x�o�b�t�@
	auto dsvH = dsvHeap->GetCPUDescriptorHandleForHeapStart();

	// �����_�[�^�[�Q�b�g
	CmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	//�@��ʃN���A
	float clearColor[] = { 0.0f,0.0f,0.0f,1.0f };
	CmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
	CmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	CmdList->RSSetViewports(1, &ViewPort);			// �r���[�|�[�g
	CmdList->RSSetScissorRects(1, &ScissorRect);	// �V�U�[��`
}

void DirectX12Util::AfterRender()
{
	// �o���A
	BarrierResouce(BackBuffers[BuckBufIdx].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	//���߂̃N���[�Y
	CmdList->Close();

	//�R�}���h���X�g�̎��s
	ID3D12CommandList* cmdlists[] = { CmdList.Get() };
	CmdQueue->ExecuteCommandLists(1, cmdlists);

	//�҂�
	FenceWait();
	CmdAllocator->Reset();							// �L���[�N���A
	CmdList->Reset(CmdAllocator.Get(), nullptr);	// �R�}���h���X�g�N���A

	//�t���b�v
	SwapChain->Present(1, 0);
}

void DirectX12Util::FenceWait()
{
	CmdQueue->Signal(Fence.Get(), ++FenceVal);

	// GPU�̏������I����Ă��Ȃ���
	if (Fence->GetCompletedValue() != FenceVal)
	{
		auto event = CreateEvent(nullptr, false, false, nullptr);			// �C�x���g�n���h���擾
		Fence->SetEventOnCompletion(FenceVal, event);					// �C�x���g�Z�b�g
		WaitForSingleObject(event, INFINITE);							// �C�x���g����������܂ő҂�
		CloseHandle(event);												// �C�x���g�I��
	}
}

void DirectX12Wrapper::DirectX12Util::SetViewPortParmeter(XMFLOAT2 Size, XMFLOAT2 Pos, XMFLOAT2 Depth)
{
	ViewPort.Width = Size.x;			// ��ʂ̉���
	ViewPort.Height = Size.y;			// ��ʂ̏c��
	ViewPort.TopLeftX = Pos.x;			// ����X���W
	ViewPort.TopLeftY = Pos.y;			// ����Y���W
	ViewPort.MaxDepth = Depth.x;		// �[�x�̍ő�l
	ViewPort.MinDepth = Depth.y;		// �[�x�̍ŏ��l
}

void DirectX12Util::BarrierResouce(ID3D12Resource* _Resouce, D3D12_RESOURCE_STATES _BeforeState, D3D12_RESOURCE_STATES _AfterState)
{
	// �ݒ�
	D3D12_RESOURCE_BARRIER BarrierDesc = {};
	BarrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	BarrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	BarrierDesc.Transition.pResource = _Resouce;
	BarrierDesc.Transition.Subresource = 0;
	BarrierDesc.Transition.StateBefore = _BeforeState;
	BarrierDesc.Transition.StateAfter = _AfterState;

	// �o���A���s
	CmdList->ResourceBarrier(1, &BarrierDesc);
}

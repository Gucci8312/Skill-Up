#include "classSPRITE.h"

bool SPRITE::CreatePipeline()
{
	auto Device = DIRECTX12::GetInstance()->GetDevice();
	auto CmdList = DIRECTX12::GetInstance()->GetCommandList();
	auto CmdQueue = DIRECTX12::GetInstance()->GetCommandQueue();
	auto CmdAllocator = DIRECTX12::GetInstance()->GetCommandAllocator();

	// インプットレイアウト
	D3D12_INPUT_ELEMENT_DESC InputLayout[] =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
	};

	// パイプライン
	{
		// 設定
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PipeLine = {};

		// シェーダー
		PipeLine.VS.pShaderBytecode = vsBlob->GetBufferPointer();
		PipeLine.VS.BytecodeLength = vsBlob->GetBufferSize();
		PipeLine.PS.pShaderBytecode = psBlob->GetBufferPointer();
		PipeLine.PS.BytecodeLength = psBlob->GetBufferSize();

		PipeLine.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		// レンダーターゲットのブレンド設定
		D3D12_RENDER_TARGET_BLEND_DESC RenderTargetBlendDesc = {};
		RenderTargetBlendDesc.BlendEnable = false;
		RenderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		RenderTargetBlendDesc.LogicOpEnable = false;

		// ブレンド
		PipeLine.BlendState.AlphaToCoverageEnable = true;
		PipeLine.BlendState.IndependentBlendEnable = false;
		PipeLine.BlendState.RenderTarget[0] = RenderTargetBlendDesc;

		// ラスタライザー
		PipeLine.RasterizerState.MultisampleEnable = false;
		PipeLine.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;	// カリング無し
		PipeLine.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		PipeLine.RasterizerState.DepthClipEnable = true;			// 深度方向のクリッピング有効
		PipeLine.RasterizerState.FrontCounterClockwise = false;
		PipeLine.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
		PipeLine.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		PipeLine.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		PipeLine.RasterizerState.AntialiasedLineEnable = false;
		PipeLine.RasterizerState.ForcedSampleCount = 0;
		PipeLine.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

		PipeLine.DepthStencilState.DepthEnable = true;
		PipeLine.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		PipeLine.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;;
		PipeLine.DepthStencilState.StencilEnable = false;
		PipeLine.DSVFormat = DXGI_FORMAT_D32_FLOAT;

		PipeLine.InputLayout.pInputElementDescs = InputLayout;			// 先頭アドレス
		PipeLine.InputLayout.NumElements = _countof(InputLayout);		// 配列数

		PipeLine.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		PipeLine.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;	// 三角形

		PipeLine.NumRenderTargets = 1;
		PipeLine.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

		PipeLine.SampleDesc.Count = 1;
		PipeLine.SampleDesc.Quality = 0;

		// ルートシグネチャ設定
		D3D12_ROOT_SIGNATURE_DESC RootSignatureDesc = {};
		RootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		// ディスクリプタレンジ
		D3D12_DESCRIPTOR_RANGE DescTblRange[2] = {};

		// SRV
		DescTblRange[0].NumDescriptors = 1;
		DescTblRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		DescTblRange[0].BaseShaderRegister = 0;
		DescTblRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		// CBV
		DescTblRange[1].NumDescriptors = 1;
		DescTblRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		DescTblRange[1].BaseShaderRegister = 0;
		DescTblRange[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		// ルートパラメータ
		D3D12_ROOT_PARAMETER RootParam = {};
		RootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		RootParam.DescriptorTable.pDescriptorRanges = &DescTblRange[0];				// ディスクリプタレンジのアドレス
		RootParam.DescriptorTable.NumDescriptorRanges = 2;							// ディスクリプタレンジ数
		RootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;					// 全シェーダからアクセス可能

		RootSignatureDesc.pParameters = &RootParam;									// ルートパラメータの先頭アドレス
		RootSignatureDesc.NumParameters = 1;										// ルートパラメータ数

		// サンプラー
		D3D12_STATIC_SAMPLER_DESC SamplerDesc = {};
		SamplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;						// 横繰り返し
		SamplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;						// 縦繰り返し
		SamplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;						// 奥行繰り返し
		SamplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		SamplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
		SamplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
		SamplerDesc.MinLOD = 0.0f;
		SamplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		SamplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;				// ピクセルシェーダからアクセス可能

		RootSignatureDesc.pStaticSamplers = &SamplerDesc;
		RootSignatureDesc.NumStaticSamplers = 1;

		ComPtr<ID3DBlob> rootSigBlob = nullptr;
		ComPtr<ID3DBlob> errorBlob = nullptr;

		// ルートシグネチャ作成
		if (FAILED(D3D12SerializeRootSignature(&RootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, rootSigBlob.GetAddressOf(), errorBlob.ReleaseAndGetAddressOf())))
		{
			MessageBox(nullptr, "ルートシグネチャのバイナリデータの作成に失敗しました。", "Error", MB_OK);
			return false;
		}

		// ルートシグネチャ作成
		if (FAILED(Device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(RootSignature.ReleaseAndGetAddressOf()))))
		{
			MessageBox(nullptr, "ルートシグネチャの作成に失敗しました。", "Error", MB_OK);
			return false;
		}

		PipeLine.pRootSignature = RootSignature.Get();

		// グラフィックスパイプライン作成
		if (FAILED(Device->CreateGraphicsPipelineState(&PipeLine, IID_PPV_ARGS(PipelineState.ReleaseAndGetAddressOf()))))
		{
			MessageBox(nullptr, "グラフィックスパイプラインの作成に失敗しました。", "Error", MB_OK);
			return false;
		}
	}

	return true;
}
// 初期化
bool SPRITE::Init(const char* _FileName, XMFLOAT3 _Pos, float _Width, float _Height)
{
	auto Device = DIRECTX12::GetInstance()->GetDevice();
	auto CmdList = DIRECTX12::GetInstance()->GetCommandList();
	auto CmdQueue = DIRECTX12::GetInstance()->GetCommandQueue();
	auto CmdAllocator = DIRECTX12::GetInstance()->GetCommandAllocator();

	_Width / 2 - _Width, _Height / 2;
	// 頂点データ
	Vertex Vertices[] =
	{
		{{-_Width / 2 ,-_Height / 2,0.0f},{0.0f,1.0f}},
		{{-_Width / 2 ,_Height / 2,0.0f},{0.0f,0.0f}},
		{{_Width / 2 ,-_Height / 2,0.0f},{1.0f,1.0f}},
		{{_Width / 2 ,_Height / 2,0.0f},{1.0f,0.0f}},
	};

	// バッファ
	{
		// ヒープ設定
		D3D12_HEAP_PROPERTIES HeapProp = {};
		HeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
		HeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		HeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

		// リソース設定
		D3D12_RESOURCE_DESC ResDesc = {};
		ResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		ResDesc.Width = sizeof(Vertices);
		ResDesc.Height = 1;
		ResDesc.DepthOrArraySize = 1;
		ResDesc.MipLevels = 1;
		ResDesc.Format = DXGI_FORMAT_UNKNOWN;
		ResDesc.SampleDesc.Count = 1;
		ResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		ResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// 作成
		if (!DirectX12_Wrapper::CreateBuffer(VertexBuffer.ReleaseAndGetAddressOf(), sizeof(Vertices)))
		{
			MessageBox(nullptr, "頂点バッファの作成に失敗しました。", "Error", MB_OK);
			return false;
		}

		Vertex* VertMap = nullptr;
		auto result = VertexBuffer->Map(0, nullptr, (void**)&VertMap);
		std::copy(std::begin(Vertices), std::end(Vertices), VertMap);
		VertexBuffer->Unmap(0, nullptr);

		// 頂点バッファビュー
		VerBuffView.BufferLocation = VertexBuffer->GetGPUVirtualAddress();
		VerBuffView.SizeInBytes = sizeof(Vertices);
		VerBuffView.StrideInBytes = sizeof(Vertices[0]);

		unsigned short Indices[] = { 0,1,2, 2,1,3 };

		ResDesc.Width = sizeof(Indices);

			// 作成
		if (!DirectX12_Wrapper::CreateBuffer(IndexBuffer.ReleaseAndGetAddressOf(), sizeof(Indices)))
		{
			MessageBox(nullptr, "インデックスバッファの作成に失敗しました。", "Error", MB_OK);
			return false;
		}

		// 作ったバッファにインデックスデータをコピー
		unsigned short* IndexMap = nullptr;
		IndexBuffer->Map(0, nullptr, (void**)&IndexMap);
		std::copy(std::begin(Indices), std::end(Indices), IndexMap);
		IndexBuffer->Unmap(0, nullptr);

		// インデックスバッファビューを作成
		IndBuffView.BufferLocation = IndexBuffer->GetGPUVirtualAddress();
		IndBuffView.Format = DXGI_FORMAT_R16_UINT;
		IndBuffView.SizeInBytes = sizeof(Indices);

		// 作成
		if (!DirectX12_Wrapper::CreateWriteBuffer(ConstBuffer.ReleaseAndGetAddressOf(), (sizeof(XMMATRIX) + 0xff) & ~0xff))
		{
			MessageBox(nullptr, "頂点バッファの作成に失敗しました。", "Error", MB_OK);
			return false;
		}
	}


	// シェーダー
	{
		// 頂点シェーダー作成
		if (FAILED(DirectX12_Wrapper:: CompileShader("Shader/VS.hlsl", "main", "vs_5_0", vsBlob.ReleaseAndGetAddressOf())))
		{
			MessageBox(nullptr, "頂点シェーダーの読み込みに失敗しました。", "Error", MB_OK);
			return false;
		}

		// ピクセルシェーダー作成
		if (FAILED(DirectX12_Wrapper::CompileShader("Shader/PS.hlsl", "main", "ps_5_0", psBlob.ReleaseAndGetAddressOf())))
		{
			MessageBox(nullptr, "ピクセルシェーダーの読み込みに失敗しました。", "Error", MB_OK);
			return false;
		}
	}

	CreatePipeline();

	// テクスチャ
	{
		// ディスクリプタヒープ設定
		D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc = {};
		DescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		DescHeapDesc.NodeMask = 0;
		DescHeapDesc.NumDescriptors = 2;
		DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

		// 作成
		if (FAILED(Device->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(BasicDescHeap.ReleaseAndGetAddressOf()))))
		{
			MessageBox(nullptr, "ディスクリプタヒープの作成に失敗しました。", "Error", MB_OK);
			return false;
		}
		DirectX12_Wrapper::CreateSRV(_FileName, TexBuffer.ReleaseAndGetAddressOf(), BasicDescHeap.Get());
		auto BasicHeapHandle = BasicDescHeap->GetCPUDescriptorHandleForHeapStart();
		BasicHeapHandle.ptr += Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = ConstBuffer->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = ConstBuffer->GetDesc().Width;

		// CBV作成
		Device->CreateConstantBufferView(&cbvDesc, BasicHeapHandle);
	}

	return true;
}

bool SPRITE::BackInit(const char* _FileName, XMFLOAT3 _Pos, float _Width, float _Height)
{
	auto Device = DIRECTX12::GetInstance()->GetDevice();
	auto CmdList = DIRECTX12::GetInstance()->GetCommandList();
	auto CmdQueue = DIRECTX12::GetInstance()->GetCommandQueue();
	auto CmdAllocator = DIRECTX12::GetInstance()->GetCommandAllocator();

	_Width / 2 - _Width, _Height / 2;
	// 頂点データ
	Vertex Vertices[] =
	{
		{{-_Width / 2 ,-_Height / 2,0.0f},{0.0f,1.0f}},
		{{-_Width / 2 ,_Height / 2,0.0f},{0.0f,0.0f}},
		{{_Width / 2 ,-_Height / 2,0.0f},{1.0f,1.0f}},
		{{_Width / 2 ,_Height / 2,0.0f},{1.0f,0.0f}},
	};

	// バッファ
	{
		// ヒープ設定
		D3D12_HEAP_PROPERTIES HeapProp = {};
		HeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
		HeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		HeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

		// リソース設定
		D3D12_RESOURCE_DESC ResDesc = {};
		ResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		ResDesc.Width = sizeof(Vertices);
		ResDesc.Height = 1;
		ResDesc.DepthOrArraySize = 1;
		ResDesc.MipLevels = 1;
		ResDesc.Format = DXGI_FORMAT_UNKNOWN;
		ResDesc.SampleDesc.Count = 1;
		ResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		ResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// 作成
		if (!DirectX12_Wrapper::CreateBuffer(VertexBuffer.ReleaseAndGetAddressOf(), sizeof(Vertices)))
		{
			MessageBox(nullptr, "頂点バッファの作成に失敗しました。", "Error", MB_OK);
			return false;
		}

		Vertex* VertMap = nullptr;
		auto result = VertexBuffer->Map(0, nullptr, (void**)&VertMap);
		std::copy(std::begin(Vertices), std::end(Vertices), VertMap);
		VertexBuffer->Unmap(0, nullptr);

		// 頂点バッファビュー
		VerBuffView.BufferLocation = VertexBuffer->GetGPUVirtualAddress();
		VerBuffView.SizeInBytes = sizeof(Vertices);
		VerBuffView.StrideInBytes = sizeof(Vertices[0]);

		unsigned short Indices[] = { 0,1,2, 2,1,3 };

		ResDesc.Width = sizeof(Indices);

		// 作成
		if (!DirectX12_Wrapper::CreateBuffer(IndexBuffer.ReleaseAndGetAddressOf(), sizeof(Indices)))
		{
			MessageBox(nullptr, "インデックスバッファの作成に失敗しました。", "Error", MB_OK);
			return false;
		}

		// 作ったバッファにインデックスデータをコピー
		unsigned short* IndexMap = nullptr;
		IndexBuffer->Map(0, nullptr, (void**)&IndexMap);
		std::copy(std::begin(Indices), std::end(Indices), IndexMap);
		IndexBuffer->Unmap(0, nullptr);

		// インデックスバッファビューを作成
		IndBuffView.BufferLocation = IndexBuffer->GetGPUVirtualAddress();
		IndBuffView.Format = DXGI_FORMAT_R16_UINT;
		IndBuffView.SizeInBytes = sizeof(Indices);

		// 作成
		if (!DirectX12_Wrapper::CreateWriteBuffer(ConstBuffer.ReleaseAndGetAddressOf(), (sizeof(XMMATRIX) + 0xff) & ~0xff))
		{
			MessageBox(nullptr, "頂点バッファの作成に失敗しました。", "Error", MB_OK);
			return false;
		}
	}


	// シェーダー
	{
		// 頂点シェーダー作成
		if (FAILED(DirectX12_Wrapper::CompileShader("Shader/VS.hlsl", "main", "vs_5_0", vsBlob.ReleaseAndGetAddressOf())))
		{
			MessageBox(nullptr, "頂点シェーダーの読み込みに失敗しました。", "Error", MB_OK);
			return false;
		}

		// ピクセルシェーダー作成
		if (FAILED(DirectX12_Wrapper::CompileShader("Shader/PSNoise.hlsl", "main", "ps_5_0", psBlob.ReleaseAndGetAddressOf())))
		{
			MessageBox(nullptr, "ピクセルシェーダーの読み込みに失敗しました。", "Error", MB_OK);
			return false;
		}
	}

	CreatePipeline();

	// テクスチャ
	{
		// ディスクリプタヒープ設定
		D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc = {};
		DescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		DescHeapDesc.NodeMask = 0;
		DescHeapDesc.NumDescriptors = 2;
		DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

		// 作成
		if (FAILED(Device->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(BasicDescHeap.ReleaseAndGetAddressOf()))))
		{
			MessageBox(nullptr, "ディスクリプタヒープの作成に失敗しました。", "Error", MB_OK);
			return false;
		}
		DirectX12_Wrapper::CreateSRV(_FileName, TexBuffer.ReleaseAndGetAddressOf(), BasicDescHeap.Get());
		auto BasicHeapHandle = BasicDescHeap->GetCPUDescriptorHandleForHeapStart();
		BasicHeapHandle.ptr += Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = ConstBuffer->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = ConstBuffer->GetDesc().Width;

		// CBV作成
		Device->CreateConstantBufferView(&cbvDesc, BasicHeapHandle);
	}

	return true;
}

void SPRITE::Update()
{
	MakeWorldMatrix(g_mtxworld, Angle, Pos);
	auto worldMat = XMLoadFloat4x4(&g_mtxworld);
	auto viewMat = XMLoadFloat4x4(&CAMERA::GetInstance()->GetViewMtx());
	auto projMat = XMLoadFloat4x4(&CAMERA::GetInstance()->GetProjMtx());

	XMMATRIX* mapMatrix;
	ConstBuffer->Map(0, nullptr, (void**)&mapMatrix);
	*mapMatrix = worldMat * viewMat * projMat;

}

// 描画
void SPRITE::Draw()
{
	//static float angle;
	//angle += 0.1f;
	//auto worldMat = XMMatrixRotationY(angle);
	//auto viewMat = XMLoadFloat4x4(&CAMERA::GetInstance()->GetViewMtx());
	//auto projMat = XMLoadFloat4x4(&CAMERA::GetInstance()->GetProjMtx());

	//XMMATRIX* mapMatrix;
	//ConstBuffer->Map(0, nullptr, (void**)&mapMatrix);
	//*mapMatrix = worldMat * viewMat * projMat;

	auto CmdList = DIRECTX12::GetInstance()->GetCommandList();
	auto Device = DIRECTX12::GetInstance()->GetDevice();

	CmdList->SetPipelineState(PipelineState.Get());														// パイプライン
	CmdList->SetGraphicsRootSignature(RootSignature.Get());												// ルートシグネチャ
	CmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);								// 形状
	CmdList->IASetVertexBuffers(0, 1, &VerBuffView);													// 頂点バッファ
	CmdList->IASetIndexBuffer(&IndBuffView);															// インデックスバッファ
	CmdList->SetGraphicsRootSignature(RootSignature.Get());												// ルートシグネチャ
	CmdList->SetDescriptorHeaps(1, BasicDescHeap.GetAddressOf());											// ディスクリプタヒープ

	CmdList->SetGraphicsRootDescriptorTable(0, BasicDescHeap->GetGPUDescriptorHandleForHeapStart());		// ディスクリプタテーブル

	CmdList->DrawIndexedInstanced(6, 1, 0, 0, 0);														// 描画
}

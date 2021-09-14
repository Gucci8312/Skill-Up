#include "classSPRITE.h"

bool SPRITE::CreatePipeline()
{
	auto Device = DIRECTX12::GetInstance()->GetDevice();
	auto CmdList = DIRECTX12::GetInstance()->GetCommandList();
	auto CmdQueue = DIRECTX12::GetInstance()->GetCommandQueue();
	auto CmdAllocator = DIRECTX12::GetInstance()->GetCommandAllocator();

	// �C���v�b�g���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC InputLayout[] =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
	};

	// �p�C�v���C��
	{
		// �ݒ�
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PipeLine = {};

		// �V�F�[�_�[
		PipeLine.VS.pShaderBytecode = vsBlob->GetBufferPointer();
		PipeLine.VS.BytecodeLength = vsBlob->GetBufferSize();
		PipeLine.PS.pShaderBytecode = psBlob->GetBufferPointer();
		PipeLine.PS.BytecodeLength = psBlob->GetBufferSize();

		PipeLine.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
		D3D12_RENDER_TARGET_BLEND_DESC RenderTargetBlendDesc = {};
		RenderTargetBlendDesc.BlendEnable = false;
		RenderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		RenderTargetBlendDesc.LogicOpEnable = false;

		// �u�����h
		PipeLine.BlendState.AlphaToCoverageEnable = true;
		PipeLine.BlendState.IndependentBlendEnable = false;
		PipeLine.BlendState.RenderTarget[0] = RenderTargetBlendDesc;

		// ���X�^���C�U�[
		PipeLine.RasterizerState.MultisampleEnable = false;
		PipeLine.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;	// �J�����O����
		PipeLine.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		PipeLine.RasterizerState.DepthClipEnable = true;			// �[�x�����̃N���b�s���O�L��
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

		PipeLine.InputLayout.pInputElementDescs = InputLayout;			// �擪�A�h���X
		PipeLine.InputLayout.NumElements = _countof(InputLayout);		// �z��

		PipeLine.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		PipeLine.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;	// �O�p�`

		PipeLine.NumRenderTargets = 1;
		PipeLine.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

		PipeLine.SampleDesc.Count = 1;
		PipeLine.SampleDesc.Quality = 0;

		// ���[�g�V�O�l�`���ݒ�
		D3D12_ROOT_SIGNATURE_DESC RootSignatureDesc = {};
		RootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		// �f�B�X�N���v�^�����W
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

		// ���[�g�p�����[�^
		D3D12_ROOT_PARAMETER RootParam = {};
		RootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		RootParam.DescriptorTable.pDescriptorRanges = &DescTblRange[0];				// �f�B�X�N���v�^�����W�̃A�h���X
		RootParam.DescriptorTable.NumDescriptorRanges = 2;							// �f�B�X�N���v�^�����W��
		RootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;					// �S�V�F�[�_����A�N�Z�X�\

		RootSignatureDesc.pParameters = &RootParam;									// ���[�g�p�����[�^�̐擪�A�h���X
		RootSignatureDesc.NumParameters = 1;										// ���[�g�p�����[�^��

		// �T���v���[
		D3D12_STATIC_SAMPLER_DESC SamplerDesc = {};
		SamplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;						// ���J��Ԃ�
		SamplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;						// �c�J��Ԃ�
		SamplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;						// ���s�J��Ԃ�
		SamplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		SamplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
		SamplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
		SamplerDesc.MinLOD = 0.0f;
		SamplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		SamplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;				// �s�N�Z���V�F�[�_����A�N�Z�X�\

		RootSignatureDesc.pStaticSamplers = &SamplerDesc;
		RootSignatureDesc.NumStaticSamplers = 1;

		ComPtr<ID3DBlob> rootSigBlob = nullptr;
		ComPtr<ID3DBlob> errorBlob = nullptr;

		// ���[�g�V�O�l�`���쐬
		if (FAILED(D3D12SerializeRootSignature(&RootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, rootSigBlob.GetAddressOf(), errorBlob.ReleaseAndGetAddressOf())))
		{
			MessageBox(nullptr, "���[�g�V�O�l�`���̃o�C�i���f�[�^�̍쐬�Ɏ��s���܂����B", "Error", MB_OK);
			return false;
		}

		// ���[�g�V�O�l�`���쐬
		if (FAILED(Device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(RootSignature.ReleaseAndGetAddressOf()))))
		{
			MessageBox(nullptr, "���[�g�V�O�l�`���̍쐬�Ɏ��s���܂����B", "Error", MB_OK);
			return false;
		}

		PipeLine.pRootSignature = RootSignature.Get();

		// �O���t�B�b�N�X�p�C�v���C���쐬
		if (FAILED(Device->CreateGraphicsPipelineState(&PipeLine, IID_PPV_ARGS(PipelineState.ReleaseAndGetAddressOf()))))
		{
			MessageBox(nullptr, "�O���t�B�b�N�X�p�C�v���C���̍쐬�Ɏ��s���܂����B", "Error", MB_OK);
			return false;
		}
	}

	return true;
}
// ������
bool SPRITE::Init(const char* _FileName, XMFLOAT3 _Pos, float _Width, float _Height)
{
	auto Device = DIRECTX12::GetInstance()->GetDevice();
	auto CmdList = DIRECTX12::GetInstance()->GetCommandList();
	auto CmdQueue = DIRECTX12::GetInstance()->GetCommandQueue();
	auto CmdAllocator = DIRECTX12::GetInstance()->GetCommandAllocator();

	_Width / 2 - _Width, _Height / 2;
	// ���_�f�[�^
	Vertex Vertices[] =
	{
		{{-_Width / 2 ,-_Height / 2,0.0f},{0.0f,1.0f}},
		{{-_Width / 2 ,_Height / 2,0.0f},{0.0f,0.0f}},
		{{_Width / 2 ,-_Height / 2,0.0f},{1.0f,1.0f}},
		{{_Width / 2 ,_Height / 2,0.0f},{1.0f,0.0f}},
	};

	// �o�b�t�@
	{
		// �q�[�v�ݒ�
		D3D12_HEAP_PROPERTIES HeapProp = {};
		HeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
		HeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		HeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

		// ���\�[�X�ݒ�
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

		// �쐬
		if (!DirectX12_Wrapper::CreateBuffer(VertexBuffer.ReleaseAndGetAddressOf(), sizeof(Vertices)))
		{
			MessageBox(nullptr, "���_�o�b�t�@�̍쐬�Ɏ��s���܂����B", "Error", MB_OK);
			return false;
		}

		Vertex* VertMap = nullptr;
		auto result = VertexBuffer->Map(0, nullptr, (void**)&VertMap);
		std::copy(std::begin(Vertices), std::end(Vertices), VertMap);
		VertexBuffer->Unmap(0, nullptr);

		// ���_�o�b�t�@�r���[
		VerBuffView.BufferLocation = VertexBuffer->GetGPUVirtualAddress();
		VerBuffView.SizeInBytes = sizeof(Vertices);
		VerBuffView.StrideInBytes = sizeof(Vertices[0]);

		unsigned short Indices[] = { 0,1,2, 2,1,3 };

		ResDesc.Width = sizeof(Indices);

			// �쐬
		if (!DirectX12_Wrapper::CreateBuffer(IndexBuffer.ReleaseAndGetAddressOf(), sizeof(Indices)))
		{
			MessageBox(nullptr, "�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s���܂����B", "Error", MB_OK);
			return false;
		}

		// ������o�b�t�@�ɃC���f�b�N�X�f�[�^���R�s�[
		unsigned short* IndexMap = nullptr;
		IndexBuffer->Map(0, nullptr, (void**)&IndexMap);
		std::copy(std::begin(Indices), std::end(Indices), IndexMap);
		IndexBuffer->Unmap(0, nullptr);

		// �C���f�b�N�X�o�b�t�@�r���[���쐬
		IndBuffView.BufferLocation = IndexBuffer->GetGPUVirtualAddress();
		IndBuffView.Format = DXGI_FORMAT_R16_UINT;
		IndBuffView.SizeInBytes = sizeof(Indices);

		// �쐬
		if (!DirectX12_Wrapper::CreateWriteBuffer(ConstBuffer.ReleaseAndGetAddressOf(), (sizeof(XMMATRIX) + 0xff) & ~0xff))
		{
			MessageBox(nullptr, "���_�o�b�t�@�̍쐬�Ɏ��s���܂����B", "Error", MB_OK);
			return false;
		}
	}


	// �V�F�[�_�[
	{
		// ���_�V�F�[�_�[�쐬
		if (FAILED(DirectX12_Wrapper:: CompileShader("Shader/VS.hlsl", "main", "vs_5_0", vsBlob.ReleaseAndGetAddressOf())))
		{
			MessageBox(nullptr, "���_�V�F�[�_�[�̓ǂݍ��݂Ɏ��s���܂����B", "Error", MB_OK);
			return false;
		}

		// �s�N�Z���V�F�[�_�[�쐬
		if (FAILED(DirectX12_Wrapper::CompileShader("Shader/PS.hlsl", "main", "ps_5_0", psBlob.ReleaseAndGetAddressOf())))
		{
			MessageBox(nullptr, "�s�N�Z���V�F�[�_�[�̓ǂݍ��݂Ɏ��s���܂����B", "Error", MB_OK);
			return false;
		}
	}

	CreatePipeline();

	// �e�N�X�`��
	{
		// �f�B�X�N���v�^�q�[�v�ݒ�
		D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc = {};
		DescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		DescHeapDesc.NodeMask = 0;
		DescHeapDesc.NumDescriptors = 2;
		DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

		// �쐬
		if (FAILED(Device->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(BasicDescHeap.ReleaseAndGetAddressOf()))))
		{
			MessageBox(nullptr, "�f�B�X�N���v�^�q�[�v�̍쐬�Ɏ��s���܂����B", "Error", MB_OK);
			return false;
		}
		DirectX12_Wrapper::CreateSRV(_FileName, TexBuffer.ReleaseAndGetAddressOf(), BasicDescHeap.Get());
		auto BasicHeapHandle = BasicDescHeap->GetCPUDescriptorHandleForHeapStart();
		BasicHeapHandle.ptr += Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = ConstBuffer->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = ConstBuffer->GetDesc().Width;

		// CBV�쐬
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
	// ���_�f�[�^
	Vertex Vertices[] =
	{
		{{-_Width / 2 ,-_Height / 2,0.0f},{0.0f,1.0f}},
		{{-_Width / 2 ,_Height / 2,0.0f},{0.0f,0.0f}},
		{{_Width / 2 ,-_Height / 2,0.0f},{1.0f,1.0f}},
		{{_Width / 2 ,_Height / 2,0.0f},{1.0f,0.0f}},
	};

	// �o�b�t�@
	{
		// �q�[�v�ݒ�
		D3D12_HEAP_PROPERTIES HeapProp = {};
		HeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
		HeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		HeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

		// ���\�[�X�ݒ�
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

		// �쐬
		if (!DirectX12_Wrapper::CreateBuffer(VertexBuffer.ReleaseAndGetAddressOf(), sizeof(Vertices)))
		{
			MessageBox(nullptr, "���_�o�b�t�@�̍쐬�Ɏ��s���܂����B", "Error", MB_OK);
			return false;
		}

		Vertex* VertMap = nullptr;
		auto result = VertexBuffer->Map(0, nullptr, (void**)&VertMap);
		std::copy(std::begin(Vertices), std::end(Vertices), VertMap);
		VertexBuffer->Unmap(0, nullptr);

		// ���_�o�b�t�@�r���[
		VerBuffView.BufferLocation = VertexBuffer->GetGPUVirtualAddress();
		VerBuffView.SizeInBytes = sizeof(Vertices);
		VerBuffView.StrideInBytes = sizeof(Vertices[0]);

		unsigned short Indices[] = { 0,1,2, 2,1,3 };

		ResDesc.Width = sizeof(Indices);

		// �쐬
		if (!DirectX12_Wrapper::CreateBuffer(IndexBuffer.ReleaseAndGetAddressOf(), sizeof(Indices)))
		{
			MessageBox(nullptr, "�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s���܂����B", "Error", MB_OK);
			return false;
		}

		// ������o�b�t�@�ɃC���f�b�N�X�f�[�^���R�s�[
		unsigned short* IndexMap = nullptr;
		IndexBuffer->Map(0, nullptr, (void**)&IndexMap);
		std::copy(std::begin(Indices), std::end(Indices), IndexMap);
		IndexBuffer->Unmap(0, nullptr);

		// �C���f�b�N�X�o�b�t�@�r���[���쐬
		IndBuffView.BufferLocation = IndexBuffer->GetGPUVirtualAddress();
		IndBuffView.Format = DXGI_FORMAT_R16_UINT;
		IndBuffView.SizeInBytes = sizeof(Indices);

		// �쐬
		if (!DirectX12_Wrapper::CreateWriteBuffer(ConstBuffer.ReleaseAndGetAddressOf(), (sizeof(XMMATRIX) + 0xff) & ~0xff))
		{
			MessageBox(nullptr, "���_�o�b�t�@�̍쐬�Ɏ��s���܂����B", "Error", MB_OK);
			return false;
		}
	}


	// �V�F�[�_�[
	{
		// ���_�V�F�[�_�[�쐬
		if (FAILED(DirectX12_Wrapper::CompileShader("Shader/VS.hlsl", "main", "vs_5_0", vsBlob.ReleaseAndGetAddressOf())))
		{
			MessageBox(nullptr, "���_�V�F�[�_�[�̓ǂݍ��݂Ɏ��s���܂����B", "Error", MB_OK);
			return false;
		}

		// �s�N�Z���V�F�[�_�[�쐬
		if (FAILED(DirectX12_Wrapper::CompileShader("Shader/PSNoise.hlsl", "main", "ps_5_0", psBlob.ReleaseAndGetAddressOf())))
		{
			MessageBox(nullptr, "�s�N�Z���V�F�[�_�[�̓ǂݍ��݂Ɏ��s���܂����B", "Error", MB_OK);
			return false;
		}
	}

	CreatePipeline();

	// �e�N�X�`��
	{
		// �f�B�X�N���v�^�q�[�v�ݒ�
		D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc = {};
		DescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		DescHeapDesc.NodeMask = 0;
		DescHeapDesc.NumDescriptors = 2;
		DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

		// �쐬
		if (FAILED(Device->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(BasicDescHeap.ReleaseAndGetAddressOf()))))
		{
			MessageBox(nullptr, "�f�B�X�N���v�^�q�[�v�̍쐬�Ɏ��s���܂����B", "Error", MB_OK);
			return false;
		}
		DirectX12_Wrapper::CreateSRV(_FileName, TexBuffer.ReleaseAndGetAddressOf(), BasicDescHeap.Get());
		auto BasicHeapHandle = BasicDescHeap->GetCPUDescriptorHandleForHeapStart();
		BasicHeapHandle.ptr += Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = ConstBuffer->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = ConstBuffer->GetDesc().Width;

		// CBV�쐬
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

// �`��
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

	CmdList->SetPipelineState(PipelineState.Get());														// �p�C�v���C��
	CmdList->SetGraphicsRootSignature(RootSignature.Get());												// ���[�g�V�O�l�`��
	CmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);								// �`��
	CmdList->IASetVertexBuffers(0, 1, &VerBuffView);													// ���_�o�b�t�@
	CmdList->IASetIndexBuffer(&IndBuffView);															// �C���f�b�N�X�o�b�t�@
	CmdList->SetGraphicsRootSignature(RootSignature.Get());												// ���[�g�V�O�l�`��
	CmdList->SetDescriptorHeaps(1, BasicDescHeap.GetAddressOf());											// �f�B�X�N���v�^�q�[�v

	CmdList->SetGraphicsRootDescriptorTable(0, BasicDescHeap->GetGPUDescriptorHandleForHeapStart());		// �f�B�X�N���v�^�e�[�u��

	CmdList->DrawIndexedInstanced(6, 1, 0, 0, 0);														// �`��
}

#include "DirectX12_Wrapper.h"

// 確認用配列
std::vector<float>test(256, 0);

//--------------------------------------------------------------------------------------
// ファイル名を取得する
//--------------------------------------------------------------------------------------
std::string ExtractFileName(std::string fullpath, char split)
{
	unsigned int path_i = static_cast<unsigned int>(fullpath.find_last_of(split) + 1);//7
	unsigned int ext_i = static_cast<unsigned int>(fullpath.find_last_of("."));//10
	std::string pathname = fullpath.substr(0, path_i + 1);//0文字目から７文字切り出す "C:\\aaa\\"
	std::string extname = fullpath.substr(ext_i, fullpath.size() - ext_i); // 10文字目から４文字切り出す ".txt"
	std::string filename = fullpath.substr(path_i, ext_i - path_i);//

	return filename + extname;
}

//--------------------------------------------------------------------------------------
// ファイルの拡張子を取得する
//--------------------------------------------------------------------------------------
std::string GetFileExt(const char* filename) {
	std::string extname;

	std::string fullpathstr(filename);
	size_t ext_i = fullpathstr.find_last_of(".");
	extname = fullpathstr.substr(ext_i + 1, fullpathstr.size() - ext_i);
	return extname;
}

//--------------------------------------------------------------------------------------
// コンパイル済みシェーダーファイルを読み込む
//--------------------------------------------------------------------------------------
bool readShader(const char* csoName, std::vector<unsigned char>& byteArray)
{
	FILE* fp;
	int ret = fopen_s(&fp, csoName, "rb");
	if (ret != 0) {
		return false;
	}
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	byteArray.resize(size);
	fseek(fp, 0, SEEK_SET);

	fread(byteArray.data(), byteArray.size(), 1, fp);
	fclose(fp);

	return true;
}


//--------------------------------------------------------------------------------------
// シェーダーをコンパイル
//--------------------------------------------------------------------------------------
HRESULT CompileShaderFromFile(const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	ID3DBlob* p1 = nullptr;

	HRESULT hr = S_OK;

	WCHAR	filename[512];
	size_t 	wLen = 0;
	int err = 0;

	// char -> wcharに変換
	setlocale(LC_ALL, "japanese");
	err = mbstowcs_s(&wLen, filename, 512, szFileName, _TRUNCATE);

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile(
		filename,							// filename LPCWST pFileName
		nullptr,							// D3D_SHADER_MACRO *pDefines
		D3D_COMPILE_STANDARD_FILE_INCLUDE,	// ID3DInclude *pInclude
		szEntryPoint,						// LPCSTR pEntrypoint
		szShaderModel,						// LPCSTR pTarget
		dwShaderFlags,						// UINT Flags1
		0,									// UINT Flags2
		ppBlobOut,							// ID3DBlob** ppCode
		&pErrorBlob);						// ID3DBlob** ppErrorMsg 
	if (FAILED(hr))
	{
		if (pErrorBlob != nullptr) {
			MessageBox(NULL,
				(char*)pErrorBlob->GetBufferPointer(), "Error", MB_OK);
		}
		if (pErrorBlob) pErrorBlob->Release();
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}

//--------------------------------------------------------------------------------------
// シェーダーをファイル拡張子に合わせてコンパイル
//--------------------------------------------------------------------------------------
HRESULT DirectX12_Wrapper::CompileShader(const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut) {

	HRESULT hr;
	static std::vector<unsigned char> byteArray;
	*ppBlobOut = nullptr;

	std::string extname = GetFileExt(szFileName);
	if (extname == "cso") {
		bool sts = readShader(szFileName, byteArray);
		if (!sts) {
			FILE* fp;
			fopen_s(&fp, "debug.txt", "a");
			fprintf(fp, "file open error \n");
			fclose(fp);
			return E_FAIL;
		}
	}
	else {
		hr = CompileShaderFromFile(szFileName, szEntryPoint, szShaderModel, ppBlobOut);
		if (FAILED(hr)) {
			if (*ppBlobOut)(*ppBlobOut)->Release();
			return E_FAIL;
		}
	}

	return S_OK;
}

bool DirectX12_Wrapper::CreateSRV(const char* _FileName, ID3D12Resource** _TexBuffer, ID3D12DescriptorHeap* _TexDescriptorHeap)
{
	auto Device = DIRECTX12::GetInstance()->GetDevice();
	auto CmdList = DIRECTX12::GetInstance()->GetCommandList();
	auto CmdQueue = DIRECTX12::GetInstance()->GetCommandQueue();
	auto CmdAllocator = DIRECTX12::GetInstance()->GetCommandAllocator();

	// 読み込み
	TexMetadata metadata = {};
	ScratchImage scratchImg = {};
	wchar_t ws[512];
	size_t ret;

	setlocale(LC_CTYPE, "jpn");
	mbstowcs_s(&ret, ws, 512, _FileName, _TRUNCATE);

	auto result = LoadFromWICFile(ws, WIC_FLAGS_NONE, &metadata, scratchImg);
	auto img = scratchImg.GetImage(0, 0, 0);							// 生データ抽出

	// 中間バッファのヒープ設定
	D3D12_HEAP_PROPERTIES UploadHeapProp = {};
	UploadHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	UploadHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	UploadHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	UploadHeapProp.CreationNodeMask = 0;
	UploadHeapProp.VisibleNodeMask = 0;

	// リソース設定
	D3D12_RESOURCE_DESC ResDesc = {};

	ResDesc.Format = DXGI_FORMAT_UNKNOWN;
	ResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	ResDesc.Width = AlignmentedSize(img->rowPitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT) * img->height;
	ResDesc.Height = 1;
	ResDesc.DepthOrArraySize = 1;
	ResDesc.MipLevels = 1;
	ResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	ResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	ResDesc.SampleDesc.Count = 1;
	ResDesc.SampleDesc.Quality = 0;

	//// 中間バッファ作成
	ComPtr<ID3D12Resource> UpLoadBuff = nullptr;

	// 作成
	if (!DirectX12_Wrapper::CreateWriteBuffer(UpLoadBuff.ReleaseAndGetAddressOf(), AlignmentedSize(img->rowPitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT) * img->height))
	{
		MessageBox(nullptr, "中間バッファの作成に失敗しました。", "Error", MB_OK);
		return false;
	}

	// テクスチャのためのヒープ設定
	D3D12_HEAP_PROPERTIES texHeapProp = {};
	texHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	texHeapProp.CreationNodeMask = 0;
	texHeapProp.VisibleNodeMask = 0;

	// リソース設定
	ResDesc.Format = metadata.format;
	ResDesc.Width = static_cast<UINT>(metadata.width);						// 横幅
	ResDesc.Height = static_cast<UINT>(metadata.height);					// 高さ
	ResDesc.DepthOrArraySize = static_cast<UINT16>(metadata.arraySize);
	ResDesc.MipLevels = static_cast<UINT16>(metadata.mipLevels);
	ResDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);
	ResDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

	// テクスチャバッファ作成
	if (FAILED(Device->CreateCommittedResource(&texHeapProp, D3D12_HEAP_FLAG_NONE, &ResDesc,
		D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(_TexBuffer))))
	{
		MessageBox(nullptr, "テクスチャバッファの作成に失敗しました。", "Error", MB_OK);
		return false;
	}

	uint8_t* mapforImg = nullptr;
	result = UpLoadBuff->Map(0, nullptr, (void**)&mapforImg);
	auto srcAddress = img->pixels;
	auto rowPitch = AlignmentedSize(img->rowPitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT);
	for (int y = 0; y < img->height; ++y)
	{
		std::copy_n(srcAddress, rowPitch, mapforImg);
		srcAddress += img->rowPitch;
		mapforImg += rowPitch;
	}
	UpLoadBuff->Unmap(0, nullptr);

	D3D12_TEXTURE_COPY_LOCATION src = {}, dst = {};
	dst.pResource = (*_TexBuffer);
	dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	dst.SubresourceIndex = 0;

	src.pResource = UpLoadBuff.Get();						// 中間バッファ
	src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;	// フットプリント
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint = {};
	UINT nrow;
	UINT64 rowsize, size;
	auto desc = (*_TexBuffer)->GetDesc();
	Device->GetCopyableFootprints(&desc, 0, 1, 0, &footprint, &nrow, &rowsize, &size);
	src.PlacedFootprint = footprint;
	src.PlacedFootprint.Offset = 0;
	src.PlacedFootprint.Footprint.Width = static_cast<UINT>(metadata.width);
	src.PlacedFootprint.Footprint.Height = static_cast<UINT>(metadata.height);
	src.PlacedFootprint.Footprint.Depth = static_cast<UINT>(metadata.depth);
	src.PlacedFootprint.Footprint.RowPitch = static_cast<UINT>(AlignmentedSize(img->rowPitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT));
	src.PlacedFootprint.Footprint.Format = img->format;

	CmdList->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);

	DIRECTX12::GetInstance()->BarrierResouce(*_TexBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	CmdList->Close();

	// コマンドリストの実行
	ID3D12CommandList* cmdlists[] = { CmdList };
	CmdQueue->ExecuteCommandLists(1, cmdlists);

	// 待ち
	DIRECTX12::GetInstance()->FenceWait();

	CmdAllocator->Reset();
	CmdList->Reset(CmdAllocator, nullptr);

	// ディスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc = {};
	DescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	DescHeapDesc.NodeMask = 0;
	DescHeapDesc.NumDescriptors = 2;
	DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	// 通常テクスチャビュー作成
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	// SRV作成
	Device->CreateShaderResourceView(*_TexBuffer, &srvDesc, _TexDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	return true;
}

bool DirectX12_Wrapper::CreateBuffer(ID3D12Resource** _Buffer, UINT64 _Size)
{
	auto Device = DIRECTX12::GetInstance()->GetDevice();

	// ヒープ設定
	D3D12_HEAP_PROPERTIES HeapProp = {};
	HeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	HeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	HeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	// リソース設定
	D3D12_RESOURCE_DESC ResDesc = {};
	ResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	ResDesc.Width = _Size;
	ResDesc.Height = 1;
	ResDesc.DepthOrArraySize = 1;
	ResDesc.MipLevels = 1;
	ResDesc.Format = DXGI_FORMAT_UNKNOWN;
	ResDesc.SampleDesc.Count = 1;
	ResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	ResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 作成
	if (FAILED(Device->CreateCommittedResource(&HeapProp, D3D12_HEAP_FLAG_NONE, &ResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(_Buffer))))
	{
		//MessageBox(nullptr, "頂点バッファの作成に失敗しました。", "Error", MB_OK);
		return false;
	}
	return true;
}

// 書き込み用バッファ作成
bool DirectX12_Wrapper::CreateWriteBuffer(ID3D12Resource** _Buffer, UINT64 _Size)
{
	auto Device = DIRECTX12::GetInstance()->GetDevice();

	// ヒープ設定
	D3D12_HEAP_PROPERTIES HeapProp = {};
	HeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	HeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	HeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	// リソース設定
	D3D12_RESOURCE_DESC ResDesc = {};
	ResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	ResDesc.Width = _Size;
	ResDesc.Height = 1;
	ResDesc.DepthOrArraySize = 1;
	ResDesc.MipLevels = 1;
	ResDesc.Format = DXGI_FORMAT_UNKNOWN;
	ResDesc.SampleDesc.Count = 1;
	ResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	ResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 作成
	if (FAILED(Device->CreateCommittedResource(&HeapProp, D3D12_HEAP_FLAG_NONE, &ResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(_Buffer))))
	{
		//MessageBox(nullptr, "定数バッファの作成に失敗しました。", "Error", MB_OK);
		return false;
	}

	return true;
}

// フェンス待ち
void DirectX12_Wrapper::FenceWait()
{
	auto CmdQueue = DIRECTX12::GetInstance()->GetCommandQueue();
	auto Fence = DIRECTX12::GetInstance()->GetFence();

	UINT64								FenceVal = 0;
	CmdQueue->Signal(Fence, ++FenceVal);

	// GPUの処理が終わっていない時
	if (Fence->GetCompletedValue() != FenceVal)
	{
		auto event = CreateEvent(nullptr, false, false, nullptr);			// イベントハンドル取得
		Fence->SetEventOnCompletion(FenceVal, event);					// イベントセット
		WaitForSingleObject(event, INFINITE);							// イベントが発生するまで待つ
		CloseHandle(event);												// イベント終了
	}
}

// バリア設定
void DirectX12_Wrapper::BarrierResouce(ID3D12Resource* _Resouce, D3D12_RESOURCE_STATES _BeforeState, D3D12_RESOURCE_STATES _AfterState)
{
	auto CmdList = DIRECTX12::GetInstance()->GetCommandList();

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

// コンピュートシェーダー初期化
bool DirectX12_Wrapper::COMPUTESHADER::Init(const char* _FileName)
{
	auto Device = DIRECTX12::GetInstance()->GetDevice();

	// コンピュートシェーダー作成
	if (FAILED(DirectX12_Wrapper::CompileShader(_FileName, "main", "cs_5_0", ComputeShader.ReleaseAndGetAddressOf())))
	{
		MessageBox(nullptr, "コンピュートシェーダーの読み込みに失敗しました。", "Error", MB_OK);
		return false;
	}

	auto memPropWB = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
	auto memPropWC = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_COMBINE,D3D12_MEMORY_POOL_L0);

	const UINT width = 1280, height = 720;
	auto texDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM, width, height);
	texDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;


	Device->CreateCommittedResource(&memPropWC,D3D12_HEAP_FLAG_NONE,
		&texDesc,D3D12_RESOURCE_STATE_UNORDERED_ACCESS,nullptr,IID_PPV_ARGS(UAVBuffer.ReleaseAndGetAddressOf()));
	
	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
	uavDesc.Format = texDesc.Format;
	uavDesc.Texture2D.MipSlice = 0;
	uavDesc.Texture2D.PlaneSlice = 0;

	// UAV使用時
	D3D12_DESCRIPTOR_RANGE Range{};
	Range.NumDescriptors = 1;
	Range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
	std::array<CD3DX12_ROOT_PARAMETER, 1> RootParams;
	RootParams[0].InitAsDescriptorTable(1,&Range);

	// RWStructuredBuffer使用時
	RootParams[0].InitAsUnorderedAccessView(0);
	
	CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc{};
	RootSignatureDesc.Init(UINT(RootParams.size()),RootParams.data(),0,
		nullptr,D3D12_ROOT_SIGNATURE_FLAG_NONE);

	ComPtr<ID3DBlob> Signature, ErrBlob;
	D3D12SerializeRootSignature(
		&RootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, Signature.ReleaseAndGetAddressOf(), ErrBlob.ReleaseAndGetAddressOf());
	
	Device->CreateRootSignature(0, Signature->GetBufferPointer(), Signature->GetBufferSize(),
		IID_PPV_ARGS(RootSignature.ReleaseAndGetAddressOf()));

	// Compute用パイプラインステート
	D3D12_COMPUTE_PIPELINE_STATE_DESC psoDesc{};
	psoDesc.CS = CD3DX12_SHADER_BYTECODE(ComputeShader.Get());
	psoDesc.pRootSignature = RootSignature.Get();
	psoDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	psoDesc.NodeMask = 0;
	Device->CreateComputePipelineState(&psoDesc,IID_PPV_ARGS(CSPipelineState.ReleaseAndGetAddressOf()));

	return true;
}

// コンピュートシェーダー実行
void DirectX12_Wrapper::COMPUTESHADER::Run()
{
	CSCmdAllocator->Reset();
	CSCmdList->Reset(CSCmdAllocator.Get(), nullptr);

	CSCmdList->SetComputeRootSignature(RootSignature.Get());
	CSCmdList->SetPipelineState(CSPipelineState.Get());


	CSCmdList->Dispatch(test.size(), 1, 1);
	CSCmdList->Close();

	ID3D12CommandList* cmdlists[] = { CSCmdList.Get() };
	DIRECTX12::GetInstance()->GetCommandQueue()->ExecuteCommandLists(1, cmdlists);
	FenceWait();
}

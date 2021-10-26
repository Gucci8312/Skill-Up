#include "ImGuiWrapper.h"
using namespace ImGuiWrapper;
ImGuiUtil* ImGuiUtil::Instance = nullptr;

// ����
void ImGuiUtil::Create()
{
	if (!Instance)
	{
		Instance = new ImGuiUtil;
	}
}

// �폜
void ImGuiUtil::Destroy()
{
	delete Instance;
	Instance = nullptr;
}

bool ImGuiUtil::Init(HWND _hWnd, ID3D12Device* _Device)
{
	ImGuiFlg = true;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	io.IniFilename = NULL;


	D3D12_DESCRIPTOR_HEAP_DESC Desc = {};
	Desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	Desc.NodeMask = 0;
	Desc.NumDescriptors = 1;
	Desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	// �쐬
	if (FAILED(_Device->CreateDescriptorHeap(&Desc, IID_PPV_ARGS(DescriptorHeapForImgui.ReleaseAndGetAddressOf()))))
	{
		MessageBox(nullptr, "ImGui�p�f�B�X�N���v�^�q�[�v�̍쐬�Ɏ��s���܂����B", "Error", MB_OK);
		return false;
	}

	if (ImGui::CreateContext() == nullptr)
	{
		MessageBox(nullptr, "ImGui�̏������Ɏ��s���܂����B", "Error", MB_OK);
		return false;
	}

	if (!ImGui_ImplWin32_Init(_hWnd))
	{
		MessageBox(nullptr, "ImGui�̏������Ɏ��s���܂����B", "Error", MB_OK);
		return false;
	}

	if (!ImGui_ImplDX12_Init(_Device, 3, DXGI_FORMAT_R8G8B8A8_UNORM, DescriptorHeapForImgui.Get(),
		DescriptorHeapForImgui->GetCPUDescriptorHandleForHeapStart(), DescriptorHeapForImgui->GetGPUDescriptorHandleForHeapStart()))
	{
		MessageBox(nullptr, "ImGui�̏������Ɏ��s���܂����B", "Error", MB_OK);
		return false;
	}

	// ���{�ꉻ
	io.Fonts->AddFontFromFileTTF("assets\\Font\\UDDigiKyokashoN-R.ttc", 14.0f, nullptr, glyphRangesJapanese);

	return true;
}

// �`��O����
void ImGuiUtil::BeforRender()

{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

// �`��㏈��
void ImGuiUtil::AfterRender(ID3D12GraphicsCommandList* _CmdList)
{
	ImGui::Render();
	_CmdList->SetDescriptorHeaps(1, DescriptorHeapForImgui.GetAddressOf());
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), _CmdList);
}

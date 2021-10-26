#include "EFFECTTOOL.h"

bool EFFECTTOOL::Init(HWND _hWnd)
{
	GuiInit(_hWnd);
	Particle = new QUAD;
	Particle->Init("Assets/Texture/Fire.png", XMFLOAT3(0.0f, 0.0f, 0.0f), 10.0f, 10.0f);

	return true;
}

void EFFECTTOOL::Update()
{
	Particle->Update();
}

// �`��
void EFFECTTOOL::Draw()
{

	Particle->Draw();
	GuiDraw();

}

// �I������
void EFFECTTOOL::UnInit()
{
	ImGui_ImplDX12_Shutdown();
	ImGui::DestroyContext();
	delete Particle;
	Particle = nullptr;
}

bool EFFECTTOOL::GuiInit(HWND _hWnd)
{
	ImGuiFlg = true;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	io.IniFilename = NULL;

	auto Device = DIRECTX12::GetInstance()->GetDevice();

	D3D12_DESCRIPTOR_HEAP_DESC Desc = {};
	Desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	Desc.NodeMask = 0;
	Desc.NumDescriptors = 1;
	Desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	// �쐬
	if (FAILED(Device->CreateDescriptorHeap(&Desc, IID_PPV_ARGS(DescriptorHeapForImgui.ReleaseAndGetAddressOf()))))
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

	if (!ImGui_ImplDX12_Init(Device, 3, DXGI_FORMAT_R8G8B8A8_UNORM, DescriptorHeapForImgui.Get(),
		DescriptorHeapForImgui->GetCPUDescriptorHandleForHeapStart(), DescriptorHeapForImgui->GetGPUDescriptorHandleForHeapStart()))
	{
		MessageBox(nullptr, "ImGui�̏������Ɏ��s���܂����B", "Error", MB_OK);
		return false;
	}

	// ���{�ꉻ
	io.Fonts->AddFontFromFileTTF("assets\\font\\UDDigiKyokashoN-R.ttc", 14.0f, nullptr, glyphRangesJapanese);

	return true;
}

void EFFECTTOOL::GuiDraw()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_Always);		// �ʒu�w��
	ImGui::SetNextWindowSize(ImVec2(400, 880), ImGuiCond_Always);	// �T�C�Y�w��

	if (ImGui::Begin(u8"�ݒ�", &ImGuiFlg))
	{
		ImGui::End();
	}

	 //if (ImGui::BeginMenuBar())
	// {
		  //if (ImGui::BeginMenu(u8"�V�K�쐬"))
		  //{
	//          // �G�~�b�^�[�ݒ�
	//          if (ImGui::TreeNode(u8"�G�~�b�^�["))
	//          {
	//              // �g�U
	//              if (ImGui::Button(u8"�g�U"))
	//              {
	//                 // Effect[EFFECT::GetCreateIdx()].SetEmitterType(TYPE_SPREAD);
	//              }

	//              // �~
	//              if (ImGui::Button(u8"�~"))
	//              {
	//                  //Effect[EFFECT::GetCreateIdx()].SetEmitterType(TYPE_CIRCLE);
	//              }

	//              // ����
	//              if (ImGui::Button(u8"�o�[�X�g"))
	//              {
	//                 // Effect[EFFECT::GetCreateIdx()].SetEmitterType(TYPE_BURST);
	//              }

	//              // ����
	//              if (ImGui::Button(u8"����"))
	//              {
	//                  //Effect[EFFECT::GetCreateIdx()].SetEmitterType(TYPE_TORNADO);
	//              }
	//              ImGui::TreePop();
	//              ImGui::Separator();
	//          }
		  //	ImGui::EndMenu();
		  //}
	     //ImGui::EndMainMenuBar();
	 //}

	  // �t���[�����[�g�`��
	ImGui::SetNextWindowPos(ImVec2(1025, 20), ImGuiCond_Always);	// �ʒu�w��
	ImGui::SetNextWindowSize(ImVec2(350, 50), ImGuiCond_Always);	// �T�C�Y�w��

	// �t���[�����[�g
	if (ImGui::Begin(u8"�t���[�����[�g", &ImGuiFlg))
	{
		// �t���[�����[�g��\��
		ImGui::Text(u8"%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		//// �J�����^�C�v��\��
		//ImGui::Text(u8"�J�����^�C�v   :");
		//ImGui::SameLine();
		//ImGui::Text(CCamera::GetInstance()->GetCameraTypeName());

		ImGui::End();
	}

	ImGui::SetNextWindowPos(ImVec2(1025, 825), ImGuiCond_Always);	// �ʒu�w��
	ImGui::SetNextWindowSize(ImVec2(350, 50), ImGuiCond_Always);	// �T�C�Y�w��

	// �J�����^�C�v
	if (ImGui::Begin(u8"�J�����^�C�v", &ImGuiFlg))
	{
		// �J�����^�C�v��\��
		//ImGui::Text(u8"�J�����^�C�v   :");
		//ImGui::SameLine();
		ImGui::Text(CAMERA::GetInstance()->GetCameraTypeName());

		ImGui::End();
	}

	ImGui::Render();

	auto CmdList = DIRECTX12::GetInstance()->GetCommandList();
	CmdList->SetDescriptorHeaps(1, DescriptorHeapForImgui.GetAddressOf());
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), CmdList);

}

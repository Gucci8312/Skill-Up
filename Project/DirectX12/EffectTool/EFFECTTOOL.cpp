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

// 描画
void EFFECTTOOL::Draw()
{

	Particle->Draw();
	GuiDraw();

}

// 終了処理
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

	// 作成
	if (FAILED(Device->CreateDescriptorHeap(&Desc, IID_PPV_ARGS(DescriptorHeapForImgui.ReleaseAndGetAddressOf()))))
	{
		MessageBox(nullptr, "ImGui用ディスクリプタヒープの作成に失敗しました。", "Error", MB_OK);
		return false;
	}

	if (ImGui::CreateContext() == nullptr)
	{
		MessageBox(nullptr, "ImGuiの初期化に失敗しました。", "Error", MB_OK);
		return false;
	}

	if (!ImGui_ImplWin32_Init(_hWnd))
	{
		MessageBox(nullptr, "ImGuiの初期化に失敗しました。", "Error", MB_OK);
		return false;
	}

	if (!ImGui_ImplDX12_Init(Device, 3, DXGI_FORMAT_R8G8B8A8_UNORM, DescriptorHeapForImgui.Get(),
		DescriptorHeapForImgui->GetCPUDescriptorHandleForHeapStart(), DescriptorHeapForImgui->GetGPUDescriptorHandleForHeapStart()))
	{
		MessageBox(nullptr, "ImGuiの初期化に失敗しました。", "Error", MB_OK);
		return false;
	}

	// 日本語化
	io.Fonts->AddFontFromFileTTF("assets\\font\\UDDigiKyokashoN-R.ttc", 14.0f, nullptr, glyphRangesJapanese);

	return true;
}

void EFFECTTOOL::GuiDraw()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_Always);		// 位置指定
	ImGui::SetNextWindowSize(ImVec2(400, 880), ImGuiCond_Always);	// サイズ指定

	if (ImGui::Begin(u8"設定", &ImGuiFlg))
	{
		ImGui::End();
	}

	 //if (ImGui::BeginMenuBar())
	// {
		  //if (ImGui::BeginMenu(u8"新規作成"))
		  //{
	//          // エミッター設定
	//          if (ImGui::TreeNode(u8"エミッター"))
	//          {
	//              // 拡散
	//              if (ImGui::Button(u8"拡散"))
	//              {
	//                 // Effect[EFFECT::GetCreateIdx()].SetEmitterType(TYPE_SPREAD);
	//              }

	//              // 円
	//              if (ImGui::Button(u8"円"))
	//              {
	//                  //Effect[EFFECT::GetCreateIdx()].SetEmitterType(TYPE_CIRCLE);
	//              }

	//              // 同時
	//              if (ImGui::Button(u8"バースト"))
	//              {
	//                 // Effect[EFFECT::GetCreateIdx()].SetEmitterType(TYPE_BURST);
	//              }

	//              // 竜巻
	//              if (ImGui::Button(u8"竜巻"))
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

	  // フレームレート描画
	ImGui::SetNextWindowPos(ImVec2(1025, 20), ImGuiCond_Always);	// 位置指定
	ImGui::SetNextWindowSize(ImVec2(350, 50), ImGuiCond_Always);	// サイズ指定

	// フレームレート
	if (ImGui::Begin(u8"フレームレート", &ImGuiFlg))
	{
		// フレームレートを表示
		ImGui::Text(u8"%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		//// カメラタイプを表示
		//ImGui::Text(u8"カメラタイプ   :");
		//ImGui::SameLine();
		//ImGui::Text(CCamera::GetInstance()->GetCameraTypeName());

		ImGui::End();
	}

	ImGui::SetNextWindowPos(ImVec2(1025, 825), ImGuiCond_Always);	// 位置指定
	ImGui::SetNextWindowSize(ImVec2(350, 50), ImGuiCond_Always);	// サイズ指定

	// カメラタイプ
	if (ImGui::Begin(u8"カメラタイプ", &ImGuiFlg))
	{
		// カメラタイプを表示
		//ImGui::Text(u8"カメラタイプ   :");
		//ImGui::SameLine();
		ImGui::Text(CAMERA::GetInstance()->GetCameraTypeName());

		ImGui::End();
	}

	ImGui::Render();

	auto CmdList = DIRECTX12::GetInstance()->GetCommandList();
	CmdList->SetDescriptorHeaps(1, DescriptorHeapForImgui.GetAddressOf());
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), CmdList);

}

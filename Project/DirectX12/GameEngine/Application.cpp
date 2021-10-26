#include "Application.h"

GameEngine Engine;

// 目的  : 初期化
// 入力  : インスタンス
// 入力  : ウィンドウハンドル
// 入力  : フルスクリーンフラグ
// 入力  : ウィンドウの横幅
// 入力  : ウィンドウの立幅
// 出力  : 初期化に成功したのか

bool Init(HINSTANCE hInstance, HWND hWnd, bool bWindow, int _WindowWidth, int _WindowHeight)
{
	// DirectX12インスタンス生成
	DirectX12Wrapper::DirectX12Util::Create();
	if (!DirectX12Wrapper::DirectX12Util::GetInstance()->Init(hWnd, _WindowWidth, _WindowHeight))
	{
		MessageBox(nullptr, "DirectX12初期化エラー", "Error", MB_OK);
		return false;
	}

	//ImGuiインスタンス生成
	ImGuiWrapper::ImGuiUtil::Create();
	if (!ImGuiWrapper::ImGuiUtil::GetInstance()->Init(hWnd, DirectX12Wrapper::DirectX12Util::GetInstance()->GetDevice()))
	{
		MessageBox(nullptr, "ImGui初期化エラー", "Error", MB_OK);
		return false;
	}

	// ゲームエンジンの初期化
	if (!Engine.Init())
	{
		MessageBox(nullptr, "ゲームエンジン初期化エラー", "Error", MB_OK);
		return false;
	}

	return true;
}


// 目的  : 更新
// 入力  : 無し
// 出力  : 無し

void Update()
{
	Engine.Update();
}


// 目的  : 描画
// 入力  : 無し
// 出力  : 無し

void Draw()
{
	DirectX12Wrapper::DirectX12Util::GetInstance()->BeforeRender();
	ImGuiWrapper::ImGuiUtil::GetInstance()->BeforRender();

	Engine.Draw();

	ImGuiWrapper::ImGuiUtil::GetInstance()->AfterRender(DirectX12Wrapper::DirectX12Util::GetInstance()->GetCmdList());
	DirectX12Wrapper::DirectX12Util::GetInstance()->AfterRender();
}


// 目的  : 終了処理
// 入力  : 無し
// 出力  : 無し

void UnInit()
{
	Engine.UnInit();
	DirectX12Wrapper::DirectX12Util::GetInstance()->Destroy();
	ImGuiWrapper::ImGuiUtil::GetInstance()->Destroy();
}

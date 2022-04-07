#include "GameEngine.h"

// コンストラクタ

GameEngine::GameEngine()
{
	//Object.clear();
	//Object = nullptr;
}


// 目的  : ゲームエンジンの初期化
// 入力  : 無し
// 出力  : 無し

bool GameEngine::Init()
{
	return true;
}


// 目的  : ゲームエンジンの更新
// 入力  : 無し
// 出力  : 無し

void GameEngine::Update()
{
}


// 目的  : ゲームエンジンの描画
// 入力  : 無し
// 出力  : 無し

void GameEngine::Draw()
{
	GuiDraw();
	//for (auto itr = Object.begin(); itr != Object.end(); ++itr)
	//{
	//	(*itr)->Draw();
	//}

}


// 目的  : ゲームエンジンの終了処理
// 入力  : 無し
// 出力  : 無し

void GameEngine::UnInit()
{
	//Object.erase(std::remove(Object.begin(), Object.end(), 1), Object.end());
}


// 目的  : GUIの描画
// 入力  : 無し
// 出力  : 無し

void GameEngine::GuiDraw()
{
	//if (ImGui::BeginMainMenuBar())
	//{
	//	if (ImGui::BeginMenu(u8"ファイル"))
	//	{
	//		ImGui::EndMenu();
	//	}

	//	if (ImGui::BeginMenu(u8"編集"))
	//	{
	//		ImGui::EndMenu();
	//	}

	//	if (ImGui::BeginMenu(u8"ゲームオブジェクト"))
	//	{
	//		if (ImGui::Button(u8"作成"))
	//		{
	//			Object.emplace_back(new ComponentObject);
	//		}

	//		ImGui::EndMenu();
	//	}

	//	if (ImGui::BeginMenu(u8"ヘルプ"))
	//	{
	//		ImGui::EndMenu();
	//	}
	//	ImGui::EndMainMenuBar();
	//}

	//ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_Always);		// 位置指定
	//ImGui::SetNextWindowSize(ImVec2(300, 880), ImGuiCond_Once);		// サイズ指定

	//// 設定
	//if (ImGui::Begin(u8"ヒエラルキー", &HierarchieFlg))
	//{
	//	int Idx = 0;
	//	for (auto itr = Object.begin(); itr != Object.end(); ++itr)
	//	{
	//		if (ImGui::Button((*itr)->GetName().c_str()))
	//		{
	//			ChoiceIdx = Idx;
	//		}
	//		Idx++;
	//	}
	//	ImGui::End();
	//}

	//ImGui::SetNextWindowPos(ImVec2(1130, 20), ImGuiCond_Always);		// 位置指定
	//ImGui::SetNextWindowSize(ImVec2(300, 880), ImGuiCond_Once);			// サイズ指定

	//// 設定
	//if (ImGui::Begin(u8"インスペクター", &InspecterFlg))
	//{
	//	if (ChoiceIdx != -1)
	//	{
	//		Object[ChoiceIdx]->ParameterOutput();

	//		// コンポーネント追加
	//		if (ImGui::TreeNode(u8"コンポーネント追加"))
	//		{
	//			ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(100, 100), ImGuiWindowFlags_NoTitleBar);
	//			if (ImGui::Button("TransformComponent"))
	//			{
	//				Object[ChoiceIdx]->AddComponent<TransformComponent>();
	//			}
	//			if (ImGui::Button("MeshRenderer"))
	//			{
	//				Object[ChoiceIdx]->AddComponent<MeshRendererComponent>();
	//			}
	//			if (ImGui::Button("Collider"))
	//			{
	//				Object[ChoiceIdx]->AddComponent<ColliderComponent>();
	//			}
	//			ImGui::EndChild();
	//			ImGui::TreePop();
	//			ImGui::Separator();
	//		}
	//	}

	//	ImGui::End();
	//}

	//ImGui::SetNextWindowPos(ImVec2(300, 600), ImGuiCond_Always);		// 位置指定
	//ImGui::SetNextWindowSize(ImVec2(830, 250), ImGuiCond_Once);		// サイズ指定

	//// 設定
	//if (ImGui::Begin(u8"コンソール", &ConsoleFlg))
	//{
	//	ImGui::Text(u8"初期化");
	//	ImGui::End();
	//}
}
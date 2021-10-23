#include "GameEngine.h"

// コンストラクタ

GameEngine::GameEngine()
{
	Object.clear();
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
}


// 目的  : ゲームエンジンの終了処理
// 入力  : 無し
// 出力  : 無し

void GameEngine::UnInit()
{

}


// 目的  : GUIの描画
// 入力  : 無し
// 出力  : 無し

void GameEngine::GuiDraw()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu(u8"ファイル"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu(u8"編集"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu(u8"ゲームオブジェクト"))
		{
			if (ImGui::Button(u8"作成"))
			{
				Object.emplace_back(new GameObject);
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu(u8"ヘルプ"))
		{
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_Always);		// 位置指定
	ImGui::SetNextWindowSize(ImVec2(200, 880), ImGuiCond_Once);		// サイズ指定

	// 設定
	if (ImGui::Begin(u8"ヒエラルキー", &HierarchieFlg))
	{
		int Idx = 0;
		for (auto itr = Object.begin(); itr != Object.end(); ++itr)
		{
			if (ImGui::Button((*itr)->GetName().c_str()))
			{
				ChoiceIdx = Idx;
			}
			Idx++;
		}
		ImGui::End();
	}

	ImGui::SetNextWindowPos(ImVec2(1230, 20), ImGuiCond_Always);		// 位置指定
	ImGui::SetNextWindowSize(ImVec2(200, 880), ImGuiCond_Once);			// サイズ指定

	// 設定
	if (ImGui::Begin(u8"インスペクター", &InspecterFlg))
	{
	/*	if (ImGui::Button(u8"All Component Delete"))
		{
			Object[ChoiceIdx]->DeleteComponent<TransformComponent>();
		}*/
		if (ChoiceIdx != -1)
		{
			Object[ChoiceIdx]->ParameterOutput();


			
			// コンポーネント追加
			if (ImGui::TreeNode(u8"コンポーネント追加"))
			{
				if (ImGui::Button(u8"Transform")) 
				{
					Object[ChoiceIdx]->AddComponent<TransformComponent>();
				}
				if (ImGui::Button(u8"Renderer"))
				{

				}

				ImGui::TreePop();
				ImGui::Separator();
			}
		}

		ImGui::End();
	}

	ImGui::SetNextWindowPos(ImVec2(200, 600), ImGuiCond_Always);		// 位置指定
	ImGui::SetNextWindowSize(ImVec2(1030, 250), ImGuiCond_Once);		// サイズ指定

	// 設定
	if (ImGui::Begin(u8"コンソール", &ConsoleFlg))
	{
		ImGui::End();
	}
}
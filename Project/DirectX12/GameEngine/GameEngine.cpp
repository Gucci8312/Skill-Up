#include "GameEngine.h"

// �R���X�g���N�^

GameEngine::GameEngine()
{
	Object.clear();
	//Object = nullptr;
}


// �ړI  : �Q�[���G���W���̏�����
// ����  : ����
// �o��  : ����

bool GameEngine::Init()
{
	return true;
}


// �ړI  : �Q�[���G���W���̍X�V
// ����  : ����
// �o��  : ����

void GameEngine::Update()
{
}


// �ړI  : �Q�[���G���W���̕`��
// ����  : ����
// �o��  : ����

void GameEngine::Draw()
{
	GuiDraw();
}


// �ړI  : �Q�[���G���W���̏I������
// ����  : ����
// �o��  : ����

void GameEngine::UnInit()
{

}


// �ړI  : GUI�̕`��
// ����  : ����
// �o��  : ����

void GameEngine::GuiDraw()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu(u8"�t�@�C��"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu(u8"�ҏW"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu(u8"�Q�[���I�u�W�F�N�g"))
		{
			if (ImGui::Button(u8"�쐬"))
			{
				Object.emplace_back(new GameObject);
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu(u8"�w���v"))
		{
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_Always);		// �ʒu�w��
	ImGui::SetNextWindowSize(ImVec2(200, 880), ImGuiCond_Once);		// �T�C�Y�w��

	// �ݒ�
	if (ImGui::Begin(u8"�q�G�����L�[", &HierarchieFlg))
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

	ImGui::SetNextWindowPos(ImVec2(1230, 20), ImGuiCond_Always);		// �ʒu�w��
	ImGui::SetNextWindowSize(ImVec2(200, 880), ImGuiCond_Once);			// �T�C�Y�w��

	// �ݒ�
	if (ImGui::Begin(u8"�C���X�y�N�^�[", &InspecterFlg))
	{
	/*	if (ImGui::Button(u8"All Component Delete"))
		{
			Object[ChoiceIdx]->DeleteComponent<TransformComponent>();
		}*/
		if (ChoiceIdx != -1)
		{
			Object[ChoiceIdx]->ParameterOutput();


			
			// �R���|�[�l���g�ǉ�
			if (ImGui::TreeNode(u8"�R���|�[�l���g�ǉ�"))
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

	ImGui::SetNextWindowPos(ImVec2(200, 600), ImGuiCond_Always);		// �ʒu�w��
	ImGui::SetNextWindowSize(ImVec2(1030, 250), ImGuiCond_Once);		// �T�C�Y�w��

	// �ݒ�
	if (ImGui::Begin(u8"�R���\�[��", &ConsoleFlg))
	{
		ImGui::End();
	}
}
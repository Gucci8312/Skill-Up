#include "GameEngine.h"

// �R���X�g���N�^

GameEngine::GameEngine()
{
	//Object.clear();
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
	//for (auto itr = Object.begin(); itr != Object.end(); ++itr)
	//{
	//	(*itr)->Draw();
	//}

}


// �ړI  : �Q�[���G���W���̏I������
// ����  : ����
// �o��  : ����

void GameEngine::UnInit()
{
	//Object.erase(std::remove(Object.begin(), Object.end(), 1), Object.end());
}


// �ړI  : GUI�̕`��
// ����  : ����
// �o��  : ����

void GameEngine::GuiDraw()
{
	//if (ImGui::BeginMainMenuBar())
	//{
	//	if (ImGui::BeginMenu(u8"�t�@�C��"))
	//	{
	//		ImGui::EndMenu();
	//	}

	//	if (ImGui::BeginMenu(u8"�ҏW"))
	//	{
	//		ImGui::EndMenu();
	//	}

	//	if (ImGui::BeginMenu(u8"�Q�[���I�u�W�F�N�g"))
	//	{
	//		if (ImGui::Button(u8"�쐬"))
	//		{
	//			Object.emplace_back(new ComponentObject);
	//		}

	//		ImGui::EndMenu();
	//	}

	//	if (ImGui::BeginMenu(u8"�w���v"))
	//	{
	//		ImGui::EndMenu();
	//	}
	//	ImGui::EndMainMenuBar();
	//}

	//ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_Always);		// �ʒu�w��
	//ImGui::SetNextWindowSize(ImVec2(300, 880), ImGuiCond_Once);		// �T�C�Y�w��

	//// �ݒ�
	//if (ImGui::Begin(u8"�q�G�����L�[", &HierarchieFlg))
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

	//ImGui::SetNextWindowPos(ImVec2(1130, 20), ImGuiCond_Always);		// �ʒu�w��
	//ImGui::SetNextWindowSize(ImVec2(300, 880), ImGuiCond_Once);			// �T�C�Y�w��

	//// �ݒ�
	//if (ImGui::Begin(u8"�C���X�y�N�^�[", &InspecterFlg))
	//{
	//	if (ChoiceIdx != -1)
	//	{
	//		Object[ChoiceIdx]->ParameterOutput();

	//		// �R���|�[�l���g�ǉ�
	//		if (ImGui::TreeNode(u8"�R���|�[�l���g�ǉ�"))
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

	//ImGui::SetNextWindowPos(ImVec2(300, 600), ImGuiCond_Always);		// �ʒu�w��
	//ImGui::SetNextWindowSize(ImVec2(830, 250), ImGuiCond_Once);		// �T�C�Y�w��

	//// �ݒ�
	//if (ImGui::Begin(u8"�R���\�[��", &ConsoleFlg))
	//{
	//	ImGui::Text(u8"������");
	//	ImGui::End();
	//}
}
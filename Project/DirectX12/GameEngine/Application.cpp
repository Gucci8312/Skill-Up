#include "Application.h"

GameEngine Engine;

// �ړI  : ������
// ����  : �C���X�^���X
// ����  : �E�B���h�E�n���h��
// ����  : �t���X�N���[���t���O
// ����  : �E�B���h�E�̉���
// ����  : �E�B���h�E�̗���
// �o��  : �������ɐ��������̂�

bool Init(HINSTANCE hInstance, HWND hWnd, bool bWindow, int _WindowWidth, int _WindowHeight)
{
	// DirectX12�C���X�^���X����
	DirectX12Wrapper::DirectX12Util::Create();
	if (!DirectX12Wrapper::DirectX12Util::GetInstance()->Init(hWnd, _WindowWidth, _WindowHeight))
	{
		MessageBox(nullptr, "DirectX12�������G���[", "Error", MB_OK);
		return false;
	}

	//ImGui�C���X�^���X����
	ImGuiWrapper::ImGuiUtil::Create();
	if (!ImGuiWrapper::ImGuiUtil::GetInstance()->Init(hWnd, DirectX12Wrapper::DirectX12Util::GetInstance()->GetDevice()))
	{
		MessageBox(nullptr, "ImGui�������G���[", "Error", MB_OK);
		return false;
	}

	// �Q�[���G���W���̏�����
	if (!Engine.Init())
	{
		MessageBox(nullptr, "�Q�[���G���W���������G���[", "Error", MB_OK);
		return false;
	}

	return true;
}


// �ړI  : �X�V
// ����  : ����
// �o��  : ����

void Update()
{
	Engine.Update();
}


// �ړI  : �`��
// ����  : ����
// �o��  : ����

void Draw()
{
	DirectX12Wrapper::DirectX12Util::GetInstance()->BeforeRender();
	ImGuiWrapper::ImGuiUtil::GetInstance()->BeforRender();

	Engine.Draw();

	ImGuiWrapper::ImGuiUtil::GetInstance()->AfterRender(DirectX12Wrapper::DirectX12Util::GetInstance()->GetCmdList());
	DirectX12Wrapper::DirectX12Util::GetInstance()->AfterRender();
}


// �ړI  : �I������
// ����  : ����
// �o��  : ����

void UnInit()
{
	Engine.UnInit();
	DirectX12Wrapper::DirectX12Util::GetInstance()->Destroy();
	ImGuiWrapper::ImGuiUtil::GetInstance()->Destroy();
}

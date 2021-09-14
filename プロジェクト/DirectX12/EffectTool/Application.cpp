#include "Application.h"
SPRITE Sprite;
//EFFECTTOOL EffectTool;

// ������
bool Init(HINSTANCE _hInstance, HWND _hWnd, bool _bWindow)
{

	if (!DIRECTX12::GetInstance()->Init(_hWnd, WINDOW_WIDTH, WINDOW_HEIGHT))
	{
		MessageBox(nullptr, "DirectX12�������G���[", "Error", MB_OK);
	}

	// DIRECTINPUT������
	//CDirectInput::GetInstance().Init(_hInstance, _hWnd, WINDOW_WIDTH, WINDOW_HEIGHT);

	XMFLOAT3 eye = { 0.0f, 500.0f, -300.0f };				// ���_
	XMFLOAT3 lookat = { 0,0,0 };					// �����_
	XMFLOAT3 up = { 0,1,0 };						// ������x�N�g��

	CAMERA::GetInstance()->Init(1.0f, 1000.0f, XM_PIDIV2, WINDOW_WIDTH, WINDOW_HEIGHT, eye, lookat, up);

	Sprite.BackInit("Assets/Texture/Tile.png",XMFLOAT3(0.0f,0.0f,100.0f),1000.0f,1000.0f);
	Sprite.AngleSet(XMFLOAT3(0.0f, 0.0f, 0.0f));


	//EffectTool.Init(_hWnd);

	return true;
}

// �X�V
void Update()
{
	//CDirectInput::GetInstance().GetKeyBuffer();			// �L�[���͎擾
	CAMERA::GetInstance()->Update(XMFLOAT3(0.0f, 0.0f, 0.0f));
	Sprite.Update();
	//EffectTool.Update();
}

// �`��
void Draw()
{
	// �`��O����
	DIRECTX12::GetInstance()->BeforeRender();

	Sprite.Draw();
	//EffectTool.Draw();

	// �`��㏈��
	DIRECTX12::GetInstance()->AfterRender();
}

// �I������
void UnInit()
{
	//EffectTool.UnInit();
}

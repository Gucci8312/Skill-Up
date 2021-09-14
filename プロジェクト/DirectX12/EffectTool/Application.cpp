#include "Application.h"
SPRITE Sprite;
//EFFECTTOOL EffectTool;

// 初期化
bool Init(HINSTANCE _hInstance, HWND _hWnd, bool _bWindow)
{

	if (!DIRECTX12::GetInstance()->Init(_hWnd, WINDOW_WIDTH, WINDOW_HEIGHT))
	{
		MessageBox(nullptr, "DirectX12初期化エラー", "Error", MB_OK);
	}

	// DIRECTINPUT初期化
	//CDirectInput::GetInstance().Init(_hInstance, _hWnd, WINDOW_WIDTH, WINDOW_HEIGHT);

	XMFLOAT3 eye = { 0.0f, 500.0f, -300.0f };				// 視点
	XMFLOAT3 lookat = { 0,0,0 };					// 注視点
	XMFLOAT3 up = { 0,1,0 };						// 上向きベクトル

	CAMERA::GetInstance()->Init(1.0f, 1000.0f, XM_PIDIV2, WINDOW_WIDTH, WINDOW_HEIGHT, eye, lookat, up);

	Sprite.BackInit("Assets/Texture/Tile.png",XMFLOAT3(0.0f,0.0f,100.0f),1000.0f,1000.0f);
	Sprite.AngleSet(XMFLOAT3(0.0f, 0.0f, 0.0f));


	//EffectTool.Init(_hWnd);

	return true;
}

// 更新
void Update()
{
	//CDirectInput::GetInstance().GetKeyBuffer();			// キー入力取得
	CAMERA::GetInstance()->Update(XMFLOAT3(0.0f, 0.0f, 0.0f));
	Sprite.Update();
	//EffectTool.Update();
}

// 描画
void Draw()
{
	// 描画前処理
	DIRECTX12::GetInstance()->BeforeRender();

	Sprite.Draw();
	//EffectTool.Draw();

	// 描画後処理
	DIRECTX12::GetInstance()->AfterRender();
}

// 終了処理
void UnInit()
{
	//EffectTool.UnInit();
}

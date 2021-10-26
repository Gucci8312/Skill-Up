#include "main.h"

LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);			// ウインドウプロシージャ

int g_nFPS;
int WINAPI WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpszArgs, int _nWinMode)
{
	HWND hWnd;
	LPCTSTR szClassName = TEXT(WINDOW_NAME);
	MSG msg;
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),				// WNDCLASSEXのメモリサイズを指定
		CS_CLASSDC,						// 表示するウインドウのスタイルを設定
		WndProc,						// ウィンドウプロシージャのアドレス(関数名)を指定
		0,								// 通常は使用しないので"0"を指定
		0,								// 通常は使用しないので"0"を指定
		_hInstance,						// WinMainのパラメータのインスタンスハンドルを設定
		NULL,							// 使用するアイコンを指定(Windowsがもっているアイコンを使うならNULL)
		LoadCursor(NULL, IDC_ARROW),	// マウスカーソルを指定
		(HBRUSH)(COLOR_WINDOW + 1),		// ウインドウのクライアント領域の背景色を設定
		NULL,							// Windowにつけるメニューを設定
		CLASS_NAME,						// ウインドウクラスの名前
		NULL							// 小さいアイコンが設定された場合の情報を記述
	};

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// ウィンドウの作成
	hWnd = CreateWindowEx(0,						// 拡張ウィンドウスタイル
		CLASS_NAME,				// ウィンドウクラスの名前
		WINDOW_NAME,			// ウィンドウの名前
		WS_OVERLAPPEDWINDOW,	// ウィンドウスタイル
		CW_USEDEFAULT,			// ウィンドウの左上Ｘ座標
		CW_USEDEFAULT,			// ウィンドウの左上Ｙ座標 
		(WINDOW_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2), // ウィンドウの幅
		(WINDOW_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 +
			GetSystemMetrics(SM_CYCAPTION)), // ウィンドウの高さ
		NULL,					// 親ウィンドウのハンドル
		NULL,					// メニューハンドルまたは子ウィンドウID
		_hInstance,				// インスタンスハンドル
		NULL);					// ウィンドウ作成データ

	// 初期化処理(ウィンドウを作成してから行う)
	if (!Init(_hInstance, hWnd, TRUE, WINDOW_WIDTH, WINDOW_HEIGHT))
	{
		return -1;
	}

	// ウインドウの表示(初期化処理の後に行う)
	ShowWindow(hWnd, _nWinMode);
	UpdateWindow(hWnd);
	timeBeginPeriod(1);
	timeBeginPeriod(1);	//分解能(物事の細かさ)を認定
	int nLastTime = timeGetTime();
	int nFPSLastTime = timeGetTime();
	int nNowTime = 0;
	int nFPSCount = 0;

	// メッセージループ
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			// メイン処理
			nNowTime = timeGetTime();
			if (nNowTime - nLastTime >= 1000 / 60) {
				nFPSCount++;
				nLastTime = nNowTime;

				// 更新処理
				Update();

				// 描画処理
				Draw();
			}
			if (nNowTime - nFPSLastTime >= 1000) {
				nFPSLastTime = nNowTime;
				g_nFPS = nFPSCount;
				nFPSCount = 0;
			}
		}
	}

	//元の設定に戻す
	timeEndPeriod(1);

	// ウィンドウクラスの登録を解除
	//  第１引数：メクラス名
	//  第２引数：アプリケーションインスタンスのハンドル
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 終了処理
	UnInit();

	return 0;
}


// ウインドウプロシージャ//
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			nID = MessageBox(hWnd, "終了しますか？", "終了", MB_YESNO);
			if (nID == IDYES)
			{
				DestroyWindow(hWnd);
			}
			break;
		}
		break;

	case WM_CLOSE:
		nID = MessageBox(hWnd, "終了しますか？", "終了", MB_YESNO);
		if (nID == IDYES)
		{
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;
		}
		break;

	default:
		break;
	}
	ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
